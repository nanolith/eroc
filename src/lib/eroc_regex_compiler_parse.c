/**
 * \file lib/eroc_regex_compiler_parse.c
 *
 * \brief Given an input string, create an AST for further processing.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>
#include <stdbool.h>

/* forward decls. */
static bool is_pseudoinstruction(const eroc_regex_ast_node* ast);
static int read_input(eroc_regex_compiler_instance* inst);
static int shift_instruction(eroc_regex_compiler_instance* inst, int ch);
static int shift_any_instruction(eroc_regex_compiler_instance* inst);
static int shift_alternate_pseudoinstruction(
    eroc_regex_compiler_instance* inst);
static int shift_start_capture_pseudoinstruction(
    eroc_regex_compiler_instance* inst);
static int shift_end_capture_pseudoinstruction(
    eroc_regex_compiler_instance* inst);
static int reduce_instructions(eroc_regex_compiler_instance* inst);
static int reduce_concat(eroc_regex_compiler_instance* inst);
static int reduce_alternate(eroc_regex_compiler_instance* inst);
static int reduce_capture(eroc_regex_compiler_instance* inst);

/**
 * \brief Given an input string, create an AST for further processing.
 *
 * \param ast           Pointer to the AST pointer to be populated with the AST
 *                      on success.
 * \param input         The input string to parse.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_compiler_parse(eroc_regex_ast_node** ast, const char* input)
{
    int retval;
    eroc_regex_compiler_instance* inst;

    /* create the compiler instance. */
    retval = eroc_regex_compiler_instance_create(&inst, input);
    if (0 != retval)
    {
        goto done;
    }

    /* process the regular expression, character-by-character. */
    for (
        int ch = read_input(inst);
        0 != ch;
        ch = read_input(inst))
    {
        switch (inst->state)
        {
            case EROC_REGEX_COMPILER_STATE_SCAN:
                /* shift this instruction onto the stack. */
                retval = shift_instruction(inst, ch);
                if (0 != retval)
                {
                    goto cleanup_inst;
                }
                break;

            default:
                /* unsupported state for now. */
                return 1;
        }

        /* reduce instructions on the stack as far as we can. */
        retval = reduce_instructions(inst);
        if (0 != retval)
        {
            goto cleanup_inst;
        }
    }

    /* If the stack is empty, then this is an error. */
    if (NULL == inst->head)
    {
        retval = 2;
        goto cleanup_inst;
    }

    /* If we are in the middle of a char class, then this is an error. */
    if (EROC_REGEX_COMPILER_STATE_IN_CHAR_CLASS == inst->state)
    {
        retval = 5;
        goto cleanup_inst;
    }

    /* If the stack hasn't been completely reduced, then this is an error. */
    if (NULL != inst->head->next)
    {
        retval = 6;
        goto cleanup_inst;
    }

    /* if the top of stack is a pseudoinstruction, then this is an error. */
    if (is_pseudoinstruction(inst->head))
    {
        retval = 3;
        goto cleanup_inst;
    }

    /* Success: assign our ast to the head of stack, and clear the stack. */
    *ast = inst->head;
    inst->head = NULL;
    retval = 0;
    goto cleanup_inst;

cleanup_inst:
    eroc_regex_compiler_instance_release(inst);

done:
    return retval;
}

/**
 * \brief Check to see if the given AST node is a pseudoinstruction.
 *
 * \param ast           The AST node to check.
 */
static bool is_pseudoinstruction(const eroc_regex_ast_node* ast)
{
    switch (ast->type)
    {
        case EROC_REGEX_AST_PLACEHOLDER_START_CAPTURE:
        case EROC_REGEX_AST_PLACEHOLDER_END_CAPTURE:
        case EROC_REGEX_AST_PLACEHOLDER_ALTERNATE:
            return true;

        default:
            return false;
    }
}

/**
 * \brief Read the next character from input.
 *
 * \param inst          The compiler instance for this operation.
 */
static int read_input(eroc_regex_compiler_instance* inst)
{
    return inst->input[inst->offset++];
}

/**
 * \brief Shift the given instruction onto the stack.
 *
 * \param inst          The compiler instance for this operation.
 * \param ch            The instruction to shift.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int shift_instruction(eroc_regex_compiler_instance* inst, int ch)
{
    int retval;

    switch (ch)
    {
        case '.':
            retval = shift_any_instruction(inst);
            break;

        case '|':
            retval = shift_alternate_pseudoinstruction(inst);
            break;

        case '(':
            retval = shift_start_capture_pseudoinstruction(inst);
            break;

        case ')':
            retval = shift_end_capture_pseudoinstruction(inst);
            break;

        /* unsupported instruction. */
        default:
            retval = 1;
            break;
    }

    return retval;
}

/**
 * \brief Shift an any instruction onto the stack.
 *
 * \param inst          The compiler instance for this operation.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int shift_any_instruction(eroc_regex_compiler_instance* inst)
{
    eroc_regex_ast_node* ast;

    /* create an any node. */
    int retval = eroc_regex_ast_node_any_create(&ast);
    if (0 != retval)
    {
        return retval;
    }

    /* shift this node onto the stack. */
    ast->next = inst->head;
    inst->head = ast;

    return 0;
}

/**
 * \brief Shift an alternate pseudoinstruction onto the stack.
 *
 * \param inst              The compiler instance for this operation.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int shift_alternate_pseudoinstruction(
    eroc_regex_compiler_instance* inst)
{
    eroc_regex_ast_node* ast;

    /* The stack can't be empty to shift an alternate. */
    if (NULL == inst->head)
    {
        return 1;
    }

    /* We can't shift an alternate after an alternate. */
    if (EROC_REGEX_AST_PLACEHOLDER_ALTERNATE == inst->head->type)
    {
        return 2;
    }

    /* We can't shift an alternate immediately after a start capture. */
    if (EROC_REGEX_AST_PLACEHOLDER_START_CAPTURE == inst->head->type)
    {
        return 3;
    }

    /* create an empty node. */
    int retval = eroc_regex_ast_node_empty_create(&ast);
    if (0 != retval)
    {
        return retval;
    }

    /* override the type to make it an alternate pseudoinstruction. */
    ast->type = EROC_REGEX_AST_PLACEHOLDER_ALTERNATE;

    /* shift this node onto the stack. */
    ast->next = inst->head;
    inst->head = ast;

    return 0;
}

/**
 * \brief Shift a start capture pseudoinstruction onto the stack.
 *
 * \param inst              The compiler instance for this operation.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int shift_start_capture_pseudoinstruction(
    eroc_regex_compiler_instance* inst)
{
    eroc_regex_ast_node* ast;

    /* create an empty node. */
    int retval = eroc_regex_ast_node_empty_create(&ast);
    if (0 != retval)
    {
        return retval;
    }

    /* override the type to make it a start capture pseudoinstruction. */
    ast->type = EROC_REGEX_AST_PLACEHOLDER_START_CAPTURE;

    /* shift this node onto the stack. */
    ast->next = inst->head;
    inst->head = ast;

    return 0;
}

/**
 * \brief Shift an end capture pseudoinstruction onto the stack.
 *
 * \param inst              The compiler instance for this operation.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int shift_end_capture_pseudoinstruction(
    eroc_regex_compiler_instance* inst)
{
    eroc_regex_ast_node* ast;

    /* the stack can't be empty. */
    if (NULL == inst->head)
    {
        return 1;
    }

    /* the top of stack can't be a start capture. */
    if (EROC_REGEX_AST_PLACEHOLDER_START_CAPTURE == inst->head->type)
    {
        return 2;
    }

    /* create an empty node. */
    int retval = eroc_regex_ast_node_empty_create(&ast);
    if (0 != retval)
    {
        return retval;
    }

    /* override the type to make it an end capture pseudoinstruction. */
    ast->type = EROC_REGEX_AST_PLACEHOLDER_END_CAPTURE;

    /* shift this node onto the stack. */
    ast->next = inst->head;
    inst->head = ast;

    return 0;
}

/**
 * \brief Reduce instructions by combining them according to stack ordering.
 *
 * \param inst          The compiler instance for this operation.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int reduce_instructions(eroc_regex_compiler_instance* inst)
{
    int retval;
    eroc_regex_ast_node* right = inst->head;
    eroc_regex_ast_node* left;

    /* if there are no instructions on the stack, that's an error. */
    if (NULL == right)
    {
        return 1;
    }

    left = right->next;

    /* loop while there is still more than one instruction on the stack. */
    while (NULL != left)
    {
        switch (right->type)
        {
            /* we can't reduce a right-hand alternate. */
            case EROC_REGEX_AST_PLACEHOLDER_ALTERNATE:
                return 0;

            /* we can't reduce a right-hand start capture. */
            case EROC_REGEX_AST_PLACEHOLDER_START_CAPTURE:
                return 0;

            /* attempt to reduce a right-hand end capture. */
            case EROC_REGEX_AST_PLACEHOLDER_END_CAPTURE:
                retval = reduce_capture(inst);
                if (0 != retval)
                {
                    return retval;
                }
                goto next;
        }

        switch (left->type)
        {
            /* we can't reduce this any further until we get the end capture. */
            case EROC_REGEX_AST_PLACEHOLDER_START_CAPTURE:
                return 0;

            /* handle alternate case. */
            case EROC_REGEX_AST_PLACEHOLDER_ALTERNATE:
                retval = reduce_alternate(inst);
                break;

            /* in all other cases, concat the two instructions. */
            default:
                retval = reduce_concat(inst);
                break;
        }

        /* if reduction failed, return the error code. */
        if (0 != retval)
        {
            return retval;
        }

    next:
        /* set up for next run. */
        right = inst->head;
        left = right->next;
    }

    /* success. */
    return 0;
}

/**
 * \brief Reduce the top two instructions on the stack into a concat.
 *
 * \param inst              The compiler instance for this operation.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int reduce_concat(eroc_regex_compiler_instance* inst)
{
    int retval;
    eroc_regex_ast_node* right = inst->head;
    eroc_regex_ast_node* left = right->next;
    eroc_regex_ast_node* ast;

    /* create a concat node to hold these values. */
    retval = eroc_regex_ast_node_concat_create(&ast, left, right);
    if (0 != retval)
    {
        return retval;
    }

    /* pop these values off of the stack. */
    inst->head = left->next;
    left->next = right->next = NULL;

    /* push the new node on the stack. */
    ast->next = inst->head;
    inst->head = ast;

    return 0;
}

/**
 * \brief Reduce the top three instructions on the stack into an alternate.
 *
 * \param inst              The compiler instance for this operation.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int reduce_alternate(eroc_regex_compiler_instance* inst)
{
    int retval;
    eroc_regex_ast_node* right = inst->head;
    eroc_regex_ast_node* alt = right->next;
    eroc_regex_ast_node* left = alt->next;
    eroc_regex_ast_node* ast;

    /* verify that left is valid. */
    if (NULL == left)
    {
        return 1;
    }

    /* create an alternate node to hold these values. */
    retval = eroc_regex_ast_node_alternate_create(&ast, left, right);
    if (0 != retval)
    {
        return retval;
    }

    /* pop these values off of the stack. */
    inst->head = left->next;
    left->next = alt->next = right->next = NULL;

    /* push the new node on the stack. */
    ast->next = inst->head;
    inst->head = ast;

    /* release alt. */
    eroc_regex_ast_node_release(alt);

    return 0;
}

/**
 * \brief Attempt to reduce a capture instruction.
 *
 * \param inst              The compiler instance for this operation.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int reduce_capture(eroc_regex_compiler_instance* inst)
{
    int retval;
    eroc_regex_ast_node* end = inst->head;
    eroc_regex_ast_node* in = end->next;
    eroc_regex_ast_node* start = in->next;
    eroc_regex_ast_node* ast;

    /* verify that start is valid. */
    if (NULL == start)
    {
        return 1;
    }

    /* for this reduction to work, start and end must start and end capture, */
    /* and in can't be a pseudoinstruction. */
    if (   EROC_REGEX_AST_PLACEHOLDER_START_CAPTURE != start->type
        || EROC_REGEX_AST_PLACEHOLDER_END_CAPTURE != end->type
        || is_pseudoinstruction(in))
    {
        return 2;
    }

    /* create a capture node to hold the instruction. */
    retval = eroc_regex_ast_node_capture_create(&ast, in, (inst->captures)++);
    if (0 != retval)
    {
        return retval;
    }

    /* pop these values off of the stack. */
    inst->head = start->next;
    start->next = in->next = end->next = NULL;

    /* push the new node onto the stack. */
    ast->next = inst->head;
    inst->head = ast;

    /* release start and end. */
    eroc_regex_ast_node_release(start);
    eroc_regex_ast_node_release(end);

    return 0;
}

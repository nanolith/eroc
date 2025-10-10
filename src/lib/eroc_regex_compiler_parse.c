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
static int shift_char_class_instruction(
    eroc_regex_compiler_instance* inst, int ch, bool maybe_invert);
static int shift_any_instruction(eroc_regex_compiler_instance* inst);
static int shift_alternate_pseudoinstruction(
    eroc_regex_compiler_instance* inst);
static int shift_start_capture_pseudoinstruction(
    eroc_regex_compiler_instance* inst);
static int shift_end_capture_pseudoinstruction(
    eroc_regex_compiler_instance* inst);
static int shift_begin_char_class_instruction(
    eroc_regex_compiler_instance* inst);
static int shift_end_char_class_instruction(
    eroc_regex_compiler_instance* inst);
static int add_member_char_class_instruction(
    eroc_regex_compiler_instance* inst, int ch);
static int add_range_char_class_instruction(eroc_regex_compiler_instance* inst);
static int invert_char_class_instruction(eroc_regex_compiler_instance* inst);
static int reduce_instructions(eroc_regex_compiler_instance* inst);
static int reduce_concat(eroc_regex_compiler_instance* inst);
static int reduce_alternate(eroc_regex_compiler_instance* inst);
static int reduce_capture(eroc_regex_compiler_instance* inst);
static int reduce_char_class_literal(
    eroc_regex_compiler_instance* inst, int next_character,
    bool has_next_character);
static int reduce_char_class_range(eroc_regex_compiler_instance* inst);
static bool has_char_class_members(const eroc_regex_ast_node* ast);

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
                break;

            case EROC_REGEX_COMPILER_STATE_IN_CHAR_CLASS_MAYBE_INVERT:
                retval = shift_char_class_instruction(inst, ch, true);
                break;

            case EROC_REGEX_COMPILER_STATE_IN_CHAR_CLASS:
                retval = shift_char_class_instruction(inst, ch, false);
                break;

            default:
                /* unsupported state for now. */
                retval =  1;
                break;
        }

        /* if instruction decoding failed, clean up. */
        if (0 != retval)
        {
            goto cleanup_inst;
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

    /* If we are NOT in the scan state, then this is an error. */
    if (EROC_REGEX_COMPILER_STATE_SCAN != inst->state)
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
        case EROC_REGEX_AST_PLACEHOLDER_LITERAL:
        case EROC_REGEX_AST_PLACEHOLDER_START_RANGE:
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

        case '[':
            retval = shift_begin_char_class_instruction(inst);
            break;

        /* unsupported instruction. */
        default:
            retval = 1;
            break;
    }

    return retval;
}

/**
 * \brief Shift a char class instruction onto the stack.
 *
 * \param inst          The compiler instance for this operation.
 * \param ch            The instruction to shift.
 * \param maybe_invert  Should we treat a caret as an invert?
 *
 * \returns 0 on success and non-zero on failure.
 */
static int shift_char_class_instruction(
    eroc_regex_compiler_instance* inst, int ch, bool maybe_invert)
{
    int retval;

    switch (ch)
    {
        case ']':
            retval = shift_end_char_class_instruction(inst);
            break;

        case '^':
            if (maybe_invert)
            {
                retval = invert_char_class_instruction(inst);
            }
            else
            {
                retval = add_member_char_class_instruction(inst, ch);
            }
            break;

        case '-':
            retval = add_range_char_class_instruction(inst);
            break;

        default:
            retval = add_member_char_class_instruction(inst, ch);
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
 * \brief Shift a char class instruction onto the stack and enter a char class
 * state.
 *
 * \param inst              The compiler instance for this operation.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int shift_begin_char_class_instruction(
    eroc_regex_compiler_instance* inst)
{
    eroc_regex_ast_node* ast;

    /* create a char class node. */
    int retval = eroc_regex_ast_node_char_class_create(&ast);
    if (0 != retval)
    {
        return retval;
    }

    /* shift this node onto the stack. */
    ast->next = inst->head;
    inst->head = ast;

    /* we are now in the char class state. */
    inst->state = EROC_REGEX_COMPILER_STATE_IN_CHAR_CLASS_MAYBE_INVERT;

    return 0;
}

/**
 * \brief Resolve the char class instruction on the stack.
 *
 * \param inst              The compiler instance for this operation.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int shift_end_char_class_instruction(
    eroc_regex_compiler_instance* inst)
{
    int retval;

    /* verify that there is an instruction at the top of stack. */
    if (NULL == inst->head)
    {
        return 1;
    }

    /* is this a literal pseudoinstruction? */
    if (EROC_REGEX_AST_PLACEHOLDER_LITERAL == inst->head->type)
    {
        retval = reduce_char_class_literal(inst, -1, false);
        if (0 != retval)
        {
            return retval;
        }
    }

    /* verify that the instruction at the top of stack is a char class. */
    if (EROC_REGEX_AST_CHAR_CLASS != inst->head->type)
    {
        return 1;
    }

    /* verify that there is AT LEAST one character in the class. */
    if (!has_char_class_members(inst->head))
    {
        return 2;
    }

    /* switch back to the scan state so this instruction can be reduced. */
    inst->state = EROC_REGEX_COMPILER_STATE_SCAN;

    return 0;
}

/**
 * \brief Invert the char class instruction on the stack.
 *
 * \param inst              The compiler instance for this operation.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int invert_char_class_instruction( eroc_regex_compiler_instance* inst)
{
    /* verify that the instruction is on the stack. */
    if (NULL == inst->head || EROC_REGEX_AST_CHAR_CLASS != inst->head->type)
    {
        return 1;
    }

    /* invert the instruction. */
    inst->head->data.char_class.inverse = true;

    /* fall to the regular char class state. */
    inst->state = EROC_REGEX_COMPILER_STATE_IN_CHAR_CLASS;

    return 0;
}

/**
 * \brief Add a character to the char class instruction on the stack.
 *
 * \param inst              The compiler instance for this operation.
 * \param ch                The character to add to this char class.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int add_member_char_class_instruction(
    eroc_regex_compiler_instance* inst, int ch)
{
    int retval;
    eroc_regex_ast_node* literal;

    /* verify that there is an instruction at top of stack. */
    if (NULL == inst->head)
    {
        return 1;
    }

    /* is this a literal pseudoinstruction? */
    if (
        EROC_REGEX_AST_PLACEHOLDER_LITERAL == inst->head->type
     || EROC_REGEX_AST_PLACEHOLDER_START_RANGE == inst->head->type)
    {
        return reduce_char_class_literal(inst, ch, true);
    }
    /* otherwise, it must be the char class at top of stack. */
    else if (EROC_REGEX_AST_CHAR_CLASS != inst->head->type)
    {
        return 2;
    }

    /* create a character literal to place on the stack. */
    retval = eroc_regex_ast_node_literal_create(&literal, ch);
    if (0 != retval)
    {
        return retval;
    }

    /* make this a pseudoliteral. */
    literal->type = EROC_REGEX_AST_PLACEHOLDER_LITERAL;

    /* push this value onto the stack. */
    literal->next = inst->head;
    inst->head = literal;

    return 0;
}

/**
 * \brief Possibly add a ranged char class instruction.
 *
 * \param inst              The compiler instance for this operation.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int add_range_char_class_instruction(eroc_regex_compiler_instance* inst)
{
    /* verify that there is an instruction at top of stack. */
    if (NULL == inst->head)
    {
        return 1;
    }

    /* is this a literal pseudoinstruction? */
    if (
        EROC_REGEX_AST_PLACEHOLDER_LITERAL == inst->head->type
     || EROC_REGEX_AST_PLACEHOLDER_START_RANGE == inst->head->type)
    {
        return reduce_char_class_range(inst);
    }

    /* otherwise, fall back to adding a member char class instruction. */
    return add_member_char_class_instruction(inst, '-');
}

/**
 * \brief Reduce a char class literal and a char class.
 *
 * \param inst                  The compiler instance for this operation.
 * \param next_character        Optional next character to use for reduction.
 * \param has_next_character    Flag to indicate whether we have a next
 *                              character.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int reduce_char_class_literal(
    eroc_regex_compiler_instance* inst, int next_character,
    bool has_next_character)
{
    int retval;
    eroc_regex_ast_node* literal = inst->head;
    eroc_regex_ast_node* char_class;

    /* if there are no instructions on the stack, that's an error. */
    if (NULL == literal)
    {
        return 1;
    }

    /* verify the char class comes after this literal. */
    char_class = literal->next;
    if (NULL == char_class || EROC_REGEX_AST_CHAR_CLASS != char_class->type)
    {
        return 2;
    }

    /* retrieve the literal value. */
    int value = literal->data.literal;

    /* add this value to the char class. */
    retval = eroc_regex_ast_char_class_member_add(char_class, value);
    if (0 != retval)
    {
        return 3;
    }

    /* do we have a next character? */
    if (has_next_character)
    {
        /* if this is a simple literal, then just shift in the new character. */
        if (EROC_REGEX_AST_PLACEHOLDER_LITERAL == literal->type)
        {
            literal->data.literal = next_character;
            return 0;
        }

        /* otherwise, verify that this is a valid range. */
        if (next_character < value)
        {
            return 4;
        }

        for(++value; value <= next_character; ++value)
        {
            retval = eroc_regex_ast_char_class_member_add(char_class, value);
            if (0 != retval)
            {
                return 3;
            }
        }

        /* fall-through to cleanup; this range is exhausted. */
    }

    /* pop this pseudoliteral off of the stack. */
    inst->head = char_class;

    /* clean up. */
    eroc_regex_ast_node_release(literal);

    return 0;
}

/**
 * \brief Reduce a char class literal to a range pseudoinstruction.
 *
 * \param inst          The compiler instance for this operation.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int reduce_char_class_range(eroc_regex_compiler_instance* inst)
{
    /* if there are no instructions on the stack, that's an error. */
    if (NULL == inst->head)
    {
        return 1;
    }

    /* verify that this is a literal pseudoinstruction. */
    if (EROC_REGEX_AST_PLACEHOLDER_LITERAL != inst->head->type)
    {
        return 2;
    }

    /* upgrade this instruction to a range pseudoinstruction. */
    inst->head->type = EROC_REGEX_AST_PLACEHOLDER_START_RANGE;

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

    /* instructions can only be reduced in certain states. */
    switch (inst->state)
    {
        /* we can't reduce while in a char class. */
        case EROC_REGEX_COMPILER_STATE_IN_CHAR_CLASS_MAYBE_INVERT:
        case EROC_REGEX_COMPILER_STATE_IN_CHAR_CLASS:
            return 0;
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

/**
 * \brief Verify whether the given character class AST node has members.
 *
 * \param ast               The AST node to check.
 *
 * \returns true if members were found and false otherwise.
 */
static bool has_char_class_members(const eroc_regex_ast_node* ast)
{
    for (int i = 0; i < 8; ++i)
    {
        if (ast->data.char_class.members[i] != 0)
            return true;
    }

    return false;
}

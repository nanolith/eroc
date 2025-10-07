/**
 * \file lib/eroc_regex_compiler_parse.c
 *
 * \brief Given an input string, create an AST for further processing.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>

/* forward decls. */
static int read_input(eroc_regex_compiler_instance* inst);

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
    }

    /* If we are still in init, then the input expression is invalid. */
    if (EROC_REGEX_COMPILER_STATE_INIT == inst->state)
    {
        retval = 1;
        goto cleanup_inst;
    }

    /* If the stack is empty, then this is an error. */
    if (NULL == inst->head)
    {
        retval = 2;
        goto cleanup_inst;
    }

    (void)ast;

cleanup_inst:
    eroc_regex_compiler_instance_release(inst);

done:
    return retval;
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

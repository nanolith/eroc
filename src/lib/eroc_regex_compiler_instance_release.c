/**
 * \file lib/eroc_regex_compiler_instance_release.c
 *
 * \brief Release a regular expression compiler instance.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>
#include <stdlib.h>

/**
 * \brief Release a regular expression compiler instance.
 *
 * \param inst          The instance to release.
 */
void eroc_regex_compiler_instance_release(eroc_regex_compiler_instance* inst)
{
    /* Release all nodes in the stack. */
    while (NULL != inst->head)
    {
        eroc_regex_ast_node* tmp = inst->head->next;
        inst->head->next = NULL;

        eroc_regex_ast_node_release(inst->head);
        inst->head = tmp;
    }

    /* release the current AST root. */
    if (NULL != inst->ast)
    {
        eroc_regex_ast_node_release(inst->ast);
    }

    /* clean up memory. */
    free(inst);
}

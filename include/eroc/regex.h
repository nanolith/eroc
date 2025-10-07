/**
 * \file eroc/regex.h
 *
 * \brief Simple and opinionated DFA regular expression parser, compiler, and
 * runtime.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief Types of AST nodes supported by the regular expression parser.
 */
enum eroc_regex_ast_node_type
{
    EROC_REGEX_AST_EMPTY,
    EROC_REGEX_AST_ANY,
    EROC_REGEX_AST_LITERAL,
    EROC_REGEX_AST_CONCAT,
    EROC_REGEX_AST_ALTERNATE,
    EROC_REGEX_AST_CHAR_CLASS,
    EROC_REGEX_AST_STAR,
    EROC_REGEX_AST_PLUS,
    EROC_REGEX_AST_OPTIONAL,
    EROC_REGEX_AST_CAPTURE,
    EROC_REGEX_AST_PLACEHOLDER_START_CAPTURE,
};

/**
 * \brief Types of combinators to use to combine current ast with next node.
 */
enum eroc_regex_combinator_type
{
    EROC_REGEX_COMBINATOR_NONE,
    EROC_REGEX_COMBINATOR_CONCAT,
    EROC_REGEX_COMBINATOR_ALTERNATE,
};

/**
 * \brief States for the regular expression compiler.
 */
enum eroc_regex_compiler_state
{
    EROC_REGEX_COMPILER_STATE_INIT,
    EROC_REGEX_COMPILER_STATE_SCAN,
    EROC_REGEX_COMPILER_STATE_IN_CHAR_CLASS,
    EROC_REGEX_COMPILER_STATE_EOF,
};

/**
 * \brief The regular expression AST node is used by the parser to represent a
 * regular expression operation.
 */
typedef struct eroc_regex_ast_node eroc_regex_ast_node;

struct eroc_regex_ast_node
{
    eroc_regex_ast_node* next;
    int type;
    union
    {
        char literal;
        struct
        {
            eroc_regex_ast_node* left;
            eroc_regex_ast_node* right;
        } binary;
        struct
        {
            eroc_regex_ast_node* child;
        } unary;
        struct
        {
            eroc_regex_ast_node* child;
            int group_index;
        } capture;
        struct
        {
            /* 256 bytes in ASCII / 8 bits per byte = 32 bytes. */
            /* 32 bytes / 4 bytes per uint32_t = 8 uint32_t values. */
            uint32_t members[8];
        } char_class;
    } data;
};

/**
 * \brief The compiler data structure is used to represent state in the modified
 * parser push-down automaton. Unlike a "true" PDA, this one also includes a
 * separate nesting depth and combinator flag.
 */
typedef struct eroc_regex_compiler_instance eroc_regex_compiler_instance;

struct eroc_regex_compiler_instance
{
    eroc_regex_ast_node* head;
    eroc_regex_ast_node* ast;
    int state;
    const char* input;
    size_t offset;
    size_t nesting_depth;
    int next_combinator;
};

/**
 * \brief Create an empty AST node.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_empty_create(eroc_regex_ast_node** node);

/**
 * \brief Create an any AST node.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_any_create(eroc_regex_ast_node** node);

/**
 * \brief Create a char literal AST node.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 * \param c             The character literal.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_literal_create(eroc_regex_ast_node** node, char c);

/**
 * \brief Create a concat AST node.
 *
 * \note On success, the created node takes ownership of the left and right
 * nodes.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 * \param left          The left-hand side of the concat.
 * \param right         The right-hand side of the concat.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_concat_create(
    eroc_regex_ast_node** node, eroc_regex_ast_node* left,
    eroc_regex_ast_node* right);

/**
 * \brief Create an alternate AST node.
 *
 * \note On success, the created node takes ownership of the left and right
 * nodes.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 * \param left          The left-hand side of the alternate.
 * \param right         The right-hand side of the alternate.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_alternate_create(
    eroc_regex_ast_node** node, eroc_regex_ast_node* left,
    eroc_regex_ast_node* right);

/**
 * \brief Create a character class AST node.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 * \param members       A bitset of the members of this character class.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_char_class_create(
    eroc_regex_ast_node** node, const uint32_t members[8]);

/**
 * \brief Create a star AST node.
 *
 * \note On success, the created node takes ownership of the child node.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 * \param child         The child of this star node.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_star_create(
    eroc_regex_ast_node** node, eroc_regex_ast_node* child);

/**
 * \brief Create a plus AST node.
 *
 * \note On success, the created node takes ownership of the child node.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 * \param child         The child of this plus node.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_plus_create(
    eroc_regex_ast_node** node, eroc_regex_ast_node* child);

/**
 * \brief Create an optional AST node.
 *
 * \note On success, the created node takes ownership of the child node.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 * \param child         The child of this optional node.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_optional_create(
    eroc_regex_ast_node** node, eroc_regex_ast_node* child);

/**
 * \brief Create a capture AST node.
 *
 * \note On success, the created node takes ownership of the child node.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 * \param child         The child of this capture node.
 * \param group_index   The group index for this capture node.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_capture_create(
    eroc_regex_ast_node** node, eroc_regex_ast_node* child, int group_index);

/**
 * \brief Release an AST node.
 *
 * \note This release mechanism is recursive; it also releases all child nodes
 * it owns.
 *
 * \param node          The node to release.
 */
void eroc_regex_ast_node_release(eroc_regex_ast_node* node);

/**
 * \brief Create a compiler instance backed by the given input string.
 *
 * \note The input string remains owned by the caller.
 *
 * \param inst          Pointer to the instance pointer to be populated with
 *                      this created instance on success.
 * \param input         The input string that this compiler transforms.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_compiler_instance_create(
    eroc_regex_compiler_instance** inst, const char* input);

/**
 * \brief Release a regular expression compiler instance.
 *
 * \param inst          The instance to release.
 */
void eroc_regex_compiler_instance_release(eroc_regex_compiler_instance* inst);

/**
 * \brief Given an input string, create an AST for further processing.
 *
 * \param ast           Pointer to the AST pointer to be populated with the AST
 *                      on success.
 * \param input         The input string to parse.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_compiler_parse(eroc_regex_ast_node** ast, const char* input);

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/

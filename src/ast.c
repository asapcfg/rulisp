#include "ast.h"

#include <stdlib.h>
#include <string.h>

AST *ast_create(ASTType type, const char *value)
{
    AST *node = calloc(1, sizeof(AST));

    if (!node)
        return NULL;

    node->type = type;

    if (value) {
        node->value = malloc(strlen(value) + 1);

        if (!node->value) {
            free(node);
            return NULL;
        }

        strcpy(node->value, value);
    }

    return node;
}

void ast_add_child(AST *parent, AST *child)
{
    if (!parent->children) {
        parent->children = child;
        return;
    }

    AST *current = parent->children;

    while (current->next)
        current = current->next;

    current->next = child;
}

void ast_free(AST *node)
{
    if (!node)
        return;

    AST *child = node->children;

    while (child) {
        AST *next = child->next;
        ast_free(child);
        child = next;
    }

    free(node->value);
    free(node);
}

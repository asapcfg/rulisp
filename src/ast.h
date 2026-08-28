#ifndef ast_h
#define ast_h

typedef enum {
    AST_STRING,
    AST_SYMBOL,
    AST_LIST
} ASTType;

typedef struct AST {
    ASTType type;

    char *value;

    struct AST *children;
    struct AST *next;

} AST;

AST *ast_create(ASTType type, const char *value);

void ast_add_child(AST *parent, AST *child);

void ast_free(AST *node);

#endif

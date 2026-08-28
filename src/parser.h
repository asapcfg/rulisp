#ifndef parser_h
#define parser_h

#include "ast.h"
#include "lexer.h"

typedef struct {
    Lexer lexer;
    Token current;
} Parser;

void parser_init(Parser *parser, const char *source);

AST *parser_parse(Parser *parser);

#endif

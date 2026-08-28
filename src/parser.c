#include "parser.h"

#include <stdio.h>

static void next_token(Parser *parser)
{
    token_free(&parser->current);
    parser->current = lexer_next(&parser->lexer);
}

void parser_init(Parser *parser, const char *source)
{
    lexer_init(&parser->lexer, source);

    parser->current.type = error;
    parser->current.value = NULL;

    next_token(parser);
}

static AST *parse_expression(Parser *parser);

static AST *parse_list(Parser *parser)
{
    AST *list = ast_create(AST_LIST, NULL);

    if (!list)
        return NULL;

    /* '(' */
    next_token(parser);

    while (parser->current.type != rskobka &&
           parser->current.type != eof) {

        AST *child = parse_expression(parser);

        if (!child) {
            ast_free(list);
            return NULL;
        }

        ast_add_child(list, child);
    }

    if (parser->current.type != rskobka) {
        fprintf(
            stderr,
            "ожидалось ) в %zu:%zu\n",
            parser->current.line,
            parser->current.column
        );

        ast_free(list);
        return NULL;
    }

    next_token(parser);

    return list;
}

static AST *parse_expression(Parser *parser)
{
    switch (parser->current.type) {

        case string: {
            AST *node = ast_create(
                AST_STRING,
                parser->current.value
            );

            next_token(parser);

            return node;
        }

        case symbol: {
            AST *node = ast_create(
                AST_SYMBOL,
                parser->current.value
            );

            next_token(parser);

            return node;
        }

        case lskobka:
            return parse_list(parser);

        case rskobka:
            fprintf(
                stderr,
                "ждал ) в %zu:%zu\n",
                parser->current.line,
                parser->current.column
            );
            return NULL;

        case TOKEN_EOF:
            fprintf(stderr, "конец файла не там где я хочу\n");
            return NULL;

        default:
            fprintf(stderr, "Это не баг, это фича. (неизвестная ошибка)\n");
            return NULL;
    }
}

AST *parser_parse(Parser *parser)
{
    AST *root = ast_create(AST_LIST, NULL);

    if (!root)
        return NULL;

    while (parser->current.type != eof) {

        AST *expression = parse_expression(parser);

        if (!expression) {
            ast_free(root);
            return NULL;
        }

        ast_add_child(root, expression);
    }

    return root;
}

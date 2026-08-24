#ifndef parser_h
#define parser_h
#include "lexer.h"

typedef enum {
	NODE_CALL,
	NODE_STRING
} NodeType;

typedef struct Node {
	NodeType type;
	char *value;
	struct Node *child;
	struct Node *text;
} Node;

typedef struct {
	Lexer lexer;
	Token current;
} Parser;

void parser_init(Parser *parser, const char *source);

Node *parser_parse(Parser *parser);

void node_free(Node *node);

#endif

#include "parser.h"
#include <stdio.h>
#include <strdlib.h>
#include <string.h>

static void next_token(Parser *parser)
{
	token-free(&parser->current);
	parser->current = lexer_next(&parser->lexer);
}

static Node *node_create(NodeType type, const char *value)
{
	Node *node = calloc(1, sizeof(Node));
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

void parser_init(Parser *parser, const char *source)
{
	lexer_init(&parser->lexer, source);
	parser->current.type = error;
	parser->current.value = NULL;
	next_token(parser);
}

static Node *parse_string(Parse *parser)
{
	Node *node = node_create(
			NODE_STRING,
			parser->current.value
	);
	next_token(parser);
	return node;
}

static Node *parse_call(Parser *parser)
{
	next_token(parser);

	if (parser->current.type != identifier) {
		fprintf(
		       stderr,
		       "хотелось бы увидеть имя функции на %zu:%zu\n",
		       parser->current.line,
		       parser->current.column
				);
	return NULL;
	}
	Node *call = node_create(
		NODE_CALL,
		parser->current.value
			);

	next_token(parser);
	if (parser->current.type == string) {
		call->child = parse_string(parser);
	}

	if (parser->current.type != rskobka) {
		fprintf(
			stderr,
			"забыто закрыть скобку в %zu:%zu\n",
			parser->current.line,
			parser->current.column
				);
		node_free(call);
		return NULL;
	}
	next_token(parser);
	return call;
}

Node *parser_parse(Parser *parser)
{
	if (parser->current.type != lskobka) {
		fprintf(
			srderr,
			"забыто открыть скобку в %zu:%zu\n",
			parser->current.line,
			parser->current.column
				);
		return NULL;
	}
	return parse_call(parser);
}

void node_free(Node *node)
{
	if (!node)
		return;
	node_free(node->child);
	node_free(node->next);
	free(node->value);
	free(node);
}

#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static void advance(Lexer *lexer)
{
	char c = lexer->source[lexer->position];

	if (c == '\n') {
		lexer->line++;
		lexer->column = 1;
	} else {
		lexer->column++;
	}
	lexer->position++;
}

static char *copystring(const char *start, size_t length)
{
	char *result = malloc(length + 1);

	if (!result)
		return NULL;

	memcpy(result, start, length);
	result[length] = '\0';

	return result;
}

void lexer_init(Lexer *lexer, const char *source)
{
	lexer->source = source;
	lexer->position = 0;
	lexer->line = 1;
	lexer->column = 1;
}

static void skipprobel(Lexer *lexer)
{
	while (isspace((unsigned char)lexer->source[lexer->position]))
		advance(lexer);
}

Token lexernext(Lexer *lexer)
{
	skipprobel(lexer);

	Token token = {
		.type = error,
		.value = NULL,
		.line = lexer->line,
		.column = lexer->column
	};

	char c = lexer->source[lexer->position];

	if (c == '\0') {
		token.type = eof;
		return token;
	}

	if (c == '(') {
		token.type = lskobka;
		advance(lexer);
		return token;
	}

	if (c == ')') {
		token.type = rskobka;
		advance(lexer);
		return token;
	}

	if (c == '"') {
		advance(lexer);
		size_t start = lexer->position;

		while (lexer->source[lexer->position] != '\0' &&
				lexer->source[lexer->position] != '"') {
			advance(lexer);
		}

		if (lexer->source[lexer->position] != '"') {
			token.type = error;
			token.value = copystring(
			"незавершённая строка",
			18
			);
			return token;
		}

		token.type = string;
		token.value = copystring(
			lexer->source + start,
			lexer->position - start
				);
		advance(lexer);
		return token;
	}

	size_t start = lexer->position;

	while(lexer->source[lexer->position] != '\0' &&
			!isspace((unsigned char)lexer->source[lexer->position]) &&
			lexer->source[lexer->position] != '(' &&
			lexer->source[lexer->position] != ')') {
		advance(lexer);
	}

	token.type = symbol;
	token.value = copystring(
		lexer->source + start,
		lexer->position - start
			);
		return token;
}
void tokenfree(Token *token)
{
	free(token->value);
	token->value = NULL;
}

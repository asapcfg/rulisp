#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static char *copy_range(const char *start, size_t length)
{
	char *result = malloc(length + 1);
	if (!result)
		return NULL;
	memcpy(result, start, length);
	result[length] = '\0';
	return result;
}

static void advance(Lexer *lexer)
{
	if (lexer->source[lexer->position] == '\n') {
		lexer->line++;
		lexer->column = 1;
	} else {
		lexer->column++;
	}
	lexer->position++;
}

static void skip_whitespace(Lexer *lexer)
{
	while (isspace((unsigned char)lexer->source[lexer->position]))
		advance(lexer);
}

void lexer_init(Lexer *lexer, const char *source)
{
	lexer->source = source;
	lexer->position = 0;
	lexer->line = 1;
	lexer->column = 1;
}

Token lexer_next(Lexer *lexer)
{
	skip_whitespace(lexer);
	Token token = {
		.type = error,
		.value = NULL,
		.line = lexer->line,
		.column = lexer->column
	};
	
	char current = lexer->source[lexer->position];
	if (current == '\0') {
		token.type = eof;
		return token;
	}

	if (current == '(') {
		advance(lexer);
		token.type = lskobka;
		return token;
	}
	
	if (current == ')') {
		advance(lexer);
		token.type = rskobka;
		return token;
	}
	if (current == '"') {
		advance(lexer);
		size_t start = lexer->position;
		size_t length = 0;

		while (lexer->source[lexer->position] != '\0' &&
               		lexer->source[lexer->position] != '"') {
            	if (lexer->source[lexer->position] == '\\' &&
                	lexer->source[lexer->position + 1] != '\0') {
                	advance(lexer);
            	}

           	advance(lexer);
            	length++;
		}
		if(lexer->source[lexer->position] != '"') {
			token.type = error;
			token.value = copy_range(
			"незакрытая строка",
			strlen("незакрытая строка")
					);
			return token;
		}
		token.type = string;
		token.value = copy_range(
			lexer->source + start,
			length
		);
		advance(lexer);
		return token;
	}

	size_t start = lexer->position;

    	while (lexer->source[lexer->position] != '\0' &&
           	!isspace((unsigned char)lexer->source[lexer->position]) &&
           	lexer->source[lexer->position] != '(' &&
           	lexer->source[lexer->position] != ')') {
	        advance(lexer);
    	}

	token.type = identifier;
    	token.value = copy_range(
        	lexer->source + start,
        	lexer->position - start
    	);

    	return token;
	}

	void token_free(Token *token)
	{
	    free(token->value);
	    token->value = NULL;
	}


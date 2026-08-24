#ifndef lexer_h
#define lexer_h

#include <stddef.h>

typedef enum {
	lskobka,
	rskobka,
	identifier,
	string,
	eof,
	error
} TokenType;

typedef struct {
	TokenType type;
	char *value;
	size_t line;
	size_t column;
} Token;

typedef struct {
	const char *source;
	size_t position;
	size_t line;
	size_t column;
} Lexer;

void lexer_init(Lexer *lexer, const char *source);
Token lexer_next(Lexer *lexer);
void token_free(Token *token);

#endif

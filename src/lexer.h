#ifndef lexer_h
#define lexer_h

#include <stddef.h>
typedef enum {
	lskobka,
	rskobka,
	string,
	symbol,
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

void lexerinit(Lexer *lexer, const char *source);
Token lexernext(Lexer *lexer);
void tokenfree(Token *token);

#endif

#include <stdio.h>
#include "common.h"
#include "compiler.h"
#include "scanner.h";

typedef struct
{
	Token current;
	Token previous;
} Parser;

Parser parser;

bool compile(const char* source, Chunk* chunk)
{
	initScanner(source);
	advance();
	expression();
	consume(TOKEN_EOF, "Expected end of session");
}

static void advance() {
	parser.previous = parser.current;

	for (;;) {
		parser.current = scanToken();
		if (parser.current.type != TOKEN_ERROR) break;

		errorAtCurrent(parser.current.start);
	}
}

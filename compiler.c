#include <stdio.h>
#include "common.h"
#include "compiler.h"
#include "scanner.h";

bool compile(const char* source, Chunk* chunk)
{
	initScanner(source);
	advance();
	expression();
	consume(TOKEN_EOF, "Expected end of session");
}
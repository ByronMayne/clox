#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "compiler.h"
#include "scanner.h";

typedef struct
{
	Token current;
	bool hadError;
	bool inPanicMode;
	Token previous;
} Parser;

typedef enum {
	PREC_NONE,
	PREC_ASSIGNMENT,  // =        
	PREC_OR,          // or       
	PREC_AND,         // and      
	PREC_EQUALITY,    // == !=    
	PREC_COMPARISON,  // < > <= >=
	PREC_TERM,        // + -      
	PREC_FACTOR,      // * /      
	PREC_UNARY,       // ! - +    
	PREC_CALL,        // . () []  
	PREC_PRIMARY
} Precedence;

Parser parser;

Chunk* compilingChunk;

static Chunk* currentChunk()
{
	return compilingChunk;
}

static errorAt(Token* token, const char* message)
{
	if (parser.inPanicMode) return;
	parser.inPanicMode = true;
	fprintf(stderr, "[line %d] Error", token->line);

	if (token->type == TOKEN_EOF) {
		fprintf(stderr, " at end");
	}
	else if (token->type == TOKEN_ERROR) {
		// Nothing.                                                
	}
	else {
		fprintf(stderr, " at '%.*s'", token->length, token->start);
	}

	fprintf(stderr, ": %s\n", message);
	parser.hadError = true;
}

static errorAtCurrent(const char* message)
{
	errorAt(&parser.current, message);
}

static void advance() {
	parser.previous = parser.current;

	for (;;) {
		parser.current = scanToken();
		if (parser.current.type != TOKEN_ERROR) break;

		errorAtCurrent(parser.current.start);
	}
}

static void consume(TokenType type, const char* message)
{
	if (parser.current.type == type)
	{
		advance();
		return;
	}
	errorAtCurrent(message);
}

bool compile(const char* source, Chunk* chunk)
{
	initScanner(source);

	compilingChunk = chunk;
	parser.hadError = false;
	parser.inPanicMode = false;

	advance();
	expression();
	consume(TOKEN_EOF, "Expected end of session");
	endCompiler();
	return !parser.hadError;
}

static void emitByte(uint8_t byte)
{
	writeChunk(currentChunk(), byte, parser.previous.line);
}

static void emitBytes(uint8_t byte1, uint8_t byte2)
{
	emitByte(byte1);
	emitByte(byte2);
}

static void emitReturn()
{
	emitByte(OP_RETURN);
}

static uint8_t makeConstant(Value value)
{
	int constant = addConstant(currentChunk(), value);
	if (constant > UINT8_MAX)
	{
		error("Too many constants in one chunk.");
		return 0;
	}
	return (uint8_t)constant;
}

static void emitConstant(Value value)
{
	emitBytes(OP_CONSTANT, makeConstant(value));
}

static void endCompiler()
{
	emitReturn();
}

static void binary()
{
	TokenType operatorType = parser.previous.type;

	ParseRule* rule = getRule(operatorType);
	parsePrecedence((Precedence)rule->precedence + 1));

	// Emit the operator instruction.                        
	switch (operatorType) 
	{
		case TOKEN_PLUS:   emitByte(OP_ADD); break;
		case TOKEN_MINUS:  emitByte(OP_SUBTRACT); break;
		case TOKEN_STAR:   emitByte(OP_MULTIPLY); break;
		case TOKEN_SLASH:  emitByte(OP_DIVIDE); break;
		default:
			return; // Unreachable.                              
	}
}

static void grouping()
{
	expression();
	consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void number()
{
	double value = strtod(parser.previous.start, NULL);
	emitConstant(value);
}

static void unary()
{
	TokenType operatorType = parser.previous.type;

	parsePrecedence(PREC_UNARY);

	switch (operatorType)
	{
		case TOKEN_MINUS: emitByte(OP_NEGATE); break;
		default:
			return; // Unreachable.
	}
}

static void parsePrecedence(Precedence precedence) {
	// What goes here?                                
}

void expression()
{
	parsePrecedence(PREC_ASSIGNMENT);
}
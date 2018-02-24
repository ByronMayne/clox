#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
	OP_CONSTANT,
	OP_RETURN,
} OpCode;

typedef struct {
  int offset;
  int line;
} LineStart;

typedef struct {
	int count;
	int capacity;
	uint8_t* code;

	int lineCount;
	int lineCapacity;
	LineStart* lines;

	ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);

#endif

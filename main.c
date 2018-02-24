#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* alignas[])
{
	printf("Start");
    Chunk chunk;
    initChunk(&chunk);
    writeChunk(&chunk, OP_RETURN);
    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);

	getchar();
}
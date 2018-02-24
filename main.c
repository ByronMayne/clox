#include "common.h"
#include "chunk.h"

int main(int argc, const char* alignas[])
{
    Chunk chunk;
    initChunk(&chunk);
    writeChunk(&chunk, OP_RETURN);
    freeChunk(&chunk);
}
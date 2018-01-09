#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

void asmcpy(char *dest, char *src, unsigned int count) {
	_asm{
		mov edi, dest
		mov esi, src
		mov ecx, count
		rep movsb
	}
}

void asmcpy2(char *dest, char *src, unsigned int count) {
	_asm{
		mov edi, dest
		mov esi, src
		mov ecx, count
		shr ecx, 1
		rep movsw
	}
}

void asmcpy4(char *dest, char *src, unsigned int count) {
	_asm{
		mov edi, dest
		mov esi, src
		mov ecx, count
		shr ecx, 2
		rep movsd
	}
}

void multiCopy(char *dest, char *src, unsigned int count, char mode) {
	switch (mode) {
		case 0:
			memcpy(dest, src, count);
			break;
		case 1:
			asmcpy(dest, src, count);
			break;
		case 2:
			asmcpy2(dest, src, count);
			break;
		case 3:
			asmcpy4(dest, src, count);
			break;
		default:
			break;
	}
};

void multiStore(char *dest, unsigned char data, unsigned int count, char mode) {
	switch (mode) {
		case 0:
			memset(dest, data, count);
			break;
		case 1:
			break;
		default:
			break;
	}
};

#define _CSV

void testMoves(char  *dest, char *source, size_t size) {
	const unsigned int N_FLIPS = 1000; 
	clock_t start, stop;
	char mode;

	for (mode = 0; mode < 4; ++mode) {
		unsigned int i;

#ifndef _CSV
		printf("Test mode %d\n", mode);
		printf("%d block moves of %d bytes... ", N_FLIPS, size);
#endif

		start = clock();
		for (i = N_FLIPS; i > 0; --i) {
			multiCopy(dest, source, size, mode);
		}
		stop = clock();
#ifndef _CSV
		printf("%.3f seconds\n", (float)(stop - start) / CLOCKS_PER_SEC);
#else
		printf("%.3f", (float)(stop - start) / CLOCKS_PER_SEC);
		if (mode != 3)
			printf(", ");
#endif

	}
}

const unsigned int BLOCK_SIZE = 64 * 1024;

int _tmain(int argc, _TCHAR* argv[])
{
	char *uniBlock = (char*)malloc(BLOCK_SIZE * 4);
	char *block1 = uniBlock; //(char*) malloc(BLOCK_SIZE);
	char *block2 = uniBlock + BLOCK_SIZE ; //(char*) malloc(BLOCK_SIZE);
	char *block3 = (char*) malloc(512 * 512);

	unsigned int offset = 0;

	printf("CLOCKS_PER_SEC = %d\n", CLOCKS_PER_SEC);
	
	printf("offset, memcpy, asmcpy, asmcpy2, asmcpy4\n");

	for (offset = 0; offset < BLOCK_SIZE; ++offset) {
		block2 = block1 + BLOCK_SIZE + offset;
		printf("%d, ", offset);
		testMoves(block2, block1, BLOCK_SIZE);
		printf("\n");
	}

	printf("Press a key...");
	getch();

	free(uniBlock);
	//free(block1);
	//free(block2);
	free(block3);
	return 0;
}

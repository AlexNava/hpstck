#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

unsigned int lev = 0;

void recur()
{
	// 800, 1004 = 1KiB minus what VS2008 reserves in debug and release config
	#ifdef _DEBUG
	unsigned char buf[800] = "Puppami la fava in debug"; // padded with zeros to size
	#else
	unsigned char buf[1004] = "Puppami la fava in release"; // padded with zeros to size
	#endif

	printf("Approx %d KiB on stack (buf at 0x%08x)\n", ++lev, &buf[0]);
	recur();
} // this will produce a stack overflow BY DESIGN :)

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned char *hpBuf;
	unsigned int i = 0;

	while (1)
	{
		hpBuf = (char*) malloc(1024);
		memset(hpBuf, 0xab, 1024);
		if (hpBuf)
		{
			//printf("Malloc'ed %d KiB (buf at 0x%x)\n", ++i, hpBuf);
			++i;
			//free(hpBuf);
		}
		else
		{
			printf("Malloc FAIL after %d KiB\n", i);
			break;
		}
	}

	printf("Press a key\n");
	_getch();

	recur();
	return 0;
}


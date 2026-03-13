#include <3ds.h>
#include <stdio.h>

int main(int argc, char **argv) {
	gfxInitDefault();

	PrintConsole topScreen, bottomScreen;

	consoleInit(GFX_TOP, &topScreen);
	consoleInit(GFX_BOTTOM, &bottomScreen);

	consoleSelect(&topScreen);
	printf("This is the top screen. We have lot of space here!\n");

	consoleSelect(&bottomScreen);
	printf("This is the bottom screen. We don't have much space...\nStill better than DS screens :D");

	consoleSelect(&topScreen);
	printf("\x1b[30;16HPress Start to exit.");

	while (aptMainLoop())
	{
		hidScanInput();

		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) break;

		gfxFlushBuffers();
		gfxSwapBuffers();

		gspWaitForVBlank();
	}

	gfxExit();

	return 0;
}

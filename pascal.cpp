#include <windows.h>
//#include<stdio.h>
#include "tiff.h"
#include "PrintFontData.h"

using namespace AV_Tiff;

void fun(int);

int out_tif_test(void) {

	CTiff fp2;
	char *pBuf;
	char fn2[] = "out_test.tif";

	fp2.CreateNew(192, 24, 72, 72, 1, 1, 0);

	pBuf = (char *)fp2.m_lpImageBuf;
	memcpy(pBuf, pPrintFontData, FONT_WIDTH *6 / 8 * FONT_LENGTH);
	fp2.SaveFile(fn2);

	return 0;
}

int main(void) {	
	
	//out_tif();
	fun(100);
	
	
	return 0;
}

void fun(int n) {
	CTiff fp2;
	char *pBuf, *pXpos;
	char fn2[] = "out.tif";
	int buf[1000] = { 1 };
	int i, j, k;
	int width = (n * FONT_WIDTH * 3 / 2 + FONT_WIDTH *2) / 8 * 8;
	int height = n * FONT_LENGTH;

	fp2.CreateNew(width, height, 72, 72, 1, 1, 0);

	pBuf = (char *)fp2.m_lpImageBuf;
	memset(pBuf, 0, width / 8 * height);

	for (i = 0; i<n; i++) {
		pXpos = pBuf;
		for (j = i; j>0; j--) buf[j] = buf[j] + buf[j - 1];
		for (j = 0; j<n - i; j++)printf(" ");
		pXpos += (n - i) * 3;
		for (j = 0; j <= i; j++)
		{
			buf[j] %= 6;
			if (buf[j] == 0)
				printf("@ ");
			else
				printf("%1d ", buf[j]);
			for (k = 0; k<FONT_LENGTH; k++)
				memcpy(pXpos + k * width / 8, pPrintFontData + k * 24 + buf[j] * 4, 4);

			pXpos += 6;
		}
		pBuf += (width / 8 * FONT_LENGTH);
		printf("\n");
	}
	fp2.SaveFile((LPCTSTR)fn2);
}


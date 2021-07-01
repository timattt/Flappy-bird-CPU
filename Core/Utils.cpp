#include "Core.hpp"

int loadTexture(char *path, tex_t * result) {
	HBITMAP bm = (HBITMAP) LoadImageW(NULL, (LPCWSTR)path,
	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (bm == NULL) {
		printf("bmp load error!\n");
		exit(-1);
	}

	BITMAP bitmap = {0};
	GetObject(bm, sizeof(bitmap), &bitmap);

	HDC hdcMem = CreateCompatibleDC(NULL//GetDC(hwnd)
	);

	SelectObject(hdcMem, bm);

	int width = bitmap.bmWidth;
	int height = bitmap.bmHeight;
	char * pixels = (char*)malloc(4 * width * height);
	memset(pixels, 0, 4 * width * height);

	BITMAPINFO bmpinfo = { 0 };

	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmpinfo.bmiHeader.biWidth = width;
	bmpinfo.bmiHeader.biHeight = height;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 32;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = 0;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	GetDIBits(hdcMem, bm, 0, height, pixels, &bmpinfo, DIB_PAL_COLORS);

	result = (tex_t*) calloc(1, sizeof(tex_t));
	result->buf = pixels;

	DeleteDC(hdcMem);
	DeleteObject(bm);
	return 0;
}
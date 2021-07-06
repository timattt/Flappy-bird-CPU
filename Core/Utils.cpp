#include "Core.hpp"

int errcode = 0;
char errorMessage[ERROR_MESSAGE_LEN] = { 0 };

int drawRect_char4(uint32_t dest[SCREEN_HEIGHT][SCREEN_WIDTH], vec2f_t pos, vec2f_t dim, char r, char g, char b, char a) {
	NOT_NULL(dest);

	int x = pos.x;
	int y = pos.y;
	int w = dim.x;
	int h = dim.y;

	for (int x1 = 0; x1 < w; x1++) {
		for (int y1 = 0; y1 < h; y1++) {
			if (0 <= (x + x1) && (x + x1) < SCREEN_WIDTH && 0 <= (y + y1) && (y + y1) < SCREEN_HEIGHT && w > 0 && h > 0) {
				dest[y + y1][x + x1] = (((uint32_t)r) << 24) + (((uint32_t) g) << 16) + (((uint32_t) b) << 8) + (uint32_t) a;
			}
		}
	}

	return 0;
}

int drawRect_int(uint32_t dest[SCREEN_HEIGHT][SCREEN_WIDTH], vec2f_t pos, vec2f_t dim, uint32_t col) {
	NOT_NULL(dest);

	int x = pos.x;
	int y = pos.y;
	int w = dim.x;
	int h = dim.y;

	for (int x1 = 0; x1 < w; x1++) {
		for (int y1 = 0; y1 < h; y1++) {
			if (0 <= (x + x1) && (x + x1) < SCREEN_WIDTH && 0 <= (y + y1) && (y + y1) < SCREEN_HEIGHT && w > 0 && h > 0) {
				dest[y + y1][x + x1] = col;
			}
		}
	}

	return 0;
}

void print_bin(uint32_t v) {
	for (int i = 0; i < 32; i++) {
		if (v & 1)
			printf("1");
		else
			printf("0");

		v >>= 1;
	}
	printf("\n");
}

int cycle_bit_move_left(int x, int k) {
	return (x >> k) | (x << (sizeof(x) * 8 - k));
}

bool aabb_intersects(vec2f_t a1, vec2f_t b1, vec2f_t a2, vec2f_t b2) {
	 return (a1.x <= a2.x + b2.x && a1.x + b1.x >= a2.x) &&
	        (a1.y <= a2.y + b2.y && a1.y + b1.y >= a2.y);
}

int min(int a, int b) {
	if (a < b) {
		return a;
	} else {
		return b;
	}
}

int max(int a, int b) {
	if (a < b) {
		return b;
	} else {
		return a;
	}
}

int drawTexture(tex_t * tex, uint32_t dest[SCREEN_HEIGHT][SCREEN_WIDTH],  vec2f_t pos, vec2f_t dim) {
	NOT_NULL(dest);
	NOT_NULL(tex);

	int x = pos.x;
	int y = pos.y;
	int w = dim.x;
	int h = dim.y;

	for (int x1 = 0; x1 < w; x1++) {
		for (int y1 = 0; y1 < h; y1++) {
			if (0 <= (x + x1) && (x + x1) < SCREEN_WIDTH && 0 <= (y + y1) && (y + y1) < SCREEN_HEIGHT && w > 0 && h > 0) {
				int tx = (float)x1 / (float)w * (float) tex->width;
				int ty = (1.0f - (float)y1 / (float)h) * (float) tex->height;

				tx = max(0, min(tx, tex->width - 1));
				ty = max(0, min(ty, tex->height - 1));

				uint32_t val = ((uint32_t*) tex->buf) [ty * tex->width + tx];

				// Transparency
				if (val != 0x00000000) {
					dest[y + y1][x + x1] = val;
				}
			}
		}
	}

	return 0;
}

int PrintErrorMessage() {
	if (strlen(errorMessage) > 0) {
		printf("%s", errorMessage);
	} else {
		printf("No error is found!\n");
	}
	fflush(stdout);
	return 0;
}

int loadTexture(const char *path, tex_t * result) {
	NOT_NULL(path);
	NOT_NULL(result);

	HBITMAP bm = (HBITMAP) LoadImageW(NULL, (LPCWSTR)path,
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	ENSURE_M(bm != NULL, "bmp load error");

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

	result->buf = pixels;
	result->width = width;
	result->height = height;

	DeleteDC(hdcMem);
	DeleteObject(bm);
	return 0;
}

int cleanupTexture(tex_t * t) {
	NOT_NULL(t);

	free(t->buf);

	return 0;
}

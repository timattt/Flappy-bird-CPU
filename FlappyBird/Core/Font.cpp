#include "Core.hpp"

int loadFont(const wchar_t * fileName, font_t * res) {
	NOT_NULL(fileName);
	NOT_NULL(res);

	wchar_t name[FONR_IMAGE_FILE_LENGTH] = {0};

	for (int i = 0; i < FONT_IMAGES_COUNT; i++) {
		memset(name, 0, FONR_IMAGE_FILE_LENGTH);
		swprintf(name, L"%ls/%d.bmp", fileName, i);
		loadTexture(name, res->numbers + i);
	}

	return 0;
}

int drawNumberInBox(uint32_t dest[SCREEN_HEIGHT][SCREEN_WIDTH], vec2f_t pos, vec2f_t dim, float step, int num, font_t * font) {
	NOT_NULL(dest);
	NOT_NULL(font);

	char str[FONR_IMAGE_FILE_LENGTH] = {0};
	sprintf(str, "%d", num);
	int sz = strlen(str);

	vec2f_t charDim = {dim.x / sz, dim.y};

	for (int i = 0; i < sz; i++) {
		drawTexture(font->numbers + (str[i] - '0'), dest, {pos.x + charDim.x * i, pos.y}, {charDim.x - step, charDim.y});
	}

	return 0;
}

int drawNumberFree(uint32_t dest[SCREEN_HEIGHT][SCREEN_WIDTH], vec2f_t pos, float height, float step, int num, font_t * font) {
	NOT_NULL(dest);
	NOT_NULL(font);

	char str[FONR_IMAGE_FILE_LENGTH] = {0};
	sprintf(str, "%d", num);
	int sz = strlen(str);

	float char_width = height / 2 + step;

	vec2f_t charDim = {char_width, height};

	for (int i = 0; i < sz; i++) {
		drawTexture(font->numbers + (str[i] - '0'), dest, {pos.x + charDim.x * i, pos.y}, {charDim.x - step, charDim.y});
	}

	return 0;
}

int setFontColor(font_t * font, int col) {
	NOT_NULL(font);

	for (int i = 0; i < FONT_IMAGES_COUNT; i++) {
		int w = font->numbers[i].width;
		int h = font->numbers[i].height;

		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				if (((uint32_t*) font->numbers[i].buf) [y * w + x] == 0x00FFFFFF) {
					((uint32_t*) font->numbers[i].buf) [y * w + x] = col;
				}
			}
		}

	}

	return 0;
}

int cleanupFont(font_t * res) {
	NOT_NULL(res);

	for (int i = 0; i < FONT_IMAGES_COUNT; i++) {
		cleanupTexture(res->numbers + i);
	}

	return 0;
}

#include "Core.hpp"

// Local fields
//================================================
// Game states
int currentGameState = STATE_START_MENU;

// Bird
struct bird actor = { 0 };

// Columns
float column_gen_interval = 0;
float current_columns_speed = COLUMNS_START_SPEED;
column columns[MAX_COLUMNS];
int total_columns = 0;

// Menus
menu startMenu = { 0 };
menu gameOverMenu = { 0 };

// Background
tex_t backGroundTexture = {0};

// FPS
#ifdef DEBUG

float timeFromLastFpsPrint = 0;
int currentFpsCount = 0;

#endif
//================================================


// Columns control
//================================================
void deleteAllColumns() {
	memset(columns, 0, MAX_COLUMNS * sizeof(column));
}

void refreshColumns() {
	column tmp[MAX_COLUMNS] = { 0 };
	int ind = 0;
	for (int i = 0; i < MAX_COLUMNS; i++) {
		if (columns[i].splits_y_count != 0) {
			tmp[ind] = columns[i];
			ind++;
		}
	}

	memcpy(columns, tmp, MAX_COLUMNS * sizeof(column));
	total_columns = ind;
}

void addColumn(column col) {
	columns[total_columns] = col;
	total_columns++;
}

void createColumn() {
	column col = {0};

	// Bottom
	col.seed = rand();
	col.dim.x = SCREEN_WIDTH / 20 * (1 + (rand() % 3));
	col.dim.y = SCREEN_HEIGHT / 20 * (rand() % 15);
	col.splits_y_count = col.dim.y / (col.dim.x / (float)TOTAL_COLUMN_TEXTURE_SPLITS_COUNT);
	col.dim.y = col.splits_y_count * col.dim.x / TOTAL_COLUMN_TEXTURE_SPLITS_COUNT;
	col.pos.x = SCREEN_WIDTH;
	col.pos.y = SCREEN_HEIGHT - col.dim.y;

	if (0 < col.splits_y_count && col.splits_y_count < 100) {
		addColumn(col);
	}

	// Top
	col.dim.y = (SCREEN_HEIGHT - col.dim.y) / 20.0f * (float)(rand() % 10);
	col.splits_y_count = col.dim.y / (col.dim.x / TOTAL_COLUMN_TEXTURE_SPLITS_COUNT);
	col.dim.y = col.splits_y_count * col.dim.x / TOTAL_COLUMN_TEXTURE_SPLITS_COUNT;
	col.pos.x = SCREEN_WIDTH;
	col.pos.y = 0;
	col.seed = rand();

	if (0 < col.splits_y_count && col.splits_y_count < 100) {
		addColumn(col);
	}
}
//================================================


// Buttons
//================================================
bool isCursorInsideButton() {
	return (MENU_MINX < get_cursor_x() && get_cursor_x() < MENU_MAXX && MENU_BUTTON_MINY < get_cursor_y() && get_cursor_y() < MENU_MAXY);
}
//================================================


// Init functions
//================================================
void restart() {
	actor.speed = { 0 };
	actor.killed = 0;
	actor.jumping_time = 0;
	actor.pos = {SCREEN_WIDTH / 5, SCREEN_HEIGHT / 2};
	deleteAllColumns();
	current_columns_speed = COLUMNS_START_SPEED;
}

// initialize game data in this function
void initialize() {
	restart();
	loadTexture((const char*)(L"Resources/Bird.bmp"), &actor.bird_texture);
	loadTexture((const char*)(L"Resources/GameOverMenu2.bmp"), &gameOverMenu.button_pressed);
	loadTexture((const char*)(L"Resources/StartMenu2.bmp"), &startMenu.button_pressed);
	loadTexture((const char*)(L"Resources/GameOverMenu1.bmp"), &gameOverMenu.button_not_pressed);
	loadTexture((const char*)(L"Resources/StartMenu1.bmp"), &startMenu.button_not_pressed);
	loadTexture((const char*)(L"Resources/BackGround.bmp"), &backGroundTexture);
}
//================================================


// Draw functions
//================================================
void drawBackGround() {
	drawTexture(&backGroundTexture, buffer, {0, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT});
}

void drawActor() {
	drawTexture(&actor.bird_texture, buffer, {actor.pos.x, actor.pos.y}, {BIRD_WIDTH, BIRD_HEIGHT});
}

void drawMenu() {
	menu * cur = NULL;
	if (currentGameState == STATE_START_MENU) {
		cur = &startMenu;
	}
	if (currentGameState == STATE_GAME_OVER_MENU) {
		cur = &gameOverMenu;
	}

	if (cur != NULL) {
		vec2f_t pos = { MENU_MINX, MENU_MINY };
		vec2f_t dim = { MENU_MAXX - MENU_MINX, MENU_MAXY - MENU_MINY };

		vec2f_t pose = { MENU_MINX - EPSILON, MENU_MINY - EPSILON };
		vec2f_t dime = { MENU_MAXX - MENU_MINX + 2 * EPSILON, MENU_MAXY - MENU_MINY + 2 * EPSILON };

		drawRect_int(buffer, pose, dime, 0xFFFFFFFF);

		if (isCursorInsideButton()) {
			drawTexture(&cur->button_pressed, buffer, pos, dim);
		} else {
			drawTexture(&cur->button_not_pressed, buffer, pos, dim);
		}
	}
}

void drawColumns() {
	for (int i = 0; i < total_columns; i++) {
		column col = columns[i];
		if (col.splits_y_count == 0) {
			continue;
		}
		float dx = col.dim.x / TOTAL_COLUMN_TEXTURE_SPLITS_COUNT;
		float dy = col.dim.y / col.splits_y_count;

		drawRect_int(buffer, {col.pos.x - 2 * EPSILON, col.pos.y - 2 * EPSILON}, {col.dim.x + 4 * EPSILON, col.dim.y + 4 * EPSILON}, 0x0);

		for (int x = 0; x < TOTAL_COLUMN_TEXTURE_SPLITS_COUNT; x++) {
			for (int y = 0; y < col.splits_y_count; y++) {
				drawRect_int(buffer, {col.pos.x + x * dx, col.pos.y + y * dy}, {dx + EPSILON, dy + EPSILON}, cycle_bit_move_left(col.seed, x + y) | 0x0F0F0F0F);
			}
		}
	}
}
//================================================


// Update functions
//================================================
void updateMenu() {
	if (isCursorInsideButton() && is_mouse_button_pressed(0) && currentGameState != STATE_GAME) {
		currentGameState = STATE_GAME;
		restart();
	}
}

void updateColumns(float dt) {
	if (currentGameState != STATE_GAME) {
		return;
	}

	float gen_interval = COLUMNS_START_GENERATION_TIME_INTERVAL * COLUMNS_START_SPEED / current_columns_speed;

	// Creating
	if (column_gen_interval > gen_interval) {
		column_gen_interval = 0;
		createColumn();
	} else {
		column_gen_interval += dt;
	}

	// Updating
	for (int i = 0; i < total_columns; i++) {
		column col = columns[i];
		if (col.splits_y_count == 0) {
			continue;
		}
		if (col.pos.x + col.dim.x < 0.0f) {
			columns[i] = {0};
			continue;
		} else {
			col.pos.x -= current_columns_speed * dt;
			columns[i] = col;
		}

		if (aabb_intersects(col.pos, col.dim, actor.pos, {BIRD_WIDTH, BIRD_HEIGHT})) {
			actor.killed = 1;
			current_columns_speed = 0;
			break;
		}
	}

	if (total_columns + 5 > MAX_COLUMNS) {
		refreshColumns();
	}

	current_columns_speed += COLUMNS_ACCELERATION * dt;
}

void updateActor(float dt) {
	if (currentGameState == STATE_GAME) {

		// Acceleration
		actor.speed.y += G * dt;

		// Jumps
		if (actor.jumping_time > 0) {
			actor.jumping_time += dt;
		}
		if (is_key_pressed(VK_SPACE) && actor.jumping_time == 0
				&& !actor.killed) {
			actor.speed.y = -BIRD_JUMP_SPEED;
			actor.jumping_time += dt;
		}
		if (actor.jumping_time >= BIRD_JUMP_INTERVAL) {
			actor.jumping_time = 0;
		}

		// Speed
		actor.pos.x += actor.speed.x * dt;
		actor.pos.y += actor.speed.y * dt;

		if (actor.pos.y > SCREEN_HEIGHT) {
			currentGameState = STATE_GAME_OVER_MENU;
		}
		if (actor.pos.y < 0) {
			actor.killed = 1;
		}
	} else {
		actor.pos = {-SCREEN_WIDTH, - SCREEN_HEIGHT};
	}
}
//================================================


// Engine functions
//================================================
// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt) {
	if (is_key_pressed(VK_ESCAPE)) {
		schedule_quit_game();
	}

	updateMenu();
	updateActor(dt);
	updateColumns(dt);

#ifdef DEBUG
	// FPS
	currentFpsCount++;
	timeFromLastFpsPrint += dt;
	if (timeFromLastFpsPrint >= 1.0f) {
		timeFromLastFpsPrint = 0;
		printf("FPS: %d, Columns: %d\n", currentFpsCount, total_columns);
		fflush(stdout);
		currentFpsCount = 0;
	}
#endif
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw() {
	// clear backbuffer
	memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

	drawBackGround();
	drawColumns();
	drawActor();
	drawMenu();
}

// free game data in this function
void finalize() {
	cleanupTexture(&actor.bird_texture);
	cleanupTexture(&startMenu.button_pressed);
	cleanupTexture(&gameOverMenu.button_pressed);
	cleanupTexture(&startMenu.button_not_pressed);
	cleanupTexture(&gameOverMenu.button_not_pressed);
	cleanupTexture(&backGroundTexture);
}
//================================================


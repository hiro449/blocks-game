
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>


#define FIELD_HEIGHT 8
#define FIELD_WIDTH 8

#define BLOCK_TYPE_MAX 7

enum {
	CELL_TYPE_NONE,
	CELL_TYPE_BLOCK_0,
	CELL_TYPE_BLOCK_1,
	CELL_TYPE_BLOCK_2,
	CELL_TYPE_BLOCK_3,
	CELL_TYPE_BLOCK_4,
	CELL_TYPE_BLOCK_5,
	CELL_TYPE_BLOCK_6,
	CELL_TYPE_MAX
};

char cellAA[][2 + 1] = {
	"・",//CELL_TYPE_NONE,
	"〇",//CELL_TYPE_BLOCK_0,
	"△",//CELL_TYPE_BLOCK_1,
	"□",//CELL_TYPE_BLOCK_2,
	"●",//CELL_TYPE_BLOCK_3,
	"▲",//CELL_TYPE_BLOCK_4,
	"■",//CELL_TYPE_BLOCK_5,
	"☆",//CELL_TYPE_BLOCK_6,

};

int CursorX, CursorY;

int cells[FIELD_HEIGHT][FIELD_WIDTH];

int check[FIELD_HEIGHT][FIELD_WIDTH];

int getConnectCount(int _x, int _y, int  CELL_TYPE, int count) {
	if (
		(_x<0) || (_x >= FIELD_WIDTH) || (_y<0) || (_y >= FIELD_HEIGHT)
		|| check[_y][_x] || (cells[_y][_x] == CELL_TYPE_NONE)
		|| (cells[_y][_x] != CELL_TYPE)

		) 
		return count;
	count++;
	check[_y][_x] = true;

	count = getConnectCount(_x, _y-1, CELL_TYPE, count);
	count = getConnectCount(_x-1, _y, CELL_TYPE, count);
	count = getConnectCount(_x, _y+1, CELL_TYPE, count);
	count = getConnectCount(_x+1, _y, CELL_TYPE, count);
}

void eraseBlocks(int _x, int _y, int  CELL_TYPE) {
	if (
		(_x < 0) || (_x >= FIELD_WIDTH) || (_y < 0) || (_y >= FIELD_HEIGHT)
		|| (cells[_y][_x] == CELL_TYPE_NONE)
		|| (cells[_y][_x] != CELL_TYPE)

		)
		return;

	cells[_y][_x] = CELL_TYPE_NONE;
	eraseBlocks(_x, _y-1, CELL_TYPE);
	eraseBlocks(_x-1, _y, CELL_TYPE);
	eraseBlocks(_x, _y+1, CELL_TYPE);
	eraseBlocks(_x+1, _y, CELL_TYPE);
}

bool lock = true;


int selectedX = -1, selectedY = -1;

void eraseBlocksAll() {
	memset(check, 0, sizeof check);
	for (int y = 0; y < FIELD_HEIGHT; y++)
		for (int x = 0; x < FIELD_WIDTH; x++) {
			int n = getConnectCount(x, y, cells[y][x], 0);
			if (n >= 3) {
				eraseBlocks(x, y, cells[y][x]);
				lock = true;
			}
		}
}

void display() {
	system("cls");
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++)
			if ((x == CursorX) && (y == CursorY) && (!lock))
				printf("◎");
			else
				printf("%s", cellAA[cells[y][x]]);

		if (y == selectedY)
			printf("←");
		printf("\n");

	}

	for (int x = 0; x < FIELD_WIDTH; x++)
		printf("%s", (x == selectedX) ? "↑" : "　");
	//ブロックが落ちてくるときfallの文字を表示
	if (lock) {
		printf("fall");
	}

}

int main() {
	srand((unsigned int)time(NULL));

	for (int y = 0; y < FIELD_HEIGHT; y++)
		for (int x = 0; x < FIELD_WIDTH; x++)
			cells[y][x] = CELL_TYPE_BLOCK_0 + rand() % BLOCK_TYPE_MAX;

	time_t t = time(NULL);


	eraseBlocksAll();

	while (1) {

		if (t < time(NULL)) {
			t = time(NULL);

			if (lock) {
				lock = false;
				for (int y = FIELD_HEIGHT - 2; y >= 0; y--)
					for (int x = 0; x < FIELD_WIDTH; x++)
						if ((cells[y][x] != CELL_TYPE_NONE)
							&& cells[y + 1][x] == CELL_TYPE_NONE) {
							cells[y + 1][x] = cells[y][x];
							cells[y][x] = CELL_TYPE_NONE;
							lock = true;
						}
				for (int x = 0; x < FIELD_WIDTH; x++)
					if (cells[0][x] == CELL_TYPE_NONE) {
						cells[0][x] = CELL_TYPE_BLOCK_0 + rand() % BLOCK_TYPE_MAX;
						lock = true;
					}

			}

			display();

		}

		if (_kbhit()) {
			if (lock) {
				_getch();
			}
			else {
				//ブロックを移動したときブロックをそれぞれ消す処理
				switch (_getch()) {
				case 'w': CursorY--; if (CursorY--) {
					cells[CursorY + 1][CursorX] = CELL_TYPE_NONE;
					CursorY++;
				}
					break;
				case 's': CursorY++; 	if (CursorY++) {
					cells[CursorY - 1][CursorX] = CELL_TYPE_NONE;
					CursorY--;
				}
						break;
				case 'a': CursorX--; if (CursorX--) {
					cells[CursorY][CursorX + 1] = CELL_TYPE_NONE;
					CursorX++;
				}
					break;
				case 'd': CursorX++; if (CursorX++) {
					cells[CursorY][CursorX - 1] = CELL_TYPE_NONE;
					CursorX--;
					break;
				}
				default:
					if (selectedX < 0) {
						selectedX = CursorX;
						selectedY = CursorY;
					}
					else {
						int distance = abs(selectedX - CursorX) + abs(selectedY - CursorY);
						if (distance == 1) {
							int temp = cells[CursorY][CursorX];
							cells[CursorY][CursorX] = cells[selectedY][selectedX];
							cells[selectedY][selectedX] = temp;

							eraseBlocksAll();

							selectedX = selectedY = -1;

							lock = true;
						}
						else {
							printf("\a");
						}
					}

					break;
				}
						display();
				}
			}

		}
	}



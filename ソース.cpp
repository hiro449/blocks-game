
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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
	"ÅE",//CELL_TYPE_NONE,
	"ÅZ",//CELL_TYPE_BLOCK_0,
	"Å¢",//CELL_TYPE_BLOCK_1,
	"Å†",//CELL_TYPE_BLOCK_2,
	"Åú",//CELL_TYPE_BLOCK_3,
	"Å£",//CELL_TYPE_BLOCK_4,
	"Å°",//CELL_TYPE_BLOCK_5,
	"Åô",//CELL_TYPE_BLOCK_6,

};


int cells[FIELD_HEIGHT][FIELD_WIDTH];

int CursorX, CursorY;

int selectedX = -1, selectedY = -1;

int check[FIELD_HEIGHT][FIELD_WIDTH];

bool lock = false;

int getConnectCount(int _x, int _y, int CellType, int count) {
	if (
		(_x < 0) || (_x >= FIELD_WIDTH)
		|| (_y < 0) || (_y >= FIELD_HEIGHT)
		|| (cells[_y][_x] == CELL_TYPE_NONE)
		|| check[_y][_x]
		|| (cells[_y][_x] != CellType)
		) 
		return count;
	count++;
	check[_y][_x] = true;

	count = getConnectCount(_x, _y-1, CellType, count);
	count = getConnectCount(_x-1, _y, CellType, count);
	count = getConnectCount(_x, _y+1, CellType, count);
	count = getConnectCount(_x+1, _y, CellType, count);
}

void eraseBlocks(int _x, int _y, int  CellType) {
	if (
		(_x < 0) || (_x >= FIELD_WIDTH) || (_y < 0) || (_y >= FIELD_HEIGHT)
		|| (cells[_y][_x] == CELL_TYPE_NONE)
		|| (cells[_y][_x] != CellType)

		)
		return;

	cells[_y][_x] = CELL_TYPE_NONE;
	eraseBlocks(_x, _y - 1, CellType);
	eraseBlocks(_x - 1, _y, CellType);
	eraseBlocks(_x, _y + 1, CellType);
	eraseBlocks(_x + 1, _y, CellType);

}

void display() {
	system("cls");

	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++)
			if ((x == CursorX) && (y == CursorY) && (!lock))
				printf("Åù");
			else
				printf("%s", cellAA[cells[y][x]]);

		if (y == selectedY)
			printf("Å©");

		printf("\n");

	}


	for (int x = 0; x < FIELD_WIDTH; x++)
		printf("%s", (x == selectedX) ? "Å™" : "Å@");
}

int main() {

	srand((unsigned int)time(NULL));

	for (int y = 0; y < FIELD_HEIGHT; y++)
		for (int x = 0; x < FIELD_WIDTH; x++)
			cells[y][x] = CELL_TYPE_BLOCK_0 + rand() % BLOCK_TYPE_MAX;


	time_t t = time(NULL);

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
			if (lock)
				_getch();
			else {

				switch (_getch()) {
				case 'w': CursorY--; break;
				case 's': CursorY++; break;
				case 'a': CursorX--; break;
				case 'd': CursorX++; break;

				default:

					if (selectedX < 0) {
						selectedX = CursorX;
						selectedY = CursorY;
					}
					else {
						int distance = abs(CursorX - selectedX) + abs(CursorY - selectedY);
						if (distance == 1) {
							int temp = cells[CursorY][CursorX];
							cells[CursorY][CursorX] = cells[selectedY][selectedX];
							cells[selectedY][selectedX] = temp;

							memset(check, 0, sizeof check);
							for (int y = 0; y < FIELD_HEIGHT; y++)
								for (int x = 0; x < FIELD_WIDTH; x++) {
									int n = getConnectCount(x, y, cells[y][x], 0);
									if (n >= 3) {
										eraseBlocks(x, y, cells[y][x]);
									}
								}
							selectedX = -1, selectedY = -1;
							lock = true;
						}
						else
							printf("\a");
					}
					break;
				}
				display();
			}
		}
	}

}



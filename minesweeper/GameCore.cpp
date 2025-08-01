#include "Dxlib.h"
#include "Constants.h"
#include <vector>

static int grid[GRID_HEIGHT][GRID_WIDTH]; // 0:空, 1:地雷
static bool opened[GRID_HEIGHT][GRID_WIDTH];
static bool gameOver = false;

void InitializeGame() {
    gameOver = false;
    // 地雷初期化（ここでは簡略化）
    for (int y = 0; y < GRID_HEIGHT; y++)
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = (GetRand(9) == 0 ? 1 : 0);
            opened[y][x] = false;
        }
}

void HandleMouseClick() {
    int mx, my;
    GetMousePoint(&mx, &my);
    int x = mx / TILE_SIZE;
    int y = my / TILE_SIZE;

    if (!opened[y][x]) {
        opened[y][x] = true;
        if (grid[y][x] == 1) {
            gameOver = true; // 爆弾だった！
        }
    }
}

bool IsGameOver() {
    return gameOver;
}

bool IsOpened(int x, int y) {
    return opened[y][x];
}

int GetCell(int x, int y) {
    return grid[y][x];
}

void ResetGame() {
    InitializeGame();
}
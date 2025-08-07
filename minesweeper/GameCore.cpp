#include "DxLib.h"
#include "Constants.h"
#include <vector>

static int grid[GRID_HEIGHT][GRID_WIDTH];          // 0:空, 1:地雷
static bool opened[GRID_HEIGHT][GRID_WIDTH];       // マスが開かれているか
static int adjacentBombs[GRID_HEIGHT][GRID_WIDTH]; // 周囲の地雷数
static bool gameOver = false;
static bool isFirstClick = true;                   // 初クリックフラグ
static bool flagged[GRID_HEIGHT][GRID_WIDTH]; // 旗の状態
bool IsFlagged(int x, int y) {
    return flagged[y][x];
}


// 指定マスを避けて地雷を配置
void PlaceBombs(int avoidX, int avoidY) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (x == avoidX && y == avoidY) {
                grid[y][x] = 0; // 最初のマスは空確定
            }
            else {
                grid[y][x] = (GetRand(6) == 0 ? 1 : 0);
            }
            opened[y][x] = false;
            adjacentBombs[y][x] = 0;
        }
    }
}

// 隣接する地雷の数を計算して格納
void CalculateAdjacentBombs() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x] == 1) continue;

            int count = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dx == 0 && dy == 0) continue;

                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < GRID_WIDTH && ny >= 0 && ny < GRID_HEIGHT) {
                        if (grid[ny][nx] == 1) count++;
                    }
                }
            }
            adjacentBombs[y][x] = count;
        }
    }
}

// ゲーム初期化（地雷配置はあとで）
void InitializeGame() {
    gameOver = false;
    isFirstClick = true;

    for (int y = 0; y < GRID_HEIGHT; y++)
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = 0;
            opened[y][x] = false;
            adjacentBombs[y][x] = 0;
        }
}
// 開封連鎖
void OpenCell(int x, int y) {
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) return;
    if (opened[y][x]) return; // ←ここで多重開封を防ぐ

    opened[y][x] = true;

    if (grid[y][x] == 1) return;

    if (adjacentBombs[y][x] == 0) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx;
                int ny = y + dy;
                OpenCell(nx, ny);
            }
        }
    }
}

// マウスクリック処理（開封、初回安全処理あり）
void HandleMouseClick() {
    int mx, my;
    GetMousePoint(&mx, &my);
    int x = mx / TILE_SIZE;
    int y = my / TILE_SIZE;

    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) return;

    if (isFirstClick) {
        PlaceBombs(x, y);
        CalculateAdjacentBombs();
        isFirstClick = false;
    }

    if (!opened[y][x]) {
        OpenCell(x, y);
        if (grid[y][x] == 1) {
            gameOver = true;
        }
    }
}

// 右クリックで地雷源に旗建てる
void HandleRightClick() {
    int mx, my;
    GetMousePoint(&mx, &my);
    int x = mx / TILE_SIZE;
    int y = my / TILE_SIZE;

    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) return;
    if (opened[y][x]) return; // 開かれてるマスは対象外

    // 旗のON/OFF切り替え
    flagged[y][x] = !flagged[y][x];
}


// 周囲の地雷数を返す
int GetAdjacentBombs(int x, int y) {
    return adjacentBombs[y][x];
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
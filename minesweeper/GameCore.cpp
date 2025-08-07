#include "DxLib.h"
#include "Constants.h"
#include <vector>

static int grid[GRID_HEIGHT][GRID_WIDTH];          // 0:��, 1:�n��
static bool opened[GRID_HEIGHT][GRID_WIDTH];       // �}�X���J����Ă��邩
static int adjacentBombs[GRID_HEIGHT][GRID_WIDTH]; // ���͂̒n����
static bool gameOver = false;
static bool isFirstClick = true;                   // ���N���b�N�t���O
static bool flagged[GRID_HEIGHT][GRID_WIDTH]; // ���̏��
bool IsFlagged(int x, int y) {
    return flagged[y][x];
}


// �w��}�X������Ēn����z�u
void PlaceBombs(int avoidX, int avoidY) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (x == avoidX && y == avoidY) {
                grid[y][x] = 0; // �ŏ��̃}�X�͋�m��
            }
            else {
                grid[y][x] = (GetRand(6) == 0 ? 1 : 0);
            }
            opened[y][x] = false;
            adjacentBombs[y][x] = 0;
        }
    }
}

// �אڂ���n���̐����v�Z���Ċi�[
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

// �Q�[���������i�n���z�u�͂��ƂŁj
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
// �J���A��
void OpenCell(int x, int y) {
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) return;
    if (opened[y][x]) return; // �������ő��d�J����h��

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

// �}�E�X�N���b�N�����i�J���A������S��������j
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

// �E�N���b�N�Œn�����Ɋ����Ă�
void HandleRightClick() {
    int mx, my;
    GetMousePoint(&mx, &my);
    int x = mx / TILE_SIZE;
    int y = my / TILE_SIZE;

    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) return;
    if (opened[y][x]) return; // �J����Ă�}�X�͑ΏۊO

    // ����ON/OFF�؂�ւ�
    flagged[y][x] = !flagged[y][x];
}


// ���͂̒n������Ԃ�
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
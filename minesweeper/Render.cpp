#include "DxLib.h"
#include "GameCore.h" // ← ここを修正
#include "Constants.h"

void DrawTitle() {
    DrawString(200, 100, "マインスイーパー", GetColor(255, 255, 255));
    DrawString(200, 130, "Enterキーで開始", GetColor(255, 200, 200));
}

void DrawGrid() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            int px = x * TILE_SIZE;
            int py = y * TILE_SIZE;

            if (IsOpened(x, y)) {
                if (GetCell(x, y) == 1) {
                    DrawBox(px, py, px + TILE_SIZE, py + TILE_SIZE, GetColor(255, 0, 0), TRUE); // 地雷
                }
                else {
                    DrawBox(px, py, px + TILE_SIZE, py + TILE_SIZE, GetColor(200, 200, 200), TRUE); // 開封済
                }
            }
            else {
                DrawBox(px, py, px + TILE_SIZE, py + TILE_SIZE, GetColor(100, 100, 100), TRUE); // 未開封
            }
        }
    }
}

void DrawResult(bool revealAll) {
    DrawString(200, 100, "ゲームオーバー！", GetColor(255, 100, 100));
    DrawString(200, 130, "Enterキーでタイトルへ", GetColor(200, 200, 255));

    if (revealAll) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (GetCell(x, y) == 1) {
                    int px = x * TILE_SIZE;
                    int py = y * TILE_SIZE;
                    DrawBox(px, py, px + TILE_SIZE, py + TILE_SIZE, GetColor(255, 0, 0), TRUE); // 爆弾表示
                }
            }
        }
    }
}
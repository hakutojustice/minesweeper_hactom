#include "DxLib.h"
#include "GameCore.h"
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
                    // 地雷マス
                    DrawBox(px, py, px + TILE_SIZE, py + TILE_SIZE, GetColor(255, 0, 0), TRUE);
                }
                else {
                    // 通常開封済みマス
                    DrawBox(px, py, px + TILE_SIZE, py + TILE_SIZE, GetColor(200, 200, 200), TRUE);

                    // 周囲の地雷数を描画（0なら何も表示しない）
                    int count = GetAdjacentBombs(x, y);
                    if (count > 0) {
                        DrawFormatString(px + 10, py + 8, GetColor(255, 255, 255), "%d", count);
                    }
                }
            }
            else {
                // 未開封マス
                DrawBox(px, py, px + TILE_SIZE, py + TILE_SIZE, GetColor(100, 100, 100), TRUE);
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
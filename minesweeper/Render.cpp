#include "DxLib.h"
#include "GameCore.h"
#include "Constants.h"


void DrawTitle() {
    DrawString(200, 100, "�}�C���X�C�[�p�[", GetColor(255, 255, 255));
    DrawString(200, 130, "Enter�L�[�ŊJ�n", GetColor(255, 200, 200));
}



void DrawGrid() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            int px = x * TILE_SIZE;
            int py = y * TILE_SIZE;

            if (IsOpened(x, y)) {
                if (GetCell(x, y) == 1) {
                    // �n���}�X
                    DrawBox(px, py, px + TILE_SIZE, py + TILE_SIZE, GetColor(255, 0, 0), TRUE);
                }
                else {
                    // �ʏ�J���ς݃}�X
                    DrawBox(px, py, px + TILE_SIZE, py + TILE_SIZE, GetColor(200, 200, 200), TRUE);

                    // ���͂̒n������`��i0�Ȃ牽���\�����Ȃ��j
                    int count = GetAdjacentBombs(x, y);
                    if (count > 0) {
                        DrawFormatString(px + 10, py + 8, GetColor(255, 255, 255), "%d", count);
                    }
                }
            }
            else {
                // ���J���}�X
                DrawBox(px, py, px + TILE_SIZE, py + TILE_SIZE, GetColor(100, 100, 100), TRUE);
            }
        }
    }
}

void DrawResult(bool revealAll) {
    DrawString(200, 100, "�Q�[���I�[�o�[�I", GetColor(255, 100, 100));
    DrawString(200, 130, "Enter�L�[�Ń^�C�g����", GetColor(200, 200, 255));

    if (revealAll) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (GetCell(x, y) == 1) {
                    int px = x * TILE_SIZE;
                    int py = y * TILE_SIZE;
                    DrawBox(px, py, px + TILE_SIZE, py + TILE_SIZE, GetColor(255, 0, 0), TRUE); // ���e�\��
                }
            }
        }
    }
}
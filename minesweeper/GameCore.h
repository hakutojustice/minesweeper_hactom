#pragma once

// �������E���Z�b�g
void InitializeGame();

// �}�X�J���ƃN���b�N����
void HandleMouseClick();

// ��Ԏ擾
bool IsGameOver();
bool IsOpened(int x, int y);
int GetCell(int x, int y);
void ResetGame();
int GetAdjacentBombs(int x, int y);

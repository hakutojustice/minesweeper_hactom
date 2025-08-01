#pragma once

// 初期化・リセット
void InitializeGame();

// マス開封とクリック判定
void HandleMouseClick();

// 状態取得
bool IsGameOver();
bool IsOpened(int x, int y);
int GetCell(int x, int y);
void ResetGame();
int GetAdjacentBombs(int x, int y);

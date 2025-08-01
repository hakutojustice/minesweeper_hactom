#include "DxLib.h"
#include <vector>
#include <ctime>

// 設定：盤面サイズ・マス表示サイズ・爆弾数
const int GRID_SIZE = 10;      // グリッド数（縦横）
const int CELL_SIZE = 40;      // 1マスの描画サイズ（ピクセル）
const int BOMB_COUNT = 10;     // 爆弾の配置数

// セルの状態
enum CellState { CLOSED, OPENED, FLAGGED };

// ゲームの画面状態（シーン管理）
enum SceneState { SCENE_TITLE, SCENE_GAME, SCENE_RESULT };

// 結果判定
enum GameResult { RESULT_NONE, RESULT_CLEAR, RESULT_GAMEOVER };

// 現在の画面状態・結果状態
SceneState currentScene = SCENE_TITLE;
GameResult resultStatus = RESULT_NONE;

// ゲーム開始フラグ（初手安全処理のため）
bool gameStarted = false;

// セル構造体：1マスの情報を保持
struct Cell {
    bool isBomb = false;
    int adjacentBombs = 0;
    CellState state = CLOSED;
};

// 盤面データ（2次元ベクタ）
std::vector<std::vector<Cell>> grid(GRID_SIZE, std::vector<Cell>(GRID_SIZE));

// ゲーム初期化処理
void ResetGame() {
    grid.assign(GRID_SIZE, std::vector<Cell>(GRID_SIZE));
    gameStarted = false;
    resultStatus = RESULT_NONE;
}

// 初手位置除外で爆弾を配置＋隣接爆弾数を算出
void InitializeGame(int safeX, int safeY) {
    srand((unsigned int)time(nullptr));
    int placed = 0;
    while (placed < BOMB_COUNT) {
        int x = rand() % GRID_SIZE;
        int y = rand() % GRID_SIZE;
        if ((x == safeX && y == safeY) || grid[y][x].isBomb) continue;
        grid[y][x].isBomb = true;
        placed++;
    }

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (grid[y][x].isBomb) continue;
            int count = 0;
            for (int dy = -1; dy <= 1; dy++)
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && ny >= 0 && nx < GRID_SIZE && ny < GRID_SIZE)
                        if (grid[ny][nx].isBomb) count++;
                }
            grid[y][x].adjacentBombs = count;
        }
    }
}

// 指定マスを開く（再帰的に0周辺も開く）
void OpenCell(int x, int y) {
    if (x < 0 || y < 0 || x >= GRID_SIZE || y >= GRID_SIZE) return;
    Cell& cell = grid[y][x];
    if (cell.state != CLOSED) return;

    cell.state = OPENED;
    if (cell.isBomb) {
        resultStatus = RESULT_GAMEOVER;
        currentScene = SCENE_RESULT;
        return;
    }

    if (cell.adjacentBombs == 0)
        for (int dy = -1; dy <= 1; dy++)
            for (int dx = -1; dx <= 1; dx++)
                OpenCell(x + dx, y + dy);
}

// クリア判定（全ての非爆弾マスが開かれているか）
bool IsGameCleared() {
    for (int y = 0; y < GRID_SIZE; y++)
        for (int x = 0; x < GRID_SIZE; x++) {
            const Cell& cell = grid[y][x];
            if (!cell.isBomb && cell.state != OPENED)
                return false;
        }
    return true;
}

// グリッド描画（revealAll=trueで全マス表示）
void DrawGrid(bool revealAll = false) {
    for (int y = 0; y < GRID_SIZE; y++)
        for (int x = 0; x < GRID_SIZE; x++) {
            int px = x * CELL_SIZE;
            int py = y * CELL_SIZE;
            const Cell& cell = grid[y][x];

            DrawBox(px, py, px + CELL_SIZE, py + CELL_SIZE, GetColor(150, 150, 150), true);  // 背景
            DrawBox(px, py, px + CELL_SIZE, py + CELL_SIZE, GetColor(255, 255, 255), false); // 枠線

            bool show = (cell.state == OPENED) || revealAll;

            if (show) {
                if (cell.isBomb)
                    DrawCircle(px + 20, py + 20, 10, GetColor(255, 0, 0)); // 爆弾
                else
                    DrawFormatString(px + 12, py + 10, GetColor(0, 255, 255), "%d", cell.adjacentBombs); // 数字
            }
            else if (cell.state == FLAGGED) {
                DrawFormatString(px + 10, py + 10, GetColor(0, 255, 0), "F"); // 旗
            }
        }
}

// タイトル画面描画
void DrawTitle() {
    DrawString(100, 100, "マインスイーパー", GetColor(255, 255, 255));
    DrawString(100, 160, "Enterキーでスタート", GetColor(200, 200, 255));
}

// 結果画面描画（全マス表示あり）
void DrawResult() {
    DrawGrid(true);
    if (resultStatus == RESULT_GAMEOVER)
        DrawString(110, 120, "GAME OVER", GetColor(255, 100, 100));
    else if (resultStatus == RESULT_CLEAR)
        DrawString(110, 120, "GAME CLEAR", GetColor(100, 255, 100));
    DrawString(90, 180, "Enterキーでタイトルに戻る", GetColor(255, 255, 255));
}

// メインループ
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    ChangeWindowMode(TRUE);
    SetGraphMode(GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE, 32);
    DxLib_Init();
    SetDrawScreen(DX_SCREEN_BACK);

    while (ProcessMessage() == 0) {
        ClearDrawScreen();

        int mx, my;
        GetMousePoint(&mx, &my);
        int x = mx / CELL_SIZE;
        int y = my / CELL_SIZE;

        if (CheckHitKey(KEY_INPUT_ESCAPE)) break;

        if (currentScene == SCENE_TITLE) {
            DrawTitle();
            if (CheckHitKey(KEY_INPUT_RETURN)) {
                ResetGame();
                currentScene = SCENE_GAME;
            }
        }
        else if (currentScene == SCENE_GAME) {
            DrawGrid();
            if (GetMouseInput() & MOUSE_INPUT_LEFT) {
                if (!gameStarted) {
                    InitializeGame(x, y);
                    gameStarted = true;
                }
                OpenCell(x, y);
                if (IsGameCleared()) {
                    resultStatus = RESULT_CLEAR;
                    currentScene = SCENE_RESULT;
                }
            }
            else if (GetMouseInput() & MOUSE_INPUT_RIGHT) {
                Cell& cell = grid[y][x];
                if (cell.state == CLOSED)
                    cell.state = FLAGGED;
                else if (cell.state == FLAGGED)
                    cell.state = CLOSED;
            }
        }
        else if (currentScene == SCENE_RESULT) {
            DrawResult();
            if (CheckHitKey(KEY_INPUT_RETURN)) {
                currentScene = SCENE_TITLE;
            }
        }

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
#include "DxLib.h"
#include "Constants.h"
#include "SceneControl.h"
#include "GameCore.h"
#include "Render.h"
#include "Input.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    // ウィンドウモード設定（全画面回避）
    ChangeWindowMode(TRUE);
    // DxLib初期化
    if (DxLib_Init() == -1) return -1;

    // 描画先を裏画面に設定
    SetDrawScreen(DX_SCREEN_BACK);

    // ゲーム状態初期化
    InitializeGame();

    // メインループ
    while (ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE)) {
        ClearDrawScreen();

        switch (GetCurrentScene()) {
        case SCENE_TITLE:
            if (IsEnterPressed()) SetScene(SCENE_GAME);
            DrawTitle();
            break;

        case SCENE_GAME:
            if (IsMouseClicked()) HandleMouseClick();
            DrawGrid();
            if (IsGameOver()) SetScene(SCENE_RESULT);
            break;

        case SCENE_RESULT:
            DrawResult(true); // 地雷表示あり
            if (IsEnterPressed()) {
                ResetGame();
                SetScene(SCENE_TITLE);
            }
            break;
        }

        // 裏画面を表に反映
        ScreenFlip();
    }

    // DxLibの終了処理
    DxLib_End();
    return 0;
}
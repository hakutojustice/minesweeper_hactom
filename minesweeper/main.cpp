#include "DxLib.h"
#include "Constants.h"
#include "SceneControl.h"
#include "GameCore.h"
#include "Render.h"
#include "Input.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
    // ウィンドウモード設定（全画面回避）
    ChangeWindowMode(TRUE);
    // DxLib初期化
    if (DxLib_Init() == -1) return -1;

    // 描画先を裏画面に設定
    SetDrawScreen(DX_SCREEN_BACK);

    // ゲーム状態初期化
    InitializeGame();

    if (GetMouseInput() & MOUSE_INPUT_RIGHT) {
        HandleRightClick();
    }
    // メインループ（毎フレーム実行される）
    while (ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE)) {
        // 画面をクリア（裏画面）
        ClearDrawScreen();

        // 現在のシーンに応じて処理を分岐
        switch (GetCurrentScene()) {

        case SCENE_TITLE:
            // タイトル画面：Enterキーでゲーム開始
            if (IsEnterPressed()) SetScene(SCENE_GAME);
            DrawTitle();
            break;

        case SCENE_GAME:
            // ゲーム画面：入力処理

            // 左クリックでマスを開く
            if (IsMouseClicked()) HandleMouseClick();

            // 右クリックで旗を立てる（トリガー検出）
            {
                static bool prevRightClick = false;
                bool currentRightClick = (GetMouseInput() & MOUSE_INPUT_RIGHT);

                if (currentRightClick && !prevRightClick) {
                    HandleRightClick(); // ← 旗のON/OFF切り替え
                }
                prevRightClick = currentRightClick;
            }

            // 盤面の描画
            DrawGrid();

            // ゲームオーバー判定
            if (IsGameOver()) SetScene(SCENE_RESULT);
            break;

        case SCENE_RESULT:
            // 結果画面：地雷表示とタイトルへの戻り
            DrawResult(true);
            if (IsEnterPressed()) {
                ResetGame();
                SetScene(SCENE_TITLE);
            }
            break;
        }

        // 裏画面を表に反映（ダブルバッファリング）
        ScreenFlip();
    }


    // DxLibの終了処理
    DxLib_End();
    return 0;
}
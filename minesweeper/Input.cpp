#include "DxLib.h"

bool IsEnterPressed() {
    return CheckHitKey(KEY_INPUT_RETURN) != 0;
}

bool IsMouseClicked() {
    return GetMouseInput() & MOUSE_INPUT_LEFT;
}
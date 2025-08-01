#include "Constants.h"

static int currentScene = SCENE_TITLE;

int GetCurrentScene() {
    return currentScene;
}

void SetScene(int next) {
    currentScene = next;
}

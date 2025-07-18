#include "EndState.h"
#include "GameData.h"
#include "Game.h"
#include "TitleState.h"
#include "SpriteRenderer.h"
#include "Camera.h"
#include "InputManager.h"

EndState::EndState() {
    GameObject* bg = new GameObject();

    if (GameData::playerVictory) {
        backgroundMusic.Open("Recursos/audio/endStateWin.ogg");

        SpriteRenderer* sr = new SpriteRenderer(*bg, "Recursos/img/Cena_vitoria.png");
        sr->SetCameraFollower(true);

        // Ajuste de escala para cobrir a tela inteira
        float screenW = Game::GetInstance().GetWidth();
        float screenH = Game::GetInstance().GetHeight();
        float imageW = sr->GetWidth();
        float imageH = sr->GetHeight();

        float scaleX = screenW / imageW;
        float scaleY = screenH / imageH;
        sr->SetScale(scaleX, scaleY);

        // Centralizar a imagem
        bg->box.x = 0;
        bg->box.y = 0;
        bg->box.w = screenW;
        bg->box.h = screenH;

        bg->AddComponent(sr);
    } else {
        backgroundMusic.Open("Recursos/audio/menu.wav");
    }

    AddObject(bg);

}


EndState::~EndState() {
    objectArray.clear();
}

void EndState::LoadAssets() {
    backgroundMusic.Play();
}

void EndState::Update(float dt) {
    textTimer.Update(dt);
    if (textTimer.Get() > 0.5f) {
        showText = !showText;
        textTimer.Restart();
    }
    InputManager& input = InputManager::GetInstance();

    if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
        quitRequested = true;
    }

    if (input.KeyPress(SDLK_SPACE)) {
        popRequested = true;
        Game::GetInstance().Push(new TitleState());
    }

    Camera::Update(dt);
    UpdateArray(dt);
}

void EndState::Render() {
    for (int i = 0; i < objectArray.size(); ++i) {
        if (objectArray[i]->GetComponent("Text") != nullptr) {
            if (showText) {
                objectArray[i]->Render();
            }
        } else {
            objectArray[i]->Render();
        }
    }
}

void EndState::Start() {
    Camera::pos = {0, 0};
    Camera::speed = {0, 0};
    LoadAssets();
    StartArray();
    started = true;
}

void EndState::Pause() {}

void EndState::Resume() {}

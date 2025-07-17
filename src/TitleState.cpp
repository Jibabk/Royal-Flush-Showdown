#include "TitleState.h"
#include "Game.h"
#include "InputManager.h"
#include "StageState.h"
#include "SpriteRenderer.h"
#include "Camera.h"

#include <SDL_include.h>
#include <Text.h>
#include <Timer.h>
#include <iostream>



TitleState::TitleState(){

    GameObject* titleGO = new GameObject();

    SpriteRenderer* sprite = new SpriteRenderer(*titleGO, "Recursos/img/Title.png");
    sprite->SetCameraFollower(true);

    titleGO->AddComponent(sprite);
    AddObject(titleGO);

    GameObject* textGO = new GameObject();

    SDL_Color white = {255, 255, 255, 255};

    textGO->box.x = 400;
    textGO->box.y = 450;

    textGO->AddComponent(new Text(*textGO, "Recursos/font/neodgm.ttf",40,Text::SOLID,"Press SPACE to Start",white));

    AddObject(textGO);


}

TitleState::~TitleState() {
    objectArray.clear();
}

void TitleState::LoadAssets() {
    // Nenhum asset extra além do background
}

void TitleState::Start() {
    Camera::pos = {0, 0};
    Camera::speed = {0, 0};
    LoadAssets();
    StartArray();
    started = true;
}

void TitleState::Pause() {
}

void TitleState::Resume() {
    for (auto& obj : objectArray) {
        Text* text = (Text*) obj->GetComponent("Text");
        if (text) {
            text->Refresh();
        }
    }
}


void TitleState::Update(float dt) {
    // Dentro de Update:
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
        Game::GetInstance().Push(new StageState());
    }
    
    UpdateArray(dt);
}

void TitleState::Render() {
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

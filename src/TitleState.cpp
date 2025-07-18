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



TitleState::TitleState() : backgroundMusic("Recursos/audio/menu.wav"){
    playerAnimDone = false;
    reiAnimDone = false;
    cartasAnimDone = false;
    cartasVisiveis = false;

    playerTimer.Restart();
    reiTimer.Restart();
    cartasTimer.Restart();

    GameObject* titleGO = new GameObject();

    SpriteRenderer* sprite = new SpriteRenderer(*titleGO, "Recursos/img/capa/BG.png");
    sprite->SetScale(2.0f, 1.8f);  // Ocupa 1200x900 na tela

    sprite->SetCameraFollower(true);

    titleGO->AddComponent(sprite);
    AddObject(titleGO);

    GameObject* textGO = new GameObject();

    SDL_Color white = {255, 255, 255, 255};

    textGO->box.x = 400;
    textGO->box.y = 450;

    textGO->AddComponent(new Text(*textGO, "Recursos/font/neodgm.ttf",40,Text::SOLID,"Press SPACE to Start",white));

    AddObject(textGO);

    backgroundMusic.SetVolume(64); // Define o volume da música para 100%, volume de 0 a 128
    backgroundMusic.Play(); // Toca a música em loop

    // Efeito BG centralizado
    efeitoBG = new GameObject();
    SpriteRenderer* efeitoSprite = new SpriteRenderer(*efeitoBG, "Recursos/img/capa/Efeito_BG.png");
    efeitoSprite->SetScale(2.0f, 1.8f); // ajuste conforme necessário
    efeitoSprite->SetCameraFollower(true);
    efeitoBG->box.x = (1200 - efeitoBG->box.w) / 2;
    efeitoBG->box.y = (900 - efeitoBG->box.h) / 2;
    std::cout << "x: " << efeitoBG->box.x << " y: " << efeitoBG->box.y << std::endl;
    AddObject(efeitoBG);

    // Player: inicia fora da tela embaixo
    player = new GameObject();
    SpriteRenderer* playerSprite = new SpriteRenderer(*player, "Recursos/img/capa/Player.png");
    playerSprite->SetScale(1.5f, 1.5f); // ajuste visual
    player->box.x = (1200 - player->box.w) / 2;
    playerYInicial = 900; // fora da tela
    playerYFinal = 600;   // posição final ajustável
    player->box.y = playerYInicial;
    std::cout << "x: " << player->box.x << " y: " << player->box.y << std::endl;
    AddObject(player);

    // Rei: começa fora da tela em cima
    rei = new GameObject();
    SpriteRenderer* reiSprite = new SpriteRenderer(*rei, "Recursos/img/capa/Rei.png");
    reiSprite->SetScale(1.5f, 1.5f); // ajuste visual
    rei->box.x = (1200 - rei->box.w) / 2;
    reiYInicial = -rei->box.h;
    reiYFinal = 250; // ajuste visual
    rei->box.y = reiYInicial;
    std::cout << "x: " << rei->box.x << " y: " << rei->box.y << std::endl;
    AddObject(rei);

    // Cartas: inicia invisível
    cartas = new GameObject();
    SpriteRenderer* cartasSprite = new SpriteRenderer(*cartas, "Recursos/img/capa/Cartas.png");
    cartasSprite->SetScale(1.3f, 1.3f);
    cartas->box.x = (1200 - cartas->box.w) / 2;
    cartas->box.y = (900 - cartas->box.h) / 2 + 150;
    std::cout << "x: " << cartas->box.x << " y: " << cartas->box.y << std::endl;
    AddObject(cartas);
    cartasVisiveis = false; // só ativa depois das animações


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

    // Timer das animações
    playerTimer.Update(dt);
    reiTimer.Update(dt);
    cartasTimer.Update(dt);

    // Player aparece primeiro (vindo de baixo)
    if (!playerAnimDone) {
        if (playerTimer.Get() > 0.5f) { // atraso para efeito BG aparecer
            player->box.y -= 18.0f * dt * 60;
            if (player->box.y <= playerYFinal) {
                player->box.y = playerYFinal;
                playerAnimDone = true;
                reiTimer.Restart();
            }
        }
    }

    // Rei aparece depois que Player termina de subir
    if (playerAnimDone && !reiAnimDone) {
        if (reiTimer.Get() > 0.6f) { // ajusta atraso após player
            rei->box.y += 18.0f * dt * 60;
            if (rei->box.y >= reiYFinal) {
                rei->box.y = reiYFinal;
                reiAnimDone = true;
                cartasTimer.Restart();
            }
        }
    }

    // Cartas aparecem por último, centralizadas
    if (reiAnimDone && !cartasAnimDone) {
        if (cartasTimer.Get() > 0.5f) {
            cartasVisiveis = true;
        }
    }

    
    UpdateArray(dt);
}

void TitleState::Render() {
    for (int i = 0; i < objectArray.size(); ++i) {
        if (objectArray[i].get() == cartas) {
            if (cartasVisiveis) {
                objectArray[i]->Render();
            }
        } else if (objectArray[i]->GetComponent("Text") != nullptr) {
            if (showText) objectArray[i]->Render();
        } else {
            objectArray[i]->Render();
        }
    }
}

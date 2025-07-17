// DeathState.cpp
#include "DeathState.h"
#include "Game.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Text.h"
#include "EndState.h"
#include "GameData.h"
#include <iostream>

DeathState::DeathState(Vec2 startPos)
    : startPos(startPos), movedToCenter(false) {}

DeathState::~DeathState() {
    objectArray.clear();
}

void DeathState::LoadAssets() {
    // Cria GameObject do personagem morto
    deadPlayer = new GameObject();
    auto* sprite = new SpriteRenderer(*deadPlayer, "Recursos/img/protag.spritesheet-v3.png", 12, 8);
    sprite->SetScale(2, 2);
    deadPlayer->AddComponent(sprite);

    auto* anim = new Animator(*deadPlayer);


    anim->AddAnimation("dead", Animation(84, 96, 6, SDL_FLIP_NONE));
    anim->AddAnimation("hit",Animation(72,75,6,SDL_FLIP_NONE));

    anim->SetAnimation("hit");
    anim->SetLoop("False");

    anim->SetAnimation("dead");
    anim->SetLoop(false);
    deadPlayer->AddComponent(anim);

    deadPlayer->box.SetCenter(startPos);
    AddObject(deadPlayer);

    // Texto "Morreu!"
    GameObject* textGO = new GameObject();
    textGO->box.x = Game::GetInstance().GetWidth() / 2 - 150;
    textGO->box.y = Game::GetInstance().GetHeight() / 2 - 200;
    textGO->AddComponent(new Text(*textGO, "Recursos/font/neodgm.ttf", 80, Text::SOLID,
                                  "Morreu!", {255, 255, 255, 255}));
    AddObject(textGO);
}

void DeathState::Start() {
    std::cout << "[DeathState] Start chamado\n";
    std::cout << "[DeathState] Render chamado\n";

    targetPos = Vec2(Game::GetInstance().GetWidth() / 2, Game::GetInstance().GetHeight() / 2);
    animTimer.Restart();
    moving = true;


    Camera::pos = {0, 0};
    Camera::speed = {0, 0};
    LoadAssets();
    StartArray();
    animTimer.Restart();
    started = true;
}

void DeathState::Update(float dt) {
    animTimer.Update(dt);
    UpdateArray(dt);

    // Interpolacao para o centro
    if (!movedToCenter) {
        Vec2 target = Vec2(Game::GetInstance().GetWidth() / 2, Game::GetInstance().GetHeight() / 2);
        Vec2 current = deadPlayer->box.Center();
        Vec2 newPos = current * (1 - 0.05f) + target * 0.05f;
        deadPlayer->box.SetCenter(newPos);

        if ((target - newPos).Magnitude() < 2.0f) {
            movedToCenter = true;
            animTimer.Restart();
        }
    }

    // Transicao para EndState
    if (movedToCenter && animTimer.Get() > 1.5f) {
        GameData::playerVictory = false;
        Game::GetInstance().Push(new EndState());
    }
}

void DeathState::Render() {
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    SDL_SetRenderDrawColor(renderer, 18, 19, 65, 255); // azul escuro
    SDL_RenderClear(renderer);

    RenderArray();
}

void DeathState::Pause() {}
void DeathState::Resume() {}

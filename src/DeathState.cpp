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

Vec2 Lerp(const Vec2& start, const Vec2& end, float t) {
    return start * (1.0f - t) + end * t;
}

DeathState::DeathState(Vec2 startPos)
    : startPos(startPos), movedToCenter(false), moveDuration(0.5), moving(false) {}

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
    anim->AddAnimation("dead", Animation(84, 95, 6, SDL_FLIP_NONE));
    anim->AddAnimation("hit", Animation(72, 75, 6, SDL_FLIP_NONE));

    anim->SetAnimation("hit");
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

    if (moving) {
        float t = std::min(animTimer.Get() / moveDuration, 1.0f);
        Vec2 newPos = Lerp(startPos, targetPos, t);
        deadPlayer->box.SetCenter(newPos);

        if (t >= 1.0f) {
            moving = false;
            animTimer.Restart();
            Animator* anim = static_cast<Animator*>(deadPlayer->GetComponent("Animator"));
            if (anim) {
                anim->SetAnimation("dead");
                anim->SetLoop(false);
        }
    }

    if (!moving && animTimer.Get() > 1.5f) {
        GameData::playerVictory = false;
        Game::GetInstance().Push(new EndState());
    }
}
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quitRequested = true;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            GameData::playerVictory = false;
            Game::GetInstance().Push(new EndState());
        }
    }

    if (!moving && animTimer.Get() > 1.5f) {
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

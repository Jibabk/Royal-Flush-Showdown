#include "StageState.h"
#include "Game.h"
#include "TileMap.h"
#include "Zombie.h"
#include "WaveSpawner.h"
#include "AiController.h"
#include "SpriteRenderer.h"
#include "Character.h"
#include "Camera.h"
#include "PlayerController.h"
#include "Collider.h"
#include "InputManager.h"
#include "Collision.cpp"
#include <iostream>
#include <GameData.h>
#include "EndState.h"
#include "Boss.h"
#include "BossAiController.h"

StageState::StageState() : backgroundMusic("Recursos/audio/BGM.wav") {
    // Background
    GameObject* bg = new GameObject();
    SpriteRenderer* bgRenderer = new SpriteRenderer(*bg, "Recursos/img/Background.png");
    bgRenderer->SetCameraFollower(true);
    bg->AddComponent(bgRenderer);
    AddObject(bg);

    // TileSet e TileMap
    tileSet = new TileSet(64, 64, "Recursos/img/Tileset.png");
    GameObject* mapGO = new GameObject();
    TileMap* tileMap = new TileMap(*mapGO, "Recursos/map/map.txt", tileSet);
    mapGO->AddComponent(tileMap);
    AddObject(mapGO);

    // Personagem (Player)
    GameObject* playerGO = new GameObject();
    playerGO->box.x = 1280;
    playerGO->box.y = 1280;
    playerGO->AddComponent(new Character(*playerGO, "Recursos/img/Player.png"));
    playerGO->AddComponent(new PlayerController(*playerGO));
    AddObject(playerGO);
    //Camera::Follow(playerGO);

    // Chefão (Boss)
    GameObject* BossGO = new GameObject();
    BossGO->box.x = 1380;
    BossGO->box.y = 1380;
    BossGO->AddComponent(new Boss(*BossGO, "Recursos/img/reiSprite.png"));
    BossGO->AddComponent(new BossAiController(*BossGO));
    AddObject(BossGO);
    Camera::Follow(BossGO);


    // Spawner
    GameObject* spawnerGO = new GameObject();
    spawnerGO->AddComponent(new WaveSpawner(*spawnerGO));
    AddObject(spawnerGO);

    backgroundMusic.Play();
}

StageState::~StageState() {
    objectArray.clear();
}

void StageState::LoadAssets() {
    // Se precisar carregar assets externos além dos que estão no construtor
}

void StageState::Start() {
    LoadAssets();
    StartArray();
    started = true;
}

void StageState::Pause() {}

void StageState::Resume() {}

void StageState::Update(float dt) {
    
    InputManager& input = InputManager::GetInstance();

    //std::cout << input.KeyPress(ESCAPE_KEY) << std::endl;
    if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
        popRequested = true;
    }

    if (input.KeyPress(SDLK_SPACE)) {
        GameObject* zombie = new GameObject();
        zombie->box.x = input.GetMouseX() + Camera::pos.x;
        zombie->box.y = input.GetMouseY() + Camera::pos.y;
        zombie->AddComponent(new Zombie(*zombie));
        AddObject(zombie);
    }

    UpdateArray(dt);

    // Checar colisões
    for (size_t i = 0; i < objectArray.size(); ++i) {
        auto colliderA = (Collider*)objectArray[i]->GetComponent("Collider");
        if (!colliderA) continue;

        for (size_t j = i + 1; j < objectArray.size(); ++j) {
            Collider* colliderB = (Collider*)objectArray[j]->GetComponent("Collider");
            if (!colliderB) continue;

            if (Collision::IsColliding(
                colliderA->box, colliderB->box,
                objectArray[i]->angleDeg,
                objectArray[j]->angleDeg)) {
                
                objectArray[i]->NotifyCollision(*objectArray[j]);
                objectArray[j]->NotifyCollision(*objectArray[i]);
            }
        }
    }
    // Remove objetos mortos
    for (size_t i = 0; i < objectArray.size(); ) {
        if (objectArray[i]->IsDead()) {
            objectArray.erase(objectArray.begin() + i);
        } else {
            ++i;
        }
    }
    if (Character::player == nullptr) { 
        GameData::playerVictory = false;
        popRequested = true;
        Game::GetInstance().Push(new EndState());
    }
    if (Boss::chefe == nullptr) { 
        GameData::playerVictory = true;
        popRequested = true;
        Game::GetInstance().Push(new EndState());
}

}

void StageState::Render() {
    bg.Render(0, 0, 0);
    RenderArray();
}

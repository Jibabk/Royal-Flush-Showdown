#include "WaveSpawner.h"
#include "Game.h"
#include "State.h"
#include "Zombie.h"
#include <cstdlib>   // rand, srand
#include <ctime>     // time
#include <cmath>     // cos, sin
#include <Camera.h>
#include <iostream>
#include "AiController.h"
#include "Character.h"

WaveSpawner* WaveSpawner::quest = nullptr;

WaveSpawner::WaveSpawner(GameObject& associated)
    : Component(associated), zombieCounter(0),npcCounter(0), currentWave(0) {
    // Definindo algumas waves
    //waves.emplace_back(2,1, 1.0f);  // 5 zumbis, spawn a cada 1s
    //waves.emplace_back(2,2, 0.8f);
    //waves.emplace_back(3,2, 0.6f);
    //waves.emplace_back(3,3, 0.5f);

    zombieCooldownTimer.Restart();

    if (quest == nullptr) {
        quest = this;  // Define a instância atual como a quest
    } else {
        std::cerr << "Warning: Multiple WaveSpawner instances detected. Only one should exist." << std::endl;
    }
}

void WaveSpawner::Update(float dt) {
    zombieCooldownTimer.Update(dt);

    if (currentWave >= (int)waves.size()) {
        associated.RequestDelete();
        return;
    }

    const Wave& wave = waves[currentWave];

    // Checa se ainda há zumbis a serem spawnados
    //std::cout << "Wave " << currentWave + 1 << ": " << zombieCounter << "/" << wave.zombies << " zombies, " << npcCounter << "/" << wave.npcs << " NPCs." << std::endl;

    
    if (zombieCounter < wave.zombies) {
        if (zombieCooldownTimer.Get() >= wave.cooldown) {
            SpawnZombie();
            zombieCooldownTimer.Restart();
            zombieCounter++;
        }
    } else if (npcCounter < wave.npcs) {
        // Checa se ainda há NPCs a serem spawnados
        if (zombieCooldownTimer.Get() >= wave.cooldown) {
            SpawnNpc();
            zombieCooldownTimer.Restart();
            npcCounter++;
        }
    }
    else {
        //std::cout << "zombieCounte:" << Zombie::zombieCount << std::endl;
        //std::cout << "npcCounter:" << Character::npcCount << std::endl;
        // Só passa para próxima wave se não tiver mais zumbis vivos
        if (Zombie::zombieCount == 0 && Character::npcCount == 0) {
            currentWave++;
            zombieCounter = 0;
            npcCounter = 0; // Reseta o contador de NPCs
        }
    }
}

void WaveSpawner::SpawnZombie() {
    GameObject* zombieGO = new GameObject();

    // Gera uma direção aleatória
    float angle = ((float)(rand() % 360)) * (M_PI / 180.0f);
    float distance = 800.0f; // Distância longe o suficiente da câmera

    // Posiciona longe do centro da câmera
    int WINDOW_WIDTH = Game::GetInstance().GetWidth();
    int WINDOW_HEIGHT = Game::GetInstance().GetHeight();
    zombieGO->box.x = Camera::pos.x + (float)(WINDOW_WIDTH / 2) + cos(angle) * distance;
    zombieGO->box.y = Camera::pos.y + (float)(WINDOW_HEIGHT / 2) + sin(angle) * distance;

    zombieGO->AddComponent(new Zombie(*zombieGO));

    Game::GetInstance().GetCurrentState().AddObject(zombieGO);
}

void WaveSpawner::SpawnNpc() {
    GameObject* npcGO = new GameObject();

    // Gera uma direção aleatória
    float angle = ((float)(rand() % 360)) * (M_PI / 180.0f);
    float distance = 800.0f; // Distância longe o suficiente da câmera

    // Posiciona longe do centro da câmera
    int WINDOW_WIDTH = Game::GetInstance().GetWidth();
    int WINDOW_HEIGHT = Game::GetInstance().GetHeight();
    npcGO->box.x = Camera::pos.x + (float)(WINDOW_WIDTH / 2) + cos(angle) * distance;
    npcGO->box.y = Camera::pos.y + (float)(WINDOW_HEIGHT / 2) + sin(angle) * distance;
    npcGO->AddComponent(new Character(*npcGO, "Recursos/img/NPC.png"));
    npcGO->AddComponent(new AiController(*npcGO));
    Game::GetInstance().GetCurrentState().AddObject(npcGO);
}

void WaveSpawner::Render() {
    // Nada para renderizar
}

bool WaveSpawner::Is(std::string type) const {
    return type == "WaveSpawner";
}

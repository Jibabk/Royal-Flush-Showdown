#ifndef WAVESPAWNER_H
#define WAVESPAWNER_H

#include "Component.h"
#include "Timer.h"
#include "Wave.h"
#include <vector>

class WaveSpawner : public Component {
public:
    WaveSpawner(GameObject& associated);

    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) const override;
    int GetZombieCount() const { return zombieCounter; }
    bool IsFinished() const { return currentWave >= (int)waves.size(); }

    static WaveSpawner* quest;

private:
    int zombieCounter;
    int npcCounter; // Contador de NPCs
    int currentWave;
    std::vector<Wave> waves;
    Timer zombieCooldownTimer;

    void SpawnZombie();
    void SpawnNpc();
};

#endif

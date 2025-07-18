#ifndef TILE_EXPLOSION_ATTACK_H
#define TILE_EXPLOSION_ATTACK_H

#include "Component.h"
#include "Vec2.h"
#include "Timer.h"

class TileExplosionAttack : public Component {
public:
    TileExplosionAttack(GameObject& associated, Vec2 targetPos, float delaySeconds);

    void Update(float dt) override;
    void Render() override {}
    bool Is(std::string type) const override;

private:
    Vec2 targetPos;   // Posição central do tile a ser atingido
    Timer timer;      // Cronômetro até a explosão
    float delay;      // Tempo de espera antes de explodir
    bool exploded;    // Garante que explode apenas uma vez
};

#endif // TILE_EXPLOSION_ATTACK_H

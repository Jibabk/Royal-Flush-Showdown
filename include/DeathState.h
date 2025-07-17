#ifndef DEATHSTATE_H
#define DEATHSTATE_H

#include "State.h"
#include "GameObject.h"
#include "Timer.h"
#include "Vec2.h"

class DeathState : public State {
public:
    // Construtor com posição inicial do personagem
    DeathState(Vec2 startPos);
    ~DeathState();

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;
    void Start() override;
    void Pause() override;
    void Resume() override;

private:
    GameObject* deadPlayer;  // Referência ao personagem morto renderizado nesse estado
    Timer animTimer;         // Controla o tempo até a transição para EndState
    Vec2 startPos;           // Posição de onde o personagem morreu
    bool movedToCenter = false; // Indica se o personagem já chegou ao centro
    Vec2 targetPos;
    float moveDuration = 1.0f;  // tempo total da animação (em segundos)
    bool moving = false;

};

#endif // DEATHSTATE_H

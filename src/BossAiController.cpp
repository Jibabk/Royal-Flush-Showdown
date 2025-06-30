#include "BossAiController.h"
#include "Character.h"
#include "GameObject.h"
#include "Camera.h"
#include <cmath>
#include <iostream>
#include "Boss.h"

BossAiController::BossAiController(GameObject& associated)
    : Component(associated), state(RESTING) {
    restTimer.Restart();
}

void BossAiController::Update(float dt) {
    Component* cpt = associated.GetComponent("Boss");
    if (static_cast<Boss*>(cpt)->IsDead()) {
        return; // NPC está morto, não faz nada
    }
    if (Character::player == nullptr || Character::player->IsDead()) {
        return; // Jogador morreu, NPC para de agir
    }
    

    if (state == RESTING) {
        restTimer.Update(dt);
        if (restTimer.Get() >= restCooldown) {
            // Pega a posição atual do jogador
            destination = Character::player->GetAssociated().box.Center();
            state = MOVING;
        }
    }
    else if (state == MOVING) {
        Vec2 playerPos = Character::player->GetAssociated().box.Center();
        if (cpt != nullptr) {
            static_cast<Boss*>(cpt)->Issue(
            Boss::Command(Boss::Command::SHOOT, playerPos.x, playerPos.y));
        }
        restTimer.Restart();
        state = RESTING;
    }
}

void BossAiController::Render() {
    // Não renderiza nada visível
}

bool BossAiController::Is(std::string type) const {
    return type == "BossAiController";
}

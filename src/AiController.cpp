#include "AiController.h"
#include "Character.h"
#include "GameObject.h"
#include "Camera.h"
#include <cmath>
#include <iostream>

AiController::AiController(GameObject& associated)
    : Component(associated), state(RESTING) {
    restTimer.Restart();
}

void AiController::Update(float dt) {
    Component* cpt = associated.GetComponent("Character");
    if (static_cast<Character*>(cpt)->IsDead()) {
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
        Vec2 pos = associated.box.Center();
        Vec2 playerPos = Character::player->GetAssociated().box.Center();

        Vec2 direction = destination - pos;
        float distance = direction.Magnitude();

        if (distance < proximityThreshold) {
            // Chegou perto o suficiente, atira
            if (cpt != nullptr) {
                static_cast<Character*>(cpt)->Issue(
                    Character::Command(Character::Command::SHOOT, playerPos.x, playerPos.y));
            }

            restTimer.Restart();
            state = RESTING;
        }
        else {
            static_cast<Character*>(cpt)->SetSpeed(direction.Normalized());
            direction = direction.Normalized();
            associated.box.x += direction.x * moveSpeed * dt;
            associated.box.y += direction.y * moveSpeed * dt;
        }
    }
}

void AiController::Render() {
    // Não renderiza nada visível
}

bool AiController::Is(std::string type) const {
    return type == "AIController";
}

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

// ataque com a esquerda = 0
// ataque com a direita = 1
// ataque com as duas = 2
// ataque com Card Spring = 3
int AttackPattern0() { // 50% de chance de atacar com a esquerda ou direita
    int chance = rand() % 2;
    return chance;
}

int AttackPattern1() { // 33% de chance de atacar com as duas
    int chance = rand() % 3;
    if (chance == 0){
        return 2;
    }
    return AttackPattern0();
}

int AttackPattern2() { // 20% de chance de atacar com o High Card
    int chance = rand() % 5;
    if (chance == 0) {
        return 3;
    } 
    return AttackPattern1();
}

int AttackPattern3() { // 20% de chance de atacar com o Card Spring
    int chance = rand() % 5;
    if (chance == 0) {
        return 4;
    } 
    return AttackPattern2();
}

int AttackPattern4() { // 20% de chance de atacar com o Laser
    int chance = rand() % 5;
    if (chance == 0) {
        return 5;
    } 
    return AttackPattern3();
}

int getAttack(int currentHp){

    if (currentHp < 30) {
        return AttackPattern4();
    } else if (currentHp < 50) {
        return AttackPattern3();
    } else if (currentHp < 70) {
        return AttackPattern2(); 
    } else if (currentHp < 90){
        return AttackPattern1(); 
    } else {
        return AttackPattern0();
    }
}

void BossAiController::Update(float dt) {
    Component* cpt = associated.GetComponent("Boss");
    Boss* boss = static_cast<Boss*>(cpt);

    if (!boss || boss->IsDead()) return;
    if (!Character::player || Character::player->IsDead()) return;

    if (state == RESTING) {
        restTimer.Update(dt);

        if (restTimer.Get() >= restCooldown) {
            state = MOVING;

            destination = Character::player->GetAssociated().box.Center();
        }
    }

    else if (state == MOVING) {
        int attack = getAttack(boss->GetHpPercent() * 100);
        Vec2 playerPos = Character::player->GetAssociated().box.Center();

        switch (attack) {
            case 0:
                boss->Issue(Boss::Command(Boss::Command::HIGH_CARD, playerPos.x, playerPos.y));  //SHOOT_LEFT
                break;
            case 1:
                boss->Issue(Boss::Command(Boss::Command::HIGH_CARD, playerPos.x, playerPos.y)); //SHOOT_RIGHT
                break;
            case 2:
                boss->Issue(Boss::Command(Boss::Command::SHOOT_BOTH, playerPos.x, playerPos.y));  //SHOOT_BOTH
                break;
            case 3:
                boss->Issue(Boss::Command(Boss::Command::HIGH_CARD, playerPos.x, playerPos.y)); //HIGH_CARD
                break;
            case 4:
                boss->Issue(Boss::Command(Boss::Command::POSITION_HANDS_FOR_LASER, playerPos.x, playerPos.y)); //POSITION_HANDS_FOR_LASER
                break;
            case 5:
                boss->Issue(Boss::Command(Boss::Command::PUNCH, playerPos.x, playerPos.y)); //PUNCH
                break;  
            default:
                std::cerr << "Unknown attack pattern: " << attack << std::endl;
                break;
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

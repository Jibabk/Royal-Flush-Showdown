#include "Gun.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Sound.h"
#include "GameObject.h"
#include "State.h"
#include "Bullet.h"
#include <cmath>
#include <Game.h>
#include <iostream>
#include <Character.h>

Gun::Gun(GameObject& associated, std::weak_ptr<GameObject> character, Vec2 relativeOffset, bool rightHand)
    : Component(associated),
      shotSound("Recursos/audio/Range.wav"),
      reloadSound("Recursos/audio/PumpAction.mp3"),
      cooldown(800),  // milissegundos
      character(character),
      angle(0),
      relativeOffset(relativeOffset), 
      defaultOffset(relativeOffset), 
      targetOffset(relativeOffset),
      rightHand(rightHand)
 {

    SpriteRenderer* spriteRenderer = new SpriteRenderer(associated, "Recursos/img/Maos.png", 15, 6);
    spriteRenderer->SetScale(2, 2); // Ajusta o tamanho do sprite
    associated.AddComponent(spriteRenderer);

    associated.box.w = spriteRenderer->GetWidth();
    if (rightHand) {
        associated.box.x += associated.box.w; // Ajusta a posição para a mão direita
    }
    Animator* animator = new Animator(associated);
    animator->AddAnimation("idle", Animation(0, 0, 0.5));
    animator->AddAnimation("emote1", Animation(1, 1, 0.5));
    animator->AddAnimation("emote2", Animation(2, 2, 0.5));
    animator->AddAnimation("emote3", Animation(3, 3, 0.5));
    animator->AddAnimation("emote4", Animation(4, 4, 0.5));
    animator->AddAnimation("emote5", Animation(5, 5, 0.5));
    animator->AddAnimation("idle_flip", Animation(0, 0, 0.5, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("emote1_flip", Animation(1, 1, 0.5, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("emote2_flip", Animation(2, 2, 0.5, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("emote3_flip", Animation(3, 3, 0.5, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("emote4_flip", Animation(4, 4, 0.5, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("emote5_flip", Animation(5, 5, 0.5, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("attack0", Animation(16, 21, 6));
    animator->AddAnimation("attack1", Animation(31, 36, 6));
    animator->AddAnimation("attack0_flip", Animation(16, 21, 6, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("attack1_flip", Animation(31, 36, 6, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("attack2Send", Animation(46, 58, 6));
    animator->AddAnimation("attack2Recive", Animation(61, 67, 6));
    animator->AddAnimation("attack3", Animation(76, 89, 6));
    animator->AddAnimation("attack3_flip", Animation(76, 89, 6, SDL_FLIP_HORIZONTAL));

    animator->SetAnimation("idle");
    associated.AddComponent(animator);

    shotSound.SetVolume(32);   // 25% do volume máximo
    reloadSound.SetVolume(32); // 25% do volume máximo
}

void Gun::Update(float dt) {
    auto owner = character.lock();
    if (!owner) {
        associated.RequestDelete();
        return;
    }

    cdTimer.Update(dt);
    animationTimer.Update(dt);  

    Vec2 ownerCenter = Vec2(owner->box.x + owner->box.w / 2, owner->box.y + owner->box.h / 2);
    associated.box.x = owner->box.Center().x + relativeOffset.x - associated.box.w / 2;
    associated.box.y = owner->box.Center().y + relativeOffset.y - associated.box.h / 2;

    // Interpolação suave de offset
    Vec2 delta = targetOffset - relativeOffset;
    float distance = delta.Magnitude();

    if (distance > 1.0f) {  // só move se estiver longe o bastante
        Vec2 step = delta.Normalized() * offsetLerpSpeed * dt;
        if (step.Magnitude() > distance)
            relativeOffset = targetOffset;
        else
            relativeOffset += step;
    } else {
        relativeOffset = targetOffset;
    }
    if (animationTimer.Get() > 0.6f) {
        currentState = IDLE;
        Animator* animator = static_cast<Animator*>(associated.GetComponent("Animator"));
        if (rightHand) {
            animator->SetAnimation("idle_flip");
        } else {
            animator->SetAnimation("idle");
        }
    }

    
    float offset = 20;
    if(currentState == IDLE && attackTarget.x != 0 && attackTarget.y != 0 && animationTimer.Get() > 0.6f) {
        Vec2 targetPos = Vec2(attackTarget.x, attackTarget.y);
        if (rightHand) {
            targetPos.x += offset; // Ajusta a posição para a mão direita
        } else {
            targetPos.x -= offset; // Ajusta a posição para a mão esquerda
        }
        if(previousState == SHOOTING){
            FireBullet(targetPos);
        } else if(previousState == HIGH_CARD){
            ShootHigh(targetPos);
        } else if(previousState == PUNCHING){
            // Aqui você pode adicionar a lógica para o ataque de soco, se necessário
        }
        attackTarget = Vec2(0, 0); // Reseta o alvo após disparar
    }
    

    
}

void Gun::Render() {}

bool Gun::Is(std::string type) const {
    return type == "Gun";
}

void Gun::Shoot(Vec2 target) {
    if (cdTimer.Get() < cooldown / 1000.0f) return;
    Animator* animator = static_cast<Animator*>(associated.GetComponent("Animator"));
    GunState currentState =  SHOOTING;
    previousState = currentState;
    if (rightHand) {
        animator->SetAnimation("attack0_flip");
    } else {
        animator->SetAnimation("attack0");
    }
    attackTarget = target;
    animationTimer.Restart();
}

void Gun::FireBullet(Vec2 target){
    Vec2 origin = Vec2(associated.box.x + associated.box.w / 2, associated.box.y + associated.box.h / 2);
    Vec2 dir = target - origin;
    angle = atan2(dir.y, dir.x) * 180 / M_PI;

    shotSound.Play(1);
    cdTimer.Restart();
    
    // Cria a bala
    Character* owner = static_cast<Character*>(character.lock()->GetComponent("Character"));
    bool targetsPlayer = (owner != Character::player);
    
    auto& state = Game::GetInstance().GetCurrentState();
    GameObject* bulletGO = new GameObject();
    bulletGO->box.x = associated.box.x + associated.box.w / 2 - 5; // centraliza a bala
    bulletGO->box.y = associated.box.y + associated.box.h / 2 - 5; // centraliza a bala
    //bulletGO->AddComponent(new SpriteRenderer(*bulletGO, "Recursos/img/Carta.png", 3, 2));
    //bulletGO->AddComponent(new Animator(*bulletGO));
    bulletGO->AddComponent(new Bullet(*bulletGO, angle, 300, 10, 1000,targetsPlayer)); // velocidade, dano, distância máxima
    state.AddObject(bulletGO);  // adiciona a bala ao estado
    reloadSound.Play(1);  // toca som de recarga
}

void Gun::ShootHigh(Vec2 target) {
    angle = 270;

    shotSound.Play(1);
    cdTimer.Restart();
    auto& state = Game::GetInstance().GetCurrentState();
    GameObject* bulletGO = new GameObject();
    bulletGO->box.x = associated.box.x + associated.box.w / 2 - 5; // centraliza a bala
    bulletGO->box.y = associated.box.y + associated.box.h / 2 - 500; // centraliza a bala
    bulletGO->AddComponent(new Bullet(*bulletGO, angle, 300, 10, 500,true, Vec2(2,2), true)); // velocidade, dano, distância máxima
    state.AddObject(bulletGO);  // adiciona a bala ao estado
    reloadSound.Play(1);  // toca som de recarga


    angle = 90;
    GameObject* bulletGO1 = new GameObject();
    bulletGO1->box.x = target.x - 36; // centraliza a bala
    bulletGO1->box.y = target.y - 1000; // centraliza a bala
    bulletGO1->AddComponent(new Bullet(*bulletGO1, angle, 300, 10, 900,false, Vec2(2,2),true)); // velocidade, dano, distância máxima
    state.AddObject(bulletGO1);  // adiciona a bala ao estado
}

void Gun::HighCard(Vec2 target){
    if (cdTimer.Get() < cooldown / 1000.0f) return;
    Animator* animator = static_cast<Animator*>(associated.GetComponent("Animator"));
    currentState = HIGH_CARD;
    previousState = currentState;
    if (rightHand) {
        animator->SetAnimation("attack1_flip");
    } else {
        animator->SetAnimation("attack1");
    }
    attackTarget = target;
    animationTimer.Restart();
}

void Gun::Punch(Vec2 target){
    if (cdTimer.Get() < cooldown / 1000.0f) return;
    Animator* animator = static_cast<Animator*>(associated.GetComponent("Animator"));
    currentState = PUNCHING;
    previousState = currentState;
    if (rightHand) {
        animator->SetAnimation("attack3_flip");
    } else {
        animator->SetAnimation("attack3");
    }
    attackTarget = target;
    animationTimer.Restart();
}

void Gun::SetOffset(Vec2 offset) {
    targetOffset = offset;
}

void Gun::ResetOffset() {
    targetOffset = defaultOffset;
}

bool Gun::HasReachedTarget() const {
    return (relativeOffset - targetOffset).Magnitude() < 1.0f;
}

bool Gun::HasReachedDefaultOffset() const {
    return relativeOffset.x == defaultOffset.x && relativeOffset.y == defaultOffset.y;
}

void Gun::SetOffsetPath(const std::vector<Vec2>& path) {
    offsetPath = path;
    currentOffsetIndex = 0;
}

bool Gun::UpdateOffsetPath(float dt) {
    if (currentOffsetIndex >= offsetPath.size()) return true;

    this->SetOffset(offsetPath[currentOffsetIndex]);
    return this->HasReachedTarget();
}

bool Gun::AdvanceOffsetStep() {
    if (currentOffsetIndex < offsetPath.size() - 1) {
        currentOffsetIndex++;
        return true;
    }
    return false;
}


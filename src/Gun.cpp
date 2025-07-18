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

Gun::Gun(GameObject& associated, std::weak_ptr<GameObject> character, Vec2 relativeOffset)
    : Component(associated),
      shotSound("Recursos/audio/Range.wav"),
      reloadSound("Recursos/audio/PumpAction.mp3"),
      cooldown(800),  // milissegundos
      character(character),
      angle(0),
      relativeOffset(relativeOffset) {
        
    associated.AddComponent(new SpriteRenderer(associated, "Recursos/img/Gun.png", 3, 2));

    Animator* animator = new Animator(associated);
    animator->AddAnimation("idle", Animation(0, 0, 0));
    animator->AddAnimation("idle_flip", Animation(0, 0, 0, SDL_FLIP_VERTICAL));
    animator->AddAnimation("reloading", Animation(1, 1, 0.2));
    animator->AddAnimation("reloading_flip", Animation(1, 1, 0.2, SDL_FLIP_VERTICAL));

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

    Vec2 ownerCenter = Vec2(owner->box.x + owner->box.w / 2, owner->box.y + owner->box.h / 2);
    associated.box.x = owner->box.Center().x + relativeOffset.x - associated.box.w / 2;
    associated.box.y = owner->box.Center().y + relativeOffset.y - associated.box.h / 2;


    // deslocar levemente na direção do ângulo
    float offset = 20;
    associated.box.x += offset * cos(angle * M_PI / 180);
    associated.box.y += offset * sin(angle * M_PI / 180);

    associated.angleDeg = angle;
    Animator* animator = static_cast<Animator*>(associated.GetComponent("Animator"));
    if (angle > 90 && angle <= 180 || angle < -90 && angle >= -180) {
        if(cdTimer.Get() < cooldown / 1000.0f) {
            animator->SetAnimation("reloading_flip");
        } else {
            animator->SetAnimation("idle_flip");
    }}else {
        if(cdTimer.Get() < cooldown / 1000.0f) {
            animator->SetAnimation("reloading");
        } else {
            animator->SetAnimation("idle");
    }
}

}

void Gun::Render() {}

bool Gun::Is(std::string type) const {
    return type == "Gun";
}

void Gun::Shoot(Vec2 target) {
    if (cdTimer.Get() < cooldown / 1000.0f) return;

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
    bulletGO->AddComponent(new SpriteRenderer(*bulletGO, "Recursos/img/cartinha.png", 3, 2));
    bulletGO->AddComponent(new Animator(*bulletGO));
    bulletGO->AddComponent(new Bullet(*bulletGO, angle, 500, 10, 1000,targetsPlayer)); // velocidade, dano, distância máxima
    state.AddObject(bulletGO);  // adiciona a bala ao estado
    reloadSound.Play(1);  // toca som de recarga
}

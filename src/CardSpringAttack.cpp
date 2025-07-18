// CardSpringAttack.cpp
#include "CardSpringAttack.h"
#include "Game.h"
#include "SpriteRenderer.h"
#include "Collider.h"
#include "Character.h"
#include "Camera.h"
#include <Animator.h>
#include <iostream>

CardSpringAttack::CardSpringAttack(GameObject& associated, Vec2 start, Vec2 end)
    : Component(associated), durationTimer(), endPos(end), active(true) {
    
    const float mapStartX = 160;
    const float mapEndX = 800;
    const float tileSize = 80.0f;
    const float tileHeight = 65.0f;

    associated.box.x = std::min(start.x, end.x);
    associated.box.y = start.y - 10; // Ajuste vertical
    associated.box.w = std::abs(start.x - end.x);
    associated.box.h = 20; // Altura do laser
    associated.AddComponent(new Collider(associated));

    SpriteRenderer* spriteRenderer = new SpriteRenderer(associated, "Recursos/img/card_laser.png", 1, 4);
    spriteRenderer->SetScale(2, 2); // Ajusta o tamanho do laser

    // Adicione sprite de laser aqui se quiser visual
    associated.AddComponent(spriteRenderer);


    Animator* animator = new Animator(associated);
    animator->AddAnimation("start", Animation(0, 0, 1, SDL_FLIP_NONE)); // Animação do laser
    animator->AddAnimation("laser", Animation(1, 3, 20, SDL_FLIP_NONE));
    animator->SetAnimation("start");
    associated.AddComponent(animator);
}

void CardSpringAttack::Update(float dt) {
    Animator* animator = static_cast<Animator*>(associated.GetComponent("Animator"));
    durationTimer.Update(dt);
    Vec2 playerPos = Character::player->GetAssociated().box.Center();

    if (durationTimer.Get() > 1.0f) { // Duração de 1 segundo
        associated.RequestDelete();
        active = false;
    }

    std::cout << "Associated box: " <<  associated.box.y  << std::endl;
    std::cout << "Player position: " << playerPos.x << ", " << playerPos.y << std::endl;
    if (associated.box.y == playerPos.y) {
        Character::player->TakeDamage(1);
    }
    if (durationTimer.Get() > 0.2f) { // Duração de 0.2 segundos
        animator->SetAnimation("laser");
    }
}

void CardSpringAttack::Render() {}
bool CardSpringAttack::Is(std::string type) const {
    return type == "CardSpringAttack";
}

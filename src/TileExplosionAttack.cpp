// TileExplosionAttack.cpp
#include "TileExplosionAttack.h"
#include "Character.h"
#include "Game.h"
#include "Camera.h"
#include "Sound.h"
#include <SpriteRenderer.h>
#include <iostream>
#include <Animator.h>

TileExplosionAttack::TileExplosionAttack(GameObject& associated, Vec2 targetPos, float delay)
    : Component(associated), targetPos(targetPos), timer(), delay(delay), exploded(false) {
    
    SpriteRenderer* renderer = new SpriteRenderer(associated, "Recursos/img/Perigo.png", 4, 2);
    renderer->SetScale(2, 2); // tamanho igual ao tile
    associated.AddComponent(renderer);

    associated.box.x = targetPos.x - associated.box.w / 2;
    associated.box.y = targetPos.y - associated.box.h / 2;
    
    Animator* animator = new Animator(associated);
    animator->AddAnimation("boom", Animation(0, 4, 3.6, SDL_FLIP_NONE));
    animator->SetAnimation("boom");
    associated.AddComponent(animator);
}
 
void TileExplosionAttack::Update(float dt) {
    timer.Update(dt);

    if (!exploded && timer.Get() > delay) {
        exploded = true;

        // Checar colisão com player
        if (Character::player) {
            GameObject& player = Character::player->GetAssociated();
            associated.box.w -= 70;
            associated.box.h -= 120; // Ajusta a posição para o centro do tile 
            if (player.box.OverlapsWith(associated.box)) {
                if (Character::player->GetDamageCooldown() > 1.0f) {
                    Character::player->TakeDamage(1);
                    Character::player->RestartDamageCooldown();
                }
            }
        }

        // Muda sprite pra explosão ou só remove
        associated.RequestDelete();
    }
}

bool TileExplosionAttack::Is(std::string type) const {
    return type == "TileExplosion";
}

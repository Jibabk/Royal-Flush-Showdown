#include "Zombie.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Collider.h"
#include "Animation.h"
#include <iostream>
#include <Character.h>

int Zombie::zombieCount = 0;

Zombie::Zombie(GameObject& associated)
    : Component(associated), hitpoints(100),isDead(false),deathSound("Recursos/audio/Dead.wav"),hitSound("Recursos/audio/Hit0.wav"),deathTimer(), facingLeft(false) {
    zombieCount++;
    // Cria e adiciona o SpriteRenderer
    SpriteRenderer* renderer = new SpriteRenderer(associated, "Recursos/img/Enemy.png", 3, 2);
    associated.AddComponent(renderer);
    associated.AddComponent(new Collider(associated));
    
    // Cria e adiciona o Animator
    Animator* animator = new Animator(associated);
    animator->AddAnimation("walking", Animation(0, 3, 10));
    animator->AddAnimation("walking_left", Animation(0, 3, 10, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("dead", Animation(5, 5, 0));
    animator->SetAnimation("walking");
    associated.AddComponent(animator);
}

void Zombie::Damage(int damage) {
     if (hitpoints > 0) {
        hitSound.Play(1); // Toca som de impacto sempre que tomar dano
        hitpoints -= damage;
        //std::cout << "Zumbi tomou " << damage << " de dano! HP restante: " << hitpoints << std::endl;
    }
}


void Zombie::Update(float dt) {
    Component* animCpt = associated.GetComponent("Animator");
    Animator* animator = dynamic_cast<Animator*>(animCpt);
    if(isDead){
        deathTimer.Update(dt);
        if (deathTimer.Get() > 3.0f) {
            associated.RequestDelete();
            zombieCount--;
        }
        return;
    }
    if (hitpoints <= 0 && !isDead) {
        isDead = true;
        deathSound.Play(1);
        Component* col = associated.GetComponent("Collider");
        if (col) {
            associated.RemoveComponent(col);
        }

        Component* spriteCpt = associated.GetComponent("SpriteRenderer");
        if (spriteCpt != nullptr) {
            SpriteRenderer* renderer = dynamic_cast<SpriteRenderer*>(spriteCpt);
            if (renderer != nullptr) {
                renderer->SetFrame(5, SDL_FLIP_NONE); // Define o frame de morte
            }
        }

        if (animCpt != nullptr) {
            if (animator != nullptr) {
                animator->SetAnimation("dead");
            }
        }
    }else if (Character::player != nullptr) {
        // Movimento na direção do player
        Vec2 playerPos = Character::player->GetAssociated().box.Center();
        Vec2 myPos = associated.box.Center();

        Vec2 direction = playerPos - myPos;
        direction = direction.Normalized();

        float speed = 100.0f;
        associated.box.x += direction.x * speed * dt;
        associated.box.y += direction.y * speed * dt;

        if (direction.Magnitude() > 0.1f) {
        if (direction.x < -0.1f) {
            animator->SetAnimation("walking_left");
            this->facingLeft = true;
        } else if (direction.x > 0.1f) {
            animator->SetAnimation("walking");
            this->facingLeft = false;
        } else {
            if (this->facingLeft) {
                animator->SetAnimation("walking_left");
            } else {
                animator->SetAnimation("walking");
            }
        }
    } else {
        if (this->facingLeft) {
            animator->SetAnimation("idle_left");
        } else
        animator->SetAnimation("idle");
        
    }
    }
}


void Zombie::Render() {
    // Vazio, zumbi não desenha nada diretamente
}

bool Zombie::Is(std::string type) const {
    return type == "Zombie";
}

void Zombie::NotifyCollision(GameObject& other) {
    if (isDead) {
        return; // Não reage a colisões se já estiver morto
    }
    if (other.GetComponent("Bullet") != nullptr) {
        Damage(25);
    }
}

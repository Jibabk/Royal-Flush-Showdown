#include "Bullet.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Sound.h"
#include "GameObject.h"
#include "State.h"
#include "Collider.h"
#include <cmath>
#include <Character.h>
#include "Boss.h"

Bullet::Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance, bool targetsPlayer)
    : Component(associated){

    associated.angleDeg = angle;
    SpriteRenderer* spriteRenderer = new SpriteRenderer(associated, "Recursos/img/Carta.png", 1, 1);
    
    spriteRenderer->SetScale(1,1);
    associated.AddComponent(spriteRenderer);
    
    associated.AddComponent(new Collider(associated));

 
    
    Vec2 direction(0, 0);
    this->speed = Vec2(std::cos(angle * M_PI / 180.0f), std::sin(angle * M_PI / 180.0f)) * speed;
    this->damage = damage;
    this->distanceLeft = maxDistance;
    this->targetsPlayer = targetsPlayer;
}

Bullet::Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance, bool targetsPlayer, Vec2 scale, bool highCard)
    : Component(associated){

    associated.angleDeg = angle;
    SpriteRenderer* spriteRenderer = new SpriteRenderer(associated, "Recursos/img/Carta.png", 1, 1);
    spriteRenderer->SetScale(scale.x, scale.y);
    associated.AddComponent(spriteRenderer);

    //associated.AddComponent(new Collider(associated));

 
    
    Vec2 direction(0, 0);
    this->speed = Vec2(std::cos(angle * M_PI / 180.0f), std::sin(angle * M_PI / 180.0f)) * speed;
    this->damage = damage;
    this->distanceLeft = maxDistance;
    this->targetsPlayer = targetsPlayer;
    this->highCard = highCard;
}

void Bullet::Update(float dt) {
    if (distanceLeft <= 0) {
        associated.RequestDelete();
        return;
    }

    // Faz o bullet girar no próprio eixo
    associated.angleDeg += 360 * dt; // 360 graus por segundo (ajuste como quiser)

    // Movimento
    Vec2 position = Vec2(associated.box.x, associated.box.y);
    position += speed * dt;
    associated.box.x = position.x;
    associated.box.y = position.y;

    distanceLeft -= speed.Magnitude() * dt;
}


void Bullet::Render() {}

bool Bullet::Is(std::string type) const {
    return type == "Bullet";
}

void Bullet::NotifyCollision(GameObject& other) {
    if (!highCard){
    if (other.GetComponent("Zombie") != nullptr) {
        associated.RequestDelete();
    }
    if (other.GetComponent("Character") != nullptr) {
        Character* character = (Character*) other.GetComponent("Character");

        if (targetsPlayer) {
            // Se a bala for para acertar o player
            if (character == Character::player) {
                associated.RequestDelete();
            }
        } else {
            // Se a bala for para acertar NPCs
            if (character != Character::player) {
                associated.RequestDelete();
            }
        }
    }
    if (other.GetComponent("Boss") != nullptr) {
        Boss* boss = (Boss*) other.GetComponent("Boss");
        if (!targetsPlayer )
        associated.RequestDelete();
    }
    }
}
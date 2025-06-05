#include "Bullet.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Sound.h"
#include "GameObject.h"
#include "State.h"
#include "Collider.h"
#include <cmath>
#include <Character.h>

Bullet::Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance, bool targetsPlayer)
    : Component(associated){

    associated.angleDeg = angle;
    associated.AddComponent(new SpriteRenderer(associated, "Recursos/img/Bullet.png", 3, 2));
    associated.AddComponent(new Collider(associated));
    Vec2 direction(0, 0);
    this->speed = Vec2(std::cos(angle * M_PI / 180.0f), std::sin(angle * M_PI / 180.0f)) * speed;
    this->damage = damage;
    this->distanceLeft = maxDistance;
    this->targetsPlayer = targetsPlayer;
}

void Bullet::Update(float dt) {
    if (distanceLeft <= 0) {
        associated.RequestDelete();
        return;
    }

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
}
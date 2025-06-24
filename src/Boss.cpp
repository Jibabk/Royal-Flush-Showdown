// Boss.cpp
#include "Boss.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Gun.h"
#include "Camera.h"
#include "Game.h"
#include "State.h"
#include <iostream>
#include <Collider.h>
#include <Bullet.h>
#include <Character.h>

Boss* Boss::chefe = nullptr;

Boss::Boss(GameObject& associated, std::string sprite)
    : Component(associated), speed(0, 0), linearSpeed(0), hp(100), maxHp(100), currentHp(100),
      deathTimer(), damageCooldown(),isDead(false),deathSound("Recursos/audio/Dead.wav"),hitSound("Recursos/audio/Hit0.wav") {

    
    SpriteRenderer* spriteRenderer = new SpriteRenderer(associated, sprite, 2, 1);
    associated.AddComponent(new Collider(associated));
    spriteRenderer->SetScale(1,1); // Ajusta o tamanho do sprite
    associated.AddComponent(spriteRenderer);

    associated.box.w = spriteRenderer->GetWidth();
    associated.box.h = spriteRenderer->GetHeight();

    Animator* animator = new Animator(associated);
    animator->AddAnimation("idle", Animation(0, 0, 9, SDL_FLIP_NONE));
    animator->SetAnimation("idle");
    associated.AddComponent(animator);

    if (!chefe)
        chefe = this;

    
}

Boss::~Boss() {
    if (chefe == this) {
        chefe = nullptr;
    }

}

void Boss::Start() {
    auto& state = Game::GetInstance().GetCurrentState();

    GameObject* gunGO = new GameObject();
    gunGO->box = associated.box;

    gunGO->AddComponent(new Gun(*gunGO, state.GetObjectPtr(&associated)));

    gun = state.AddObject(gunGO);  // Guarda o weak_ptr
    
}


void Boss::Update(float dt) {
    Animator* animator = static_cast<Animator*>(associated.GetComponent("Animator"));

    damageCooldown.Update(dt);
    if (hp <= 0) {
        auto gunPtr = gun.lock();
        if (gunPtr) {
            gunPtr->RequestDelete();
        }
        isDead = true;
        //animator->SetAnimation("dead");
    }

    if (isDead) {
        deathTimer.Update(dt);
        if (deathTimer.Get() > 3.0f) {
            associated.RequestDelete();
        }
        return;
    }

    // Executar comandos pendentes
    while (!taskQueue.empty() && !isDead) {
        Command cmd = taskQueue.front();
        taskQueue.pop();

        if (cmd.type == Command::MOVE) {
            speed = (cmd.pos - associated.box.Center()).Normalized();
            linearSpeed = 200;
        } else if (cmd.type == Command::SHOOT) { //VERIFICAR
            auto gunPtr = gun.lock();
            if (!gunPtr) {
                std::cerr << "[Boss] Gun pointer expired!\n";
            } else {
                Gun* gunComponent = (Gun*)gunPtr->GetComponent("Gun");
                if (!gunComponent) {
                    std::cerr << "[Boss] Gun component not found!\n";
                } else {
                    std::cerr << "[Boss] Shooting towards (" << cmd.pos.x << ", " << cmd.pos.y << ")\n";
                    (gunComponent)->Shoot(cmd.pos);
                }
            }

        }
    }

    // Movimento
    associated.box.x += speed.x * linearSpeed * dt;
    associated.box.y += speed.y * linearSpeed * dt;

    const float mapStartX = 640;
    const float mapStartY = 512;
    const float mapEndX = 640 + 1280;
    const float mapEndY = 512 + 1536;

    //associated.box.x = std::max(mapStartX, std::min(associated.box.x, mapEndX - associated.box.w));
    //associated.box.y = std::max(mapStartY, std::min(associated.box.y, mapEndY - associated.box.h));


    // Atualizar animação
    if (speed.Magnitude() > 0.1f) {
        if (speed.x < -0.1f) {
            //animator->SetAnimation("walking_left");
            this->facingLeft = true;
        } else if (speed.x > 0.1f) {
            //animator->SetAnimation("walking");
            this->facingLeft = false;
        } else {
            if (this->facingLeft) {
                //animator->SetAnimation("walking_left");
            } else {
                //animator->SetAnimation("walking");
            }
        }
    } else {
        if (this->facingLeft) {
            //animator->SetAnimation("idle_left");
        } else
        animator->SetAnimation("idle");
        
    }

    

    // Reset velocidade
    linearSpeed = 0;
    speed = Vec2(0, 0);

    
}

void Boss::Render() {
    if (currentHp < maxHp) {
        SDL_Rect barBackground;
        SDL_Rect barFill;

        // Tamanho da barra
        int barWidth = associated.box.w;
        int barHeight = 8;
        int offsetY = 10;

        // Ajuste de posição com base na câmera
        int screenX = associated.box.x - Camera::pos.x;
        int screenY = associated.box.y - Camera::pos.y - offsetY;

        // Retângulo de fundo (vermelho)
        barBackground.x = screenX;
        barBackground.y = screenY;
        barBackground.w = barWidth;
        barBackground.h = barHeight;

        // Retângulo de preenchimento (verde)
        barFill = barBackground;
        barFill.w = static_cast<int>((float)hp / maxHp * barWidth);

        SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, 255); // vermelho
        SDL_RenderFillRect(Game::GetInstance().GetRenderer(), &barBackground);

        SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 0, 255, 0, 255); // verde
        SDL_RenderFillRect(Game::GetInstance().GetRenderer(), &barFill);
    }
}


bool Boss::Is(std::string type) const {
    return type == "Boss";
}

void Boss::NotifyCollision(GameObject& other) {
    if (other.GetComponent("Bullet") != nullptr) {
        Bullet* bullet = (Bullet*)other.GetComponent("Bullet");
        if (!bullet->targetsPlayer) {
            TakeDamage(10);
        }
    }
}

void Boss::Issue(Command task) {
    taskQueue.push(task);
}

void Boss::TakeDamage(int damage) {
    if (hp <= 0 || isDead) {
        return; // Já está morto ou não pode receber dano
    }
    hp -= damage;
    currentHp = hp; // <-- Adicione isso
    hitSound.Play(1);
    //std::cout << "[Boss] Took damage. HP: " << hp << std::endl;

    if (hp <= 0) {
        deathSound.Play(1);
        Component* col = associated.GetComponent("Collider");
        if (col) {
            associated.RemoveComponent(col);
        }
    }
}
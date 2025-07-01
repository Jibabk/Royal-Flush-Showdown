// Character.cpp
#include "Character.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Gun.h"
#include "Camera.h"
#include "Game.h"
#include "State.h"
#include <iostream>
#include <Collider.h>
#include <Bullet.h>
#include <Resources.h>

Character* Character::player = nullptr;
int Character::npcCount = 0;

Character::Character(GameObject& associated, std::string sprite)
    : Component(associated), speed(0, 0), linearSpeed(0), hp(3), 
      deathTimer(), damageCooldown(),isDead(false),deathSound("Recursos/audio/Dead.wav"),hitSound("Recursos/audio/Hit0.wav") {

    
    SpriteRenderer* spriteRenderer = new SpriteRenderer(associated, sprite, 4, 3);
    spriteRenderer->SetScale(2,2);
    associated.AddComponent(new Collider(associated));
    associated.AddComponent(spriteRenderer);

    associated.box.w = spriteRenderer->GetWidth();
    associated.box.h = spriteRenderer->GetHeight();

    Animator* animator = new Animator(associated);
    animator->AddAnimation("idle", Animation(0, 2, 9, SDL_FLIP_NONE));
    animator->AddAnimation("idle_left", Animation(0, 2, 9, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("walking", Animation(8, 11, 3, SDL_FLIP_NONE));
    animator->AddAnimation("walking_left", Animation(8, 11, 3, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("dead", Animation(10, 11, 12,SDL_FLIP_NONE));
    animator->SetAnimation("idle");
    associated.AddComponent(animator);

    if (player){
        npcCount++;
    }
    
    if (!player)
        player = this;

    
}

Character::~Character() {
    if (player == this) {
        player = nullptr;
    }
}

void Character::Start() {
    auto& state = Game::GetInstance().GetCurrentState();

    //GameObject* gunGO = new GameObject();
    //gunGO->box = associated.box;

    //gunGO->AddComponent(new Gun(*gunGO, state.GetObjectPtr(&associated)));

    //gun = state.AddObject(gunGO);  // Guarda o weak_ptr
    
}


void Character::Update(float dt) {
    Animator* animator = static_cast<Animator*>(associated.GetComponent("Animator"));
    walkTimer.Update(dt);

    damageCooldown.Update(dt);
    if (hp <= 0) {
        auto gunPtr = gun.lock();
        if (gunPtr) {
            gunPtr->RequestDelete();
        }
        isDead = true;
        animator->SetAnimation("dead");
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
            const float tileWidth = 80.0f; // tamanho do quadrado
            const float tileHeight = 65.0f;

        Vec2 current = associated.box.Center();
        Vec2 direction = cmd.pos - current;

        // Decide a direção do movimento com base no vetor direção
        if (std::abs(direction.x) > std::abs(direction.y)) {
            // Movimento horizontal
            speed = Vec2((direction.x > 0) ? 1 : -1, 0);
            facingLeft = speed.x <0;
        } else {
            // Movimento vertical
            speed = Vec2(0, (direction.y > 0) ? 1 : -1);
        }

        // Move imediatamente 1 tile (modo tabuleiro)
        associated.box.x += speed.x * tileWidth;
        associated.box.y += speed.y * tileHeight;

        walkTimer.Restart();

        // Atualiza o animator
        Animator* animator = static_cast<Animator*>(associated.GetComponent("Animator"));
        if (speed.x < 0)
            animator->SetAnimation("walking_left");
        else if (speed.x > 0)
            animator->SetAnimation("walking");
        else
            animator->SetAnimation(facingLeft ? "walking_left" : "walking");
        } else if (cmd.type == Command::SHOOT) { //VERIFICAR
            auto gunPtr = gun.lock();
            if (!gunPtr) {
                std::cerr << "[Character] Gun pointer expired!\n";
            } else {
                Gun* gunComponent = (Gun*)gunPtr->GetComponent("Gun");
                if (!gunComponent) {
                    std::cerr << "[Character] Gun component not found!\n";
                } else {
                    std::cerr << "[Character] Shooting towards (" << cmd.pos.x << ", " << cmd.pos.y << ")\n";
                    (gunComponent)->Shoot(cmd.pos);
                }
            }

        }
    }

    // Movimento
    //associated.box.x += speed.x * linearSpeed * dt;
    //associated.box.y += speed.y * linearSpeed * dt;

    const float mapStartX = 160;
    const float mapStartY = 310;
    const float mapEndX = 800;
    const float mapEndY = 628;

    associated.box.x = std::max(mapStartX, std::min(associated.box.x, mapEndX - associated.box.w));
    associated.box.y = std::max(mapStartY, std::min(associated.box.y, mapEndY - associated.box.h));

    // Animação baseada no temporizador de caminhada
    if (walkTimer.Get() < 0.2f) {
        if (facingLeft)
            animator->SetAnimation("walking_left");
        else
            animator->SetAnimation("walking");
    } else {
        if (facingLeft)
            animator->SetAnimation("idle_left");
        else
            animator->SetAnimation("idle");
    }


    

    // Reset velocidade
    linearSpeed = 0;
    speed = Vec2(0, 0);
}

void Character::Render() {
    // Desenha os corações (vida) no canto superior esquerdo da tela
    SDL_Texture* heartTex = Resources::GetImage("Recursos/img/cartinha.png");
    if (!heartTex) return;

    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();

    int heartSize = 32;  // Tamanho do coração
    int spacing = 4;     // Espaço entre os corações
    int startX = 20;
    int startY = 20;

    for (int i = 0; i < hp ; ++i) {  // Supondo que cada coração representa 10 de HP
        SDL_Rect dst;
        dst.x = startX + i * (heartSize + spacing);
        dst.y = startY;
        dst.w = heartSize;
        dst.h = heartSize;
        SDL_RenderCopy(renderer, heartTex, nullptr, &dst);
    }
}

bool Character::Is(std::string type) const {
    return type == "Character";
}

void Character::Issue(Command task) {
    taskQueue.push(task);
}

void Character::NotifyCollision(GameObject& other) {
    // Bullet
    bool isPlayer = (this == Character::player);
    if (other.GetComponent("Bullet") != nullptr) {
        Bullet* bullet = static_cast<Bullet*>(other.GetComponent("Bullet"));

        if (bullet->targetsPlayer) {
            if (isPlayer) {
                if (damageCooldown.Get() > 0.5f) {
                    TakeDamage(1);
                    damageCooldown.Restart();
                }
            }
        } else {
            if (!isPlayer) {
                if (damageCooldown.Get() > 0.5f) {
                    TakeDamage(1);
                    damageCooldown.Restart();
                }
            }
        }
    }

    // Zombie
    if (other.GetComponent("Zombie") != nullptr) {
        if (isPlayer){
            if (damageCooldown.Get() > 0.5f) {
            TakeDamage(1);
            damageCooldown.Restart();
            }
        }
    }
}

void Character::TakeDamage(int damage) {
    if (hp <= 0 || isDead) {
        return; // Já está morto ou não pode receber dano
    }
    hp -= damage;
    hitSound.Play(1);
    //std::cout << "[Character] Took damage. HP: " << hp << std::endl;

    if (hp <= 0) {
        deathSound.Play(1);
        Component* col = associated.GetComponent("Collider");
        if (col) {
            associated.RemoveComponent(col);
        }
        if (this == Character::player) {
            Camera::Unfollow();
        } else {
            npcCount--;
        }
    }
}
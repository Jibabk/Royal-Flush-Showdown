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
#include "GameData.h"
#include "DeathState.h"
#include "EndState.h"




Character* Character::player = nullptr;
int Character::npcCount = 0;

Character::Character(GameObject& associated, std::string sprite)
    : Component(associated), speed(0, 0), linearSpeed(0), hp(3), 
      deathTimer(), damageCooldown(),isDead(false),deathSound("Recursos/audio/Dead.wav"),hitSound("Recursos/audio/Hit0.wav") {

    
    SpriteRenderer* spriteRenderer = new SpriteRenderer(associated, sprite, 12, 8);
    spriteRenderer->SetScale(2,2);
    associated.AddComponent(new Collider(associated));
    associated.AddComponent(spriteRenderer);

    associated.box.w = spriteRenderer->GetWidth();
    associated.box.h = spriteRenderer->GetHeight();

    Animator* animator = new Animator(associated);

    //Idle

    animator->AddAnimation("idle_front", Animation(0, 2, 9, SDL_FLIP_NONE));
    animator->AddAnimation("idle_side_right", Animation(12, 14, 9, SDL_FLIP_NONE));
    animator->AddAnimation("idle_side_left", Animation(12, 14, 9, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("idle_back", Animation(24, 26, 9, SDL_FLIP_NONE));


    //Walking
    animator->AddAnimation("walk_side_right",  Animation(36, 39, 4, SDL_FLIP_NONE)); // Linha 3
    animator->AddAnimation("walk_side_left",  Animation(36, 39, 4, SDL_FLIP_HORIZONTAL)); // Linha 3
    animator->AddAnimation("walk_back",  Animation(48, 51, 4, SDL_FLIP_NONE));// Linha 4
    animator->AddAnimation("walk_front", Animation(60, 63, 4, SDL_FLIP_NONE));// Linha 5

    //Death
    animator->AddAnimation("dead", Animation(84, 96, 6,SDL_FLIP_NONE));

    //Damage
    animator->AddAnimation("hit",Animation(72,75,6,SDL_FLIP_NONE));

    animator->SetAnimation("idle_front");
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
    dashTimer.Update(dt);


    damageCooldown.Update(dt);
    if (hp <= 0 && !isDead) {
        isDead = true;

        if (this == Character::player) {
            Game::GetInstance().Push(new DeathState(associated.box.Center()));
        }

        //associated.SetVisible(false);  // remove o personagem original
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
            currentDirection = (speed.x > 0) ? "right" : "left";
            facingLeft = speed.x <0;
        } else {
            // Movimento vertical
            speed = Vec2(0, (direction.y > 0) ? 1 : -1);
            currentDirection = (speed.y > 0) ? "down" : "up";
        }

        // Move imediatamente 1 tile (modo tabuleiro)
        //associated.box.x += speed.x * tileWidth;
        //associated.box.y += speed.y * tileHeight;

        //Personagem desliza
        dashStartPos = associated.box.Pos();  // posição atual (top-left)
        dashTargetPos = dashStartPos + Vec2(speed.x * tileWidth, speed.y * tileHeight);

        isDashing = true;
        dashTimer.Restart();
        walkTimer.Restart();
            // Set animação de caminhada
        if (currentDirection == "up")
            animator->SetAnimation("walk_back");
        else if (currentDirection == "down")
            animator->SetAnimation("walk_front");
        else if (currentDirection == "left")
            animator->SetAnimation("walk_side_left");
        else
            animator->SetAnimation("walk_side_right");

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


    // Dash em andamento
    if (isDashing) {
        float t = std::min(dashTimer.Get() / dashDuration, 1.0f);
        Vec2 newPos = dashStartPos * (1 - t) + dashTargetPos * t;
        associated.box.SetPos(newPos);

        if (t >= 1.0f) {
            isDashing = false;

            // ✅ Transição imediata para animação idle ao fim do dash
            if (currentDirection == "up")
                animator->SetAnimation("idle_back");
            else if (currentDirection == "down")
                animator->SetAnimation("idle_front");
            else if (currentDirection == "left")
                animator->SetAnimation("idle_side_left");
            else
                animator->SetAnimation("idle_side_right");
        }
    }

    if (playingHitAnim) {
    hitTimer.Update(dt);
    if (hitTimer.Get() > 0.3f) { // tempo da animação de hit
        playingHitAnim = false;
        Animator* animator = static_cast<Animator*>(associated.GetComponent("Animator"));
        if (animator) {
            animator->SetAnimation("idle_front");
        }
    }
}



    const float mapStartX = 160;
    const float mapStartY = 310;
    const float mapEndX = 800;
    const float mapEndY = 628;

    associated.box.x = std::max(mapStartX, std::min(associated.box.x, mapEndX - associated.box.w));
    associated.box.y = std::max(mapStartY, std::min(associated.box.y, mapEndY - associated.box.h));


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
    Animator* animator = static_cast<Animator*>(associated.GetComponent("Animator"));

    if (hp <= 0 || isDead) {
        return; // Já está morto ou não pode receber dano
    }

    animator->SetAnimation("hit");
    animator->SetLoop(false);
    playingHitAnim = true;
    hitTimer.Restart();

    hp -= damage;
    hitSound.Play(1);


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
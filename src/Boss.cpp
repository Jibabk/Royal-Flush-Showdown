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
#include <StageState.h>
#include <CardSpringAttack.h>
#include <iostream>

Boss* Boss::chefe = nullptr;
Vec2 inicialHandPos = Vec2(160, 40);

Boss::Boss(GameObject& associated, std::string sprite)
    : Component(associated), speed(0, 0), linearSpeed(0), hp(100), maxHp(100), currentHp(100),
      deathTimer(), damageCooldown(),isDead(false),deathSound("Recursos/audio/Dead.wav"),hitSound("Recursos/audio/Hit0.wav") {

    
    SpriteRenderer* spriteRenderer = new SpriteRenderer(associated, sprite, 2, 1);
    associated.AddComponent(new Collider(associated));
    spriteRenderer->SetScale(2,2); // Ajusta o tamanho do sprite
    associated.AddComponent(spriteRenderer);

    associated.box.w = spriteRenderer->GetWidth();
    associated.box.h = spriteRenderer->GetHeight();

    Animator* animator = new Animator(associated);
    animator->AddAnimation("actionIdle", Animation(0, 0, 9, SDL_FLIP_NONE));
    animator->AddAnimation("cardIdle", Animation(1, 1, 9, SDL_FLIP_NONE));
    animator->SetAnimation("cardIdle");
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


    GameObject* gunGO1 = new GameObject();
    gunGO1->box = associated.box;
    gunGO1->AddComponent(new Gun(*gunGO1, state.GetObjectPtr(&associated),Vec2(-inicialHandPos.x, inicialHandPos.y), false));
    gunLeft = state.AddObject(gunGO1);

    GameObject* gunGO2 = new GameObject();
    gunGO2->box = associated.box;
    gunGO2->AddComponent(new Gun(*gunGO2, state.GetObjectPtr(&associated),Vec2(inicialHandPos.x, inicialHandPos.y), true));
    gunRight = state.AddObject(gunGO2);

    
}


void Boss::Update(float dt) {
    Animator* animator = static_cast<Animator*>(associated.GetComponent("Animator"));
    auto& state = Game::GetInstance().GetCurrentState();
    StageState* stage = dynamic_cast<StageState*>(&state);

    
    damageCooldown.Update(dt);
    if (hp <= 0) {
        auto gunPtrL = gunLeft.lock();
        auto gunPtrR = gunRight.lock();
        if (gunPtrL) {
            gunPtrL->RequestDelete();
        }
        if (gunPtrR) {
            gunPtrR->RequestDelete();
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
    if(stage->GetCurrentMode() == StageState::CARD_MODE) {
        auto gunA = gunLeft.lock();
        auto gunB = gunRight.lock();
        if (gunA && gunB) {
            Gun* gunCompA = (Gun*)gunA->GetComponent("Gun");
            Gun* gunCompB = (Gun*)gunB->GetComponent("Gun");
            if (gunCompA) gunCompA->ResetOffset();
            if (gunCompB) gunCompB->ResetOffset();
        }
        // Se estiver no modo de cartas, não atualiza o Boss
        waitingHandsReturn = false;
        preparingCardSpring = false;
        if (animator) {
            animator->SetAnimation("cardIdle");
        }
        speed = Vec2(0, 0);
        linearSpeed = 0;
        
        return;
    }else{

    if (waitingToReturnHands) {
        auto gunA = gunLeft.lock();
        auto gunB = gunRight.lock();

        if (gunA && gunB) {
            Gun* gunCompA = (Gun*)gunA->GetComponent("Gun");
            Gun* gunCompB = (Gun*)gunB->GetComponent("Gun");


            if (gunCompA && gunCompB &&
                gunCompA->HasReachedDefaultOffset() && gunCompB->HasReachedDefaultOffset()) {
                waitingToReturnHands = false;
            }
        }
    }


    while (!taskQueue.empty() && !isDead && !waitingToReturnHands && !preparingCardSpring) {
        Command cmd = taskQueue.front();
        taskQueue.pop();

        if (cmd.type == Command::MOVE) {
            speed = (cmd.pos - associated.box.Center()).Normalized();
            linearSpeed = 200;
        } if (cmd.type == Command::SHOOT_LEFT) {
            if (auto gunPtr = gunLeft.lock()) {
                auto* gunComp = (Gun*)gunPtr->GetComponent("Gun");
                if (gunComp) gunComp->Shoot(cmd.pos);
            }
        } else if (cmd.type == Command::SHOOT_RIGHT) {
            if (auto gunPtr = gunRight.lock()) {
                auto* gunComp = (Gun*)gunPtr->GetComponent("Gun");
                if (gunComp) gunComp->Shoot(cmd.pos);
            }
        } else if (cmd.type == Command::SHOOT_BOTH) {
            if (auto gunA = gunLeft.lock()) {
                auto* gunCompA = (Gun*)gunA->GetComponent("Gun");
                if (gunCompA) gunCompA->Shoot(cmd.pos);
            }
            if (auto gunB = gunRight.lock()) {
                auto* gunCompB = (Gun*)gunB->GetComponent("Gun");
                if (gunCompB) gunCompB->Shoot(cmd.pos);
            } 
        } else if (cmd.type == Command::POSITION_HANDS_FOR_LASER) {
            auto gunA = gunLeft.lock();
            auto gunB = gunRight.lock();

            if (gunA && gunB) {
                Gun* gunCompA = static_cast<Gun*>(gunA->GetComponent("Gun"));
                Gun* gunCompB = static_cast<Gun*>(gunB->GetComponent("Gun"));
                if (gunCompA && gunCompB) {
                    Vec2 center = associated.box.Center();
                    Vec2 playerPos = Character::player->GetAssociated().box.Center();

                    Vec2 finalOffsetA(160 - 60 - associated.box.Center().x, cmd.pos.y - 100);
                    Vec2 finalOffsetB(800 + 100 - associated.box.Center().x, cmd.pos.y - 100);

                    Vec2 midOffsetA = Vec2((finalOffsetA.x + 0) / 1.2, inicialHandPos.y - 10); // primeiro ponto: abaixo
                    Vec2 preFinalA  = Vec2(finalOffsetA.x, midOffsetA.y - 15);           // segundo ponto: acima do final

                    Vec2 midOffsetB = Vec2((finalOffsetB.x + 0) / 1.2, inicialHandPos.y - 10);
                    Vec2 preFinalB  = Vec2(finalOffsetB.x, midOffsetB.y - 15);

                    gunCompA->SetOffsetPath({midOffsetA, preFinalA, finalOffsetA});
                    gunCompB->SetOffsetPath({midOffsetB, preFinalB, finalOffsetB});

                    preparingCardSpring = true;
                }

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
        animator->SetAnimation("actionIdle");
        
    }

    

    // Reset velocidade
    linearSpeed = 0;
    speed = Vec2(0, 0);


if (preparingCardSpring) {
    auto gunA = gunLeft.lock();
    auto gunB = gunRight.lock();

    if (gunA && gunB) {
        Gun* gunCompA = static_cast<Gun*>(gunA->GetComponent("Gun"));
        Gun* gunCompB = static_cast<Gun*>(gunB->GetComponent("Gun"));

        if (gunCompA && gunCompB) {
        bool reachedA = gunCompA->UpdateOffsetPath(dt);
        bool reachedB = gunCompB->UpdateOffsetPath(dt);

        if (reachedA && reachedB) {
            // Avança para o próximo passo do caminho
            bool nextA = gunCompA->AdvanceOffsetStep();
            bool nextB = gunCompB->AdvanceOffsetStep();

            if (!nextA && !nextB) {
                // Ambas chegaram no final do caminho
                Vec2 start = gunA->box.Center();
                Vec2 end = gunB->box.Center();

                GameObject* laserGO = new GameObject();
                laserGO->AddComponent(new CardSpringAttack(*laserGO, start, end));
                state.AddObject(laserGO);

                preparingCardSpring = false;
                waitingToReturnHands = true;
                handReturnTimer.Restart();
            }
        }
    }

    }
}



if (waitingToReturnHands && !preparingCardSpring) {
    handReturnTimer.Update(dt);
    if (handReturnTimer.Get() > 1.2f) {
        auto gunA = gunLeft.lock();
        auto gunB = gunRight.lock();
        if (gunA && gunB) {
            Gun* gunCompA = (Gun*)gunA->GetComponent("Gun");
            Gun* gunCompB = (Gun*)gunB->GetComponent("Gun");
            if (gunCompA) gunCompA->ResetOffset();
            if (gunCompB) gunCompB->ResetOffset();
        }
    }
}



    
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
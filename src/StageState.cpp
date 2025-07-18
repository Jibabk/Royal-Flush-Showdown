#include "StageState.h"
#include "Game.h"
#include "TileMap.h"
#include "Zombie.h"
#include "WaveSpawner.h"
#include "AiController.h"
#include "SpriteRenderer.h"
#include "Character.h"
#include "Camera.h"
#include "PlayerController.h"
#include "Collider.h"
#include "InputManager.h"
#include "Collision.cpp"
#include <iostream>
#include <GameData.h>
#include "EndState.h"
#include "Boss.h"
#include "BossAiController.h"
#include "CardComponent.h"
#include <Deck.h>
#include <Card.h>
#include <PokerHand.h>

StageState::StageState() : backgroundMusic("Recursos/audio/BGM.wa"),deck(Deck()),cards() {
    // Background
    GameObject* bg = new GameObject();
    SpriteRenderer* bgRenderer = new SpriteRenderer(*bg, "Recursos/img/background1.png");
    bgRenderer->SetScale(2, 2);
    bgRenderer->SetCameraFollower(true);
    bg->AddComponent(bgRenderer);
    AddObject(bg);
    currentMode = CARD_MODE;
    modeTimer.Restart();


    // Arena
    //GameObject* arena = new GameObject();
    //SpriteRenderer* arenaRenderer = new SpriteRenderer(*arena, "Recursos/img/arena.png");
    //arena-> box.x = 160;
    //arena-> box.y = 340;
    //arenaRenderer->SetScale(2, 2);
    //arenaRenderer->SetCameraFollower(true);
    //arena->AddComponent(arenaRenderer);
    //AddObject(arena);



    // TileSet e TileMap
    //tileSet = new TileSet(64, 64, "Recursos/img/Tileset.png");
    //GameObject* mapGO = new GameObject();
    //TileMap* tileMap = new TileMap(*mapGO, "Recursos/map/map.txt", tileSet);
    ///mapGO->AddComponent(tileMap);
    //AddObject(mapGO);

    // Personagem (Player)
    GameObject* playerGO = new GameObject();
    playerGO->box.x = 320;
    playerGO->box.y = 480;
    playerGO->AddComponent(new Character(*playerGO, "Recursos/img/protag.spritesheet-v3.png"));
    playerGO->AddComponent(new PlayerController(*playerGO));
    AddObject(playerGO);
    //Camera::Follow(playerGO);

    // Chefão (Boss)
    GameObject* BossGO = new GameObject();
    BossGO->box.x = 360;
    BossGO->box.y = 50;
    BossGO->AddComponent(new Boss(*BossGO, "Recursos/img/boss_v2.png"));
    BossGO->AddComponent(new BossAiController(*BossGO));
    AddObject(BossGO);
    //Camera::Follow(BossGO);


    // Spawner
    GameObject* spawnerGO = new GameObject();
    spawnerGO->AddComponent(new WaveSpawner(*spawnerGO));
    AddObject(spawnerGO);




    backgroundMusic.Play();
}

StageState::~StageState() {
    objectArray.clear();
}

void StageState::LoadAssets() {
    // Se precisar carregar assets externos além dos que estão no construtor
}

void StageState::Start() {
    LoadAssets();
    StartArray();
    started = true;
}

void StageState::Pause() {}

void StageState::Resume() {}

void StageState::Update(float dt) {

    modeTimer.Update(dt);

    if (modeTimer.Get() > modeDuration) {
        if (currentMode == CARD_MODE) {
            currentMode = ACTION_MODE;
            //std::cout << "[MODO] Ação iniciada!" << std::endl;
        } else {
            currentMode = CARD_MODE;
            std::cout << "[MODO] Cartas iniciada!" << std::endl;
        }

        modeTimer.Restart();
    }

    if (currentMode == CARD_MODE && !cardsSpawned) {
        cards.clear();

        for (int i = 0; i < 5; i++) {
            Card card = deck.Draw();
            GameObject* cardGO = new GameObject();
            cardGO->box.x = 100 + i * 150;
            cardGO->box.y = 800;
            

            SpriteRenderer* renderer = new SpriteRenderer(*cardGO, card.GetImagePath(), 1, 1);
            renderer->SetScale(1, 1);
            cardGO->AddComponent(renderer);

            CardComponent* cardComp = new CardComponent(*cardGO, card);
            cardComp->selected = true;  // Inicialmente não selecionada
            cardGO->AddComponent(cardComp);

            std::weak_ptr<GameObject> weak = AddObject(cardGO);
            if (auto shared = weak.lock()) {
                cards.push_back(shared);
            }
        }

        cardsSpawned = true;
    }

    if (currentMode == ACTION_MODE) {
        //std::cout << "[MODO] Ação iniciada!" << std::endl;

        // Remove cartas
        for (auto& card : cards) {
            if (card) card->RequestDelete();
        }
        cards.clear();
        cardsSpawned = false;
    }


    
    InputManager& input = InputManager::GetInstance();

    //std::cout << input.KeyPress(ESCAPE_KEY) << std::endl;
    if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
        popRequested = true;
    }

    //if (input.KeyPress(SDLK_SPACE)) {
    //    GameObject* zombie = new GameObject();
    //    zombie->box.x = input.GetMouseX() + Camera::pos.x;
    //    zombie->box.y = input.GetMouseY() + Camera::pos.y;
    //    zombie->AddComponent(new Zombie(*zombie));
    //    AddObject(zombie);
    //}

    // ENTER -> selecionar tudo e avaliar
    if (input.KeyPress(SDLK_RETURN)) {
        std::vector<Card> selectedCards;
        for (int i=0; i < cards.size(); i++) {
            auto comp = (CardComponent*)cards[i]->GetComponent("CardComponent");
            if (comp) {
                //comp->selected = true;
                selectedCards.push_back(comp->card);  // Supondo que você tenha o atributo 'card'
            }
        }

        if (selectedCards.size() == 5) {
            PokerHand hand(selectedCards);
            PokerHand::HandRank result = hand.Evaluate(hand);
            std::cout << "[Poker] Mão avaliada: " << PokerHand::ToString(result) << std::endl;
            // Aplica dano ao Boss
            int damage = PokerHand::PokerHandToDamage(result);
            if (Boss::chefe) {
                Boss::chefe->TakeDamage(damage);
                std::cout << "[Boss] Recebeu " << damage << " de dano!\n";
            }
        }

        currentMode = ACTION_MODE;
        modeTimer.Restart();
        //std::cout << "[MODO] Ação iniciada!" << std::endl;
    }

    // X -> descartar a carta selecionada e comprar nova
    if (input.KeyPress(SDLK_x)) {
        for (size_t i = 0; i < cards.size(); ++i) {
            auto comp = (CardComponent*)cards[i]->GetComponent("CardComponent");
            if (comp && comp->selected) {
                cards[i]->RequestDelete();

               // Supõe que 'i' é o índice da carta a ser substituída
                Card newCard = deck.Draw();
                GameObject* newGO = new GameObject();
                newGO->box = cards[i]->box;  // mantém a posição anterior

                SpriteRenderer* renderer = new SpriteRenderer(*newGO, newCard.GetImagePath(), 1, 1);
                renderer->SetScale(1, 1);
                newGO->AddComponent(renderer);
                newGO->AddComponent(new CardComponent(*newGO, newCard));

                std::weak_ptr<GameObject> weak = AddObject(newGO);
                if (auto shared = weak.lock()) {
                    cards[i]->RequestDelete();  // descarta a anterior
                    cards[i] = shared;          // substitui pela nova
                }

                break;  // só troca uma carta
            }
        }
    }



    UpdateArray(dt);

    // Checar colisões
    for (size_t i = 0; i < objectArray.size(); ++i) {
        auto colliderA = (Collider*)objectArray[i]->GetComponent("Collider");
        if (!colliderA) continue;

        for (size_t j = i + 1; j < objectArray.size(); ++j) {
            Collider* colliderB = (Collider*)objectArray[j]->GetComponent("Collider");
            if (!colliderB) continue;

            if (Collision::IsColliding(
                colliderA->box, colliderB->box,
                objectArray[i]->angleDeg,
                objectArray[j]->angleDeg)) {
                
                objectArray[i]->NotifyCollision(*objectArray[j]);
                objectArray[j]->NotifyCollision(*objectArray[i]);
            }
        }
    }
    // Remove objetos mortos
    for (size_t i = 0; i < objectArray.size(); ) {
        if (objectArray[i]->IsDead()) {
            objectArray.erase(objectArray.begin() + i);
        } else {
            ++i;
        }
    }
    if (Character::player == nullptr) { 
        GameData::playerVictory = false;
        popRequested = true;
        Game::GetInstance().Push(new EndState());
    }
    if (Boss::chefe == nullptr) { 
        GameData::playerVictory = true;
        popRequested = true;
        Game::GetInstance().Push(new EndState());
}

}

void StageState::Render() {
    bg.Render(0, 0, 0);
    RenderArray();
}

void StageState::DeselectAllCards() {
    for (auto& card : cards) {
        auto cardComp = (CardComponent*)card->GetComponent("CardComponent");
        if (cardComp) {
            cardComp->selected = false;
        }
    }
}

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

StageState::StageState() : backgroundMusic("Recursos/audio/gameplay.wav"),deck(Deck()),cards() {
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




    backgroundMusic.SetVolume(64); // Define o volume da música para 100%, volume de 0 a 128
    backgroundMusic.Play(); // Toca a música em loop
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

    float screenW = Game::GetInstance().GetWidth();
    float screenH = Game::GetInstance().GetHeight();

    if (modeTimer.Get() > modeDuration) {
        if (currentMode == CARD_MODE) {
            currentMode = ACTION_MODE;
        } else {
            currentMode = CARD_MODE;
            std::cout << "[MODO] Cartas iniciada!" << std::endl;
        }
        modeTimer.Restart();
    }

    if (currentMode == CARD_MODE && !cardsSpawned) {
        GameObject* bg = new GameObject();
        SpriteRenderer* bgSprite = new SpriteRenderer(*bg, "Recursos/img/fundo_placeholder.png");
        bgSprite->SetCameraFollower(true);

        float imageW = bgSprite->GetWidth(), imageH = bgSprite->GetHeight();
        bgSprite->SetScale(screenW / imageW, screenH / imageH);
        bg->AddComponent(bgSprite);
        cardHudObjects.push_back(AddObject(bg));

        GameObject* hudBoss = new GameObject();
        hudBoss->box.x = 360; hudBoss->box.y = 50;
        SpriteRenderer* bossRenderer = new SpriteRenderer(*hudBoss, "Recursos/img/boss_v2.png", 2, 1);
        bossRenderer->SetFrame(1, SDL_FLIP_NONE);
        bossRenderer->SetScale(2, 2);
        hudBoss->AddComponent(bossRenderer);
        cardHudObjects.push_back(AddObject(hudBoss));

        cards.clear();
        const int CARD_WIDTH = 100, CARD_SPACING = 30, TOTAL_WIDTH = 5 * CARD_WIDTH + 4 * CARD_SPACING;
        const int START_X = (screenW - TOTAL_WIDTH) / 2, CARD_Y = 350;

        for (int i = 0; i < 5; i++) {
            Card card = deck.Draw();
            GameObject* cardGO = new GameObject();
            cardGO->box.x = START_X + i * (CARD_WIDTH + CARD_SPACING);
            cardGO->box.y = CARD_Y;

            auto* renderer = new SpriteRenderer(*cardGO, card.GetImagePath(), 1, 1);
            renderer->SetScale(1.5, 1.5);
            cardGO->AddComponent(renderer);

            auto* cardComp = new CardComponent(*cardGO, card);
            cardComp->selected = true;
            cardGO->AddComponent(cardComp);

            std::weak_ptr<GameObject> weak = AddObject(cardGO);
            if (auto shared = weak.lock()) cards.push_back(shared);
        }

        const int BUTTON_WIDTH = 100, BUTTON_SPACING = 50;
        const int BUTTON_Y = 350 + 170;
        int centerX = screenW / 2;

        GameObject* discardButton = new GameObject();
        discardButton->box.x = centerX - BUTTON_WIDTH - BUTTON_SPACING / 2;
        discardButton->box.y = BUTTON_Y;
        auto* discardRenderer = new SpriteRenderer(*discardButton, "Recursos/img/btn_descartar.png", 2, 1);
        discardRenderer->SetFrame(0, SDL_FLIP_NONE);
        discardRenderer->SetScale(1.5, 1.5);
        discardButton->AddComponent(discardRenderer);
        discardButtonRef = AddObject(discardButton);
        cardHudObjects.push_back(discardButtonRef);

        GameObject* playButton = new GameObject();
        playButton->box.x = centerX + BUTTON_SPACING / 2;
        playButton->box.y = BUTTON_Y;
        auto* playRenderer = new SpriteRenderer(*playButton, "Recursos/img/btn_confirmar.png", 2, 1);
        playRenderer->SetFrame(0, SDL_FLIP_NONE);
        playRenderer->SetScale(1.5, 1.5);
        playButton->AddComponent(playRenderer);
        cardHudObjects.push_back(AddObject(playButton));

        cardsSpawned = true;
    }

    InputManager& input = InputManager::GetInstance();

if (currentMode == CARD_MODE && discardButtonRef.lock()) {
    int mouseX = input.GetMouseX(), mouseY = input.GetMouseY();
    auto discardButton = discardButtonRef.lock();
    if (discardButton->box.Contains(Vec2((float)mouseX, (float)mouseY)) && input.MousePress(LEFT_MOUSE_BUTTON)) {
        for (size_t i = 0; i < cards.size(); ++i) {
            auto& card = cards[i];
            auto comp = (CardComponent*)card->GetComponent("CardComponent");
            if (comp && comp->selected) {
                if (deck.IsEmpty()) return; // Protege contra deck vazio

                Rect oldBox = card->box;
                Card newCard = deck.Draw();
                GameObject* newGO = new GameObject();
                newGO->box = oldBox;

                auto* renderer = new SpriteRenderer(*newGO, newCard.GetImagePath(), 1, 1);
                renderer->SetScale(1.5, 1.5);
                newGO->AddComponent(renderer);
                newGO->AddComponent(new CardComponent(*newGO, newCard));

                std::weak_ptr<GameObject> weak = AddObject(newGO);

                card->RequestDelete();
                cards[i].reset();

                if (auto shared = weak.lock()) cards[i] = shared;

                // Desmarca todas as seleções
                for (auto& c : cards) {
                    if (c) {
                        auto cc = (CardComponent*)c->GetComponent("CardComponent");
                        if (cc) cc->selected = false;
                    }
                }
                break; // só permite descartar 1 por vez
            }
        }
    }
}

    if (currentMode == ACTION_MODE) {
        std::cout << "[MODO] Ação iniciada!" << std::endl;
        for (auto& obj : cardHudObjects) {
            if (auto shared = obj.lock()) shared->RequestDelete();
        }
        cardHudObjects.clear();

        for (auto& card : cards) {
            if (card) card->RequestDelete();
        }
        cards.clear();
        cardsSpawned = false;
    }

    if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
        popRequested = true;
    }

    if (input.KeyPress(SDLK_KP_PLUS) || input.KeyPress(SDLK_PLUS)) {
        int currentVol = Mix_VolumeMusic(-1);
        backgroundMusic.SetVolume(currentVol + 8);
    }
    if (input.KeyPress(SDLK_KP_MINUS) || input.KeyPress(SDLK_MINUS)) {
        int currentVol = Mix_VolumeMusic(-1);
        backgroundMusic.SetVolume(currentVol - 8);
    }

    if (input.KeyPress(SDLK_RETURN)) {
        std::vector<Card> selectedCards;
        for (const auto& card : cards) {
            auto* comp = (CardComponent*)card->GetComponent("CardComponent");
            if (comp) selectedCards.push_back(comp->card);
        }

        if (selectedCards.size() == 5) {
            PokerHand hand(selectedCards);
            PokerHand::HandRank result = hand.Evaluate(hand);
            std::cout << "[Poker] Mão avaliada: " << PokerHand::ToString(result) << std::endl;
            int damage = PokerHand::PokerHandToDamage(result);
            if (Boss::chefe) {
                Boss::chefe->TakeDamage(damage);
                std::cout << "[Boss] Recebeu " << damage << " de dano!\n";
            }
        }

        currentMode = ACTION_MODE;
        modeTimer.Restart();
    }

    // ✅ Seleção de cartas com clique
    if (currentMode == CARD_MODE) {
        if (input.MousePress(LEFT_MOUSE_BUTTON)) {
            int mouseX = input.GetMouseX(), mouseY = input.GetMouseY();
            for (auto& card : cards) {
                if (!card) continue;
                Rect box = card->box;
                if (box.Contains(Vec2((float)mouseX, (float)mouseY))) {
                    auto* comp = (CardComponent*)card->GetComponent("CardComponent");
                    if (comp) comp->selected = !comp->selected;
                }
            }
        }

        const int CARD_Y = 350;
        const int OFFSET_Y = 20;
        for (auto& card : cards) {
            auto* comp = (CardComponent*)card->GetComponent("CardComponent");
            if (comp) {
                card->box.y = comp->selected ? CARD_Y + OFFSET_Y : CARD_Y;
            }
        }

        return; // ❗ importante: não processa lógica de ação enquanto estiver no modo de cartas
    }

    UpdateArray(dt);

    for (size_t i = 0; i < objectArray.size(); ++i) {
        auto colliderA = (Collider*)objectArray[i]->GetComponent("Collider");
        if (!colliderA) continue;

        for (size_t j = i + 1; j < objectArray.size(); ++j) {
            auto colliderB = (Collider*)objectArray[j]->GetComponent("Collider");
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

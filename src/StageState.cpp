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
            //std::cout << "[MODO] Ação iniciada!" << std::endl;
        } else {
            currentMode = CARD_MODE;
            std::cout << "[MODO] Cartas iniciada!" << std::endl;
        }

        modeTimer.Restart();
    }

if (currentMode == CARD_MODE && !cardsSpawned) {


    // Fundo com imagem completa do menu
    GameObject* bg = new GameObject();
    SpriteRenderer* bgSprite = new SpriteRenderer(*bg, "Recursos/img/fundo_placeholder.png");
    bgSprite->SetCameraFollower(true);

    // Ajusta escala da imagem para o tamanho da tela
    float screenW = Game::GetInstance().GetWidth();
    float screenH = Game::GetInstance().GetHeight();
    float imageW = bgSprite->GetWidth();
    float imageH = bgSprite->GetHeight();

    float scaleX = screenW / imageW;
    float scaleY = screenH / imageH;

    bgSprite->SetScale(scaleX, scaleY);
    bg->AddComponent(bgSprite);
    cardHudObjects.push_back(AddObject(bg));

    // Boss no topo
    GameObject* hudBoss = new GameObject();
    hudBoss->box.x = 360;
    hudBoss->box.y = 50;
    // centralizado
    SpriteRenderer* bossRenderer = new SpriteRenderer(*hudBoss, "Recursos/img/boss_v2.png",2,1);
    bossRenderer->SetFrame(1,SDL_FLIP_NONE);          // <- segundo sprite
    bossRenderer->SetScale(2, 2); // dobra o tamanho
    hudBoss->AddComponent(bossRenderer);

    cardHudObjects.push_back(AddObject(hudBoss));

    // Limpa cartas anteriores (por segurança)
    cards.clear();

    // Posição das cartas no centro da tela
    const int CARD_WIDTH = 100;     // ajuste conforme o sprite real
    const int CARD_SPACING = 30;
    const int TOTAL_WIDTH = 5 * CARD_WIDTH + 4 * CARD_SPACING;
    const int START_X = (screenW - TOTAL_WIDTH) / 2;
    const int CARD_Y = 350;

    for (int i = 0; i < 5; i++) {
        Card card = deck.Draw();
        GameObject* cardGO = new GameObject();

        // Alinha horizontalmente
        cardGO->box.x = START_X + i * (CARD_WIDTH + CARD_SPACING);
        cardGO->box.y = CARD_Y;

        SpriteRenderer* renderer = new SpriteRenderer(*cardGO, card.GetImagePath(), 1, 1);
        renderer->SetScale(1.5, 1.5);  // ajuste se necessário
        cardGO->AddComponent(renderer);

        CardComponent* cardComp = new CardComponent(*cardGO, card);
        cardComp->selected = true;
        cardGO->AddComponent(cardComp);

        std::weak_ptr<GameObject> weak = AddObject(cardGO);
        if (auto shared = weak.lock()) {
            cards.push_back(shared);
        }
    }

        // Botões abaixo das cartas
    const int BUTTON_WIDTH = 100;
    const int BUTTON_HEIGHT = 50;
    const int BUTTON_SPACING = 50;
    const int BUTTON_Y = CARD_Y + 170;  // distância abaixo das cartas
    int centerX = screenW / 2;


    // Botão "Descartar"
    GameObject* discardButton = new GameObject();
    discardButton->box.x = centerX - BUTTON_WIDTH - BUTTON_SPACING / 2;
    discardButton->box.y = BUTTON_Y;
    SpriteRenderer* discardRenderer = new SpriteRenderer(*discardButton, "Recursos/img/btn_descartar.png",2,1);
    discardRenderer->SetFrame(0,SDL_FLIP_NONE);
    discardRenderer->SetScale(1.5, 1.5);  // se quiser aumentar
    discardButton->AddComponent(discardRenderer);
    cardHudObjects.push_back(AddObject(discardButton));

    // Botão "Jogar"
    GameObject* playButton = new GameObject();
    playButton->box.x = centerX + BUTTON_SPACING / 2;
    playButton->box.y = BUTTON_Y;
    SpriteRenderer* playRenderer = new SpriteRenderer(*playButton, "Recursos/img/btn_confirmar.png",2,1);
    playRenderer->SetFrame(0,SDL_FLIP_NONE);
    playRenderer->SetScale(1.5, 1.5);
    playButton->AddComponent(playRenderer);
    cardHudObjects.push_back(AddObject(playButton));


    cardsSpawned = true;
}

    if (currentMode == ACTION_MODE) {
        std::cout << "[MODO] Ação iniciada!" << std::endl;
        for (auto& obj : cardHudObjects) {
    if (auto shared = obj.lock()) shared->RequestDelete();
    }
    cardHudObjects.clear();

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

    // Ajuste de volume com teclas
    if (input.KeyPress(SDLK_KP_PLUS) || input.KeyPress(SDLK_PLUS)) {
    int currentVol = Mix_VolumeMusic(-1);
    backgroundMusic.SetVolume(currentVol + 8);
    std::cout << "Volume atual: " << Mix_VolumeMusic(-1) << std::endl;
    }
    if (input.KeyPress(SDLK_KP_MINUS) || input.KeyPress(SDLK_MINUS)) {
    int currentVol = Mix_VolumeMusic(-1);
    backgroundMusic.SetVolume(currentVol - 8);
    std::cout << "Volume atual: " << Mix_VolumeMusic(-1) << std::endl;
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
                renderer->SetScale(2, 2);
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

    if (currentMode == CARD_MODE) {
    return; // cancela o update de comportamento
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

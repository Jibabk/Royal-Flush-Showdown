#ifndef STAGESTATE_H
#define STAGESTATE_H

#include "State.h"
#include "TileSet.h"
#include "Music.h"
#include "Deck.h"
#include "Timer.h"
class StageState : public State {
public:
    StageState();
    ~StageState();

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;
    void Start() override;
    void Pause() override;
    void Resume() override;
    void DeselectAllCards();
    enum GameMode { CARD_MODE, ACTION_MODE };
    GameMode GetCurrentMode() const { return currentMode; }

private:
    TileSet* tileSet;
    Music backgroundMusic;
    Sprite bg;
    Deck deck;
    std::vector<std::shared_ptr<GameObject>> cards;
    GameMode currentMode;
    Timer modeTimer;
    float modeDuration = 10.0f; // tempo de duração de cada fase
    bool cardsSpawned = false;

};

#endif // STAGESTATE_H

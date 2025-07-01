#ifndef STAGESTATE_H
#define STAGESTATE_H

#include "State.h"
#include "TileSet.h"
#include "Music.h"
#include "Deck.h"

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

private:
    TileSet* tileSet;
    Music backgroundMusic;
    Sprite bg;
    Deck deck;
    std::vector<std::shared_ptr<GameObject>> cards;

};

#endif // STAGESTATE_H

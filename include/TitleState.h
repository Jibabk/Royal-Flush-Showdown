#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include "State.h"
#include "Music.h"
#include "Timer.h"

class TitleState : public State {
public:
    TitleState();
    ~TitleState();

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;
    void Start() override;
    void Pause() override;
    void Resume() override;

private:
    bool showText = true;
    Music backgroundMusic;
    Timer textTimer;

};

#endif // TITLE_STATE_H
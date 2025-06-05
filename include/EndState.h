#ifndef ENDSTATE_H
#define ENDSTATE_H

#include "State.h"
#include "Music.h"
#include "GameObject.h"
#include "Text.h"
#include "Timer.h"

class EndState : public State {
public:
    EndState();
    ~EndState();

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;
    void Start() override;
    void Pause() override;
    void Resume() override;

private:
    Music backgroundMusic;
    bool showText = true;
    Timer textTimer;
};

#endif

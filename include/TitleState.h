#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include "State.h"
#include "GameObject.h"
#include "Rect.h"

class TitleState : public State {
public:
    TitleState();
    ~TitleState();

    void Start() override;
    void Pause() override;
    void Resume() override;
    void LoadAssets() override;

    void Update(float dt) override;
    void Render() override;

private:
    bool showText = true;
    Rect jogarArea;
    Music backgroundMusic;
    Rect sairArea;
    Timer textTimer;

};

#endif // TITLE_STATE_H
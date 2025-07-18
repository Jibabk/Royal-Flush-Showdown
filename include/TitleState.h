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
    Rect jogarArea;
    Rect sairArea;
};

#endif

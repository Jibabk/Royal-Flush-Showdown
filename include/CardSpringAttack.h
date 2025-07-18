// CardSpringAttack.h
#pragma once
#include "Component.h"
#include "Timer.h"
#include "Vec2.h"

class CardSpringAttack : public Component {
public:
    CardSpringAttack(GameObject& associated, Vec2 start, Vec2 end);

    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) const override;

private:
    Timer durationTimer;
    Vec2 startPos;
    Vec2 endPos;
    bool active;
};

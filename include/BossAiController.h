#ifndef BOSSAICONTROLLER_H
#define BOSSAICONTROLLER_H

#include "Component.h"
#include "Timer.h"
#include "Vec2.h"
#include <string>

class BossAiController : public Component {
public:
    BossAiController(GameObject& associated);

    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) const override;

private:
    enum AiState { MOVING, RESTING };

    AiState state;
    Timer restTimer;
    Vec2 destination;

    static constexpr float moveSpeed = 150.0f;
    static constexpr float restCooldown = 1.5f;
    static constexpr float proximityThreshold = 30.0f;
};

#endif // AICONTROLLER_H

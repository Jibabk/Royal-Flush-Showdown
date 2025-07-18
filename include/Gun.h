#ifndef GUN_H
#define GUN_H

#include <memory>
#include "Component.h"
#include "Timer.h"
#include "Sound.h"
#include "Vec2.h"
#include <vector>

class Gun : public Component {
public:
    Gun(GameObject& associated, std::weak_ptr<GameObject> character, Vec2 relativeOffset = Vec2(), bool rightHand = false);
    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) const override;
    void Shoot(Vec2 target);
    void SetOffset(Vec2 offset);
    void ResetOffset();
    bool HasReachedTarget() const;
    bool HasReachedDefaultOffset() const;
    void SetOffsetPath(const std::vector<Vec2>& path);
    bool UpdateOffsetPath(float dt);
    bool AdvanceOffsetStep();

private:
    Sound shotSound;
    Sound reloadSound;
    int cooldown;
    Timer cdTimer;
    std::weak_ptr<GameObject> character;
    float angle;
    Vec2 relativeOffset;        // posição atual
    Vec2 targetOffset;          // posição desejada
    float offsetLerpSpeed = 400.0f; // velocidade do movimento em pixels por segundo
    Vec2 defaultOffset; // posição inicial
    std::vector<Vec2> offsetPath;
    int currentOffsetIndex = 0;
    bool rightHand;




};

#endif

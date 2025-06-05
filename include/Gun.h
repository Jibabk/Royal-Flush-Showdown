#ifndef GUN_H
#define GUN_H

#include <memory>
#include "Component.h"
#include "Timer.h"
#include "Sound.h"
#include "Vec2.h"

class Gun : public Component {
public:
    Gun(GameObject& associated, std::weak_ptr<GameObject> character);
    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) const override;
    void Shoot(Vec2 target);

private:
    Sound shotSound;
    Sound reloadSound;
    int cooldown;
    Timer cdTimer;
    std::weak_ptr<GameObject> character;
    float angle;
};

#endif

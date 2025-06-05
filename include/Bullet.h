#ifndef BULLET_H
#define BULLET_H

#include <memory>
#include "Component.h"
#include "Timer.h"
#include "Sound.h"
#include "Vec2.h"

class Bullet : public Component {
public:
    Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance,bool targetsPlayer);
    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) const override;
    int GetDamage();
    void NotifyCollision(GameObject& other) override;
    bool targetsPlayer;
private:
    Vec2 speed;
    int damage;
    float distanceLeft;
};

#endif

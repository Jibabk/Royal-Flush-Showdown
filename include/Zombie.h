#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Component.h"
#include "Sound.h"
#include <InputManager.h>
#include "Timer.h"

class Zombie : public Component {
public:
    Zombie(GameObject& associated);

    void Damage(int damage);

    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) const override;

    void NotifyCollision(GameObject& other) override;
    static int zombieCount;
private:
    int hitpoints;
    bool isDead;
    Sound deathSound;
    Sound hitSound;
    Timer deathTimer;
    bool facingLeft = false;
};

#endif // ZOMBIE_H

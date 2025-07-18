#ifndef BOSS_H
#define BOSS_H

#include <queue>
#include <memory>
#include "Component.h"
#include "Vec2.h"
#include "Timer.h"
#include "Sound.h"

class Boss : public Component {
public:
    Boss(GameObject& associated, std::string sprite);
    ~Boss();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) const override;

    void NotifyCollision(GameObject& other) override;
    class Command {
    public:
        enum CommandType { MOVE, SHOOT_LEFT, SHOOT_RIGHT, SHOOT_BOTH, CARD_SPRING,HIGH_CARD, POSITION_HANDS_FOR_LASER, PUNCH};
        CommandType type;
        Vec2 pos;

        Command(CommandType type, float x, float y): type(type), pos(x, y){}
    };

    static Boss* chefe;
    void Issue(Command task);

    

    void TakeDamage(int damage);
    bool IsDead() const { return isDead; }

    void SetSpeed(const Vec2& newSpeed) {
        speed = newSpeed;
        linearSpeed = speed.Magnitude();
    }
    int GetHP() const { return hp; }
    int GetMaxHP() const { return maxHp; }

    float GetHpPercent() const {
    return currentHp / (float)maxHp;
}



private:
    std::weak_ptr<GameObject> gunLeft;
    std::weak_ptr<GameObject> gunRight;
    std::queue<Command> taskQueue;
    Vec2 speed;
    float linearSpeed;
    int hp;
    int currentHp;
    int maxHp;
    Timer deathTimer;
    Timer damageCooldown;
    bool facingLeft = false;
    bool isDead = false;
    Sound deathSound;
    Sound hitSound;
    Timer handReturnTimer;
    bool waitingToReturnHands = false;
    bool preparingCardSpring = false;
    bool waitingHandsReturn = false;


};

#endif

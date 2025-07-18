#ifndef CHARACTER_H
#define CHARACTER_H

#include <queue>
#include <memory>
#include "Component.h"
#include "Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include "SDL_include.h"

class Character : public Component {
public:
    Character(GameObject& associated, std::string sprite);
    ~Character();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) const override;

    void NotifyCollision(GameObject& other) override;
    class Command {
    public:
        enum CommandType { MOVE, SHOOT };
        CommandType type;
        Vec2 pos;

        Command(CommandType type, float x, float y): type(type), pos(x, y){}
    };

    void Issue(Command task);

    static Character* player;

    void TakeDamage(int damage);
    bool IsDead() const { return isDead; }

    void SetSpeed(const Vec2& newSpeed) {
        speed = newSpeed;
        linearSpeed = speed.Magnitude();
    }

    float GetDamageCooldown() const {
        return damageCooldown.Get();
    }
    void RestartDamageCooldown() {
        damageCooldown.Restart();
    }

    static int npcCount;

private:
    std::weak_ptr<GameObject> gun;
    std::queue<Command> taskQueue;
    Vec2 speed;
    float linearSpeed;
    int hp;
    Timer deathTimer;
    Timer damageCooldown;
    bool facingLeft = false;
    bool isDead = false;
    Sound deathSound;
    Sound hitSound;
    Timer walkTimer;
    bool isDashing = false;
    Vec2 dashStartPos;
    Vec2 dashTargetPos;
    float dashDuration = 0.2f;  // em segundos
    Timer dashTimer;
    std::string currentDirection = "down"; // Pode ser "up", "down", "left", "right"
    Timer hitTimer;
    bool playingHitAnim = false;

    };

#endif

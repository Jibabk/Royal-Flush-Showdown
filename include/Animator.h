#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <unordered_map>
#include <string>
#include "Component.h"
#include "Sprite.h"
#include "Animation.h"

class Animator : public Component {
public:
    Animator(GameObject& associated);

    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) const override;

    void SetAnimation(std::string name);
    void AddAnimation(std::string name, Animation anim);
    void SetLoop(bool loop);
    bool IsFinished(std::string animationName) const;




private:
    std::unordered_map<std::string, Animation> animations;
    int frameStart;
    int frameEnd;
    float frameTime;
    int currentFrame;
    float timeElapsed;
    bool loop = true;
    bool finished = false;

    std::string current;
};

#endif

#include "Animator.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include <iostream>

Animator::Animator(GameObject& associated)
    : Component(associated),
      frameStart(0), frameEnd(0), frameTime(0),
      currentFrame(0), timeElapsed(0) {}

void Animator::Update(float /*dt*/) {
    if (frameTime != 0) {
        timeElapsed++;

        if (timeElapsed > frameTime) {
            currentFrame++;
            timeElapsed -= frameTime;

            if (currentFrame > frameEnd) {
                currentFrame = frameStart;
            }

            // Atualiza o frame no SpriteRenderer
            Component* cpt = associated.GetComponent("SpriteRenderer");
            if (cpt != nullptr) {
                SpriteRenderer* renderer = dynamic_cast<SpriteRenderer*>(cpt);
                if (renderer != nullptr) {
                    renderer->SetFrame(currentFrame, animations[current].flip);
                }
            }
        }
    }
}

void Animator::Render() {
    // Não renderiza nada diretamente
}

bool Animator::Is(std::string type) const {
    return type == "Animator";
}

void Animator::SetAnimation(std::string name) {
    if (current == name) return;
    auto it = animations.find(name);
    if (it != animations.end()) {
        Animation anim = it->second;

        frameStart = anim.frameStart;
        frameEnd = anim.frameEnd;
        frameTime = anim.frameTime;
        currentFrame = frameStart;
        timeElapsed = 0;

        current = name;
        // Atualiza imediatamente o frame no SpriteRenderer
        Component* cpt = associated.GetComponent("SpriteRenderer");
        if (cpt != nullptr) {
            SpriteRenderer* renderer = dynamic_cast<SpriteRenderer*>(cpt);
            if (renderer != nullptr) {
                renderer->SetFrame(currentFrame,anim.flip);
            }
        }
    }
}

void Animator::AddAnimation(std::string name, Animation anim) {
    if (animations.find(name) == animations.end()) {
        animations[name] = anim;
    }
}

#include <InputManager.h>
#include <SDL_include.h>
#include <Component.h>

#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

class PlayerController : public Component {
public:
    PlayerController(GameObject& associated);

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) const override;
};

#endif

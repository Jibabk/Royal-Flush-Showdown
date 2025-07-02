// CardComponent.cpp
#include "CardComponent.h"
#include "InputManager.h"
#include "Camera.h"
#include <SDL_include.h>
#include <Game.h>
#include <StageState.h>

CardComponent::CardComponent(GameObject& associated, const Card& card)
    : Component(associated), card(card), selected(false) {}

void CardComponent::Update(float dt) {
    InputManager& input = InputManager::GetInstance();
    int mx = input.GetMouseX() + Camera::pos.x;
    int my = input.GetMouseY() + Camera::pos.y;

    if (input.MousePress(LEFT_MOUSE_BUTTON)) {
        if (associated.box.Contains({(float)mx, (float)my})) {
            State& state = Game::GetInstance().GetCurrentState();
            StageState* stage = dynamic_cast<StageState*>(&state);
            if (stage) {
                stage->DeselectAllCards();
            }
            selected = true;
        }
    }

    const float moveSpeed = 100 * dt;
    const float yMin = 550;
    const float yMax = 600;

    if (selected) {
        associated.box.y = std::max(yMin, associated.box.y - moveSpeed);
    } else {
        associated.box.y = std::min(yMax, associated.box.y + moveSpeed);
    }
}





void CardComponent::Render() {}

bool CardComponent::Is(std::string type) const {
    return type == "CardComponent";
}

void CardComponent::ToggleSelection() {
    selected = !selected;
}

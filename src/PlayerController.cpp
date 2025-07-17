#include "PlayerController.h"
#include "InputManager.h"
#include "Character.h"
#include "Camera.h"
#include "Vec2.h"

PlayerController::PlayerController(GameObject& associated)
    : Component(associated) {}

void PlayerController::Start() {}

void PlayerController::Update(float dt) {
    InputManager& input = InputManager::GetInstance();

    Character* character = static_cast<Character*>(associated.GetComponent("Character"));
    if (!character) {
        return;
    }

    // Direção
    Vec2 direction(0, 0);

    if (input.KeyPress(W_KEY) || input.KeyPress(UP_ARROW_KEY)) {
        direction.y -= 100;
    }
    if (input.KeyPress(S_KEY) || input.KeyPress(DOWN_ARROW_KEY)) {
        direction.y += 100;
    }
    if (input.KeyPress(A_KEY) || input.KeyPress(LEFT_ARROW_KEY)) {
        direction.x -= 100;
    }
    if (input.KeyPress(D_KEY) || input.KeyPress(RIGHT_ARROW_KEY)) {
        direction.x += 100;
    }

    // Se há direção, emite comando de movimento
    if (direction.Magnitude() > 0) {
        Vec2 target = associated.box.Center() + direction.Normalized() * 100;
        character->Issue(Character::Command(Character::Command::MOVE, target.x, target.y));
    }

    // Comando de tiro (clique do mouse)
    if (input.MousePress(LEFT_MOUSE_BUTTON)) {
        Vec2 mouse(input.GetMouseX() + Camera::pos.x, input.GetMouseY() + Camera::pos.y);
        character->Issue(Character::Command(Character::Command::SHOOT, mouse.x, mouse.y));
    }
}

void PlayerController::Render() {}

bool PlayerController::Is(std::string type) const {
    return type == "PlayerController";
}

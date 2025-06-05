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

    if (input.IsKeyDown(W_KEY) || input.IsKeyDown(UP_ARROW_KEY)) {
        direction.y -= 1;
    }
    if (input.IsKeyDown(S_KEY) || input.IsKeyDown(DOWN_ARROW_KEY)) {
        direction.y += 1;
    }
    if (input.IsKeyDown(A_KEY) || input.IsKeyDown(LEFT_ARROW_KEY)) {
        direction.x -= 1;
    }
    if (input.IsKeyDown(D_KEY) || input.IsKeyDown(RIGHT_ARROW_KEY)) {
        direction.x += 1;
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

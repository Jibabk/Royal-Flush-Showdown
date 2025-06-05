#include "Camera.h"
#include "Game.h"

Vec2 Camera::pos = {0, 0};
Vec2 Camera::speed = {0, 0};
GameObject* Camera::focus = nullptr;

void Camera::Follow(GameObject* newFocus) {
    focus = newFocus;
}

void Camera::Unfollow() {
    focus = nullptr;
}

void Camera::Update(float dt) {
    if (focus) {
        int windowWidth = Game::GetInstance().GetWidth();
        int windowHeight = Game::GetInstance().GetHeight();

        pos.x = focus->box.Center().x - windowWidth / 2;
        pos.y = focus->box.Center().y - windowHeight / 2;
    } else {
        pos += speed * dt;
    }
}

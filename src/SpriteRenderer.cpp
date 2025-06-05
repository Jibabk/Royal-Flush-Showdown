#include "SpriteRenderer.h"
#include "GameObject.h"
#include <Camera.h>

SpriteRenderer::SpriteRenderer(GameObject& associated)
    : Component(associated), sprite() {
    // Nenhum sprite carregado ainda, box permanece inalterado
    SetFrame(0, SDL_FLIP_NONE);
}

SpriteRenderer::SpriteRenderer(GameObject& associated, const std::string& file, int frameCountW, int frameCountH)
    : Component(associated), sprite(file, frameCountW, frameCountH) {
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();
    SetFrame(0, SDL_FLIP_NONE);
}

void SpriteRenderer::Open(const std::string& file) {
    sprite.Open(file);
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();
}

void SpriteRenderer::SetFrameCount(int frameCountW, int frameCountH) {
    sprite.SetFrameCount(frameCountW, frameCountH);
}


void SpriteRenderer::SetFrame(int frame, SDL_RendererFlip flip) {
    sprite.SetFlip(flip & SDL_FLIP_HORIZONTAL, flip & SDL_FLIP_VERTICAL);
    sprite.SetFrame(frame);
}

void SpriteRenderer::Update(float /*dt*/) {
    // Função propositalmente vazia neste trabalho
}

void SpriteRenderer::Render() {
    if (sprite.cameraFollower) {
        sprite.Render((int)associated.box.x, (int)associated.box.y,associated.angleDeg);
    } else {
        sprite.Render((int)(associated.box.x - Camera::pos.x), 
                      (int)(associated.box.y - Camera::pos.y),associated.angleDeg);
    }
}


bool SpriteRenderer::Is(std::string type) const {
    return type == "SpriteRenderer";
}

void SpriteRenderer :: SetCameraFollower(bool follow) {
    sprite.cameraFollower = follow;
}

int SpriteRenderer::GetWidth() {
    return sprite.GetWidth();
}
int SpriteRenderer::GetHeight() {
    return sprite.GetHeight();
}

void SpriteRenderer::SetScale(float scaleX, float scaleY) {
    sprite.SetScale(scaleX, scaleY);
    associated.box.w = sprite.GetWidth() * scaleX;
    associated.box.h = sprite.GetHeight() * scaleY;
}
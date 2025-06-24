#include "Sprite.h"
#include "Game.h"
#include <iostream>

// Construtor padrão
Sprite::Sprite() 
    : cameraFollower(false) ,texture(nullptr), width(0), height(0), frameCountW(1), frameCountH(1), flip(SDL_FLIP_NONE),scale(1,1){}

// Construtor com suporte a animação
Sprite::Sprite(const std::string file, int frameCountW, int frameCountH)
    : Sprite() {
    this->frameCountW = frameCountW;
    this->frameCountH = frameCountH;
    Open(file);
}

Sprite::~Sprite() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
}

void Sprite::Open(const std::string file) {
    SDL_Texture* newTexture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());

    if (newTexture == nullptr) {
        std::cerr << "Erro ao carregar imagem: " << SDL_GetError() << std::endl;
        return;
    }

    texture = newTexture;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    clipRect = { 0, 0, width, height };
}

void Sprite::SetClip(int x, int y, int w, int h) {
    clipRect = { x, y, w, h };
}

void Sprite::SetFrame(int frame) {
    int maxFrames = frameCountW * frameCountH;
    if (frame < 0 || frame >= maxFrames) {
        std::cerr << "Frame inválido: " << frame << std::endl;
        return;
    }

    int frameW = width / frameCountW;
    int frameH = height / frameCountH;

    int row = frame / frameCountW;
    int col = frame % frameCountW;

    int x = col * frameW;
    int y = row * frameH;

    SetClip(x, y, frameW, frameH);
}

void Sprite::SetFrameCount(int frameCountW, int frameCountH) {
    this->frameCountW = frameCountW;
    this->frameCountH = frameCountH;

    // Atualiza clip com o primeiro frame
    if (texture != nullptr) {
        int frameW = width / frameCountW;
        int frameH = height / frameCountH;
        SetClip(0, 0, frameW, frameH);
    }
}

void Sprite::Render(int x, int y,float angle=0) {
    SDL_Rect dstRect = { x, y, clipRect.w * scale.x, clipRect.h * scale.y };
    SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstRect ,angle, nullptr, flip);
}

void Sprite::Render(int x, int y, int w, int h, float angle=0) {
    SDL_Rect dstRect = { x, y, w * scale.x, h * scale.y };
    SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstRect ,angle, nullptr, flip);
}



int Sprite::GetWidth() {
    return (frameCountW > 0) ? width / frameCountW * scale.x : width * scale.x;
}

int Sprite::GetHeight() {
    return (frameCountH > 0) ? height / frameCountH * scale.y : height * scale.y;
}

bool Sprite::IsOpen() {
    return texture != nullptr;
}

void Sprite::SetScale(float scaleX, float scaleY) {
    scale.x = scaleX;
    scale.y = scaleY;
}
Vec2 Sprite::GetScale() {
    return scale;
}
void Sprite::SetFlip(bool flipX, bool flipY) {
    if (flipX) {
        flip = SDL_FLIP_HORIZONTAL;
    } else if (flipY) {
        flip = SDL_FLIP_VERTICAL;
    } else {
        flip = SDL_FLIP_NONE;
    }
}

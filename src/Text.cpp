#include "Text.h"
#include "Game.h"
#include "Resources.h"
#include "Camera.h"

//#include <SDL_ttf.h>
#include <iostream>

Text::Text(GameObject& associated,
           std::string fontFile,
           int fontSize,
           TextStyle style,
           std::string text,
           SDL_Color color)
    : Component(associated),
      text(text),
      style(style),
      fontFile(fontFile),
      fontSize(fontSize),
      color(color),
      texture(nullptr) {
    RemakeTexture();
}

Text::~Text() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
}

void Text::Update(float) {
    // Nada
}

void Text::Render() {
    if (texture) {
        SDL_Rect dstRect;
        dstRect.x = associated.box.x - Camera::pos.x;
        dstRect.y = associated.box.y - Camera::pos.y;
        dstRect.w = associated.box.w;
        dstRect.h = associated.box.h;

        SDL_RenderCopyEx(Game::GetInstance().GetRenderer(),
                          texture,
                          nullptr,
                          &dstRect,
                          associated.angleDeg,
                          nullptr,
                          SDL_FLIP_NONE);
    }
}

bool Text::Is(std::string type) const {
    return type == "Text";
}

void Text::SetText(std::string text) {
    this->text = text;
    RemakeTexture();
}

void Text::SetColor(SDL_Color color) {
    this->color = color;
    RemakeTexture();
}

void Text::SetStyle(TextStyle style) {
    this->style = style;
    RemakeTexture();
}

void Text::SetFontFile(std::string fontFile) {
    this->fontFile = fontFile;
    RemakeTexture();
}

void Text::SetFontSize(int fontSize) {
    this->fontSize = fontSize;
    RemakeTexture();
}

void Text::RemakeTexture() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    TTF_Font* font = Resources::GetFont(fontFile, fontSize);
    if (font == nullptr) {
        std::cerr << "[Text] Failed to load font: " << fontFile << std::endl;
        return;
    }

    SDL_Surface* surface = nullptr;
    switch (style) {
        case SOLID:
            surface = TTF_RenderText_Solid(font, text.c_str(), color);
            break;
        case SHADED:
            surface = TTF_RenderText_Shaded(font, text.c_str(), color, {0, 0, 0, 255});
            break;
        case BLENDED:
            surface = TTF_RenderText_Blended(font, text.c_str(), color);
            break;
    }

    if (!surface) {
        std::cerr << "[Text] Failed to create text surface: " << TTF_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), surface);
    if (!texture) {
        std::cerr << "[Text] Failed to create texture from surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    associated.box.w = surface->w;
    associated.box.h = surface->h;

    SDL_FreeSurface(surface);
    if (!texture) {
    std::cerr << "[ERROR] Text texture is NULL for text: " << text << std::endl;
    }
}

#include "Resources.h"
#include "Game.h"
#include <iostream>

// === Definição dos atributos estáticos ===
std::unordered_map<std::string, SDL_Texture*> Resources::imageTable;
std::unordered_map<std::string, Mix_Music*> Resources::musicTable;
std::unordered_map<std::string, Mix_Chunk*> Resources::soundTable;
std::unordered_map<std::string, TTF_Font*> Resources::fontTable;

// === IMAGENS ===
SDL_Texture* Resources::GetImage(const std::string& file) {
    auto it = imageTable.find(file);
    if (it != imageTable.end()) {
        return it->second;
    }

    SDL_Texture* texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
    if (texture == nullptr) {
        std::cerr << "Failed to load image " << file << ": " << IMG_GetError() << std::endl;
        return nullptr;
    }

    imageTable[file] = texture;
    return texture;
}

void Resources::ClearImages() {
    for (auto& item : imageTable) {
        SDL_DestroyTexture(item.second);
    }
    imageTable.clear();
}

// === MUSICAS ===
Mix_Music* Resources::GetMusic(const std::string& file) {
    auto it = musicTable.find(file);
    if (it != musicTable.end()) {
        return it->second;
    }

    Mix_Music* music = Mix_LoadMUS(file.c_str());
    if (music == nullptr) {
        std::cerr << "Failed to load music " << file << ": " << Mix_GetError() << std::endl;
        return nullptr;
    }

    musicTable[file] = music;
    return music;
}

void Resources::ClearMusics() {
    for (auto& item : musicTable) {
        Mix_FreeMusic(item.second);
    }
    musicTable.clear();
}

// === SONS ===
Mix_Chunk* Resources::GetSound(const std::string& file) {
    auto it = soundTable.find(file);
    if (it != soundTable.end()) {
        return it->second;
    }

    Mix_Chunk* chunk = Mix_LoadWAV(file.c_str());
    if (chunk == nullptr) {
        std::cerr << "Failed to load sound " << file << ": " << Mix_GetError() << std::endl;
        return nullptr;
    }

    soundTable[file] = chunk;
    return chunk;
}

void Resources::ClearSounds() {
    for (auto& item : soundTable) {
        Mix_FreeChunk(item.second);
    }
    soundTable.clear();
}

// === FONTES ===
TTF_Font* Resources::GetFont(const std::string& file, int size) {
    std::string key = file + std::to_string(size);

    auto it = fontTable.find(key);
    if (it != fontTable.end()) {
        return it->second;
    }

    TTF_Font* font = TTF_OpenFont(file.c_str(), size);
    if (font == nullptr) {
        std::cerr << "Failed to load font " << file << ": " << TTF_GetError() << std::endl;
        return nullptr;
    }

    fontTable[key] = font;
    return font;
}

void Resources::ClearFonts() {
    for (auto& item : fontTable) {
        TTF_CloseFont(item.second);
    }
    fontTable.clear();
}

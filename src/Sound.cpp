#include "Sound.h"
#include <iostream>
#define INCLUDE_SDL_MIXER

Sound::Sound() : chunk(nullptr),channel(-1) {}

Sound::Sound(const std::string file) : Sound() {
    Open(file);
}

Sound::~Sound() {
    if (chunk != nullptr) {
        //Stop();
        Mix_FreeChunk(chunk);
    }
}

void Sound::SetVolume(int volume) {
    if (chunk != nullptr) {
        if (volume < 0) volume = 0;
        if (volume > MIX_MAX_VOLUME) volume = MIX_MAX_VOLUME;
        Mix_VolumeChunk(chunk, volume);
    }
}

void Sound::Play(int times) {
    std::cout << "Tocando som: " << times << " vezes." << std::endl;
    if (chunk != nullptr) {
        // Mix_PlayChannel escolhe o primeiro canal livre quando passado -1
        // loops = times - 1 (para tocar exatamente "times" vezes)
        channel = Mix_PlayChannel(-1, chunk, times - 1);
        if (channel == -1) {
            std::cerr << "Erro ao tocar som: " << Mix_GetError() << std::endl;
        }
    }
}

void Sound::Stop() {
    if (chunk != nullptr && channel != -1) {
        if (Mix_Playing(channel)) {
            Mix_HaltChannel(channel);
        }
        channel = -1;
    }
}


void Sound::Open(std::string file) {
    chunk = Mix_LoadWAV(file.c_str());

    if (chunk == nullptr) {
        std::cerr << "Erro ao carregar som: " << file << " - " << Mix_GetError() << std::endl;
    }
}

bool Sound::IsOpen() {
    return chunk != nullptr;
}
#include "Music.h"
#include <iostream>

Music::Music() : music(nullptr) {}

Music::Music(const std::string file) : music(nullptr) {
    Open(file);
}

Music::~Music() {
    if (music != nullptr) {
        Mix_FreeMusic(music);
    }
}

void Music::Open(const std::string file) {
    if (music != nullptr) {
        Mix_FreeMusic(music);
    }

    music = Mix_LoadMUS(file.c_str());

    if (!music) {
        std::cerr << "Erro ao carregar música: " << Mix_GetError() << std::endl;
    }
}

void Music::Play(int times) {
    //std::cout<<"chamado\n";
    if (music) {
        //std::cout<<"tocando\n";
        Mix_PlayMusic(music, times);
    }
}

void Music::Stop(int msToStop) {
    Mix_FadeOutMusic(msToStop);
}

bool Music::IsOpen() {
    return music != nullptr;
}

#include "Game.h"
#include "Resources.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <InputManager.h>
#include <Camera.h>
#include <Character.h>
#include <WaveSpawner.h>

Game* Game::instance = nullptr;

Game::Game(const std::string& title, int width, int height)
    : window(nullptr), renderer(nullptr), storedState(nullptr),
      frameStart(0), dt(0), width(width), height(height) {

    if (instance != nullptr) {
        throw std::runtime_error("Game já instanciado.");
    }
    instance = this;

    // Inicializa SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        throw std::runtime_error("Erro ao inicializar SDL.");
    }

    // Inicializa SDL_Image
    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG))) {
        throw std::runtime_error("Erro ao inicializar SDL_Image.");
    }

    // Inicializa SDL_Mixer
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0) {
        throw std::runtime_error("Erro ao inicializar SDL_Mixer.");
    }
    Mix_AllocateChannels(32);

    // Inicializa SDL_TTF
    if (TTF_Init() != 0) {
        throw std::runtime_error("Erro ao inicializar SDL_TTF.");
    }

    // Cria janela
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               width, height, 0);
    if (window == nullptr) {
        throw std::runtime_error("Erro ao criar janela.");
    }

    // Cria renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        throw std::runtime_error("Erro ao criar renderer.");
    }

    // Seed para aleatoriedade
    srand(time(nullptr));
}

Game::~Game() {
    while (!stateStack.empty()) {
        stateStack.pop();
    }

    if (storedState != nullptr) {
        storedState = nullptr;
    }

    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
    Resources::ClearFonts();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

Game& Game::GetInstance() {
    if (instance == nullptr) {
        instance = new Game("Wave Survival", 1200, 900);
    }
    return *instance;
}

SDL_Renderer* Game::GetRenderer() const {
    return renderer;
}

State& Game::GetCurrentState() {
    if (stateStack.empty()) {
        throw std::runtime_error("stateStack está vazio.");
    }
    return *stateStack.top();
}

void Game::Push(State* state) {
    storedState = state;
}

void Game::Run() {
    if (storedState != nullptr) {
        stateStack.emplace(storedState);
        stateStack.top()->Start();
        storedState = nullptr;
    }

    while (!stateStack.empty() && !stateStack.top()->QuitRequested()) {
        CalculateDeltaTime();
        InputManager::GetInstance().Update();
        Camera::Update(dt);

        // -------- POP --------
        while (!stateStack.empty() && stateStack.top()->PopRequested()) {
            stateStack.top()->Pause();
            stateStack.pop();
            Character::player = nullptr; // Reseta o player se o estado for removido
            WaveSpawner::quest = nullptr; // Reseta a quest se o estado for removido

            if (!stateStack.empty()) {
                stateStack.top()->Resume();
            }
        }

        // -------- PUSH --------
        if (storedState != nullptr) {
            if (!stateStack.empty()) {
                stateStack.top()->Pause();
            }
            stateStack.emplace(storedState);
            stateStack.top()->Start();
            storedState = nullptr;
        }

        if (stateStack.empty()) {
            break;
        }

        // -------- EXECUÇÃO --------
        State& currentState = *stateStack.top();

        currentState.Update(dt);
        currentState.Render();

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}



void Game::CalculateDeltaTime() {
    int time = SDL_GetTicks();
    dt = (time - frameStart) / 1000.0f;
    frameStart = time;
}

float Game::GetDeltaTime() const {
    return dt;
}

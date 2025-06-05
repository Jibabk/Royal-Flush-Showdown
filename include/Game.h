#ifndef GAME_H
#define GAME_H

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_TTF

#include "SDL_include.h"
#include <string>
#include <stack>
#include <memory>

#include "State.h"

class Game {
public:
    Game(const std::string& title, int width, int height);
    ~Game();

    static Game& GetInstance();

    SDL_Renderer* GetRenderer() const;
    State& GetCurrentState();
    void Push(State* state);
    void Run();
    float GetDeltaTime() const;

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

private:
    void CalculateDeltaTime();

    SDL_Window* window;
    SDL_Renderer* renderer;

    static Game* instance;

    std::stack<std::unique_ptr<State>> stateStack;
    State* storedState;

    int frameStart;
    float dt;

    int width;
    int height;
};

#endif // GAME_H

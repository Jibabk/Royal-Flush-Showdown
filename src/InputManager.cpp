#include "InputManager.h"
#include <iostream>
#include "Game.h"


InputManager::InputManager() : quitRequested(false), updateCounter(0), mouseX(0), mouseY(0) {
    for (int i = 0; i < 6; ++i) {
        mouseState[i] = false;
        mouseUpdate[i] = 0;
    }
}

InputManager::~InputManager() {}

InputManager& InputManager::GetInstance() {
    static InputManager instance;  // Meyers' Singleton
    return instance;
}

void InputManager::Update() {
    SDL_Event event;

    // Atualiza posição do mouse
    SDL_GetMouseState(&mouseX, &mouseY);

    // Incrementa contador de atualização
    updateCounter++;

    // Reset da flag de quit
    quitRequested = false;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quitRequested = true;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button < 6) {
                    mouseState[event.button.button] = true;
                    mouseUpdate[event.button.button] = updateCounter;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button < 6) {
                    mouseState[event.button.button] = false;
                    mouseUpdate[event.button.button] = updateCounter;
                }
                break;

            case SDL_KEYDOWN:
                if (event.key.repeat == 0) {
                    keyState[event.key.keysym.sym] = true;
                    keyUpdate[event.key.keysym.sym] = updateCounter;
                }
                break;

            case SDL_KEYUP:
                keyState[event.key.keysym.sym] = false;
                keyUpdate[event.key.keysym.sym] = updateCounter;
                break;

            default:
                break;
        }
    }
}

bool InputManager::KeyPress(int key) {
    return keyUpdate[key] == updateCounter && keyState[key];
}

bool InputManager::KeyRelease(int key) {
    return keyUpdate[key] == updateCounter && !keyState[key];
}

bool InputManager::IsKeyDown(int key) {
    return keyState[key];
}

bool InputManager::MousePress(int button) {
    return button < 6 && mouseUpdate[button] == updateCounter && mouseState[button];
}

bool InputManager::MouseRelease(int button) {
    return button < 6 && mouseUpdate[button] == updateCounter && !mouseState[button];
}

bool InputManager::IsMouseDown(int button) {
    return button < 6 && mouseState[button];
}

int InputManager::GetMouseX() {
    return mouseX;
}

int InputManager::GetMouseY() {
    return mouseY;
}

bool InputManager::QuitRequested() {
    return quitRequested;
}

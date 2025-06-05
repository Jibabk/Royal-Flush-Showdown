#include "State.h"

State::State() 
    : popRequested(false), quitRequested(false), started(false) {}

State::~State() {
    objectArray.clear();
}

std::weak_ptr<GameObject> State::AddObject(GameObject* go) {
    std::shared_ptr<GameObject> ptr(go);
    objectArray.push_back(ptr);

    if (started) {
        ptr->Start();
    }

    return std::weak_ptr<GameObject>(ptr);
}

std::weak_ptr<GameObject> State::GetObjectPtr(GameObject* go) {
    for (int i = 0; i < objectArray.size(); ++i) {
        if (objectArray[i].get() == go) {
            return std::weak_ptr<GameObject>(objectArray[i]);
        }
    }
    return std::weak_ptr<GameObject>();
}

void State::StartArray() {
    for (size_t i = 0; i < objectArray.size(); ++i) {
        objectArray[i]->Start();
    }
}

void State::UpdateArray(float dt) {
    for (size_t i = 0; i < objectArray.size(); ++i) {
        objectArray[i]->Update(dt);
    }

    // Remoção de objetos mortos
    for (size_t i = 0; i < objectArray.size();) {
        if (objectArray[i]->IsDead()) {
            objectArray.erase(objectArray.begin() + i);
        } else {
            ++i;
        }
    }
}

void State::RenderArray() {
    for (size_t i = 0; i < objectArray.size(); ++i) {
        objectArray[i]->Render();
    }
}

bool State::PopRequested() const {
    return popRequested;
}

bool State::QuitRequested() const {
    return quitRequested;
}

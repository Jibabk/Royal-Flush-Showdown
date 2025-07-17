#include "GameObject.h"
#include <algorithm>
#include <typeinfo>

GameObject::GameObject() : angleDeg(0), isDead(false), started(false){}

GameObject::~GameObject() {
    // Percorre o vetor de trás para frente e deleta os componentes
    for (int i = components.size() - 1; i >= 0; --i) {
        delete *(components.begin() + i);
    }
    components.clear();
}

void GameObject::Update(float dt) {
    for (Component* cpt : components) {
        cpt->Update(dt);
    }
}

void GameObject::Render() {
    for (Component* cpt : components) {
        cpt->Render();
    }
}

bool GameObject::IsDead() const {
    return isDead;
}

void GameObject::RequestDelete() {
    isDead = true;
}

void GameObject::AddComponent(Component* cpt) {
    components.emplace_back(cpt);
    
    if (started) {
        cpt->Start(); // chama Start imediatamente se já começou
    }
}


void GameObject::RemoveComponent(Component* cpt) {
    auto it = std::find(components.begin(), components.end(), cpt);
    if (it != components.end()) {
        components.erase(it);
    }
}

Component* GameObject::GetComponent(std::string type) {
    for (Component* cpt : components) {
        if (cpt->Is(type)) {
            return cpt;
        }
    }
    return nullptr;
}

void GameObject::Start() {
    for (Component* cpt : components) {
        cpt->Start(); // chama Start de todos os componentes
    }
    started = true;
}

//VERIFICAR POR BUGS
void GameObject::NotifyCollision(GameObject& other) {
    for (Component* cpt : components) {
        cpt->NotifyCollision(other);
    }
}
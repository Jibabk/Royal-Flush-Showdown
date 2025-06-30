#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject& associated) : associated(associated) {}

Component::~Component() {}

void Component::Start() {
}

void Component::NotifyCollision(GameObject& other) {
    // Método padrão não faz nada, pode ser sobrescrito por subclasses
}

GameObject& Component::GetAssociated() {
    return associated;
}

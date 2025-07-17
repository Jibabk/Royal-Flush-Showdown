#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <string>
#include "Rect.h"
#include "Component.h"

class GameObject {
public:
    GameObject();
    ~GameObject();

    void Update(float dt);
    void Render();
    void Start();

    bool IsDead() const;
    void RequestDelete();

    void AddComponent(Component* cpt);
    void RemoveComponent(Component* cpt);
    Component* GetComponent(std::string type);
    void NotifyCollision(GameObject& other);
    double angleDeg;
    Rect box;
    

private:
    std::vector<Component*> components;
    bool isDead;
    bool started;
    
    
};

#endif

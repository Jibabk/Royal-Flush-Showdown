// CardComponent.h
#pragma once
#include "Component.h"
#include "Card.h"

class CardComponent : public Component {
public:
    Card card;
    bool selected= false;

    CardComponent(GameObject& associated, const Card& card);

    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) const override;
    void DeselectAllCards(); // Desmarca todas as cartas
    void ToggleSelection(); // Inverte estado selecionado
};

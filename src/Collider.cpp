#include "Collider.h"
#include "GameObject.h"
#include "Camera.h"
#include "SDL_include.h"

Collider::Collider(GameObject& associated, Vec2 scale, Vec2 offset)
    : Component(associated), scale(scale), offset(offset){}

void Collider::Update(float) {
    box = associated.box;
    box.w *= scale.x;
    box.h *= scale.y;
    box.x = associated.box.Center().x - box.w / 2;
    box.y = associated.box.Center().y - box.h / 2;

    Vec2 rotatedOffset = offset.GetRotated(associated.angleDeg);
    box.x += rotatedOffset.x;
    box.y += rotatedOffset.y;
}

bool Collider::Is(std::string type) const {
    return type == "Collider";
}

void Collider::SetScale(Vec2 scale) {
    this->scale = scale;
}

void Collider::SetOffset(Vec2 offset) {
    this->offset = offset;
}

// Adicione o trecho abaixo no começo do seu Collider.cpp,
// logo após os seus includes e antes de qualquer função.

#ifdef DEBUG
#include "Camera.h"
#include "Game.h"
#include <SDL2/SDL.h>
#define PI 3.14159265358979323846
#endif // DEBUG





// Copie o conteúdo dessa função para dentro da sua e adapte o nome das funções para as suas.
// Funções usadas:
// Rect::GetCenter()				- Retorna um Vec2 no centro do Rect
// Vec2::operator-( const Vec2& )	- Subtrai dois Vec2
// Vec2::Rotate( float rad )		- Rotaciona um Vec2 pelo ângulo em radianos passado
void Collider::Render() {
#ifdef DEBUG
	Vec2 center( box.Center() );
	SDL_Point points[5];

	Vec2 point = (Vec2(box.x, box.y) - center).Rotate( associated.angleDeg/(180/PI) )
					+ center - Camera::pos;
	points[0] = {(int)point.x, (int)point.y};
	points[4] = {(int)point.x, (int)point.y};
	
	point = (Vec2(box.x + box.w, box.y) - center).Rotate( associated.angleDeg/(180/PI) )
					+ center - Camera::pos;
	points[1] = {(int)point.x, (int)point.y};
	
	point = (Vec2(box.x + box.w, box.y + box.h) - center).Rotate( associated.angleDeg/(180/PI) )
					+ center - Camera::pos;
	points[2] = {(int)point.x, (int)point.y};
	
	point = (Vec2(box.x, box.y + box.h) - center).Rotate( associated.angleDeg/(180/PI) )
					+ center - Camera::pos;
	points[3] = {(int)point.x, (int)point.y};

	SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}


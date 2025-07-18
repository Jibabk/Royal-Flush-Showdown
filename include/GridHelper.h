// GridHelper.h

#ifndef GRID_HELPER_H
#define GRID_HELPER_H

#include "Vec2.h"

struct GridPosition {
    int row;
    int col;
};

// Função existente
GridPosition MapToGrid(Vec2 worldPosition);

// --- NOVA FUNÇÃO ---
// Retorna as coordenadas do centro de um tile com base nos seus índices.
Vec2 GetTileWorldPosition(int row, int col);

#endif
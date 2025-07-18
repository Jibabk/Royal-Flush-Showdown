// GridHelper.h ou similar

#ifndef GRIDHELPER_H
#define GRIDHELPER_H

#include "Vec2.h" // Inclua o seu ficheiro de vetor

// Estrutura para armazenar o resultado: a fileira e a coluna na grade.
struct GridPosition {
    int row; // Índice da fileira (de 0 a 4)
    int col; // Índice da coluna (de 0 a 7)
};

// Declaração da função
GridPosition MapToGrid(Vec2 worldPosition);

#endif
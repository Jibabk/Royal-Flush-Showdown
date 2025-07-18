// GridHelper.cpp ou no ficheiro .cpp onde for mais conveniente

#include "GridHelper.h" // Se tiver criado o ficheiro
#include <vector>
#include <cmath> // Para std::abs
#include <limits> // Para std::numeric_limits

/**
 * @brief Mapeia uma coordenada do mundo para a fileira e coluna mais próxima na grade do jogo.
 * @param worldPosition A posição (x, y) a ser mapeada.
 * @return Uma struct GridPosition com os índices da fileira (row) e coluna (col) mais próximas.
 */
GridPosition MapToGrid(Vec2 worldPosition) {
    // Coordenadas centrais das colunas da sua grade
    const std::vector<float> col_coords = {200, 280, 360, 440, 520, 600, 680, 760};
    
    // Coordenadas centrais das fileiras da sua grade
    const std::vector<float> row_coords = {347, 412, 477, 542, 591};

    float min_dist_x = std::numeric_limits<float>::max();
    int best_col_index = -1;

    // Encontra a coluna mais próxima
    for (int i = 0; i < col_coords.size(); ++i) {
        // Calcula a distância horizontal absoluta
        float dist = std::abs(worldPosition.x - col_coords[i]);
        if (dist < min_dist_x) {
            min_dist_x = dist;
            best_col_index = i;
        }
    }

    float min_dist_y = std::numeric_limits<float>::max();
    int best_row_index = -1;

    // Encontra a fileira mais próxima
    for (int i = 0; i < row_coords.size(); ++i) {
        // Calcula a distância vertical absoluta
        float dist = std::abs(worldPosition.y - row_coords[i]);
        if (dist < min_dist_y) {
            min_dist_y = dist;
            best_row_index = i;
        }
    }

    // Retorna os melhores índices encontrados
    return {best_row_index, best_col_index};
}
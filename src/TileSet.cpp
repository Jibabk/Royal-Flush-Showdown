#include "TileSet.h"
#include <iostream>

TileSet::TileSet(int tileWidth, int tileHeight, std::string file)
    : tileSet(file),tileWidth(tileWidth), tileHeight(tileHeight),tileCount(0) {
    
        // Verifica se a imagem foi carregada corretamente
    if (!tileSet.IsOpen()) {
        std::cerr << "Erro ao carregar tileset: " << file << std::endl;
        return;
    }
     // Calcula quantos tiles cabem por linha e por coluna na imagem
     int tilesPerRow = tileSet.GetWidth() / tileWidth;
     int tilesPerCol = tileSet.GetHeight() / tileHeight;
 
     // Define a quantidade total de tiles (frames)
     tileCount = tilesPerRow * tilesPerCol;
 
     // Configura o Sprite para funcionar como uma grade de tiles
     tileSet.SetFrameCount(tilesPerRow, tilesPerCol);
 }

int TileSet::GetTileWidth() {
    return tileWidth;
}

int TileSet::GetTileHeight() {
    return tileHeight;
}

void TileSet::RenderTile(unsigned index, float x, float y) {
    // Verifica se o índice está dentro dos limites válidos
    if (index >= static_cast<unsigned>(tileCount)) {
        std::cerr << "Índice de tile inválido: " << index << std::endl;
        return;
    }

    // Define o frame (recorte) correto do Sprite
    tileSet.SetFrame(static_cast<int>(index));

    // Renderiza o tile na posição desejada
    tileSet.Render(static_cast<int>(x), static_cast<int>(y),0);
}
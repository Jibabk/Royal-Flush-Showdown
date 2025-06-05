#include "TileMap.h"
#include "GameObject.h"
#include "TileSet.h"
#include "Game.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <Camera.h>

TileMap::TileMap(GameObject& associated, std::string file, TileSet* tileSet)
    : Component(associated),mapWidth(0), mapHeight(0), mapDepth(0) {
    SetTileSet(tileSet);
    Load(file);
}

void TileMap::SetTileSet(TileSet* tileSet) {
    this->tileSet = std::unique_ptr<TileSet>(tileSet);
}

void TileMap::Load(std::string file) {
    std::ifstream mapFile(file);
    if (!mapFile.is_open()) {
        std::cerr << "Erro ao abrir tilemap: " << file << std::endl;
        return;
    }

    char sep;

    // Lê as dimensões do mapa
    mapFile >> mapWidth >> sep >> mapHeight >> sep >> mapDepth;

    tileMatrix.clear();

    for (int z = 0; z < mapDepth; ++z) {
        for (int y = 0; y < mapHeight; ++y) {
            for (int x = 0; x < mapWidth; ++x) {
                int tile;
                mapFile >> sep >> tile;
                tileMatrix.push_back(tile);
            }
        }
    }

    mapFile.close();
}

int& TileMap::At(int x, int y, int z) {
    return tileMatrix[z * mapWidth * mapHeight + y * mapWidth + x];
}

void TileMap::RenderLayer(int layer) {
    if (tileSet == nullptr) return;

    int tileW = tileSet->GetTileWidth();
    int tileH = tileSet->GetTileHeight();

    int windowWidth = Game::GetInstance().GetWidth();
    int windowHeight = Game::GetInstance().GetHeight();

    int xStart = std::max(0, (int)(Camera::pos.x / tileW));
    int yStart = std::max(0, (int)(Camera::pos.y / tileH));

    int xEnd = std::min(mapWidth, xStart + (windowWidth / tileW) + 2);
    int yEnd = std::min(mapHeight, yStart + (windowHeight / tileH) + 2);

    for (int y = yStart; y < yEnd; ++y) {
        for (int x = xStart; x < xEnd; ++x) {
            int index = At(x, y, layer);
            float posX = (float)(x * tileW) - Camera::pos.x;
            float posY = (float)(y * tileH) - Camera::pos.y;
            tileSet->RenderTile(index, posX, posY);
        }
    }
}


void TileMap::Render() {
    for (int layer = 0; layer < mapDepth; ++layer) {
        RenderLayer(layer);
    }
}

int TileMap::GetWidth() {
    return mapWidth;
}

int TileMap::GetHeight(){
    return mapHeight;
}

int TileMap::GetDepth(){
    return mapDepth;
}

bool TileMap::Is(std::string type) const {
    return type == "TileMap";
}
#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <string>
#include <memory>
#include "TileSet.h"
#include "Component.h"

class TileMap : public Component {
public:
    TileMap(GameObject& associated, std::string file, TileSet* tileSet);
    
    void Load(std::string file);
    void SetTileSet(TileSet* tileSet);
    int& At(int x, int y, int z = 0);

    void Render() override;
    void RenderLayer(int layer);
    void Update(float dt) override {}
    bool Is(std::string type) const override;
    int GetWidth();
    int GetHeight();
    int GetDepth();

private:
    std::vector<int> tileMatrix;
    std::unique_ptr<TileSet> tileSet;
    int mapWidth;
    int mapHeight;
    int mapDepth;
};

#endif

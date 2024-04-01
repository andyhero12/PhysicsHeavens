//
//  World.cpp
//  Heavan
//
//  Created by Henry Gao on 3/24/24.
//

#include "World.h"

using namespace cugl;
/** The name of a platform (for object identification) */
#define TILE_NAME   "TILE"

cugl::Size size(1,1);

World::World (cugl::Vec2 bottomleft,
              const std::vector<std::vector<int>> &map,
              const std::vector<std::vector<int>> &passable, std::shared_ptr<cugl::AssetManager> assets)
    : start(bottomleft)
{
    
    tile = assets->get<Texture>("tile");
        
    backgroundWorld.resize(map.size());
    for(int i = 0; i < map.size(); i++){
        backgroundWorld.at(i).resize(map[0].size());
    }
    int originalRows = (int) backgroundWorld.size();
    int originalCols = (int) backgroundWorld.at(0).size();
    int printIndexJ = 0;
    for (int j =0; j< originalCols; j++){
        int printIndexI = 0;
        for (int i = originalRows -1; i > -1; i--){
            Rect temp = Rect(Vec2(printIndexJ,printIndexI), Size(1,1));
            if (i == 0 || j == 0 || i == originalRows -1 || j == originalCols -1){
                backgroundWorld.at(i).at(j) = TileInfo::alloc(temp.origin, size, Terrain::IMPASSIBLE, getBox(map.at(i).at(j))); 
                
            }else{
                backgroundWorld.at(i).at(j) = TileInfo::alloc(temp.origin, size, Terrain::PASSABLE, getBox(map.at(i).at(j)));
            }
            backgroundWorld.at(i).at(j)->_tileObstacle->setName(std::string(TILE_NAME) +cugl::strtool::to_string(i * originalRows+ j));
            printIndexI++;
        }
        printIndexJ++;
    }
}

bool TileInfo::init(const cugl::Vec2& pos, const cugl::Size& size,Terrain m_type, std::shared_ptr<cugl::Texture> m_texture)
{
    _tileObstacle = cugl::physics2::BoxObstacle::alloc(pos,size);
    if (_tileObstacle == nullptr){
        return false;
    }
    _tileObstacle->setBodyType(b2_staticBody);
    _tileObstacle->setDensity(10.0f);
    _tileObstacle->setFriction(0.4f);
    _tileObstacle->setRestitution(0.1);
    return true;
}

std::shared_ptr<cugl::Texture> World::getBox(int position){
    int boxSize = 32;  // Each box is 32x32 pixels
    float textureWidth = tile->getWidth();
    float textureHeight = tile->getHeight();
    position = position - 1;
    int boxesPerRow = textureWidth / boxSize;
    int row = position / boxesPerRow;
    int column = position % boxesPerRow;

    // Calculate the pixel coordinates for the kth box
    int minS_px = (column) * boxSize;
    int maxS_px = minS_px + boxSize;
    int minT_px = row * boxSize;
    int maxT_px = minT_px + boxSize;

    GLfloat minS = (minS_px) / textureWidth;
    GLfloat maxS = (maxS_px) / textureWidth;
    GLfloat minT = (minT_px) / textureHeight;
    GLfloat maxT = (maxT_px) / textureHeight;

    // Get the subTexture
    return tile->getSubTexture(minS, maxS, minT, maxT);
}
    



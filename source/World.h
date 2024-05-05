//
//  World.hpp
//  Heavan
//
//  Created by Henry Gao on 3/24/24.
//

#ifndef World_hpp
#define World_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <unordered_set>
#include <vector>
#include "NLLevelModel.h"
//
// Might want to be more specific with tile types
enum Terrain {
    PASSABLE,
    IMPASSIBLE
};

// Information for a tile, add to this later since idk what this will include
class TileInfo{
public:
#pragma mark Static Constructors
    
    /**
     * Creates a new Dog at the given position.
     *
     * The Dog is sized according to the given drawing scale.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     * @param size  The size of the dude in world units
     * @param scale The drawing scale (world to screen)
     *
     * @return  A newly allocated DudeModel at the given position with the given scale
     */
    static std::shared_ptr<TileInfo> alloc(const cugl::Vec2& pos, const cugl::Size& size,Terrain m_type, std::shared_ptr<cugl::Texture> m_texture) {
        std::shared_ptr<TileInfo> result = std::make_shared<TileInfo>();
        return (result->init(pos, size, m_type, m_texture) ? result : nullptr);
    }
#pragma mark Constructors
    
    bool init(const cugl::Vec2& pos, const cugl::Size& size,Terrain m_type, std::shared_ptr<cugl::Texture> m_texture);
    std::shared_ptr<cugl::Texture> texture;
    std::shared_ptr<scene2::PolygonNode> tileSprite;
    bool isUpper;
    Vec2 pos;
    
    
    Vec2 getPos() const {
        return pos;
    }
    void setUpperTrue(){
        isUpper = true;
    }
    bool getIsUpperDecor(){
        return isUpper;
    }
    const std::shared_ptr<scene2::PolygonNode>& getTileSprite(){
        return tileSprite;
    }
    Terrain type;
    
    TileInfo(){
        isUpper = false;
        texture = nullptr;
        tileSprite = nullptr;
        pos = Vec2(0,0);
        type = Terrain::IMPASSIBLE;
    }
private:
};

class World{
public:
        
private:
    std::shared_ptr<cugl::AssetManager> _assets;
    
public:
    static std::shared_ptr<World> alloc(std::shared_ptr<LevelModel> _level,std::shared_ptr<cugl::AssetManager> assets) {
        std::shared_ptr<World> result = std::make_shared<World>();
        return (result->init(_level, assets) ? result : nullptr);
    }
    ~World(){
        _assets = nullptr;
    }
    std::vector<std::vector<std::shared_ptr<TileInfo>>> tileWorld;
    std::vector<std::vector<std::shared_ptr<TileInfo>>> boundaryWorld;
    std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>> lowerDecorWorld;
    std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>> upperDecorWorld;
    
    std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>> tilesAtCoords;
    std::vector<std::shared_ptr<TileInfo>> allTiles;
    bool init(std::shared_ptr<LevelModel> _level,std::shared_ptr<cugl::AssetManager> assets);

    std::shared_ptr<cugl::Texture> getBoxFromTileSet(int position, const std::map<int,TileSet>& tileSets);
    
    
    const std::vector<std::shared_ptr<TileInfo>>& getVisibleNodes(){
        return allTiles;
    }
    const std::vector<std::vector<std::shared_ptr<TileInfo>>>& getTileWorld(){
        return tileWorld;
    }
    const std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>>& getTileDisplay(){
        return tilesAtCoords;
    }
    
    const std::vector<std::vector<std::shared_ptr<TileInfo>>>& getBoundaryWorld() const {
        return boundaryWorld;
    }
    
    const std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>>& getLowerDecorWorld(){
        return lowerDecorWorld;
    }
    const std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>>& getUpperDecorWorld(){
        return upperDecorWorld;
    }
    
    // Get whether a tile is passible or not
    bool isPassable(int x, int y) const;
    
    // Get the number of rows of tiles in the world
    int getRows() const{
        return(int) boundaryWorld.size();
    }
    
    // Get the number of columns of tiles in the world
    int getCols() const{
        return (int) boundaryWorld.at(0).size();
    }
};

#endif /* World_hpp */

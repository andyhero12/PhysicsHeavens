//
//  OverWorld.hpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//
#pragma once
#ifndef OverWorld_hpp
#define OverWorld_hpp
#include <cugl/cugl.h>
#include <unordered_set>
#include "Base.h"
#include "NLDog.h"
#include "Devil.h"
#include "Animation.h"
#include "BaseSet.h"
#include "NLInput.h"
#include "AttackPolygons.h"
#include "DecoySet.h"
#include "NLLevelModel.h"

class OverWorld{
private:
    std::shared_ptr<LevelModel> _level;
    float _scale;
    cugl::Size _activeSize;
    std::shared_ptr<Dog> _dog;
    std::shared_ptr<Devil> _devil;
    std::shared_ptr<DecoySet> _decoys;
    std::shared_ptr<BaseSet> _bases;
    std::shared_ptr<cugl::JsonValue> _constants;
    std::shared_ptr<cugl::AssetManager> _assets;
    AttackPolygons _attackPolygonSet;
    
    void drawDecoy(const std::shared_ptr<cugl::SpriteBatch>& batch);
public:
    
    std::shared_ptr<cugl::scene2::SceneNode> _worldnode;
    
    std::shared_ptr<cugl::scene2::SceneNode> getWorldNode(){
        return _worldnode;
    }
    OverWorld(){
        
    }
    ~OverWorld(){
        
    }
    
    void reset();
    bool init(const std::shared_ptr<cugl::AssetManager>& assets, const std::shared_ptr<LevelModel>& _level, float scale, cugl::Size activeSize);
    bool initDogModel();
    bool initDevil();
    bool initBases();
    bool initDecoys();
    
    bool setRootNode(const std::shared_ptr<scene2::SceneNode>& _worldNode, const std::shared_ptr<scene2::SceneNode>& _debugNode, std::shared_ptr<cugl::physics2::net::NetWorld> _world, bool isHost);
    // will add Obstacle nodes too
    void dogUpdate(InputController& _input,cugl::Size totalSize);
    void devilUpdate(InputController& _input,cugl::Size totalSize);
    void update(InputController& input, cugl::Size totalSize, float timestep);
    void postUpdate();
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,cugl::Size totalSize);
    
    // Might want to be more specific with tile types
    enum Terrain {
        PASSABLE,
        IMPASSIBLE
    };
    
    // Information for a tile, add to this later since idk what this will include
    class TileInfo {
    public:
    private:
        Terrain type;
    };
    
    // Matrix with information about the overworld
    std::vector<std::vector<TileInfo>> overworld;
    
    std::shared_ptr<Dog> getDog() const {
        return _dog;
    }
    std::shared_ptr<Devil> getDevil()const {
        return _devil;
    }
    std::shared_ptr<DecoySet> getDecoys() const{
        return _decoys;
    }
    std::shared_ptr<BaseSet> getBaseSet(){
        return _bases;
    }
    int getTotalTargets() const {
        return 1 + (int) _bases->_bases.size() + (int) _decoys->getCurrentDecoys().size();
    }
    AttackPolygons& getAttackPolygons(){
        return _attackPolygonSet;
    }
    void setDog(std::shared_ptr<Dog> m_dog){
        _dog = m_dog;
    }
    void setDevil(std::shared_ptr<Devil> m_devil){
        _devil = m_devil;
    }
    
    /**
    Get the type of terrain at the tile at (x, y)
     */
    Terrain getTerrain(int x, int y);
    
    /**
    Whether the terrain at (x, y) is passable
     */
    bool isPassable(int x, int y);
    
    /**
     Gets the width of the overworld in tile coordinates
     */
    int getRows();
    
    /**
     Gets the height of the overworld in tile coordinates
     */
    int getCols();
    
    
};
#endif /* OverWorld_hpp */

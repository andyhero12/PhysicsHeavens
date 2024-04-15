//
//  SpawnerController.hpp
//  Dog
//
//  Created by Henry Gao on 2/23/24.
//
#pragma once

#ifndef SpawnerController_hpp
#define SpawnerController_hpp

#include "Spawner.h"
#include <cugl/cugl.h>
#include <unordered_set>
#include <vector>
#include "MonsterController.h"
#include "AbstractSpawner.h"
#include "MeleeSpawner.h"
#include "OverWorld.h"


class SpawnerController{
public:
    std::shared_ptr<cugl::Texture> _texture;
    
    /* Set of spawners */
    std::unordered_set<std::shared_ptr<AbstractSpawner>> _spawners;
    
    /* Functions */
    SpawnerController();
    
    ~SpawnerController();

    void setTexture(const std::shared_ptr<cugl::Texture>& value);
    void update(MonsterController& monsterController, OverWorld& overWorld, float timestep);
    
    bool init(const std::vector<LevelModel::Spawner>& startLocs);
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
    void setRootNode(const std::shared_ptr<scene2::SceneNode>& _worldNode, bool isHost);
    bool win(){return _spawners.empty();}
    
    std::shared_ptr<cugl::scene2::SceneNode> getSpawnerNode() const{
        return baseSpawnerNode;
    }
private:
    std::shared_ptr<cugl::scene2::SceneNode> baseSpawnerNode;
    std::shared_ptr<NetEventController> _network;
    bool _isHost;
};


#endif /* SpawnerController_hpp */

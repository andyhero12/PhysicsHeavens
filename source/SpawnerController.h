//
//  SpawnerController.hpp
//  Dog
//
//  Created by Henry Gao on 2/23/24.
//
#pragma once

#ifndef SpawnerController_hpp
#define SpawnerController_hpp

#include <cugl/cugl.h>
#include <unordered_set>
#include <vector>
#include "MonsterController.h"
#include "AbstractSpawner.h"
#include "SimpleSpawner.h"
#include "OverWorld.h"
#include "NLDeathEvent.h"
#include "NLSpawnerDeathEvent.h"


class SpawnerController{
public:
    
    /* Set of spawners */
    std::unordered_set<std::shared_ptr<AbstractSpawner>> _spawners;
    std::vector<std::shared_ptr<SpriteAnimationNode>> animationNodes;

    float difficulty;
    float accumulatedTime;
    
    /* Functions */
    SpawnerController();
    
    ~SpawnerController();
    
    void drawFlames();
    
    void update(MonsterController& monsterController, OverWorld& overWorld, float timestep);
    
    bool init(const std::vector<LevelModel::Spawner>& startLocs, std::shared_ptr<cugl::AssetManager> assets, std::shared_ptr<NetEventController> net);
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
    void setRootNode(const std::shared_ptr<scene2::SceneNode>& _worldNode, bool isHost);
    void setAnimNode(const std::shared_ptr<scene2::SceneNode>& _worldNode);
    bool win(){return _spawners.empty();}
    void processDeathEvent(const std::shared_ptr<DeathEvent>& deathEvent);
    void processSpawnerDeathEvent(const std::shared_ptr<SpawnerDeathEvent>& spawnerDeathEvent);
    std::shared_ptr<cugl::scene2::SceneNode> getSpawnerNode() const{
        return baseSpawnerNode;
    }
    
    std::unordered_set<std::shared_ptr<AbstractSpawner>> getSpawner() const {
        return _spawners;
    }
    
    void dispose();
private:
    std::shared_ptr<cugl::scene2::SceneNode> baseSpawnerNode;
    std::shared_ptr<cugl::scene2::SceneNode> animSpawnerNode;
    std::shared_ptr<NetEventController> _network;
    bool _isHost;
    
    std::shared_ptr<cugl::Texture> _deathTexture;
    std::shared_ptr<cugl::Texture> _spawnTexture;
    std::shared_ptr<cugl::Texture> _deathSpawner;
    std::unordered_set<std::shared_ptr<SpriteAnimationNode>> _curAnimations;
};


#endif /* SpawnerController_hpp */

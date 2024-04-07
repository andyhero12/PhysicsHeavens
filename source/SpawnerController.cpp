//
//  SpawnerController.cpp
//
//  Created by Henry Gao on 2/23/24.
//
#include "SpawnerController.h"
#include <random>

SpawnerController::SpawnerController(){
    
}
SpawnerController::~SpawnerController(){
    
}

// Function to generate a random value between 1 and 3
int generateRandomValue(int left, int right) {
    // Static used for the seed to ensure it's only seeded once
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(left, right); // Range is 1 to 3, inclusive
    return dis(gen);
}

void SpawnerController::update(MonsterController& monsterController, OverWorld& overWorld, float timestep){
    for(auto& spawner : _spawners) {
        spawner->update(monsterController, overWorld, timestep);
    }
    
    
    auto it = _spawners.begin();
    while (it != _spawners.end()){
        std::shared_ptr<AbstractSpawner> spawner = *it;
        
        if (spawner->dead()){
            it = _spawners.erase(it);
        }
        else{
            ++it;
        }
    }
    
}

bool SpawnerController::init(const std::vector<cugl::Vec2>& startLocs) {
    _spawners.clear();
    baseSpawnerNode = cugl::scene2::SceneNode::alloc();
    for (int i =0; i< startLocs.size(); i++){
        cugl::Vec2 pos;
        pos = startLocs.at(i)/64;
        int spawnRate = 200;
        int health = 10;
        std::shared_ptr<MeleeSpawner> curSpawner = std::make_shared<MeleeSpawner>(spawnRate,pos,health,0);
        curSpawner->setSceneNode(_texture);
        baseSpawnerNode->addChild(curSpawner->getSpawnerNode());
        curSpawner->getSpawnerNode()->setPosition(pos);
        _spawners.insert(curSpawner);
    }
    return true;
}

void SpawnerController::setTexture(const std::shared_ptr<cugl::Texture>& value ){
    _texture = value;
}

void SpawnerController::setRootNode(const std::shared_ptr<scene2::SceneNode>& _worldNode, bool isHost){
    
    _isHost = isHost;
    // Add the Base Spawner Node
    _worldNode->addChild(baseSpawnerNode);
    
    
}

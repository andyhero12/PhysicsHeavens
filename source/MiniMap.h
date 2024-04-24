//
//  MiniMap.hpp
//  Heaven
//
//  Created by Henry Gao on 4/23/24.
//

#ifndef MiniMap_h
#define MiniMap_h
#include <cugl/cugl.h>
#include <stdio.h>
#include "SpawnerController.h"
#include "NLDog.h"
#include "OverWorld.h"



class Minimap:  public cugl::scene2::SceneNode {
private:
    std::unordered_set<std::shared_ptr<AbstractSpawner>> _spawners;
    std::vector<std::shared_ptr<Base>> _bases;
    std::shared_ptr<Dog> _dog;
    std::vector<cugl::scene2::SceneNode> minimapObjects;
    cugl::Size _screenSize;
public:
    Minimap(){};
    bool init(std::shared_ptr<cugl::AssetManager> &assets, cugl::Size screenSize, OverWorld &overWorld, SpawnerController &spawnerController);
    void update();
};

#endif /* MiniMap_hpp */

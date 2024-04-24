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
#include "BaseSet.h"


class Minimap:  public cugl::scene2::SceneNode {
private:
    std::unordered_set<std::shared_ptr<AbstractSpawner>> _spawners;
    std::vector<std::shared_ptr<Base>> _bases;
    std::shared_ptr<Dog> _dog;
public:
    Minimap(){};
    bool init(std::shared_ptr<cugl::AssetManager> &assets, cugl::Size screenSize,  std::unordered_set<std::shared_ptr<AbstractSpawner>> spawners, std::vector<std::shared_ptr<Base>> bases, std::shared_ptr<Dog> dog);
};

#endif /* MiniMap_hpp */

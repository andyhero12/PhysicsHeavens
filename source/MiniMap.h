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
    // scene graph for minimap
    std::vector<std::shared_ptr<cugl::scene2::SceneNode>> minimapObjects;
    // scene graph for all mutable objects
    std::shared_ptr<cugl::scene2::SceneNode> mutableObj;
    cugl::Size _screenSize;
    
    float x;
    float y;
    float radius;
    
    std::shared_ptr<cugl::Texture> spawnerTexture;
    
    std::shared_ptr<cugl::Texture> baseTexture;
    
    
    std::shared_ptr<cugl::Texture> spawnerArrowTexture;
    std::shared_ptr<cugl::Texture> baseArrowTexture;
    
    
    void createMap();
public:
    Minimap(){};
    bool init(std::shared_ptr<cugl::AssetManager> &assets, cugl::Size screenSize, OverWorld &overWorld, SpawnerController &spawnerController);
    void update();
    
    static std::shared_ptr<Minimap> alloc(std::shared_ptr<cugl::AssetManager> &assets, cugl::Size screenSize, OverWorld &overWorld, SpawnerController &spawnerController) {
        std::shared_ptr<Minimap> node = std::make_shared<Minimap>();
        return (node->init(assets, screenSize, overWorld, spawnerController) ? node : nullptr);
    }
};

#endif /* MiniMap_hpp */

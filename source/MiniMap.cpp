//
//  MiniMap.cpp
//  Heaven
//
//  Created by Henry Gao on 4/23/24.
//
#define MAPSCALE 6.0f
#define OBJECTSCALE 1.5f

#include "MiniMap.h"

bool Minimap::init(std::shared_ptr<cugl::AssetManager> &assets, cugl::Size screenSize, OverWorld &overWorld, SpawnerController &spawnerController){
    if (!cugl::scene2::SceneNode::init()){
        return false;
    }
    
    _screenSize = screenSize;
    _dog = overWorld.getDog();
    _bases = overWorld.getBaseSet()->getBases();
    _spawners = spawnerController.getSpawner();
    
    std::shared_ptr<cugl::Texture> miniMapTexture = assets->get<Texture>("minimap");
    std::shared_ptr<cugl::Texture> dogTexture = assets->get<Texture>("dogmap");
    spawnerTexture = assets->get<Texture>("spawnermap");
    baseTexture = assets->get<Texture>("basemap");
    
    std::shared_ptr<cugl::scene2::PolygonNode> map = cugl::scene2::PolygonNode::allocWithTexture(miniMapTexture);
    map->setScale(MAPSCALE);
    
    // Static do not change
    radius = MAPSCALE * map->getTexture()->getHeight()/2;
    x = screenSize.width - MAPSCALE * map->getTexture()->getWidth()/2;
    y = MAPSCALE * map->getTexture()->getHeight()/2;
    map->setPosition(x,y);
    
    std::shared_ptr<cugl::scene2::PolygonNode> dog = cugl::scene2::PolygonNode::allocWithTexture(dogTexture);
    dog->setPosition(x,y);
    dog->setScale(OBJECTSCALE);
    
    addChild(map);
    addChild(dog);
    mutableObj = scene2::SceneNode::alloc();
    addChild(mutableObj);
    
  
    createMap();
    
    return true;
}

void Minimap::update(){
    createMap();
}

void Minimap::createMap(){
    mutableObj->removeAllChildren();
    
    // Mini-map radius and scaling constants
    cugl::Vec2 dogPos = _dog->getPosition();
    float scalingFactor = 10.0; // Smoothing factor for distance calculation
    float A = 3.0; // Scaling amplitude at zero distance
    float k = 0.005; // Decay control parameter

    for (const auto& spawner : _spawners) {
        if(spawner->dead()){
            continue;
        }
        std::shared_ptr<cugl::scene2::PolygonNode> spawnerObj = cugl::scene2::PolygonNode::allocWithTexture(spawnerTexture);
        cugl::Vec2 dist = spawner->getPos() - dogPos;
        float actualDistance = dist.length();
        float scaledDistance = radius * actualDistance / (actualDistance + scalingFactor);

        cugl::Vec2 normalized = dist.getNormalization();
        cugl::Vec2 newPosition = normalized * scaledDistance;

        spawnerObj->setPosition(x + newPosition.x, y + newPosition.y);
        mutableObj->addChild(spawnerObj);
        
        float scale = OBJECTSCALE * std::max(1.0f, A / (1 + k * actualDistance * actualDistance));
        spawnerObj->setScale(scale);
    }

    for (const auto& base : _bases) {
        std::shared_ptr<cugl::scene2::PolygonNode> baseObj = cugl::scene2::PolygonNode::allocWithTexture(baseTexture);
        cugl::Vec2 dist = base->getPos() - dogPos;
        float actualDistance = dist.length();
        float scaledDistance = radius * actualDistance / (actualDistance + scalingFactor);

        cugl::Vec2 normalized = dist.getNormalization();
        cugl::Vec2 newPosition = normalized * scaledDistance;

        baseObj->setPosition(x + newPosition.x, y + newPosition.y);
        mutableObj->addChild(baseObj);

        float scale = OBJECTSCALE * std::max(1.0f, A / (1 + k * actualDistance * actualDistance));
        baseObj->setScale(scale);
    }
}

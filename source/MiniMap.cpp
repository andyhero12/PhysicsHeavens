//
//  MiniMap.cpp
//  Heaven
//
//  Created by Henry Gao on 4/23/24.
//

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
    std::shared_ptr<cugl::Texture> spawnerTexture = assets->get<Texture>("spawnermap");
    std::shared_ptr<cugl::Texture> baseTexture = assets->get<Texture>("basemap");
    
    std::shared_ptr<cugl::scene2::PolygonNode> map = cugl::scene2::PolygonNode::allocWithTexture(miniMapTexture);
    
    
    float mapx = map->getTexture()->getWidth()/2;
    float mapy = screenSize.height -  map->getTexture()->getHeight()/2;
    map->setPosition(mapx, mapy);
    
    addChild(map);
    
    
    return true;
}

void Minimap::update(){
    
}

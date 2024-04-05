//
//  DecoySet.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/22/24.
//

#include "DecoySet.h"

void DecoySet::init(float scale){
    _currentDecoys.clear();
    _pendingDecoys.clear();
    _removedDecoys.clear();
    decoySetNode = cugl::scene2::SceneNode::alloc();
    _scale = scale;
}

void DecoySet::update(float timestep){
    auto itD = _currentDecoys.begin();
    while (itD != _currentDecoys.end()){
        std::shared_ptr<Decoy> curDecoy = *itD;
        curDecoy->update(timestep);
        if (curDecoy->destroyed()){
            _removedDecoys.emplace_back(curDecoy);
            itD = _currentDecoys.erase(itD);
        }else {
            ++itD;
        }
    }
}

void DecoySet::setTexture(const std::shared_ptr<cugl::Texture> &value){
    _texture = value;
}

void DecoySet::addNewDecoy(cugl::Vec2 pos){
    std::shared_ptr<Decoy> newDecoy = std::make_shared<Decoy>(pos);
    std::shared_ptr<cugl::scene2::SceneNode> decoyNode = cugl::scene2::PolygonNode::allocWithTexture(_texture);
    decoyNode->setAnchor(Vec2::ANCHOR_CENTER);
    decoyNode->setPosition(pos*_scale);
    newDecoy->setSceneNode(decoyNode);
    decoySetNode->addChild(decoyNode);
    _pendingDecoys.emplace_back(newDecoy);
}
void DecoySet::postUpdate(){
    for (auto& decoy : _pendingDecoys){
        _currentDecoys.emplace_back(decoy);
    }
    for (auto& removedDecoy : _removedDecoys){
        std::shared_ptr<cugl::scene2::SceneNode> decoyDrawNode = removedDecoy->getSceneNode();
        decoySetNode->removeChild(decoyDrawNode);
    }
    _pendingDecoys.clear();
    _removedDecoys.clear();
}

//
//  DecoySet.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/22/24.
//

#include "DecoySet.h"

#define DECOY_SIZE Size(1, 1)

void DecoySet::init(){
    _currentDecoys.clear();
    _pendingDecoys.clear();
    _removedDecoys.clear();
    _curAnimations.clear();
    decoySetNode = cugl::scene2::SceneNode::alloc();
    justAdded = false;
}

void DecoySet::update(float timestep){
    auto itD = _currentDecoys.begin();
    while (itD != _currentDecoys.end()){
        std::shared_ptr<Decoy> curDecoy = *itD;
        curDecoy->update(timestep);
        if (curDecoy->destroyed()){
            std::shared_ptr<SpriteAnimationNode> explodeAnim = SpriteAnimationNode::allocWithSheet(_explodeTexture, 4, 5, 20, 1);
            explodeAnim->setScale(DECOY_SIZE/64);
            explodeAnim->setAnchor(Vec2::ANCHOR_CENTER);
            explodeAnim->setPosition(curDecoy->getPos());
            
            _curAnimations.emplace(explodeAnim);
            decoySetNode->addChild(explodeAnim);
            
            _removedDecoys.emplace_back(curDecoy);
            itD = _currentDecoys.erase(itD);
            
//            auto source = _assets->get<Sound>(DUMMY_GATE_EXPLOSION);
//            AudioEngine::get()->play(DUMMY_GATE_EXPLOSION + std::to_string(_removedDecoys.size()), source, false , source->getVolume());
        }else {
            ++itD;
        }
    }
    auto itAnim = _curAnimations.begin();
    while(itAnim != _curAnimations.end()){
        std::shared_ptr<SpriteAnimationNode> curAnim = *itAnim;
        auto curA = itAnim;
        curAnim->update();
        itAnim++;
        if (curAnim->getFrame() == 0){
            auto source = _assets->get<Sound>(DUMMY_GATE_EXPLOSION);
            AudioEngine::get()->play(DUMMY_GATE_EXPLOSION + std::to_string(_currentDecoys.size()), source, false , source->getVolume());
        }
        if (curAnim->getFrame() == curAnim->getSpan()-1){
            _curAnimations.erase(curA);
            decoySetNode->removeChild(curAnim);
        }
    }
    
    auto decAnim = _decAnimations.begin();
    while(decAnim != _decAnimations.end()){
        std::shared_ptr<SpriteAnimationNode> curAnim = *decAnim;
        curAnim->update();
        decAnim++;
        if (curAnim->getFrame() == curAnim->getSpan()-1){
            curAnim->setFrame(0);
        }
    }
}

void DecoySet::setTexture(const std::shared_ptr<cugl::Texture> &value){
    _texture = value;
}

void DecoySet::setExplodeTexture(const std::shared_ptr<cugl::Texture>& value){
    _explodeTexture = value;
}

void DecoySet::addNewDecoy(cugl::Vec2 pos){
    std::shared_ptr<Decoy> newDecoy = std::make_shared<Decoy>(pos);
    std::shared_ptr<SpriteAnimationNode> decoyAnim = SpriteAnimationNode::allocWithSheet(_texture, 3, 5, 15, 5);
    decoyAnim->setScale(DECOY_SIZE/128);
    decoyAnim->setAnchor(Vec2::ANCHOR_CENTER);
    decoyAnim->setPosition(pos);
    _decAnimations.emplace(decoyAnim);
    newDecoy->setSceneNode(decoyAnim);
    decoySetNode->addChild(decoyAnim);
    _pendingDecoys.emplace_back(newDecoy);
}
void DecoySet::postUpdate(){
    justAdded = (_pendingDecoys.size() != 0);
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

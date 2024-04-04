//
//  OverWorld.cpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#include "OverWorld.h"

#define WORLD_SIZE 3

void OverWorld::reset(cugl::Size resetSize) {
    initDog();
    _devil->setPosition(resetSize/2);
    _bases->init(_constants->get("base"));
    _decoys->init();
    _attackPolygonSet.init();
}

bool OverWorld::initDog(){
//    _dog = Dog::alloc(_constants->get("ship"),Vec2(2, 2), cugl::Size(1, 1), 1.0);
////    _dog = std::make_shared<Dog>(_totalSize/2, _constants->get("ship"));
//    
//    // medium dog assets
//    std::vector<std::shared_ptr<cugl::Texture>> textures;
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightrun"));
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackrun"));
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftrun"));
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontrun"));
//    _dog->setRunTextureMedium(textures);
//    
//    textures.clear();
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightbite"));
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackbite"));
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftbite"));
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontbite"));
//    _dog->setBiteTextureMedium(textures);
//    
//    textures.clear();
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightidle"));
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackidle"));
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftidle"));
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontidle"));
//    _dog->setIdleTextureMedium(textures);
//    
//    
//    textures.clear();
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightshoot"));
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackshoot"));
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftshoot"));
//    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontshoot"));
//    _dog->setShootTextureMedium(textures);
//
//    // small dog assets
//    textures.clear();
//    textures.push_back(_assets->get<cugl::Texture>("smalldogrightrun"));
//    textures.push_back(_assets->get<cugl::Texture>("smalldogbackrun"));
//    textures.push_back(_assets->get<cugl::Texture>("smalldogleftrun"));
//    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontrun"));
//    _dog->setRunTextureSmall(textures);
//
//    textures.clear();
//    textures.push_back(_assets->get<cugl::Texture>("smalldogrightbite"));
//    textures.push_back(_assets->get<cugl::Texture>("smalldogbackbite"));
//    textures.push_back(_assets->get<cugl::Texture>("smalldogleftbite"));
//    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontbite"));
//    _dog->setBiteTextureSmall(textures);
//
//    textures.clear();
//    textures.push_back(_assets->get<cugl::Texture>("smalldogrightidle"));
//    textures.push_back(_assets->get<cugl::Texture>("smalldogbackidle"));
//    textures.push_back(_assets->get<cugl::Texture>("smalldogleftidle"));
//    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontidle"));
//    _dog->setIdleTextureSmall(textures);
//
//
//    textures.clear();
//    textures.push_back(_assets->get<cugl::Texture>("smalldogrightshoot"));
//    textures.push_back(_assets->get<cugl::Texture>("smalldogbackshoot"));
//    textures.push_back(_assets->get<cugl::Texture>("smalldogleftshoot"));
//    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontshoot"));
//    _dog->setShootTextureSmall(textures);
//    
//    // large dog assets
//    textures.clear();
//    textures.push_back(_assets->get<cugl::Texture>("largedogrightrun"));
//    textures.push_back(_assets->get<cugl::Texture>("largedogbackrun"));
//    textures.push_back(_assets->get<cugl::Texture>("largedogleftrun"));
//    textures.push_back(_assets->get<cugl::Texture>("largedogfrontrun"));
//    _dog->setRunTextureLarge(textures);
//
//    textures.clear();
//    textures.push_back(_assets->get<cugl::Texture>("largedogrightbite"));
//    textures.push_back(_assets->get<cugl::Texture>("largedogbackbite"));
//    textures.push_back(_assets->get<cugl::Texture>("largedogleftbite"));
//    textures.push_back(_assets->get<cugl::Texture>("largedogfrontbite"));
//    _dog->setBiteTextureLarge(textures);
//
//    textures.clear();
//    textures.push_back(_assets->get<cugl::Texture>("largedogrightidle"));
//    textures.push_back(_assets->get<cugl::Texture>("largedogbackidle"));
//    textures.push_back(_assets->get<cugl::Texture>("largedogleftidle"));
//    textures.push_back(_assets->get<cugl::Texture>("largedogfrontidle"));
//    _dog->setIdleTextureLarge(textures);
//
//
//    textures.clear();
//    textures.push_back(_assets->get<cugl::Texture>("largedogrightshoot"));
//    textures.push_back(_assets->get<cugl::Texture>("largedogbackshoot"));
//    textures.push_back(_assets->get<cugl::Texture>("largedogleftshoot"));
//    textures.push_back(_assets->get<cugl::Texture>("largedogfrontshoot"));
//    _dog->setShootTextureLarge(textures);
//    
    return true;
}

bool OverWorld::initDevil(){
    _devil = std::make_shared<Devil>(_dog, Vec2(1, 1), _constants->get("devil"));
    std::vector<std::shared_ptr<cugl::Texture>> textures;
    textures.push_back(_assets->get<cugl::Texture>("player0"));
    textures.push_back(_assets->get<cugl::Texture>("player1"));
    _devil->setRunTexture(textures);
    return true;
}

bool OverWorld::initBases(){
    _bases = std::make_shared<BaseSet>();
    _bases->init(_constants->get("base"));
    _bases->setTexture(_assets->get<cugl::Texture>("base"));
    return true;
}

bool OverWorld::initDecoys(){
    _decoys = std::make_shared<DecoySet>();
    _decoys->setTexture(_assets->get<cugl::Texture>("base"));
    return true;
}

bool OverWorld::init(const std::shared_ptr<cugl::AssetManager>& assets, cugl::Size totalSize){
    _assets = assets;
    _constants = assets->get<cugl::JsonValue>("constants");
    _totalSize = totalSize;
    initDog();
    initDevil();
    initBases();
    initDecoys();
    return true;
}

void OverWorld::dogUpdate(InputController& _input, cugl::Size totalSize){
    _dog->moveOnInputSetAction(_input);
    if (_input.didPressFire() && _dog->canFireWeapon()){
        _attackPolygonSet.addBite(_dog);
        _dog->reloadWeapon();
    }
    if (_input.didPressSpecial() && _dog->canFireWeapon()){
        _dog->reloadWeapon();
        if (_dog->getMode() == "SHOOT" && _dog->getAbsorb() > 5){
            _dog->subAbsorb(5);
            _attackPolygonSet.addShoot(_dog);
        }else if (_dog->getMode() == "BUILD" && _dog->getAbsorb() > 5 ){
            _dog->subAbsorb(5);
            _decoys->addNewDecoy(_dog->getPosition());
        }else if (_dog->getMode() == "EXPLODE" && _dog->getAbsorb() > 10){
            _dog->subAbsorb(10);
            _attackPolygonSet.addExplode(_dog);
        }else {
            CULog("NOTHING\n");
        }
    }
}

void OverWorld::devilUpdate(InputController& _input,cugl::Size totalSize){
    _devil->move(totalSize * WORLD_SIZE);
}
void OverWorld::update(InputController& _input, cugl::Size totalSize, float timestep){
    dogUpdate(_input,totalSize);
    devilUpdate(_input, totalSize);
    _attackPolygonSet.update(totalSize);
    _bases->update();
    _decoys->update(timestep);
} 

void OverWorld::postUpdate(){
    _decoys->postUpdate();
}
void OverWorld::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,cugl::Size totalSize){
    _attackPolygonSet.draw(batch,totalSize);
    _bases->draw(batch,totalSize);
//    _dog->draw(batch, totalSize);
//    _devil->draw(batch, totalSize);
    _decoys->draw(batch);
}

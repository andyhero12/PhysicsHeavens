//
//  OverWorld.cpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//
/** Opacity of the physics outlines */
#define DYNAMIC_COLOR   Color4::YELLOW
#include "OverWorld.h"

#define WORLD_SIZE 3

void OverWorld::reset() {
    initDogModel();
//    _devil->setPosition(resetSize/2);
//    _bases->init(_constants->get("base"));
//    _decoys->init();
//    _attackPolygonSet.init();
}

bool OverWorld::initDogModel(){
    // should start at northeast and add to vector with animation in clockwise direction
    std::vector<std::shared_ptr<cugl::Texture>> textures;
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightrun"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightrun"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontrun"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontrun"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftrun"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftrun"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogbackrun"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogbackrun"));

    std::shared_ptr<AnimationSceneNode> smallDogRun = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    smallDogRun->setAnchor(Vec2::ANCHOR_CENTER);
    
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightidle"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightidle"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontidle"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontidle"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftidle"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftidle"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogbackidle"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogbackidle"));
    
    std::shared_ptr<AnimationSceneNode> smallDogIdle = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    smallDogIdle->setAnchor(Vec2::ANCHOR_CENTER);
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogbackbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogbackbite"));

    std::shared_ptr<AnimationSceneNode> smallDogBite = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    smallDogIdle->setAnchor(Vec2::ANCHOR_CENTER);
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightshoot"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightshoot"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontshoot"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontshoot"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftshoot"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftshoot"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogbackshoot"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogbackshoot"));
    
    std::shared_ptr<AnimationSceneNode> smallDogShoot = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    smallDogIdle->setAnchor(Vec2::ANCHOR_CENTER);

    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightrun"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightrun"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontrun"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontrun"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftrun"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftrun"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackrun"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackrun"));

    std::shared_ptr<AnimationSceneNode> mediumDogRun = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    mediumDogRun->setAnchor(Vec2::ANCHOR_CENTER);
    
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightidle"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightidle"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontidle"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontidle"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftidle"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftidle"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackidle"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackidle"));
    
    std::shared_ptr<AnimationSceneNode> mediumDogIdle = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    mediumDogIdle->setAnchor(Vec2::ANCHOR_CENTER);
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackbite"));

    std::shared_ptr<AnimationSceneNode> mediumDogBite = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    mediumDogIdle->setAnchor(Vec2::ANCHOR_CENTER);
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightshoot"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightshoot"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontshoot"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontshoot"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftshoot"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftshoot"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackshoot"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackshoot"));
    
    std::shared_ptr<AnimationSceneNode> mediumDogShoot = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    mediumDogIdle->setAnchor(Vec2::ANCHOR_CENTER);
    
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("largedogrightrun"));
    textures.push_back(_assets->get<cugl::Texture>("largedogrightrun"));
    textures.push_back(_assets->get<cugl::Texture>("largedogfrontrun"));
    textures.push_back(_assets->get<cugl::Texture>("largedogfrontrun"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftrun"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftrun"));
    textures.push_back(_assets->get<cugl::Texture>("largedogbackrun"));
    textures.push_back(_assets->get<cugl::Texture>("largedogbackrun"));

    std::shared_ptr<AnimationSceneNode> largeDogRun = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    largeDogRun->setAnchor(Vec2::ANCHOR_CENTER);
    
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("largedogrightidle"));
    textures.push_back(_assets->get<cugl::Texture>("largedogrightidle"));
    textures.push_back(_assets->get<cugl::Texture>("largedogfrontidle"));
    textures.push_back(_assets->get<cugl::Texture>("largedogfrontidle"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftidle"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftidle"));
    textures.push_back(_assets->get<cugl::Texture>("largedogbackidle"));
    textures.push_back(_assets->get<cugl::Texture>("largedogbackidle"));
    
    std::shared_ptr<AnimationSceneNode> largeDogIdle = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    largeDogIdle->setAnchor(Vec2::ANCHOR_CENTER);
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("largedogrightbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogrightbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogfrontbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogfrontbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogbackbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogbackbite"));

    std::shared_ptr<AnimationSceneNode> largeDogBite = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    largeDogIdle->setAnchor(Vec2::ANCHOR_CENTER);
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("largedogrightshoot"));
    textures.push_back(_assets->get<cugl::Texture>("largedogrightshoot"));
    textures.push_back(_assets->get<cugl::Texture>("largedogfrontshoot"));
    textures.push_back(_assets->get<cugl::Texture>("largedogfrontshoot"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftshoot"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftshoot"));
    textures.push_back(_assets->get<cugl::Texture>("largedogbackshoot"));
    textures.push_back(_assets->get<cugl::Texture>("largedogbackshoot"));
    
    std::shared_ptr<AnimationSceneNode> largeDogShoot = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    largeDogIdle->setAnchor(Vec2::ANCHOR_CENTER);
    
    
    Vec2 dogPos = _level->getPlayerPos() / _scale;
    Size mySize(64,64);
    Size dogSize(mySize/_scale);
    
    _dog = Dog::alloc(dogPos,dogSize);
    _dog->setDebugColor(DYNAMIC_COLOR);

    _dog->setSmallAnimation(smallDogIdle, smallDogRun, smallDogBite, smallDogShoot);
    _dog->setMediumAnimation(mediumDogIdle, mediumDogRun, mediumDogBite, mediumDogShoot);
    _dog->setLargeAnimation(largeDogIdle, largeDogRun, largeDogBite, largeDogShoot);
    
    std::shared_ptr<AnimationSceneNode> placeHolderDrawOver = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    _dog->setFinalDog(placeHolderDrawOver);
    _dog->setDrawScale(_scale);
    
    std::shared_ptr<scene2::SceneNode> _uinode = scene2::SceneNode::alloc();
    _uinode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    std::shared_ptr<UIController> _uiController = std::make_shared<UIController>();
    _uiController->init(_uinode, _assets, _activeSize);
    _dog->setUIController(_uiController);
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

bool OverWorld::init(const std::shared_ptr<cugl::AssetManager>& assets, const std::shared_ptr<LevelModel>& level, float scale, cugl::Size activeSize){
    _assets = assets;
    _level = level;
    _activeSize = activeSize;
    _constants = assets->get<cugl::JsonValue>("constants");
    _scale = scale;
    
    initDogModel();
//    initDevil();
//    initBases();
//    initDecoys();
    return true;
}

bool OverWorld::setRootNode(const std::shared_ptr<scene2::SceneNode>& _worldNode, const std::shared_ptr<scene2::SceneNode>& _debugNode, std::shared_ptr<cugl::physics2::net::NetWorld> _world, bool isHost){
    
    
    // Add Dog to Obstacle World
    _world->initObstacle(_dog);
    _dog->setDebugScene(_debugNode);
    if (isHost){
        _world->getOwnedObstacles().insert({_dog,0});
    }
    _worldNode->addChild(_dog->getDogNode());
    
    // Add Obstacles
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
//    devilUpdate(_input, totalSize);
//    _attackPolygonSet.update(totalSize);
//    _bases->update();
//    _decoys->update(timestep);
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

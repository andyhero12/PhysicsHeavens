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

#include "NLDog.h"

void OverWorld::reset() {
    initDogModel();
//    _devil->setPosition(resetSize/2);
    initBases();
    initDecoys();
    _attackPolygonSet.init();
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
    //surely there is a more intelligent way to do this??
    smallDogRun->setContentSize(DOG_SIZE);
    
    
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
    smallDogIdle->setContentSize(DOG_SIZE);
    
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
    smallDogBite->setAnchor(Vec2::ANCHOR_CENTER);
    smallDogBite->setContentSize(DOG_SIZE);
    
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
    smallDogShoot->setAnchor(Vec2::ANCHOR_CENTER);
    smallDogShoot->setContentSize(DOG_SIZE);

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
    mediumDogRun->setContentSize(DOG_SIZE);
    
    
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
    mediumDogIdle->setContentSize(DOG_SIZE);
    
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
    mediumDogBite->setAnchor(Vec2::ANCHOR_CENTER);
    mediumDogBite->setContentSize(DOG_SIZE);
    
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
    mediumDogShoot->setAnchor(Vec2::ANCHOR_CENTER);
    mediumDogShoot->setContentSize(DOG_SIZE);
    
    
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
    largeDogRun->setContentSize(DOG_SIZE);
    
    
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
    largeDogIdle->setContentSize(DOG_SIZE);
    
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
    largeDogBite->setAnchor(Vec2::ANCHOR_CENTER);
    largeDogBite->setContentSize(DOG_SIZE);
    
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
    largeDogShoot->setAnchor(Vec2::ANCHOR_CENTER);
    largeDogShoot->setContentSize(DOG_SIZE);
    
    
    // MAGIC NUMBER ALERT!!
    Vec2 dogPos = _level->getPlayerPos() / 64;
    _dog = Dog::alloc(dogPos, DOG_SIZE);
    _dog->setDebugColor(DYNAMIC_COLOR);

    _dog->setSmallAnimation(smallDogIdle, smallDogRun, smallDogBite, smallDogShoot);
    _dog->setMediumAnimation(mediumDogIdle, mediumDogRun, mediumDogBite, mediumDogShoot);
    _dog->setLargeAnimation(largeDogIdle, largeDogRun, largeDogBite, largeDogShoot);
    
    //std::shared_ptr<AnimationSceneNode> placeHolderDrawOver = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    std::shared_ptr<cugl::scene2::SceneNode> placeHolderDrawOver = scene2::SceneNode::allocWithBounds(DOG_SIZE);
    // MORE MAGIC NUMBER ALERT
    
    //placeHolderDrawOver->setContentSize(DOG_SIZE);
    _dog->setFinalDog(placeHolderDrawOver);
    placeHolderDrawOver->setScale(DOG_SIZE.y / textures.at(0)->getHeight());
    //placeHolderDrawOver->setAnchor(Vec2::ANCHOR_CENTER);
    
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
    _bases->init(_level->getBasesPos()); 
//    _bases->init(_constants->get("base"));
    _bases->setTexture(_assets->get<cugl::Texture>("base"));
    return true;
}

bool OverWorld::initDecoys(){
    _decoys = std::make_shared<DecoySet>();
    _decoys->init();
    _decoys->setTexture(_assets->get<cugl::Texture>("base"));
    return true;
}


// Placeholder function if we want to move world instantiation into here later, imo moving it here makes more sense - Colin
bool OverWorld::initWorld(){
    return true;
}

bool OverWorld::init(const std::shared_ptr<cugl::AssetManager>& assets, const std::shared_ptr<LevelModel>& level, cugl::Size activeSize,std::shared_ptr<cugl::physics2::net::NetEventController> network, bool isHost, std::shared_ptr<World> world){
    _assets = assets;
    _level = level;
    _network = network;
    _isHost = isHost;
    _activeSize = activeSize;
    _constants = assets->get<cugl::JsonValue>("constants");
    _world = world;
    
    initWorld();
    initDogModel();
    initBases();
    initDecoys();
    _attackPolygonSet.init();
//    initDevil();
    return true;
}

bool OverWorld::setRootNode(const std::shared_ptr<scene2::SceneNode>& _worldNode, const std::shared_ptr<scene2::SceneNode>& _debugNode, std::shared_ptr<cugl::physics2::net::NetWorld> _worldNet){
    // Add Base Decoy node
    _worldNode->addChild(_decoys->getDecoySetNode());
    
    
    // Attack Polygon base to the world nOde
    _worldNode->addChild(_attackPolygonSet.getAttackPolygonNode());
    // Add Bases to the World Node
    for (auto& base : _bases->_bases){
        std::shared_ptr<scene2::SceneNode> baseNode = base->getSceneNode();
        _worldNode->addChild(base->getSceneNode());
    }
    
    // Add Dog to Obstacle World
    _worldNet->initObstacle(_dog);
    _dog->setDebugScene(_debugNode);
    if (_isHost){
        _worldNet->getOwnedObstacles().insert({_dog,0});
    }
    _worldNode->addChild(_dog->getDogNode());
    
    // Add Obstacles
    return true;
}
void OverWorld::processBiteEvent(const std::shared_ptr<BiteEvent>& biteEvent){
    Vec2 center = biteEvent->getPos();
    float ang = biteEvent->getAng();
    _attackPolygonSet.addBite(center,ang, _dog->getBiteRadius());
}

void OverWorld::processShootEvent(const std::shared_ptr<ShootEvent>& shootEvent){
    Vec2 center = shootEvent->getPos();
    float ang = shootEvent->getAng();
    _attackPolygonSet.addShoot(center,ang, _dog->getShootRadius());
}
void OverWorld::processExplodeEvent(const std::shared_ptr<ExplodeEvent>& explodeEvent){
    Vec2 center = explodeEvent->getPos();
    _attackPolygonSet.addExplode(center, _dog->getExplosionRadius());
}
void OverWorld::dogUpdate(InputController& _input, cugl::Size totalSize){
    if (_isHost){
        _dog->moveOnInputSetAction(_input);
        _dog->updateUI();
        if (_input.didPressFire() && _dog->canFireWeapon()){
            CULog("Send Attack");
            _network->pushOutEvent(BiteEvent::allocBiteEvent(_dog->getBiteCenter(), _dog->getDirInDegrees()));
            _dog->reloadWeapon();
        }
        if (_input.didPressSpecial() && _dog->canFireWeapon()){
            _dog->reloadWeapon();
            if (_dog->getMode() == "SHOOT" && _dog->getAbsorb() >= 5){
                _dog->subAbsorb(5);
                _network->pushOutEvent(ShootEvent::allocShootEvent(_dog->getShootCenter(), _dog->getDirInDegrees()));
            }else if (_dog->getMode() == "BAIT" && _dog->getAbsorb() >= 5){
                _dog->subAbsorb(5);
                //            _decoys->addNewDecoy(_dog->getPosition());
                _network->pushOutEvent(DecoyEvent::allocDecoyEvent(_dog->getPosition()));
            }else if (_dog->getMode() == "BOMB" && _dog->getAbsorb() >= 5){
                _dog->subAbsorb(5);
                _network->pushOutEvent(ExplodeEvent::allocExplodeEvent(_dog->getPosition()));
            }else {
                CULog("NOTHING\n");
            }
        }
    }else{
        _dog->updateClientAnimations();
    }
}

void OverWorld::devilUpdate(InputController& _input,cugl::Size totalSize){
    _devil->move(totalSize * WORLD_SIZE);
}
void OverWorld::update(InputController& _input, cugl::Size totalSize, float timestep){
    dogUpdate(_input,totalSize);
    _bases->update();
    _decoys->update(timestep);
//    devilUpdate(_input, totalSize);
    _attackPolygonSet.update();
} 

void OverWorld::postUpdate(){
    _decoys->postUpdate();
}

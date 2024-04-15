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
    
    std::shared_ptr<AnimationSceneNode> clientSmallDogRun = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    clientSmallDogRun->setAnchor(Vec2::ANCHOR_CENTER);
    //surely there is a more intelligent way to do this??
    clientSmallDogRun->setContentSize(DOG_SIZE);
    
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
    
    std::shared_ptr<AnimationSceneNode> clientSmallDogIdle = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    clientSmallDogIdle->setAnchor(Vec2::ANCHOR_CENTER);
    clientSmallDogIdle->setContentSize(DOG_SIZE);
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogbackbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogbackbite"));

    std::shared_ptr<AnimationSceneNode> smallDogBite = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 3);
    smallDogBite->setAnchor(Vec2::ANCHOR_CENTER);
    smallDogBite->setContentSize(DOG_SIZE);
    
    std::shared_ptr<AnimationSceneNode> clientSmallDogBite = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    clientSmallDogBite->setAnchor(Vec2::ANCHOR_CENTER);
    clientSmallDogBite->setContentSize(DOG_SIZE);
    
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
    
    std::shared_ptr<AnimationSceneNode> clientSmallDogShoot = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    clientSmallDogShoot->setAnchor(Vec2::ANCHOR_CENTER);
    clientSmallDogShoot->setContentSize(DOG_SIZE);
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightshoot"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightshoot"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontshoot"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontshoot"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftshoot"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftshoot"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogbackshoot"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogbackshoot"));
    
    std::shared_ptr<AnimationSceneNode> smallDogDash = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    smallDogDash->setAnchor(Vec2::ANCHOR_CENTER);
    smallDogDash->setContentSize(DOG_SIZE);
    
    std::shared_ptr<AnimationSceneNode> clientSmallDogDash = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    clientSmallDogDash->setAnchor(Vec2::ANCHOR_CENTER);
    clientSmallDogDash->setContentSize(DOG_SIZE);

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
    
    std::shared_ptr<AnimationSceneNode> clientMediumDogRun = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    clientMediumDogRun->setAnchor(Vec2::ANCHOR_CENTER);
    clientMediumDogRun->setContentSize(DOG_SIZE);
    
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
    
    std::shared_ptr<AnimationSceneNode> clientMediumDogIdle = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    clientMediumDogIdle->setAnchor(Vec2::ANCHOR_CENTER);
    clientMediumDogIdle->setContentSize(DOG_SIZE);
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackbite"));

    std::shared_ptr<AnimationSceneNode> mediumDogBite = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 3);
    mediumDogBite->setAnchor(Vec2::ANCHOR_CENTER);
    mediumDogBite->setContentSize(DOG_SIZE);
    
    std::shared_ptr<AnimationSceneNode> clientMediumDogBite = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    clientMediumDogBite->setAnchor(Vec2::ANCHOR_CENTER);
    clientMediumDogBite->setContentSize(DOG_SIZE);
    
    
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
    
    std::shared_ptr<AnimationSceneNode> clientMediumDogShoot = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    clientMediumDogShoot->setAnchor(Vec2::ANCHOR_CENTER);
    clientMediumDogShoot->setContentSize(DOG_SIZE);
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightshoot"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightshoot"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontshoot"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontshoot"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftshoot"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftshoot"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackshoot"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogbackshoot"));
    
    std::shared_ptr<AnimationSceneNode> mediumDogDash = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    mediumDogDash->setAnchor(Vec2::ANCHOR_CENTER);
    mediumDogDash->setContentSize(DOG_SIZE);
    
    
    std::shared_ptr<AnimationSceneNode> clientMediumDogDash = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    clientMediumDogDash->setAnchor(Vec2::ANCHOR_CENTER);
    clientMediumDogDash->setContentSize(DOG_SIZE);
    
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
    
    std::shared_ptr<AnimationSceneNode> clientLargeDogRun = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    clientLargeDogRun->setAnchor(Vec2::ANCHOR_CENTER);
    clientLargeDogRun->setContentSize(DOG_SIZE);
    
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
    
    std::shared_ptr<AnimationSceneNode> clientLargeDogIdle = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    clientLargeDogIdle->setAnchor(Vec2::ANCHOR_CENTER);
    clientLargeDogIdle->setContentSize(DOG_SIZE);
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("largedogrightbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogrightbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogfrontbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogfrontbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogbackbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogbackbite"));

    std::shared_ptr<AnimationSceneNode> largeDogBite = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 3);
    largeDogBite->setAnchor(Vec2::ANCHOR_CENTER);
    largeDogBite->setContentSize(DOG_SIZE);
    
    std::shared_ptr<AnimationSceneNode> clientLargeDogBite = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    clientLargeDogBite->setAnchor(Vec2::ANCHOR_CENTER);
    clientLargeDogBite->setContentSize(DOG_SIZE);
    
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
    
    std::shared_ptr<AnimationSceneNode> clientLargeDogShoot = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 5);
    clientLargeDogShoot->setAnchor(Vec2::ANCHOR_CENTER);
    clientLargeDogShoot->setContentSize(DOG_SIZE);
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("largedogrightshoot"));
    textures.push_back(_assets->get<cugl::Texture>("largedogrightshoot"));
    textures.push_back(_assets->get<cugl::Texture>("largedogfrontshoot"));
    textures.push_back(_assets->get<cugl::Texture>("largedogfrontshoot"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftshoot"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftshoot"));
    textures.push_back(_assets->get<cugl::Texture>("largedogbackshoot"));
    textures.push_back(_assets->get<cugl::Texture>("largedogbackshoot"));
    
    std::shared_ptr<AnimationSceneNode> largeDogDash = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    largeDogDash->setAnchor(Vec2::ANCHOR_CENTER);
    largeDogDash->setContentSize(DOG_SIZE);
    
    
    std::shared_ptr<AnimationSceneNode> clientLargeDogDash = AnimationSceneNode::allocWithTextures(textures, 1,4, 4, 15);
    clientLargeDogDash->setAnchor(Vec2::ANCHOR_CENTER);
    clientLargeDogDash->setContentSize(DOG_SIZE);
    
    // MAGIC NUMBER ALERT!!
    Vec2 dogPos = _level->getPlayerPos();
    _dog = Dog::alloc(dogPos, DOG_SIZE);
    _dog->setDebugColor(DYNAMIC_COLOR);

    _dog->setSmallAnimation(smallDogIdle, smallDogRun, smallDogBite, smallDogShoot, smallDogDash);
    _dog->setMediumAnimation(mediumDogIdle, mediumDogRun, mediumDogBite, mediumDogShoot, mediumDogDash);
    _dog->setLargeAnimation(largeDogIdle, largeDogRun, largeDogBite, largeDogShoot, largeDogDash);
    
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
    
    
    Vec2 dogClientPos = _level->getPlayerPos() + Vec2(2,0);
    _dogClient = Dog::alloc(dogClientPos, DOG_SIZE);
    _dogClient->setDebugColor(DYNAMIC_COLOR);

    _dogClient->setSmallAnimation(clientSmallDogIdle, clientSmallDogRun, clientSmallDogBite, clientSmallDogShoot, clientSmallDogDash);
    _dogClient->setMediumAnimation(clientMediumDogIdle, clientMediumDogRun, clientMediumDogBite, clientMediumDogShoot, clientMediumDogDash);
    _dogClient->setLargeAnimation(clientLargeDogIdle, clientLargeDogRun, clientLargeDogBite, clientLargeDogShoot, clientLargeDogDash);
    
    std::shared_ptr<cugl::scene2::SceneNode> clientPlaceHolderDrawOver = scene2::SceneNode::allocWithBounds(DOG_SIZE);
    // MORE MAGIC NUMBER ALERT
    
    //placeHolderDrawOver->setContentSize(DOG_SIZE);
    _dogClient->setFinalDog(clientPlaceHolderDrawOver);
    clientPlaceHolderDrawOver->setScale(DOG_SIZE.y / textures.at(0)->getHeight());
    
    std::shared_ptr<scene2::SceneNode> _clientUINode = scene2::SceneNode::alloc();
    _uinode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    std::shared_ptr<UIController> _clientUIController = std::make_shared<UIController>();
    _uiController->init(_clientUINode, _assets, _activeSize);
    _dogClient->setUIController(_uiController);
    
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

bool OverWorld::initPolygons(){
    _attackPolygonSet.init();
    _attackPolygonSet.setTexture(_assets->get<cugl::Texture>("leftbite"),_assets->get<cugl::Texture>("rightbite"),_assets->get<cugl::Texture>("frontbite"),_assets->get<cugl::Texture>("leftbite"));
    return true;
}

bool OverWorld::init(const std::shared_ptr<cugl::AssetManager>& assets, const std::shared_ptr<LevelModel>& level, cugl::Size activeSize,std::shared_ptr<cugl::physics2::net::NetEventController> network, bool isHost){
    _assets = assets;
    _level = level;
    _network = network;
    _isHost = isHost;
    _activeSize = activeSize;
    _constants = assets->get<cugl::JsonValue>("constants");
    
    initDogModel();
    initBases();
    initDecoys();
    initPolygons();
//    initDevil();
    return true;
}

bool OverWorld::setRootNode(const std::shared_ptr<scene2::SceneNode>& _worldNode, const std::shared_ptr<scene2::SceneNode>& _debugNode, std::shared_ptr<cugl::physics2::net::NetWorld> _world){
    // Add Base Decoy node
    _worldNode->addChild(_decoys->getDecoySetNode());
    
    // Add Bases to the World Node
    for (auto& base : _bases->_bases){
        std::shared_ptr<scene2::SceneNode> baseNode = base->getSceneNode();
        _worldNode->addChild(base->getSceneNode());
    }
    
    // Add Dog to Obstacle World
    _world->initObstacle(_dog);
    _dog->setDebugScene(_debugNode);
    if (_isHost){
        _world->getOwnedObstacles().insert({_dog,0});
    }
    _worldNode->addChild(_dog->getDogNode());
    
    if (_network->getNumPlayers() == 2){
        _world->initObstacle(_dogClient);
        _dogClient->setDebugScene(_debugNode);
        if (_isHost){
            _world->getOwnedObstacles().insert({_dogClient,0});
        }
        _worldNode->addChild(_dogClient->getDogNode());
        if(!_isHost){
            _network->getPhysController()->acquireObs(_dogClient, 0);
        }
    }
    
    // Attack Polygon base to the world nOde
    _worldNode->addChild(_attackPolygonSet.getAttackPolygonNode());
    
    
    // Add Obstacles
    return true;
}
void OverWorld::processDashEvent(const std::shared_ptr<DashEvent>& dashEvent){
    bool incomingHost = dashEvent->getHost();
    if (incomingHost){
        _dog->startDash();
    }else{
        _dogClient->startDash();
    }
}
void OverWorld::processSizeEvent(const std::shared_ptr<SizeEvent>& sizeEvent){
    bool incomingHost = sizeEvent->isHost();
    bool currentHost = _isHost;
    CULog("processing Size Event %d %d", incomingHost, currentHost);
    if (incomingHost != currentHost){ // means we received from other person
        if (incomingHost){ // means incoming is from Original host
            _dog->updateDogSize(sizeEvent->getSize());
        }
    }
}
void OverWorld::processBiteEvent(const std::shared_ptr<BiteEvent>& biteEvent){
    Vec2 center = biteEvent->getPos();
    float ang = biteEvent->getAng();
    _attackPolygonSet.addBite(center, ang, _dog->getBiteRadius(), _dog->getAbsorb()/_dog->getMaxAbsorb());
    bool incomingHost = biteEvent->isHost();
    if (incomingHost){

        _dog->startBite();
    }else{
        _dogClient->startBite();
    }
}

void OverWorld::processShootEvent(const std::shared_ptr<ShootEvent>& shootEvent){
    Vec2 center = shootEvent->getPos();
    float ang = shootEvent->getAng();
    _attackPolygonSet.addShoot(center,ang, _dog->getShootRadius());
    bool incomingHost = shootEvent->isHost();
    if (incomingHost){
        _dog->startShoot();
    }else{
        _dogClient->startShoot();
    }
}
void OverWorld::processExplodeEvent(const std::shared_ptr<ExplodeEvent>& explodeEvent){
    Vec2 center = explodeEvent->getPos();
    _attackPolygonSet.addExplode(center, _dog->getExplosionRadius());
}

void OverWorld::ownedDogUpdate(InputController& _input, cugl::Size, std::shared_ptr<Dog> _curDog){
    _curDog->moveOnInputSetAction(_input);
    _curDog->updateUI();
    if (_curDog->shouldSendSize()){
        _curDog->resetSendSize();
        _network->pushOutEvent(SizeEvent::allocSizeEvent(_curDog->getAbsorb(),_isHost));
    }
    if (_input.didPressDash() && _curDog->canDash()){
        _network->pushOutEvent(DashEvent::allocDashEvent(_isHost));
        _curDog->resetDash();
    }
    if (_input.didPressFire() && _curDog->canFireWeapon()){
        CULog("Send Attack");
        _network->pushOutEvent(BiteEvent::allocBiteEvent(_curDog->getBiteCenter(), _curDog->getDirInDegrees(), _isHost));
        _curDog->reloadWeapon();
    }
    if (_input.didPressSpecial() && _curDog->canFireWeapon()){
        _curDog->reloadWeapon();
        if (_curDog->getMode() == "SHOOT" && _curDog->getAbsorb() >= 5){
            _curDog->subAbsorb(5);
            _network->pushOutEvent(ShootEvent::allocShootEvent(_curDog->getShootCenter(), _curDog->getDirInDegrees(),_isHost));
        }else if (_curDog->getMode() == "BAIT" && _curDog->getAbsorb() >= 5){
            _curDog->subAbsorb(5);
            _network->pushOutEvent(DecoyEvent::allocDecoyEvent(_curDog->getPosition()));
        }else if (_curDog->getMode() == "BOMB" && _curDog->getAbsorb() >= 5){
            _curDog->subAbsorb(5);
            _network->pushOutEvent(ExplodeEvent::allocExplodeEvent(_curDog->getPosition()));
        }else {
            CULog("NOTHING\n");
        }
    }
}
void OverWorld::dogUpdate(InputController& _input, cugl::Size totalSize){
    if (_isHost){
        _dogClient->updateClientAnimations();
        ownedDogUpdate(_input,totalSize, _dog);
    }else{
        _dog->updateClientAnimations();
        ownedDogUpdate(_input,totalSize, _dogClient);
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

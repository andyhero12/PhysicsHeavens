//
//  OverWorld.cpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//
/** Opacity of the physics outlines */
#define DYNAMIC_COLOR Color4::YELLOW
#include "OverWorld.h"

#define WORLD_SIZE 3
#define SHOOT_COST 5
#define BOMB_COST 10
#include "NLDog.h"

void OverWorld::reset()
{
    initDogModel();
    initBases();
    initDecoys();
    _attackPolygonSet.init();
    _clientAttackPolygonSet.init();
}

bool OverWorld::initDogModel()
{
    // should start at east and add to vector with animation in clockwise direction
    std::vector<std::shared_ptr<cugl::Texture>> textures;
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightrun"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightrun"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontrun"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontrun"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftrun"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftrun"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogbackrun"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogbackrun"));

    std::shared_ptr<AnimationSceneNode> smallDogRun = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 5);
    smallDogRun->setAnchor(Vec2::ANCHOR_CENTER);
    // surely there is a more intelligent way to do this??
    smallDogRun->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> clientSmallDogRun = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 5);
    clientSmallDogRun->setAnchor(Vec2::ANCHOR_CENTER);
    // surely there is a more intelligent way to do this??
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

    std::shared_ptr<AnimationSceneNode> smallDogIdle = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 15);
    smallDogIdle->setAnchor(Vec2::ANCHOR_CENTER);
    smallDogIdle->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> clientSmallDogIdle = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 15);
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

    std::shared_ptr<AnimationSceneNode> smallDogBite = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 3);
    smallDogBite->setAnchor(Vec2::ANCHOR_CENTER);
    smallDogBite->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> clientSmallDogBite = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 15);
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

    std::shared_ptr<AnimationSceneNode> smallDogShoot = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 15);
    smallDogShoot->setAnchor(Vec2::ANCHOR_CENTER);
    smallDogShoot->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> clientSmallDogShoot = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 15);
    clientSmallDogShoot->setAnchor(Vec2::ANCHOR_CENTER);
    clientSmallDogShoot->setContentSize(DOG_SIZE);

    std::vector<std::shared_ptr<cugl::Texture>> dashTextures;
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));

    std::shared_ptr<AnimationSceneNode> smallDogDash = AnimationSceneNode::allocWithTextures(dashTextures, 4, 5, 20, 1);
    smallDogDash->setAnchor(Vec2::ANCHOR_CENTER);
    smallDogDash->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> clientSmallDogDash = AnimationSceneNode::allocWithTextures(dashTextures, 4, 5, 20, 1);
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

    std::shared_ptr<AnimationSceneNode> mediumDogRun = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 5);
    mediumDogRun->setAnchor(Vec2::ANCHOR_CENTER);
    mediumDogRun->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> clientMediumDogRun = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 5);
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

    std::shared_ptr<AnimationSceneNode> mediumDogIdle = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 15);
    mediumDogIdle->setAnchor(Vec2::ANCHOR_CENTER);
    mediumDogIdle->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> clientMediumDogIdle = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 15);
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

    std::shared_ptr<AnimationSceneNode> mediumDogBite = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 3);
    mediumDogBite->setAnchor(Vec2::ANCHOR_CENTER);
    mediumDogBite->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> clientMediumDogBite = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 5);
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

    std::shared_ptr<AnimationSceneNode> mediumDogShoot = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 5);
    mediumDogShoot->setAnchor(Vec2::ANCHOR_CENTER);
    mediumDogShoot->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> clientMediumDogShoot = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 5);
    clientMediumDogShoot->setAnchor(Vec2::ANCHOR_CENTER);
    clientMediumDogShoot->setContentSize(DOG_SIZE);

    dashTextures.clear();
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));

    std::shared_ptr<AnimationSceneNode> mediumDogDash = AnimationSceneNode::allocWithTextures(dashTextures, 4, 5, 20, 1);
    mediumDogDash->setAnchor(Vec2::ANCHOR_CENTER);
    mediumDogDash->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> clientMediumDogDash = AnimationSceneNode::allocWithTextures(dashTextures, 4, 5, 20, 1);
    clientMediumDogDash->setAnchor(Vec2::ANCHOR_CENTER);
    clientMediumDogDash->setContentSize(DOG_SIZE);

    std::vector<std::shared_ptr<cugl::Texture>> largeRun;
    largeRun.push_back(_assets->get<cugl::Texture>("largedogrightrun"));
    largeRun.push_back(_assets->get<cugl::Texture>("largedogrightrun"));
    largeRun.push_back(_assets->get<cugl::Texture>("largedogfrontrun"));
    largeRun.push_back(_assets->get<cugl::Texture>("largedogfrontrun"));
    largeRun.push_back(_assets->get<cugl::Texture>("largedogleftrun"));
    largeRun.push_back(_assets->get<cugl::Texture>("largedogleftrun"));
    largeRun.push_back(_assets->get<cugl::Texture>("largedogbackrun"));
    largeRun.push_back(_assets->get<cugl::Texture>("largedogbackrun"));

    std::shared_ptr<AnimationSceneNode> largeDogRun = AnimationSceneNode::allocWithTextures(largeRun, 1, 4, 4, 5);
    largeDogRun->setAnchor(Vec2::ANCHOR_CENTER);
    largeDogRun->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> clientLargeDogRun = AnimationSceneNode::allocWithTextures(largeRun, 1, 4, 4, 5);
    clientLargeDogRun->setAnchor(Vec2::ANCHOR_CENTER);
    clientLargeDogRun->setContentSize(DOG_SIZE);

    
    std::vector<std::shared_ptr<cugl::Texture>> largeIdle;
    largeIdle.clear();
    largeIdle.push_back(_assets->get<cugl::Texture>("largedogrightidle"));
    largeIdle.push_back(_assets->get<cugl::Texture>("largedogrightidle"));
    largeIdle.push_back(_assets->get<cugl::Texture>("largedogfrontidle"));
    largeIdle.push_back(_assets->get<cugl::Texture>("largedogfrontidle"));
    largeIdle.push_back(_assets->get<cugl::Texture>("largedogleftidle"));
    largeIdle.push_back(_assets->get<cugl::Texture>("largedogleftidle"));
    largeIdle.push_back(_assets->get<cugl::Texture>("largedogbackidle"));
    largeIdle.push_back(_assets->get<cugl::Texture>("largedogbackidle"));

    std::shared_ptr<AnimationSceneNode> largeDogIdle = AnimationSceneNode::allocWithTextures(largeIdle, 1, 4, 4, 15);
    largeDogIdle->setAnchor(Vec2::ANCHOR_CENTER);
    largeDogIdle->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> clientLargeDogIdle = AnimationSceneNode::allocWithTextures(largeIdle, 1, 4, 4, 15);
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

    std::shared_ptr<AnimationSceneNode> largeDogBite = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 3);
    largeDogBite->setAnchor(Vec2::ANCHOR_CENTER);
    largeDogBite->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> clientLargeDogBite = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 5);
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

    std::shared_ptr<AnimationSceneNode> largeDogShoot = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 5);
    largeDogShoot->setAnchor(Vec2::ANCHOR_CENTER);
    largeDogShoot->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> clientLargeDogShoot = AnimationSceneNode::allocWithTextures(textures, 1, 4, 4, 5);
    clientLargeDogShoot->setAnchor(Vec2::ANCHOR_CENTER);
    clientLargeDogShoot->setContentSize(DOG_SIZE);

    dashTextures.clear();
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));
    dashTextures.push_back(_assets->get<cugl::Texture>("dash"));

    std::shared_ptr<AnimationSceneNode> largeDogDash = AnimationSceneNode::allocWithTextures(dashTextures, 4, 5, 20, 1);
    largeDogDash->setAnchor(Vec2::ANCHOR_CENTER);
    largeDogDash->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> clientLargeDogDash = AnimationSceneNode::allocWithTextures(dashTextures, 4, 5, 20, 1);
    clientLargeDogDash->setAnchor(Vec2::ANCHOR_CENTER);
    clientLargeDogDash->setContentSize(DOG_SIZE);
        
    std::vector<std::shared_ptr<cugl::Texture>> recallTextures;
    recallTextures.push_back(_assets->get<cugl::Texture>("recall"));
    recallTextures.push_back(_assets->get<cugl::Texture>("recall"));
    recallTextures.push_back(_assets->get<cugl::Texture>("recall"));
    recallTextures.push_back(_assets->get<cugl::Texture>("recall"));
    recallTextures.push_back(_assets->get<cugl::Texture>("recall"));
    recallTextures.push_back(_assets->get<cugl::Texture>("recall"));
    recallTextures.push_back(_assets->get<cugl::Texture>("recall"));
    recallTextures.push_back(_assets->get<cugl::Texture>("recall"));

    std::shared_ptr<AnimationSceneNode> recall = AnimationSceneNode::allocWithTextures(recallTextures, 6,5, 30, 2);
    recall->setAnchor(Vec2::ANCHOR_CENTER);
    recall->setContentSize(DOG_SIZE);
    
    std::shared_ptr<AnimationSceneNode> recallClient = AnimationSceneNode::allocWithTextures(recallTextures, 6,5, 30, 2);
    recallClient->setAnchor(Vec2::ANCHOR_CENTER);
    recallClient->setContentSize(DOG_SIZE);
    
    recallTextures.clear();
    recallTextures.push_back(_assets->get<cugl::Texture>("penta"));
    recallTextures.push_back(_assets->get<cugl::Texture>("penta"));
    recallTextures.push_back(_assets->get<cugl::Texture>("penta"));
    recallTextures.push_back(_assets->get<cugl::Texture>("penta"));
    recallTextures.push_back(_assets->get<cugl::Texture>("penta"));
    recallTextures.push_back(_assets->get<cugl::Texture>("penta"));
    recallTextures.push_back(_assets->get<cugl::Texture>("penta"));
    recallTextures.push_back(_assets->get<cugl::Texture>("penta"));

    std::shared_ptr<AnimationSceneNode> belowPenta = AnimationSceneNode::allocWithTextures(recallTextures, 6,5, 30, 2);
    belowPenta->setAnchor(Vec2::ANCHOR_CENTER);
    belowPenta->setContentSize(DOG_SIZE);

    std::shared_ptr<AnimationSceneNode> belowPentaClient = AnimationSceneNode::allocWithTextures(recallTextures,6,5, 30, 2);
    belowPentaClient->setAnchor(Vec2::ANCHOR_CENTER);
    belowPentaClient->setContentSize(DOG_SIZE);
    
    // MAGIC NUMBER ALERT!!
    Vec2 dogPos = _level->getPlayerPos();
    _dog = Dog::alloc(dogPos, DOG_SIZE);
    _dog->setDebugColor(DYNAMIC_COLOR);

    _dog->setSmallAnimation(smallDogIdle, smallDogRun, smallDogBite, smallDogShoot, smallDogDash);
    _dog->setMediumAnimation(mediumDogIdle, mediumDogRun, mediumDogBite, mediumDogShoot, mediumDogDash);
    _dog->setLargeAnimation(largeDogIdle, largeDogRun, largeDogBite, largeDogShoot, largeDogDash);

    std::shared_ptr<cugl::scene2::OrderedNode> placeHolderDrawOver = cugl::scene2::OrderedNode::allocWithOrder(cugl::scene2::OrderedNode::Order::PRE_ORDER, DOG_SIZE);
    // MORE MAGIC NUMBER ALERT
    std::shared_ptr<cugl::Texture> barImage = _assets->get<cugl::Texture>("progress_dog");
    
    float textureWidth = barImage->getWidth();
    float textureHeight = barImage->getHeight();
    
    std::shared_ptr<cugl::Texture> bg = barImage->getSubTexture(0/textureWidth, 320/textureWidth, 0/textureHeight, 45/textureHeight);
    std::shared_ptr<cugl::Texture> fg = barImage->getSubTexture(24/textureWidth, 296/textureWidth, 45/textureHeight, 90/textureHeight);
    std::shared_ptr<cugl::Texture> left_cap = barImage->getSubTexture(0/textureWidth, 24/textureWidth, 45/textureHeight, 90/textureHeight);
    std::shared_ptr<cugl::Texture> right_cap = barImage->getSubTexture(296/textureWidth, 320/textureWidth, 45/textureHeight, 90/textureHeight);
    
    std::shared_ptr<cugl::scene2::ProgressBar> _bar = cugl::scene2::ProgressBar::allocWithCaps(bg, fg, left_cap, right_cap);
    _bar->setProgress(1.0f);
    _bar->setPosition(-32, 44);
    //placeHolderDrawOver->addChild(_bar);
    _dog->setHealthBar(_bar);
    
    _dog->setRecallAnimation(recall);
    _dog->setBelowPenta(belowPenta);
    _dog->setFinalDog(placeHolderDrawOver);
    //placeHolderDrawOver->setScale(DOG_SIZE.y / textures.at(0)->getHeight());
    _dog->updateDogSize(_dog->getAbsorb());
    // placeHolderDrawOver->setAnchor(Vec2::ANCHOR_CENTER);

    std::shared_ptr<scene2::SceneNode> _uinode = scene2::SceneNode::alloc();
    _uinode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    std::shared_ptr<UIController> _uiController = std::make_shared<UIController>();
    _uiController->init(_uinode, _assets, _activeSize);
    _dog->setUIController(_uiController);

    Vec2 dogClientPos = _level->getPlayerPos() + Vec2(2, 0);
    _dogClient = Dog::alloc(dogClientPos, DOG_SIZE);
    _dogClient->setDebugColor(DYNAMIC_COLOR);

    _dogClient->setSmallAnimation(clientSmallDogIdle, clientSmallDogRun, clientSmallDogBite, clientSmallDogShoot, clientSmallDogDash);
    _dogClient->setMediumAnimation(clientMediumDogIdle, clientMediumDogRun, clientMediumDogBite, clientMediumDogShoot, clientMediumDogDash);
    _dogClient->setLargeAnimation(clientLargeDogIdle, clientLargeDogRun, clientLargeDogBite, clientLargeDogShoot, clientLargeDogDash);

    std::shared_ptr<cugl::scene2::OrderedNode> clientPlaceHolderDrawOver = cugl::scene2::OrderedNode::allocWithOrder(cugl::scene2::OrderedNode::Order::PRE_ORDER, DOG_SIZE);
    // MORE MAGIC NUMBER ALERT

    std::shared_ptr<cugl::scene2::ProgressBar> _barClient = cugl::scene2::ProgressBar::allocWithCaps(bg, fg, left_cap, right_cap);
    _barClient->setProgress(1.0f);
    _barClient->setPosition(-32, 44);
    //placeHolderDrawOver->addChild(_bar);
    _dogClient->setHealthBar(_barClient);

    _dogClient->setRecallAnimation(recallClient);
    _dogClient->setBelowPenta(belowPentaClient);
    _dogClient->setFinalDog(clientPlaceHolderDrawOver);
    //clientPlaceHolderDrawOver->setScale(DOG_SIZE.y / textures.at(0)->getHeight());
    _dogClient->updateDogSize(_dog->getAbsorb());

    std::shared_ptr<scene2::SceneNode> _clientUINode = scene2::SceneNode::alloc();
    _uinode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    std::shared_ptr<UIController> _clientUIController = std::make_shared<UIController>();
    _clientUIController->init(_clientUINode, _assets, _activeSize);
    _dogClient->setUIController(_clientUIController);

    return true;
}

bool OverWorld::initBases()
{
    _bases = std::make_shared<BaseSet>();
    _bases->init(_level->getBasesPos(), _assets);
    
    std::shared_ptr<scene2::SceneNode> _uinode = scene2::SceneNode::alloc();
    _uinode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    
    _gateUIController = std::make_shared<GateUIController>();
    _gateUIController->init(_uinode, _assets, _activeSize, _bases);
    
    return true;
}

bool OverWorld::initDecoys()
{
    _decoys = std::make_shared<DecoySet>();
    _decoys->init();
    _decoys->setAsset(_assets);
    _decoys->setTexture(_assets->get<cugl::Texture>("base"));
    _decoys->setExplodeTexture(_assets->get<cugl::Texture>("explodingGate"));
    return true;
}

bool OverWorld::initPolygons()
{
    _attackPolygonSet.init();
    _attackPolygonSet.setTexture(_assets->get<cugl::Texture>("leftbite"), _assets->get<cugl::Texture>("rightbite"), _assets->get<cugl::Texture>("frontbite"), _assets->get<cugl::Texture>("backbite"));
    _attackPolygonSet.setTextureShoot(_assets->get<cugl::Texture>("leftShoot"), _assets->get<cugl::Texture>("rightShoot"), _assets->get<cugl::Texture>("downShoot"), _assets->get<cugl::Texture>("upShoot"));
    
    _attackPolygonSet.setBombTexture(_assets->get<cugl::Texture>("explodingGate"));
    
    _clientAttackPolygonSet.init();
    _clientAttackPolygonSet.setTexture(_assets->get<cugl::Texture>("leftbite"), _assets->get<cugl::Texture>("rightbite"), _assets->get<cugl::Texture>("frontbite"), _assets->get<cugl::Texture>("backbite"));
    _clientAttackPolygonSet.setTextureShoot(_assets->get<cugl::Texture>("leftShoot"), _assets->get<cugl::Texture>("rightShoot"), _assets->get<cugl::Texture>("downShoot"), _assets->get<cugl::Texture>("upShoot"));
    _clientAttackPolygonSet.setBombTexture(_assets->get<cugl::Texture>("explodingGate"));
    return true;
}

// Placeholder function if we want to move world instantiation into here later, imo moving it here makes more sense - Colin
bool OverWorld::initWorld()
{
    return true;
}

bool OverWorld::init(const std::shared_ptr<cugl::AssetManager> &assets, const std::shared_ptr<LevelModel> &level, cugl::Size activeSize, std::shared_ptr<cugl::physics2::net::NetEventController> network, bool isHost, std::shared_ptr<World> world, std::shared_ptr<AudioController> audioController)
{
    _audioController = audioController;
    _assets = assets;
    _level = level;
    _network = network;
    _isHost = isHost;
    _activeSize = activeSize;
    _constants = assets->get<cugl::JsonValue>("constants");
    _world = world;

    dogSeq = 0;
    
    initWorld();
    initDogModel();
    initBases();
    initDecoys();
    initPolygons();
    return true;
}

bool OverWorld::setRootNode(const std::shared_ptr<scene2::SceneNode> &_worldNode, const std::shared_ptr<scene2::SceneNode> &_debugNode, std::shared_ptr<cugl::physics2::net::NetWorld> _worldNet)
{
    // Add Base Decoy node
    _worldNode->addChild(_decoys->getDecoySetNode());

    _worldNode->addChild(_bases->getBaseSetNode());

    // Add Dog to Obstacle World
    _worldNet->initObstacle(_dog);
    _dog->setDebugScene(_debugNode);
    if (_isHost)
    {
        _worldNet->getOwnedObstacles().insert({_dog, 0});
    }
    _worldNode->addChild(_dog->getDogNode());
    _dog->addEffects(_attackPolygonSet.getFrontAttackPolygonNode(), _attackPolygonSet.getBackAttackPolygonNode());
    _dogClient->addEffects(_clientAttackPolygonSet.getFrontAttackPolygonNode(), _clientAttackPolygonSet.getBackAttackPolygonNode());
    //    _dog->getDogNode()->addChildWithTag(_attackPolygonSet.getAttackPolygonNode(), 0);
    //    _attackPolygonSet.getAttackPolygonNode()->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);

    if (_network->getNumPlayers() == 2)
    {
        _worldNet->initObstacle(_dogClient);
        _dogClient->setDebugScene(_debugNode);
        _worldNode->addChild(_dogClient->getDogNode());
        if (!_isHost)
        {
            _network->getPhysController()->acquireObs(_dogClient, 0);
        }
    }

    // Attack Polygon base to the world nOde
    //    _worldNode->addChild(_attackPolygonSet.getAttackPolygonNode());

    // Add Obstacles
    return true;
}
void OverWorld::processDashEvent(const std::shared_ptr<DashEvent> &dashEvent)
{
    bool incomingHost = dashEvent->getHost();
    if (incomingHost)
    {
        _dog->startDash();
    }
    else
    {
        _dogClient->startDash();
    }
    if (incomingHost == _isHost){
        playSound("dogDash" + std::to_string(dogSeq), DOG_DASH);
    }
}
void OverWorld::processDecoyEvent(const std::shared_ptr<DecoyEvent>& decoyEvent){
    getDecoys()->addNewDecoy(Vec2(decoyEvent->getPos().x, decoyEvent->getPos().y));
    playSound("dogGate"+ std::to_string(dogSeq), DUMMY_GATE_PLACEMENT);
}
void OverWorld::processClientHealthEvent(const std::shared_ptr<ClientHealthEvent>& clientHealthEvent){
    bool incomingHost = clientHealthEvent->isHostDog();
    int health = 0;
    bool deathCnt = true;
    if (incomingHost){
        health = _dog->getHealth() - clientHealthEvent->getHealthLost();
        _dog->setHealth(health);
    }else{
        health = _dogClient->getHealth() - clientHealthEvent->getHealthLost();
        _dogClient->setHealth(health);
    }
    if(health <= 0){
        if (deathCnt){
            auto source = _assets->get<Sound>(DOG_DIE);
            AudioEngine::get()->play(DOG_DIE, source, false , source->getVolume());
            deathCnt = false;
        }
    }
    else{
        playSound("heal" + std::to_string(dogSeq), DOG_DAMAGE);
    }
}
void OverWorld::processSizeEvent(const std::shared_ptr<SizeEvent> &sizeEvent)
{
    bool incomingHost = sizeEvent->isHost();
    bool maxSize = false;
    if (incomingHost)
    { // means incoming is from Original host
        maxSize = _dog->addAbsorb(sizeEvent->getSize());
    }else{
        maxSize = _dogClient->addAbsorb(sizeEvent->getSize());
    }
    if (incomingHost == _isHost && maxSize){
        playSound("max size" + std::to_string(dogSeq), SIZE_BAR_MAX);
    }
}
void OverWorld::processBiteEvent(const std::shared_ptr<BiteEvent> &biteEvent)
{
    Vec2 center = biteEvent->getPos();
    float ang = biteEvent->getAng();
    bool incomingHost = biteEvent->isHost();
    float scale = biteEvent->getScale();
    std::string sound;
    std::shared_ptr<Dog> playerDog;
  
    if (incomingHost)
    {
        playerDog = _dog;
        _attackPolygonSet.addBite(center, ang, _dog->getBiteRadius(), scale);

    }
    else
    {
        playerDog = _dogClient;
        _clientAttackPolygonSet.addBite(center, ang, _dogClient->getBiteRadius(), scale);
    }
    
    
    playerDog->startBite();
    
    if (incomingHost == _isHost){
        switch (playerDog->getSize()) {
            case Dog::DogSize::SMALL:
                sound = DOG_BITE_SMALL;
                break;
            case Dog::DogSize::MEDIUM:
                sound = DOG_BITE_MEDIUM;
                break;
            case Dog::DogSize::LARGE:
                sound = DOG_BITE_LARGE;
                break;
            default:
                CUAssert(false);
                break;
        }
        playSound("dogBite"+ std::to_string(dogSeq), sound);
    }
}
void OverWorld::processRecallEvent(const std::shared_ptr<RecallEvent>& recallEvent){
    bool incomingHost = recallEvent->isHost();
    if (incomingHost){
        _dog->startRecall();
    }else{
        _dogClient->startRecall();
    }
    
    if (incomingHost == _isHost){
        playSound("dogTeleport"+ std::to_string(dogSeq), DOG_TELEPORT);
    }
}

void OverWorld::processShootEvent(const std::shared_ptr<ShootEvent> &shootEvent)
{
    Vec2 center = shootEvent->getPos();
    float ang = shootEvent->getAng();
    float scale = shootEvent->getScale();
    Dog::DogSize size = _dog->getSize();
    bool incomingHost = shootEvent->isHost();
    if (incomingHost)
    {
        if (incomingHost != _isHost){
            _dog->subAbsorb(SHOOT_COST);
            size = _dog->getSize();
        }
        _attackPolygonSet.addShoot(center, ang, scale, _dog->getShootRadius());
        _dog->startShoot();
    }
    else
    {
        if (incomingHost != _isHost){
            _dogClient->subAbsorb(SHOOT_COST);
            size = _dogClient->getSize();
        }
        _clientAttackPolygonSet.addShoot(center, ang, scale, _dog->getShootRadius());
        _dogClient->startShoot();
    }
    if (incomingHost == _isHost){
        if (size == Dog::DogSize::SMALL){
            playSound("dogBark"+ std::to_string(dogSeq), SMALL_BARK);
        }
        else if (size == Dog::DogSize::MEDIUM){
            playSound("dogBark"+ std::to_string(dogSeq), MED_BARK);
        }
        else if (size == Dog::DogSize::LARGE){
            playSound("dogBark"+ std::to_string(dogSeq), BIG_BARK);
        }
//        playSound("dogBark"+ std::to_string(dogSeq), DOG_BARK);
    }
}
void OverWorld::processExplodeEvent(const std::shared_ptr<ExplodeEvent> &explodeEvent)
{
    Vec2 center = explodeEvent->getPos();
    float scale = explodeEvent->getScale();
    bool incomingHost = explodeEvent->isHost();
    if (incomingHost)
    {
        if (incomingHost != _isHost){
            _dog->subAbsorb(BOMB_COST);
        }
        _attackPolygonSet.addExplode(center, scale, _dog->getExplosionRadius());
        _dog->startShoot();
    }
    else
    {
        if (incomingHost != _isHost){
            _dogClient->subAbsorb(BOMB_COST);
        }
        _clientAttackPolygonSet.addExplode(center, scale, _dog->getExplosionRadius());
        _dogClient->startShoot();
    }
    if (incomingHost == _isHost){
        playSound("dogBomb"+ std::to_string(dogSeq), DUMMY_GATE_EXPLOSION);
    }
}

void OverWorld::processBaseHealthEvent(const std::shared_ptr<BaseHealthEvent>& basEvent){
    for (std::shared_ptr<Base> base : getBaseSet()->_bases){
        if (base->getPos() == basEvent->getPos()){
            base->reduceHealth(basEvent->getDamage());
            playSound("gateTakesDamage"+ std::to_string(dogSeq), GATE_TAKES_DAMAGE);
            
        }
    }
}
void OverWorld::recallDogToClosetBase(std::shared_ptr<Dog> _curDog){
    float shortestDist = 1000000.0f;
    Vec2 location;
    for (std::shared_ptr<Base> base : getBaseSet()->_bases){
        Vec2 norm = base->getPos() - _curDog->getPosition();
        float dist = norm.length();
        if (dist < shortestDist){
            shortestDist = dist;
            location = base->getPos();
        }
    }
    _curDog->setPosition(location);
}
void OverWorld::ownedDogUpdate(InputController& _input, cugl::Size, std::shared_ptr<Dog> _curDog){
    _curDog->moveOnInputSetAction(_input);
    _curDog->updateUI();
    if (_curDog->readyToRecall()){
        recallDogToClosetBase(_curDog);
    }
    if (_input.didPressDash() && _curDog->canDash()){
        _network->pushOutEvent(DashEvent::allocDashEvent(_isHost));
        _curDog->resetDash();
    }
    if (_input.didPressFire() && _curDog->canFireWeapon() && _curDog->getAction() != Dog::Actions::DASH)
    {
        float scale = float(_curDog->getAbsorb())/ (float) _curDog->getMaxAbsorb();
        _network->pushOutEvent(BiteEvent::allocBiteEvent(_curDog->getBiteCenter(), _curDog->getDirInDegrees(),scale,  _isHost));
        _curDog->reloadWeapon();
    }
    if (_input.didPressSpecial() && _curDog->canFireWeapon()&& _curDog->getAction() != Dog::Actions::DASH)
    {
        _curDog->reloadWeapon();
        if (_curDog->getMode() == "SHOOT" && _curDog->getAbsorb() >= 5)
        {
            _curDog->subAbsorb(SHOOT_COST);
            float scale = float(_curDog->getAbsorb())/ (float) _curDog->getMaxAbsorb()/2;
            _network->pushOutEvent(ShootEvent::allocShootEvent(_curDog->getShootCenter(), _curDog->getDirInDegrees(), scale, _isHost));
        }
        else if (_curDog->getMode() == "BAIT" && _curDog->getAbsorb() >= 5)
        {
            _curDog->subAbsorb(5);
            _network->pushOutEvent(DecoyEvent::allocDecoyEvent(_curDog->getPosition()));
        }
        else if (_curDog->getMode() == "BOMB" && _curDog->getAbsorb() >= 5)
        {
            _curDog->subAbsorb(BOMB_COST);
            float scale = float(_curDog->getAbsorb())/ (float) _curDog->getMaxAbsorb()/2;
            _network->pushOutEvent(ExplodeEvent::allocExplodeEvent(_curDog->getPosition(),scale,  _isHost));
        }else if (_curDog->getMode() == "RECALL"){
            _network->pushOutEvent(RecallEvent::allocRecallEvent(_curDog->getPosition(),_isHost));
        }else {
            CULog("NOTHING\n");
        }
    }
}
void OverWorld::dogUpdate(InputController &_input, cugl::Size totalSize)
{
    std::shared_ptr<Dog> playerDog;
    std::shared_ptr<Dog> otherDog;
    if (_isHost)
    {
        playerDog = _dogClient;
        otherDog = _dog;
    }
    else
    {
        playerDog = _dog;
        otherDog = _dogClient;
    }
    playerDog->updateClientAnimations();
    ownedDogUpdate(_input, totalSize, otherDog);
}

void OverWorld::update(InputController &_input, cugl::Size totalSize, float timestep, bool dogActive)
{
    if(dogActive) {
        dogUpdate(_input, totalSize);
    }
    _bases->update();
    _decoys->update(timestep);
    _attackPolygonSet.update();
    _clientAttackPolygonSet.update();
    _gateUIController->update(timestep);
}

void OverWorld::postUpdate()
{
    _decoys->postUpdate();
}
void OverWorld::dispose(){
    _level = nullptr;
    _network = nullptr;
    _dog = nullptr;
    _dogClient = nullptr;
    _decoys = nullptr;
    _bases = nullptr;
    _constants = nullptr;
    _assets = nullptr;
    _attackPolygonSet.dispose();
    _clientAttackPolygonSet.dispose();
    _world = nullptr;
}

void OverWorld::playSound(std::string key, std::string sound){
    dogSeq += 1;
    _audioController->playSFX(key, sound);
}

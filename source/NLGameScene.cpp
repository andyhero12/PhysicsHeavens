//
//  NLGameScene.h
//  Networked Physics Demo
//
//  This is the most important class in this demo.  This class manages the
//  gameplay for this demo.  It also handles collision detection. There is not
//  much to do for collisions; our ObstacleWorld class takes care of all
//  of that for us.  This controller mainly transforms input into gameplay.
//
//  You will notice that we do not use a Scene asset this time.  While we could
//  have done this, we wanted to highlight the issues of connecting physics
//  objects to scene graph objects.  Hence we include all of the API calls.
//
//  WARNING: There are a lot of shortcuts in this design that will do not adapt
//  well to data driven design.  This demo has a lot of simplifications to make
//  it a bit easier to see how everything fits together.  However, the model
//  classes and how they are initialized will need to be changed if you add
//  dynamic level loading.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White
//  Version: 1/10/17
//
#include "NLGameScene.h"
#include <box2d/b2_world.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_collision.h>

#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include "SaveManager.h"

using namespace cugl;
using namespace cugl::physics2::net;

#pragma mark -
#pragma mark Level Geography

/** This is the size of the active portion of the screen */
#define SCENE_WIDTH 1280
#define SCENE_HEIGHT 800

#define CANVAS_TILE_HEIGHT 8
#define TILE_NAME   "TILE"
/** Width of the game world in Box2d units */
#define DEFAULT_WIDTH 300.0f
/** Height of the game world in Box2d units */
#define DEFAULT_HEIGHT 300.0f
/** The default value of gravity (going down) */
#define DEFAULT_GRAVITY -0.0f

#define DEFAULT_TURN_RATE 0.05f

#pragma mark Assset Constants
/** The key for the fire textures in the asset manager */
#define MAIN_FIRE_TEXTURE "flames"
#define RGHT_FIRE_TEXTURE "flames-right"
#define LEFT_FIRE_TEXTURE "flames-left"

/** Color to outline the physics nodes */
#define STATIC_COLOR Color4::WHITE
/** Opacity of the physics outlines */
#define DYNAMIC_COLOR Color4::YELLOW

/** The key for collisions sounds */
#define COLLISION_SOUND "bump"
/** The key for the main afterburner sound */
#define MAIN_FIRE_SOUND "burn"
/** The key for the right afterburner sound */
#define RGHT_FIRE_SOUND "right-burn"
/** The key for the left afterburner sound */
#define LEFT_FIRE_SOUND "left-burn"

/** The key for the font reference */
#define PRIMARY_FONT "retro"

#pragma mark Physics Constants

// Physics constants for initialization
/** Density of non-crate objects */
#define BASIC_DENSITY 0.0f
/** Friction of non-crate objects */
#define BASIC_FRICTION 0.1f
/** Collision restitution for all objects */
#define BASIC_RESTITUTION 0.1f
/** Threshold for generating sound on collision */
#define SOUND_THRESHOLD 3

#define FIXED_TIMESTEP_S 0.02f
#define ROOT_NODE_SCALE 1

#define SHAKING_DECAY 100.0f

#pragma mark -
#pragma mark Constructors
/**
 * Creates a new game world with the default values.
 *
 * This constructor does not allocate any objects or start the controller.
 * This allows us to use a controller without a heap pointer.
 */
GameScene::GameScene() : cugl::Scene2(),
                         _debug(false),
                         _isHost(false),
                         olddogPos(Vec2(0, 0)),
                         _network(nullptr),
                         winNode(nullptr),
                         loseNode(nullptr),
                         repeatWinNode(nullptr),
                         tutorialIndex(0),
tutorialArrow(nullptr),
gameOverLoss(false),
gameOverWin(false),
gameOverDelay(0)

{
}

/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * The game world is scaled so that the screen coordinates do not agree
 * with the Box2d coordinates.  This initializer uses the default scale.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool GameScene::init(const std::shared_ptr<AssetManager> &assets, const std::shared_ptr<cugl::physics2::net::NetEventController> network, bool isHost, std::string level, int levelNum)
{
    return init(assets, Rect(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT), Vec2(0, DEFAULT_GRAVITY), network, isHost, level, levelNum);
}

/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * The game world is scaled so that the screen coordinates do not agree
 * with the Box2d coordinates.  The bounds are in terms of the Box2d
 * world, not the screen.
 *
 * @param assets    The (loaded) assets for this game mode
 * @param rect      The game bounds in Box2d coordinates
 *
 * @return  true if the controller is initialized properly, false otherwise.
 */
bool GameScene::init(const std::shared_ptr<AssetManager> &assets, const Rect rect, const std::shared_ptr<NetEventController> network, bool isHost, std::string level, int levelNum)
{
    return init(assets, rect, Vec2(0, DEFAULT_GRAVITY), network, isHost, level, levelNum);
}

/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * The game world is scaled so that the screen coordinates do not agree
 * with the Box2d coordinates.  The bounds are in terms of the Box2d
 * world, not the screen.
 *
 * @param assets    The (loaded) assets for this game mode
 * @param rect      The game bounds in Box2d coordinates
 * @param gravity   The gravitational force on this Box2d world
 *
 * @return  true if the controller is initialized properly, false otherwise.
 */
bool GameScene::init(const std::shared_ptr<AssetManager> &assets, const Rect rect, const Vec2 gravity, const std::shared_ptr<NetEventController> network, bool isHost, std::string level_string, int levelNum)
{
    Size dimen = computeActiveSize();

    if (assets == nullptr)
    {
        return false;
    }
    else if (!Scene2::init(dimen))
    {
        return false;
    }

    _audioController = AudioController::alloc(assets);
    
    _levelNum = levelNum;
    _isHost = isHost;
    gameOverLoss = false;
    gameOverWin = false;
    gameOverDelay = 0;
    _network = network;

    // Start up the input handler
    _level = assets->get<LevelModel>(level_string);
    _level->setTileSetAssets(assets);
    _constants = assets->get<JsonValue>("constants");
    if (_level == nullptr)
    {
        // Might need to change later if too many assets Copy Tiled Demo?
        CULog("Fail!");
        return false;
    }
    _assets = assets;
    _input.init();

    // IMPORTANT: SCALING MUST BE UNIFORM
    // This means that we cannot change the aspect ratio of the physics world
    // Shift to center if a bad fit
    Vec2 offset((dimen.width - SCENE_WIDTH) / 2.0f, (dimen.height - SCENE_HEIGHT) / 2.0f);
    float zoom = dimen.height / CANVAS_TILE_HEIGHT;

    _backgroundWrapper = World::alloc(_level, _assets);

    for(cugl::Rect r : _level->getTransparentRects()) {
        int top = ceil(r.origin.y);
        int bottom = floor(r.origin.y - r.size.height);
        int left = floor(r.origin.x);
        int right = ceil(r.origin.x + r.size.width);
        _transparentRects.emplace_back(cugl::Rect(left, bottom, right - left - 1, top - bottom - 1));
    }

    _worldnode = scene2::SceneNode::alloc();
    _worldnode->setScale(zoom);
    _worldnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);

    _monsterSceneNode = scene2::SceneNode::alloc();

//    _debugnode = nullptr;
    _debugnode = scene2::SceneNode::alloc();
    _debugnode->setScale(zoom);
    _debugnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);

    _uinode = scene2::SceneNode::alloc();

    _rootnode = scene2::ScrollPane::allocWithBounds(computeActiveSize() * zoom * 10);
    _rootnode->setContentSize(computeActiveSize() * zoom * 10);
    _rootnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _rootnode->setPosition(offset);

    addChild(_rootnode);
    _rootnode->addChild(_worldnode);
    _rootnode->addChild(_debugnode);

    addChild(_uinode);

    populate();
    std::function<void(const std::shared_ptr<physics2::Obstacle> &, const std::shared_ptr<scene2::SceneNode> &)> linkSceneToObsFunc = [=](const std::shared_ptr<physics2::Obstacle> &obs, const std::shared_ptr<scene2::SceneNode> &node)
    {
        this->linkSceneToObs(obs, node);
    };

    _network->enablePhysics(_world, linkSceneToObsFunc);

    addChildBackground();
    _spawnerController.init(_level->getSpawnersPos(), assets, _network, _audioController);
    _spawnerController.setRootNode(_worldnode, _isHost);
    _worldnode->addChild(_monsterSceneNode);
    overWorld.init(assets, _level, computeActiveSize(), _network, isHost, _backgroundWrapper, _audioController);
    overWorld.setRootNode(_worldnode, _debugnode, _world);
    if (isHost)
    {
        _uinode->addChild(overWorld.getDog()->getUINode());
        overWorld.getClientDog()->getUINode()->setVisible(false);
    }
    else
    {
        _uinode->addChild(overWorld.getClientDog()->getUINode());
        overWorld.getDog()->getUINode()->setVisible(false);
    }
    
    _uinode->addChild(overWorld.getGateUIController()->getUINode());

    _monsterController.setNetwork(_network);
    _monsterController.setAudioController(_audioController);
    
    _monsterController.setMeleeAnimationData(_constants->get("basicEnemy"), assets);
    _monsterController.setSpawnerAnimationData(_constants->get("spawnerEnemy"), assets);
    _monsterController.setBombAnimationData(_constants->get("bomb"), assets);
    _monsterController.setAbsorbAnimationData(_constants->get("absorbEnemy"), assets);
    _monsterController.init(overWorld, _debugnode);

    _spawnerController.setAnimNode(_worldnode);
    _collisionController.init(_network, _assets, _audioController);

    _active = true;
    setDebug(false);

    _network->attachEventType<MonsterHealthEvent>();
    _network->attachEventType<BaseHealthEvent>();
    _network->attachEventType<DecoyEvent>();
    _network->attachEventType<BiteEvent>();
    _network->attachEventType<WinEvent>();
    _network->attachEventType<LoseEvent>();
    _network->attachEventType<RecallEvent>();
    _network->attachEventType<ExplodeEvent>();
    _network->attachEventType<DashEvent>();
    _network->attachEventType<AbsorbEvent>();
    _network->attachEventType<SizeEvent>();
    _network->attachEventType<DeathEvent>();
    _network->attachEventType<ShootEvent>();
    _network->attachEventType<GameResEvent>();
    _network->attachEventType<SpawnerDeathEvent>();
    _network->attachEventType<ClientHealthEvent>();

    // XNA nostalgia
    Application::get()->setClearColor(Color4f::CORNFLOWER);

    _uinode->setContentSize(dimen);
    _uinode->doLayout();
    loseNode = SpriteAnimationNode::allocWithSheet(_assets->get<cugl::Texture>("lose_screen"), 4, 5, 18, 4);
    loseNode->setScale(5*SCENE_WIDTH/loseNode->getTexture()->getWidth());
    loseNode->setPosition(0.5 * loseNode->getSize());

    winNode = SpriteAnimationNode::allocWithSheet(_assets->get<cugl::Texture>("win_screen"), 1, 11, 11, 4);
    winNode->setScale(SCENE_HEIGHT/winNode->getTexture()->getHeight());
    winNode->setPosition(0.5 * winNode->getSize());
    repeatWinNode = SpriteAnimationNode::allocWithSheet(_assets->get<cugl::Texture>("repeat_win"), 2, 5, 8, 8);
    repeatWinNode->setScale(2* SCENE_HEIGHT/repeatWinNode->getTexture()->getHeight());
    repeatWinNode->setPosition(0.5 * repeatWinNode->getSize());
    _pause = std::make_shared<PauseScene>();
    _pause->init(_assets, computeActiveSize());

    _pause->setContentSize(dimen);
    _pause->doLayout();

    _zoom = ROOT_NODE_SCALE;
    
    Vec2 delta;
    if (isHost)
    {
        delta = overWorld.getDog()->getDogNode()->getWorldPosition();
    }
    else
    {
        delta = overWorld.getClientDog()->getDogNode()->getWorldPosition();
    }
    delta -= (computeActiveSize() / 2);
    _rootnode->applyPan(-delta / _zoom);
    _rootnode->setScale(_zoom);
    previousPan = (-delta / _zoom);
    
    if (isActive()){
        _audioController->playMusic(BGM, BGM);
    }
    else {
        cugl::AudioEngine::get()->clear(BGM);
    }

    addChildForeground();
    resetDraw();
    _minimap = Minimap::alloc(_assets, computeActiveSize(), overWorld, _spawnerController);
    _uinode->addChild(_minimap);
    _uinode->addChild(loseNode);
    _uinode->addChild(winNode);
    _uinode->addChild(repeatWinNode);
    loseNode->setVisible(false);
    winNode->setVisible(false);
    repeatWinNode->setVisible(false);
    if (level_string == LEVEL_ONE_KEY){
        initTutorialOne();
    }
    else if(level_string == LEVEL_TWO_KEY){
        initTutorialTwo();
    }
    else if(level_string == LEVEL_THREE_KEY){
        initTutorialThree();
    }
    _uinode->addChild(_pause);
    return true;
}
void GameScene::resetDraw()
{
    const std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>>& tileDisplay = _backgroundWrapper->getTileDisplay();
    for (int i = 0; i < tileDisplay.size(); i++)
    {
        for (int j = 0; j < tileDisplay.at(i).size(); j++)
        {
            for (const std::shared_ptr<TileInfo> &tile : tileDisplay.at(i).at(j))
            {
                Vec2 dogPos = _isHost ? overWorld.getDog()->getPosition() : overWorld.getClientDog()->getPosition();
                Vec2 tilePos = tile->getTileSprite()->getPosition();
                if (abs(tilePos.x - dogPos.x) > 16 || abs(tilePos.y - dogPos.y) > 16)
                {
                    tile->getTileSprite()->setVisible(false);
                }
                else
                {
                    tile->getTileSprite()->setVisible(true);
                }
            }
        }
    }
}
/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void GameScene::dispose()
{
    if (_active)
    {
        _audioController = nullptr;
        _pause->dispose();
        tutorialTiles.clear();
        _world = nullptr;
        _worldnode = nullptr;
        _rootnode = nullptr;
        _monsterSceneNode = nullptr;
        _network = nullptr;
        _decorToHide.clear();
        _transparentRects.clear();
        _pause = nullptr;
        _level = nullptr;
        winNode = nullptr;
        loseNode = nullptr;
        repeatWinNode = nullptr;
        _uinode = nullptr;
        _debug = false;
        _constants = nullptr;
        _assets = nullptr;

        _monsterController.dispose();
        _spawnerController.dispose();
        _collisionController.dispose();
        overWorld.dispose();
        _backgroundWrapper = nullptr;
        _debugnode = nullptr;
        Scene2::dispose();
    }
}

#pragma mark -
#pragma mark Level Layout
/**
 * Lays out the game geography.
 *
 * Pay close attention to how we attach physics objects to a scene graph.
 * The simplest way is to make a subclass.  However,
 * for simple objects you can just use a callback function to lightly couple
 * them.  This is what we do with the crates.
 *
 * This method is really, really long.  In practice, you would replace this
 * with your serialization loader, which would process a level file.
 */
void GameScene::populate()
{
    _world = physics2::net::NetWorld::alloc(Rect(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT), Vec2(0, DEFAULT_GRAVITY));
//    _world->activateCollisionCallbacks(true);
//    _world->onBeginContact = [this](b2Contact* contact) {
//        this->beginContact(contact);
//    };
//    _world->onEndContact = [this](b2Contact* contact) {
//        this->endContact(contact);
//    };
//    _world->beforeSolve = [this](b2Contact *contact, const b2Manifold *oldManifold)
//    {
//        beforeSolve(contact, oldManifold);
//    };

#pragma mark : Background
}
void GameScene::linkSceneToObs(const std::shared_ptr<physics2::Obstacle> &obj,
                               const std::shared_ptr<scene2::SceneNode> &node)
{

    node->setPosition(obj->getPosition());
    _monsterSceneNode->addChild(node);
    // Dynamic objects need constant updating
    if (obj->getBodyType() == b2_dynamicBody)
    {
        scene2::SceneNode *weak = node.get(); // No need for smart pointer in callback
        obj->setListener([=](physics2::Obstacle *obs)
                         {
            float leftover = Application::get()->getFixedRemainder() / 1000000.f;
            Vec2 pos = obs->getPosition() + leftover * obs->getLinearVelocity();
            float angle = obs->getAngle() + leftover * obs->getAngularVelocity();
            weak->setPosition(pos);
            weak->setAngle(angle); });
    }
}

/**
 * Adds the physics object to the physics world and loosely couples it to the scene graph
 *
 * There are two ways to link a physics object to a scene graph node on the
 * screen.  One way is to make a subclass of a physics object.
 * The other is to use callback functions to loosely couple
 * the two.  This function is an example of the latter.
 *
 * param obj    The physics object to add
 * param node   The scene graph node to attach it to
 */
void GameScene::addInitObstacle(const std::shared_ptr<physics2::Obstacle> &obj,
                                const std::shared_ptr<scene2::SceneNode> &node)
{
    _world->initObstacle(obj);
    obj->setDebugScene(_debugnode);
    if (_isHost)
    {
        _world->getOwnedObstacles().insert({obj, 0});
    }
    linkSceneToObs(obj, node);
}

#pragma mark
#pragma mark Physics Handling

void GameScene::preUpdate(float dt)
{
    updateInputController();

    if (gameOverLoss || gameOverWin){
        if ( gameOverDelay < 120){
            gameOverDelay++;
        }
        if (gameOverDelay >= 120 ){
            if (gameOverWin){
                
                winNode->setVisible(true);
                _pause->setPause(true);
                _minimap->setVisible(false);
                
                
                /** stop all sound and play win screen sound*/
                AudioEngine::get()->clear();
                _audioController->playMusic(VICTORY_SCREEN, VICTORY_SCREEN);
                _audioController->playSFX(WIN_CASH, WIN_CASH);
                _audioController->playSFX(KACHING, KACHING);
            }
            if (gameOverLoss){
                loseNode->setVisible(true);
                _pause->setPause(true);
                _minimap->setVisible(false);
                AudioEngine::get()->clear();
                _audioController->playSFX(LOSS_STAMP, LOSS_STAMP);
                _audioController->playMusic(LOSS_SCREEN, LOSS_SCREEN);
            }
            gameOverLoss = false;
            gameOverWin = false;
        }
    }
    if (loseNode->isVisible() || winNode->isVisible() || repeatWinNode->isVisible())
    {
        return;
    }
    if (_isHost)
    {
        float zoom = _zoom - (ROOT_NODE_SCALE - 0.5f * (float)overWorld.getDog()->getAbsorb() / (float)overWorld.getDog()->getMaxAbsorb());
        _zoom -= fmin(zoom, 0.01f) * (zoom < 0 ? 0.12f : 0.3f);
        _rootnode->setScale(_zoom);
    }
    else
    {
        float zoom = _zoom - (ROOT_NODE_SCALE - 0.5f * (float)overWorld.getClientDog()->getAbsorb() / (float)overWorld.getClientDog()->getMaxAbsorb());
        _zoom -= fmin(zoom, 0.01f) * (zoom < 0 ? 0.12f : 0.3f);
        _rootnode->setScale(_zoom);
    }

    overWorld.update(_input, computeActiveSize(), dt);
    
    if (tutorialIndex < tutorialTiles.size()){
        const std::shared_ptr<Tutorial>& tile = tutorialTiles.at(tutorialIndex);
        bool atLocation = tile->atArea(overWorld.getDog()->getX());
        
        // just do tile->setVisible(tutorial) to draw stuff
        if (atLocation && !tile->didPass())
        {
            if (tile->getProgress() == Tutorial::MODE::RECALLGIVE || tile->getProgress() == Tutorial::MODE::BARKGIVE || tile->getProgress() == Tutorial::MODE::BAITGIVE || tile->getProgress() == Tutorial::MODE::BOMBGIVE || tile->getProgress() == Tutorial::CHANGEABILITYTWO || tile->getProgress() == Tutorial::CHANGEABILITYTHREE || tile->getProgress() == Tutorial::CHANGEABILITYFOUR || tile->getProgress() == Tutorial::SPECIALSONE || tile->getProgress() == Tutorial::SPECIALSTWO || tile->getProgress() == Tutorial::SPECIALSTHREE || tile->getProgress() == Tutorial::SPECIALSFOUR){
                return;
            }
        }
    }
    
    
    _spawnerController.update(_monsterController, overWorld, dt);
    _monsterController.update(dt, overWorld);
    if (!_isHost){
        clientMonsterUpdate(dt);
    }
    if (_isHost && overWorld.getDog()->readyToRecall())
    {
        resetDraw();
    }
    if (!_isHost && overWorld.getClientDog()->readyToRecall()){
        resetDraw();
    }
    if (_isHost)
    {
        _collisionController.intraOverWorldCollisions(overWorld);
        _collisionController.overWorldMonsterControllerCollisions(overWorld, _monsterController);
        _collisionController.attackCollisions(overWorld, _monsterController, _spawnerController);

        if (_monsterController.isEmpty() && _spawnerController.win() && !winNode->isVisible())
        {
            //            winNode->setVisible(true);
            _network->pushOutEvent(WinEvent::allocWinEvent(overWorld.getDog()->getPosition(), _isHost));
        }
        else if ((overWorld.getDog()->getHealth() == 0 || overWorld.getBaseSet()->baseLost()) && !loseNode->isVisible())
        {
            //            loseNode->setVisible(true);
            _network->pushOutEvent(LoseEvent::allocLoseEvent(overWorld.getDog()->getPosition(), _isHost));
        }
    }
    else
    {
        _collisionController.clientIntraWorld(overWorld);
        _collisionController.clienOverMonster(overWorld, _monsterController);
        _collisionController.clientAttackCollide(overWorld, _monsterController, _spawnerController);
        
        if (overWorld.getClientDog()->getHealth() == 0 && !loseNode->isVisible())
        {
            //            loseNode->setVisible(true);
            _network->pushOutEvent(LoseEvent::allocLoseEvent(overWorld.getClientDog()->getPosition(), _isHost));
        }
    }
}

void GameScene::postUpdate(float dt)
{
    // Nothing to do now
    if (loseNode->isVisible() || winNode->isVisible()|| repeatWinNode->isVisible())
    {
        return;
    }
    _monsterController.postUpdate();
    overWorld.postUpdate();

    _rootnode->resetPane();

    Vec2 delta;
    std::shared_ptr<scene2::SceneNode> dogNode;
    if (_isHost)
    {
        dogNode = overWorld.getDog()->getDogNode();
    }
    else
    {
        dogNode = overWorld.getClientDog()->getDogNode();
    }
    //delta = dogNode->getWorldPosition();


    float h = CANVAS_TILE_HEIGHT / 2.0f / _zoom;
    float w = h * computeActiveSize().width / computeActiveSize().height;

    float bottom = dogNode->getPosition().y - h;
    float left = dogNode->getPosition().x - w;
    float top = dogNode->getPosition().y + h;
    float right = dogNode->getPosition().x + w;

    //this is horrible XD
    Vec2 positionCache = dogNode->getPosition();


    if(left < 1) {
        dogNode->setPosition(Vec2(w + 1, dogNode->getPosition().y));
    }
    if(bottom < 1) {
        dogNode->setPosition(Vec2(dogNode->getPosition().x, h + 1));
    }
    if(right > _backgroundWrapper->getCols() - 2) {
        dogNode->setPosition(Vec2(_backgroundWrapper->getCols() - w - 2, dogNode->getPosition().y));
    }
    if(top > _backgroundWrapper->getRows() - 2) {
        dogNode->setPosition(Vec2(dogNode->getPosition().x, _backgroundWrapper->getRows() - 2 - h));
    }

    delta = dogNode->getWorldPosition();
    dogNode->setPosition(positionCache);
    

    delta -= (computeActiveSize() / 2);
    Vec2 curr = - delta / _zoom;
    Vec2 pan;
    if((curr - previousPan).length() < computeActiveSize().height) {
        pan = curr.lerp(previousPan, 0.9f);
    }
    else {
        pan = curr;
    }
    _rootnode->applyPan(pan + shakeMagnitude * Vec2(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX));
    previousPan = pan;

    Vec2 dogPos = _isHost ? overWorld.getDog()->getPosition() : overWorld.getClientDog()->getPosition();
    executeSlidingWindow(dogPos);

    _minimap->update();
    olddogPos = dogPos;

    shakeMagnitude -= SHAKING_DECAY * dt;
    if(shakeMagnitude < 0) {
        shakeMagnitude = 0;
    }
    
}

void GameScene::fixedUpdate()
{
    if (loseNode->isVisible() || winNode->isVisible() || repeatWinNode->isVisible())
    {
        if (loseNode->isVisible() && loseNode->getFrame() != loseNode->getSpan() - 1){
            loseNode->update();
//            loseNode->setFrame(5);
//            CULog("here frame %d", loseNode->getFrame());
        }
        if (winNode->isVisible() && winNode->getFrame() == winNode->getSpan() -1){
            repeatWinNode->setVisible(true);
            winNode->setVisible(false);
        }
        if (winNode->isVisible() && winNode->getFrame() != winNode->getSpan() -1){
            winNode->update();
        }
        if (repeatWinNode->isVisible()){
            repeatWinNode->update();
        }
        return;
    }
    // TODO: check for available incoming events from the network controller and call processCrateEvent if it is a CrateEvent.

    // Hint: You can check if ptr points to an object of class A using std::dynamic_pointer_cast<A>(ptr). You should always check isInAvailable() before popInEvent().

#pragma mark BEGIN SOLUTION
            while (_network->isInAvailable())
            {
                auto e = _network->popInEvent();
                if (auto monsterHealthEvent = std::dynamic_pointer_cast<MonsterHealthEvent>(e))
                {
                    _input.applyRumble(0, 10000, 50);
                    if (!_isHost)
                    { // These are client health Update
                        clientProcessMonsterHealth(monsterHealthEvent);
                    }
                }
                if (auto baseHealthEvent = std::dynamic_pointer_cast<BaseHealthEvent>(e))
                {
                    overWorld.processBaseHealthEvent(baseHealthEvent);
                }
                if (auto decoyEvent = std::dynamic_pointer_cast<DecoyEvent>(e))
                {
                    //            CULog("Decoy Event Got");
                    overWorld.processDecoyEvent(decoyEvent);
                }
                if (auto biteEvent = std::dynamic_pointer_cast<BiteEvent>(e))
                {
                    // shakeMagnitude = std::max(shakeMagnitude, 50.0f);
                    _input.applyRumble(0, 10000, 50);
                    overWorld.processBiteEvent(biteEvent);
                }
                if (auto recallEvent = std::dynamic_pointer_cast<RecallEvent>(e))
                {
                    overWorld.processRecallEvent(recallEvent);
                }
                if (auto explodeEvent = std::dynamic_pointer_cast<ExplodeEvent>(e))
                {
                    shakeMagnitude = std::max(shakeMagnitude, 40.0f);
                    _input.applyRumble(0, 30000, 200);
                    //            CULog("Explode Event Got");
                    overWorld.processExplodeEvent(explodeEvent);
                }
                if (auto shootEvent = std::dynamic_pointer_cast<ShootEvent>(e))
                {
                    if (shootEvent->isHost() == _isHost)
                    {
                        shakeMagnitude = std::max(shakeMagnitude, 40.0f);
                        _input.applyRumble(30000, 0, 200);
                    }
                    //            CULog("Explode Event Got");
                    overWorld.processShootEvent(shootEvent);
                }
                if (auto dashEvent = std::dynamic_pointer_cast<DashEvent>(e))
                {
                    //            CULog("Explode Event Got");
                    overWorld.processDashEvent(dashEvent);
                }
                if (auto sizeEvent = std::dynamic_pointer_cast<SizeEvent>(e))
                {
                    //            CULog("Explode Event Got");
                    overWorld.processSizeEvent(sizeEvent);
                }
                if (auto deathEvent = std::dynamic_pointer_cast<DeathEvent>(e))
                {
                    if (!deathEvent->isGate())
                        _spawnerController.processDeathEvent(deathEvent);
                    if (deathEvent->isBomb())
                    {
                        _collisionController.enemyExplodedCollision(deathEvent->getPos(), 2 * deathEvent->getSize().width, overWorld.getDog(), _monsterController, true);
                        _collisionController.enemyExplodedCollision(deathEvent->getPos(), 2 * deathEvent->getSize().width, overWorld.getClientDog(), _monsterController, false);
                    }
                }
                if (auto winEvent = std::dynamic_pointer_cast<WinEvent>(e))
                {
                    
                    // Unlock new levels by writing to save file
                    shared_ptr<SaveManager> saveFile = make_shared<SaveManager>();
                    std::shared_ptr<JsonValue> json_root = saveFile->read();
                    int unlockedLevels = json_root->getInt("unlocked", 1);
    
                    if(_levelNum == unlockedLevels){
                        json_root->get("unlocked")->set((long) unlockedLevels + 1);
                        saveFile->write(json_root);
                    }
                    
                    gameOverWin = true;
                }
                if (auto loseEvent = std::dynamic_pointer_cast<LoseEvent>(e))
                {
                    gameOverLoss = true;
                }

                if (auto spawnerDeathEvent = std::dynamic_pointer_cast<SpawnerDeathEvent>(e))
                {
                    _spawnerController.processSpawnerDeathEvent(spawnerDeathEvent);
                    _audioController->playSFX("spawnerDeath", SPAWNER_DEATH);
                }
                if (auto clientHealthEvent = std::dynamic_pointer_cast<ClientHealthEvent>(e))
                {
                    //            CULog("Got Health Event");
                    overWorld.processClientHealthEvent(clientHealthEvent);
                }
                if (auto absorbEvent = std::dynamic_pointer_cast<AbsorbEvent>(e))
                {
                    //CULog("Got Absorb Event");
                    processAbsorbEvent(absorbEvent);
                    //overWorld.processClientHealthEvent(clientHealthEvent);
                }
            }
#pragma mark END SOLUTION

    _world->update(FIXED_TIMESTEP_S);
}

/**
 * Executes the core gameplay loop of this world.
 *
 * This method contains the specific update code for this mini-game. It does
 * not handle collisions, as those are managed by the parent class WorldController.
 * This method is called after input is read, but before collisions are resolved.
 * The very last thing that it should do is apply forces to the appropriate objects.
 *
 * @param  delta    Number of seconds since last animation frame
 */
void GameScene::update(float dt)
{
    // deprecated
}

/**
 * Processes the start of a collision
 *
 * Since this game has no real need to determine collisions, right now this is left empty.
 *
 * @param  contact  The two bodies that collided
 */
void GameScene::beginContact(b2Contact *contact)
{
    auto* bodyA = contact->GetFixtureA()->GetBody();
    auto* bodyB = contact->GetFixtureB()->GetBody();

    // Check if either body is the correct type and then cast
    auto* enemyA = dynamic_cast<AbstractEnemy*>(reinterpret_cast<AbstractEnemy*>(bodyA->GetUserData().pointer));
    auto* enemyB = dynamic_cast<AbstractEnemy*>(reinterpret_cast<AbstractEnemy*>(bodyB->GetUserData().pointer));

    if (enemyA) {
        enemyA->setInContact(true);
    }
    if (enemyB) {
        enemyB->setInContact(true);
    }
}

void GameScene::endContact(b2Contact *contact)
{
    auto* bodyA = contact->GetFixtureA()->GetBody();
    auto* bodyB = contact->GetFixtureB()->GetBody();

    // Check if either body is the correct type and then cast
    auto* enemyA = dynamic_cast<AbstractEnemy*>(reinterpret_cast<AbstractEnemy*>(bodyA->GetUserData().pointer));
    auto* enemyB = dynamic_cast<AbstractEnemy*>(reinterpret_cast<AbstractEnemy*>(bodyB->GetUserData().pointer));

    // Set inContact to false if the object is an AbstractEnemy
    if (enemyA) {
        enemyA->setInContact(false);
    }
    if (enemyB) {
        enemyB->setInContact(false);
    }
}

/**
 * Handles any modifications necessary before collision resolution
 *
 * This method is called just before Box2D resolves a collision.  We use this method
 * to implement sound on contact, using the algorithms outlined in Ian Parberry's
 * "Introduction to Game Physics with Box2D".
 *
 * @param  contact      The two bodies that collided
 * @param  oldManfold      The collision manifold before contact
 */
void GameScene::beforeSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
}

/**
 * Returns the active screen size of this scene.
 *
 * This method is for graceful handling of different aspect
 * ratios
 */
Size GameScene::computeActiveSize() const
{
    Size dimen = Application::get()->getDisplaySize();
    float ratio1 = dimen.width / dimen.height;
    float ratio2 = ((float)SCENE_WIDTH) / ((float)SCENE_HEIGHT);
    if (ratio1 < ratio2)
    {
        dimen *= SCENE_WIDTH / dimen.width;
    }
    else
    {
        dimen *= SCENE_HEIGHT / dimen.height;
    }
    return dimen;
}
void GameScene::addChildBackground()
{
    const std::vector<std::vector<std::shared_ptr<TileInfo>>> &currentBackground = _backgroundWrapper->getTileWorld();
    int originalRows = (int)currentBackground.size();
    int originalCols = (int)currentBackground.at(0).size();
    for (int j = 0; j < originalCols; j++)
    {
        for (int i = originalRows - 1; i > -1; i--)
        {
            const std::shared_ptr<TileInfo>& t = currentBackground.at(i).at(j);
            if (t->texture != nullptr)
            {
                _worldnode->addChild(t->getTileSprite());
            }
        }
    }
    
    const std::vector<std::vector<std::shared_ptr<TileInfo>>> &currentBoundaries = _backgroundWrapper->getBoundaryWorld();
    for (int i = 0; i < originalRows; i++)
    {
        for (int j = 0; j < originalCols; j++)
        {
            const std::shared_ptr<TileInfo>& t = currentBoundaries.at(i).at(j);
            if (t->texture != nullptr)
            {
                
                std::shared_ptr<cugl::physics2::BoxObstacle> boundary = cugl::physics2::BoxObstacle::alloc(t->getPos(),cugl::Size(0.9,0.9));
                boundary->clearSharingDirtyBits();
                boundary->setShared(false);
                boundary->setBodyType(b2_staticBody);
                boundary->setDensity(10.0f);
                boundary->setFriction(0.4f);
                boundary->setRestitution(0.1);
                boundary->setDebugColor(DYNAMIC_COLOR); // Don't add these back
                boundary->setDebugScene(_debugnode);
                _world->initObstacle(boundary);
                if(_isHost){
                    _world->getOwnedObstacles().insert({boundary,0});
                }
            }
        }
    }
    const std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>> &lowerDecorWorld = _backgroundWrapper->getLowerDecorWorld();
    for (int n = 0; n < lowerDecorWorld.size(); n++)
    {
        for (int i = 0; i < originalRows; i++)
        {
            for (int j = 0; j < originalCols; j++)
            {
                const std::shared_ptr<TileInfo>& t = lowerDecorWorld.at(n).at(i).at(j);
                if (t->texture != nullptr)
                {
                    _worldnode->addChild(t->getTileSprite());
                }
            }
        }
    }
}
void GameScene::addChildForeground()
{

    const std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>> &upperDecorWorld = _backgroundWrapper->getUpperDecorWorld();
    if (upperDecorWorld.size() == 0)
    {
        return;
    }
    int originalRows = (int)upperDecorWorld.at(0).size();
    int originalCols = (int)upperDecorWorld.at(0).at(0).size();
    for (int n = 0; n < upperDecorWorld.size(); n++)
    {
        for (int i = 0; i < originalRows; i++)
        {
            for (int j = 0; j < originalCols; j++)
            {
                const std::shared_ptr<TileInfo>& t = upperDecorWorld.at(n).at(i).at(j);
                if (t->texture != nullptr)
                {
                    _worldnode->addChild(t->getTileSprite());
                }
            }
        }
    }
}

void GameScene::updateInputController()
{
    
//    std::cout <<overWorld.getDog()->getX() << std::endl;
    if (tutorialIndex < tutorialTiles.size())
    {
        tutorialArrow->setVisible(true);
        tutorialArrow->update();
//        CULog("Tutorial Index %d Pos: %f", tutorialIndex, overWorld.getDog()->getX());
        
        const std::shared_ptr<Tutorial>& tile = tutorialTiles.at(tutorialIndex);
        bool atLocation = tile->atArea(overWorld.getDog()->getX());
        const std::shared_ptr<scene2::SceneNode>& node = tutorialTiles.at(tutorialIndex)->getSprite();
        const std::shared_ptr<SpriteAnimationNode>& spriteNode = std::dynamic_pointer_cast<SpriteAnimationNode>(node);
        const std::shared_ptr<SpriteAnimationNode>& spriteNodeRepeat = tutorialTiles.at(tutorialIndex)->getSpriteRepeat();
        const std::shared_ptr<SpriteAnimationNode>& pressA = tutorialTiles.at(tutorialIndex)->getPressA();
        const std::shared_ptr<scene2::Label>& message = tutorialTiles.at(tutorialIndex)->getMessage();
        // just do tile->setVisible(tutorial) to draw stuff
        if (atLocation && !tile->didPass() && spriteNode)
        {
            if (spriteNode->getFrame() == 0 && newTrick->isVisible() ==false ){
                _audioController->playSFX(TUTORIAL, TUTORIAL);
            }
            if (tile->getProgress() == Tutorial::MODE::RECALLGIVE || tile->getProgress() == Tutorial::MODE::BARKGIVE || tile->getProgress() == Tutorial::MODE::BAITGIVE || tile->getProgress() == Tutorial::MODE::BOMBGIVE){
                newTrick->setVisible(true);
                _audioController->stopSFX(TUTORIAL);
                
                
                if(newTrick->getFrame() != newTrick->getSpan()-1){
                    if (newTrick->getFrame() == 0){
                        _audioController->playSFX(NEW_TRICK, NEW_TRICK);
                    }
                    newTrick->update();
                    return;
                }
                else{
                    newTrick->setVisible(false);
                }
                
                if (spriteNode->getFrame() != spriteNode->getSpan() -1){
                    if (spriteNode->getFrame() == 0){
//                        _audioController->stopSFX(TUTORIAL);
                        _audioController->playSFX(NEW_TRICK, NEW_TRICK);
                    }
                    spriteNode->setVisible(true);
                    spriteNode->update();
                } else{
                    spriteNode->setVisible(false);
                    spriteNodeRepeat->setVisible(true);
                    spriteNodeRepeat->update();
                    pressA->setVisible(true);
                    pressA->update();
                }
            }else{
                textBox->setVisible(true);
                spriteNode->setVisible(true);
                spriteNode->update();
                message->setVisible(true);
                if(tile->getProgress() == Tutorial::MODE::MOVEMENT){
                    pressA->setVisible(false);
                }
                else{
                    pressA->setVisible(true);
                    pressA->update();
                }
            }
        }
        if (tutorialIndex == tutorialTiles.size() - 1){
            if (tutorialArrow != nullptr){
                tutorialArrow->setVisible(false);
            }
        }
        
        if (tile->getProgress() == Tutorial::MODE::RECALLGIVE || tile->getProgress() == Tutorial::MODE::BARKGIVE || tile->getProgress() == Tutorial::MODE::BAITGIVE || tile->getProgress() == Tutorial::MODE::BOMBGIVE){
            if (_input.update(tile->getProgress(), atLocation)){
                tile->setPass(true);
                spriteNode->setVisible(false);
                spriteNodeRepeat->setVisible(false);
                pressA->setVisible(false);
                tutorialIndex++;
                newTrick->setFrame(0);
                _audioController->stopSFX(TUTORIAL);
            }
        } else if (_input.update(tile->getProgress(), atLocation))
        {
            textBox->setVisible(false);
            tile->setPass(true);
            node->setVisible(false);
            pressA->setVisible(false);
            message->setVisible(false);
            tutorialIndex++;
            _audioController->stopSFX(TUTORIAL);
        }
        
    }
    else{
        _input.update();
    }
    
    
    
    if (_input.didPressExit())
    {
        Application::get()->quit();
    }
    if (_input.didPressPause())
    {
        _pause->togglePause();
        _audioController->playSFX(PAUSE_SCREEN, PAUSE_SCREEN);
    }

    if (_input.didPressHome())
    {
        _pause->exitToMain();
    }
    if (_input.didChangeMode())
    {
        _audioController->playSFX(BUTTON_SWAP, BUTTON_SWAP);
    }
}

void GameScene::initTutorialOne(){
    tutorialIndex = 0;
    _tutorialnode = scene2::SceneNode::alloc();
    _uinode->addChild(_tutorialnode);
    tutorialTiles = std::vector<std::shared_ptr<Tutorial>>();
    
    tutorialTiles.push_back(Tutorial::alloc(0, Tutorial::MODE::GREETING, "Ready to go on our first day on the job, boy? We'd better go over your abilities again, just in case."));
    tutorialTiles.push_back(Tutorial::alloc(0, Tutorial::MODE::MOVEMENT, "MOVE with (joystick)."));
    tutorialTiles.push_back(Tutorial::alloc(15, Tutorial::MODE::DASH, "Dash with (x)"));
    tutorialTiles.push_back(Tutorial::alloc(25, Tutorial::MODE::BITE, "Look, boy, our first threat! Let's get 'em! BITE with (A)."));
    tutorialTiles.push_back(Tutorial::alloc(35, Tutorial::MODE::GROW, "Whenever you eat an enemy, you ABSORB its power. You can store power up to a certain point. Remember, as you eat, you'll gain size, and become stronger. But don't overeat!"));
    tutorialTiles.push_back(Tutorial::alloc(20, Tutorial::MODE::RECALLGIVE, ""));
    tutorialTiles.push_back(Tutorial::alloc(22, Tutorial::MODE::SPECIALSONE, "Let's try using a power. We passed the job interview so quick, I couldn't teach you everything in our arsenal yet. "));
    tutorialTiles.push_back(Tutorial::alloc(10, Tutorial::MODE::DEFENDGATE, "Here we are, at the most important thing of our lives!"));
    tutorialTiles.push_back(Tutorial::alloc(10, Tutorial::MODE::DEFENDGATE, "These gates are what lead to Heaven. We gotta protect these, boy- our paycheck depends on it!"));
    tutorialTiles.push_back(Tutorial::alloc(10, Tutorial::MODE::DEFENDGATE, "But protecting these gates are only one part of the job- the other essential part is to ensure these guys can't ever come back. That is, to finish a shift successfully, we gotta destroy the sources they spawn from."));
    tutorialTiles.push_back(Tutorial::alloc(24, Tutorial::MODE::CHANGEABILITYTWO, "SWITCH ABILITIES with (bumper). ACTIVATE YOUR ABILITY with (trigger). Let's try switching abilities now, boy."));
    tutorialTiles.push_back(Tutorial::alloc(20, Tutorial::MODE::BARKGIVE, ""));
    tutorialTiles.push_back(Tutorial::alloc(22, Tutorial::MODE::SPECIALSTWO, "Good job, boy! Next, let’s try using that power to take down some bad guys. Your BARK is stronger than your BITE. Remember, switch abilities first, and then unleash your power!"));
    tutorialTiles.push_back(Tutorial::alloc(50, Tutorial::MODE::DESTROYSPAWNER, "I think that's all we can discuss right now, 'cuz I see a bunch of angry things coming at us now! Get 'em, boy!"));

    std::vector<std::string> modes = {"SHOOT", "RECALL"};
    overWorld.getDog()->setAbility(modes);
    overWorld.getDog()->toggleMode();
    overWorld.getClientDog()->setAbility(modes);
    overWorld.getClientDog()->toggleMode();
    // each one need to write # of frames

    overWorld.getDog()->addAbsorb(6);
    initTutorial();
    
}

void GameScene::initTutorialTwo(){
    tutorialIndex = 0;
    _tutorialnode = scene2::SceneNode::alloc();
    _uinode->addChild(_tutorialnode);
    tutorialTiles = std::vector<std::shared_ptr<Tutorial>>();
    tutorialTiles.push_back(Tutorial::alloc(0, Tutorial::MODE::CHANGEABILITYTWO, "Day two, boy! Good work yesterday. Since we got a bit of time again, let's go over how to set up a BAIT gate."));
    tutorialTiles.push_back(Tutorial::alloc(0, Tutorial::MODE::BAITGIVE, ""));
    tutorialTiles.push_back(Tutorial::alloc(22, Tutorial::MODE::SPECIALSTHREE, "You can use energy to set up fake gates that will fool the baddies. They disappear after a while, so don’t try to rely on them forever. Let’s try it now!"));
    std::vector<int> frame = {21};
    std::vector<std::string> modes = {"SHOOT", "RECALL", "BAIT"};
    overWorld.getDog()->setAbility(modes);
    overWorld.getDog()->toggleMode();
    overWorld.getClientDog()->setAbility(modes);
    overWorld.getClientDog()->toggleMode();
    overWorld.getDog()->addAbsorb(6);
    initTutorial();
}

void GameScene::initTutorialThree(){
    tutorialIndex = 0;
    _tutorialnode = scene2::SceneNode::alloc();
    _uinode->addChild(_tutorialnode);
    tutorialTiles = std::vector<std::shared_ptr<Tutorial>>();
    tutorialTiles.push_back(Tutorial::alloc(0, Tutorial::MODE::CHANGEABILITYFOUR, "Good job! The final ability is BOMB. You can use your stored power to BOMB an area. This is great for a concentrated burst of power around you if you find yourself surrounded. Let’s try it now!"));
    tutorialTiles.push_back(Tutorial::alloc(0, Tutorial::MODE::BOMBGIVE, ""));
    tutorialTiles.push_back(Tutorial::alloc(22, Tutorial::MODE::SPECIALSFOUR, "Attaboy! That’s all we got- but it’s more than enough to take down these lil devils. Let’s get them!"));
    std::vector<int> frame = {21};
    std::vector<std::string> modes = {"BOMB", "BAIT", "RECALL", "SHOOT"};
    overWorld.getDog()->setAbility(modes);
    overWorld.getDog()->toggleMode();
    overWorld.getClientDog()->setAbility(modes);
    overWorld.getClientDog()->toggleMode();
    overWorld.getDog()->addAbsorb(10);
    initTutorial();
}

void GameScene::initTutorial()
{
    Size screen = computeActiveSize();
    std::shared_ptr<scene2::Label> message;
    std::string str;
    
    tutorialArrow = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("arrow"), 2, 5, 8, 8);
    tutorialArrow->setAnchor(Vec2::ANCHOR_CENTER);
    tutorialArrow->setScale(4);
    tutorialArrow->setPositionX(screen.width / 2 + 320);
    tutorialArrow->setPositionY(screen.height / 2);
    _tutorialnode->addChild(tutorialArrow);
    
    newTrick = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("newTrick"), 7, 5, 35, 2);
    newTrick->setAnchor(Vec2::ANCHOR_CENTER);
    
    newTrick->setScale(8*SCENE_HEIGHT/newTrick->getTexture()->getHeight());
    newTrick->setPosition(0.5 * newTrick->getSize());
    newTrick->setVisible(false);
    _tutorialnode->addChild(newTrick);
    
    
    textBox = cugl::scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("textBubble"));
    textBox->setScale(SCENE_HEIGHT/textBox->getTexture()->getHeight());
    textBox->setPosition(0.5 * textBox->getSize());
    textBox->setVisible(false);
    _tutorialnode->addChild(textBox);
    
    
    devilNormal = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("girlTalk"), 4, 5, 20, 5);
    devilNormal->setScale(4 * SCENE_HEIGHT/devilNormal->getTexture()->getHeight());
    devilNormal->setPosition(0.5 * devilNormal->getSize());
    devilNormal->setVisible(false);
    _tutorialnode->addChild(devilNormal);
    
    devilLaugh = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("girlTalk"), 4, 5, 20, 5);
    devilLaugh->setScale(4 * SCENE_HEIGHT/devilNormal->getTexture()->getHeight());
    devilLaugh->setPosition(0.5 * devilNormal->getSize());
    devilLaugh->setVisible(false);
    _tutorialnode->addChild(devilLaugh);
    
    for (int i = 0; i < tutorialTiles.size(); i++)
    {
        
        Tutorial::MODE mode = tutorialTiles.at(i)->getProgress();
        if (mode == Tutorial::MODE::RECALLGIVE || mode == Tutorial::MODE::BARKGIVE || mode == Tutorial::MODE::BAITGIVE ||  mode == Tutorial::MODE::BOMBGIVE){
            
            std::shared_ptr<SpriteAnimationNode> init;
            std::shared_ptr<SpriteAnimationNode> repeat;
            std::shared_ptr<SpriteAnimationNode> pressA = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("pressA"), 1, 2, 2, 30);
            
            if (mode == Tutorial::MODE::RECALLGIVE){
                init = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("recallInit"), 3, 5,  14,  2);
                repeat = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("recallRepeat"), 4, 5, 16,  5);
                init->setScale(3* SCENE_HEIGHT/init->getTexture()->getHeight());
                init->setPosition(0.5 * init->getSize());
                repeat->setScale(4*SCENE_HEIGHT/repeat->getTexture()->getHeight());
                repeat->setPosition(0.5 * repeat->getSize());
            }else if (mode == Tutorial::MODE::BARKGIVE){
                init = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("barkInit"), 3, 5,  14,  2);
                repeat = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("barkRepeat"), 1, 2, 2,  30);
                init->setScale(3* SCENE_HEIGHT/init->getTexture()->getHeight());
                init->setPosition(0.5 * init->getSize());
                repeat->setScale(SCENE_HEIGHT/repeat->getTexture()->getHeight());
                repeat->setPosition(0.5 * repeat->getSize());
            }else if (mode == Tutorial::MODE::BAITGIVE){
                init = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("baitInit"), 3, 5,  14,  2);
                repeat = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("baitRepeat"), 2, 5, 9,  6);
                init->setScale(3* SCENE_HEIGHT/init->getTexture()->getHeight());
                init->setPosition(0.5 * init->getSize());
                repeat->setScale(2*SCENE_HEIGHT/repeat->getTexture()->getHeight());
                repeat->setPosition(0.5 * repeat->getSize());
            }else if (mode == Tutorial::MODE::BOMBGIVE){
                init = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("bombInit"), 3, 5,  14,  2);
                repeat = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("bombRepeat"), 4, 5, 18,  3);
                init->setScale(3* SCENE_HEIGHT/init->getTexture()->getHeight());
                init->setPosition(0.5 * init->getSize());
                repeat->setScale(4*SCENE_HEIGHT/repeat->getTexture()->getHeight());
                repeat->setPosition(0.5 * repeat->getSize());
            }
            
            
            tutorialTiles.at(i)->setSprite(init);
            tutorialTiles.at(i)->setSpriteRepeat(repeat);
            tutorialTiles.at(i)->setPressButton(pressA);
            
            pressA->setAnchor(Vec2::ANCHOR_CENTER);
            pressA->setScale(6);
            pressA->setPosition(screen/2 - Size(0, screen.height/4));
            
            _tutorialnode->addChild(init);
            _tutorialnode->addChild(repeat);
            _tutorialnode->addChild(pressA);
            
            init->setVisible(false);
            repeat->setVisible(false);
            pressA->setVisible(false);
            
        }else {
            tutorialTiles.at(i)->setSprite(devilNormal);
            Size box = Size(1.5f * screen.width, 3*screen.height/4);
            
            
            message = scene2::Label::allocWithTextBox(box, tutorialTiles.at(i)->getText() ,_assets->get<Font>(PRIMARY_FONT));
            message->setHorizontalAlignment(HorizontalAlign::CENTER);
            message->setVerticalAlignment(VerticalAlign::MIDDLE);
            message->setWrap(true);
            message->setScale(.25);
            _tutorialnode->addChild(message);
            message->setAnchor(Vec2(0.8f , 0.5f));
            message->setPositionX(3 * screen.width / 4);
            message->setPositionY(0.18f * screen.height);
            message->setVisible(false);
            tutorialTiles.at(i)->setMessage(message);
            
            std::shared_ptr<SpriteAnimationNode> pressA;
            if (mode == Tutorial::MODE::DASH){
                // should make this press X
                pressA = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("pressB"), 1, 2, 2, 30);
            }
            else if (mode == Tutorial::SPECIALSONE || mode == Tutorial::SPECIALSTWO || mode == Tutorial::SPECIALSTHREE || mode == Tutorial::SPECIALSFOUR){
                pressA = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("rightTrigger"), 1, 2, 2, 30);
            }
            else if(mode == Tutorial::MODE::CHANGEABILITYTWO || mode == Tutorial::MODE::CHANGEABILITYTHREE || mode == Tutorial::MODE::CHANGEABILITYFOUR){
                pressA = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("rightBumper"), 1, 2, 2, 30);
            }
            else{
                pressA = SpriteAnimationNode::allocWithSheet(_assets->get<Texture>("pressA"), 1, 2, 2, 30);
            }
            tutorialTiles.at(i)->setPressButton(pressA);
            pressA->setAnchor(Vec2::ANCHOR_CENTER);
            pressA->setScale(6);
//            pressA->setPosition(screen/2);
            pressA->setPositionX(screen.width * 0.90);
            pressA->setPositionY(devilNormal->getTexture()->getHeight() / 8 - 40);
            _tutorialnode->addChild(pressA);
            pressA->setVisible(false);
        }
    }
}


void GameScene::executeSlidingWindow(Vec2 dogPos)
{

    const int DRAW_SIZE = 15;
    // New Values
    int minY = fmax(dogPos.y - DRAW_SIZE, 0);
    int maxY = fmin(dogPos.y + DRAW_SIZE, _backgroundWrapper->getRows() - 1);
    int minX = fmax(dogPos.x - DRAW_SIZE, 0);
    int maxX = fmin(dogPos.x + DRAW_SIZE, _backgroundWrapper->getCols() - 1);

    // Old Values
    int oldminY = fmax(olddogPos.y - DRAW_SIZE, 0);
    int oldmaxY = fmin(olddogPos.y + DRAW_SIZE, _backgroundWrapper->getRows() - 1);
    int oldminX = fmax(olddogPos.x - DRAW_SIZE, 0);
    int oldmaxX = fmin(olddogPos.x + DRAW_SIZE, _backgroundWrapper->getCols() - 1);

    if (maxY > oldmaxY)
    { // move tiles Up
        const std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>> &tileDisplay = _backgroundWrapper->getTileDisplay();
        int rowToAdd = maxY;
        int rowToRemove = minY - 1;
        for (int j = 0; j < tileDisplay.at(0).size(); j++)
        {
            for (const std::shared_ptr<TileInfo> &tile : tileDisplay.at(rowToAdd).at(j))
            {
                tile->getTileSprite()->setVisible(true);
            }
            if (rowToRemove >= 0)
            {
                for (const std::shared_ptr<TileInfo> &tile : tileDisplay.at(rowToRemove).at(j))
                {
                    tile->getTileSprite()->setVisible(false);
                }
            }
        }
    }
    else if (minY < oldminY)
    { // move tiles Down
        const std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>> &tileDisplay = _backgroundWrapper->getTileDisplay();
        int rowToAdd = minY;
        int rowToRemove = maxY + 1;
        for (int j = 0; j < tileDisplay.at(0).size(); j++)
        {
            for (const std::shared_ptr<TileInfo> &tile : tileDisplay.at(rowToAdd).at(j))
            {
                tile->getTileSprite()->setVisible(true);
            }
            if (rowToRemove < tileDisplay.size())
            {
                for (const std::shared_ptr<TileInfo> &tile : tileDisplay.at(rowToRemove).at(j))
                {
                    tile->getTileSprite()->setVisible(false);
                }
            }
        }
    }
    if (maxX > oldmaxX)
    { // move tiles right
        const std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>> &tileDisplay = _backgroundWrapper->getTileDisplay();
        int colToAdd = maxX;
        int colToRemove = minX - 1;
        for (int i = 0; i < tileDisplay.size(); i++)
        {
            for (std::shared_ptr<TileInfo> tile : tileDisplay.at(i).at(colToAdd))
            {
                tile->getTileSprite()->setVisible(true);
            }
            if (colToRemove >= 0)
            {
                for (std::shared_ptr<TileInfo> tile : tileDisplay.at(i).at(colToRemove))
                {
                    tile->getTileSprite()->setVisible(false);
                }
            }
        }
    }
    else if (minX < oldminX)
    { // move tiles left
        const std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>> &tileDisplay = _backgroundWrapper->getTileDisplay();
        int colToAdd = minX;
        int colToRemove = maxX + 1;
        for (int i = 0; i < tileDisplay.size(); i++)
        {
            for (const std::shared_ptr<TileInfo> &tile : tileDisplay.at(i).at(colToAdd))
            {
                tile->getTileSprite()->setVisible(true);
            }
            if (colToRemove < tileDisplay.at(i).size())
            {
                for (const std::shared_ptr<TileInfo> &tile : tileDisplay.at(i).at(colToRemove))
                {
                    tile->getTileSprite()->setVisible(false);
                }
            }
        }
    }
}

void GameScene::processAbsorbEvent(std::shared_ptr<AbsorbEvent> absorbEvent) {
    std::shared_ptr<cugl::physics2::Obstacle> it = _world->getObstacle(absorbEvent->getObstacleID());
    if (it == nullptr){
//        CULog("lagged event?");
        return;
    }
    if (std::shared_ptr<AbstractEnemy> absorbEnemy = std::dynamic_pointer_cast<AbstractEnemy>(it)){
//        CULog("Actual Enemy");
        absorbEnemy->setDimension(Size(absorbEvent->getSize(), absorbEvent->getSize()));
    }
}

void GameScene::clientProcessMonsterHealth(std::shared_ptr<MonsterHealthEvent> monsterHealthEvent){
    
    std::shared_ptr<cugl::physics2::Obstacle> it = _world->getObstacle(monsterHealthEvent->getObstacleID());
    if (it == nullptr){
//        CULog("lagged event?");
        return;
    }
    if (std::shared_ptr<AbstractEnemy> enemy = std::dynamic_pointer_cast<AbstractEnemy>(it)){
//        CULog("Actual Enemy");
        enemy->setHealth(enemy->getHealth() - monsterHealthEvent->getHealthDiff());
    }
    
}


void GameScene::clientMonsterUpdate(float dt){
    for (std::shared_ptr<cugl::physics2::Obstacle> obs : _world->getObstacles()){
        if (std::shared_ptr<AbstractEnemy> enemy  = std::dynamic_pointer_cast<AbstractEnemy>(obs)){
            enemy->clientAnimationUpdate(overWorld, dt);
        }
    }
}

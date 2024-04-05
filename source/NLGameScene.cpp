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

using namespace cugl;
using namespace cugl::physics2::net;

#pragma mark -
#pragma mark Level Geography

/** This is the size of the active portion of the screen */
#define SCENE_WIDTH 1200
#define SCENE_HEIGHT 800

#define CANVAS_TILE_HEIGHT 8

/** Width of the game world in Box2d units */
#define DEFAULT_WIDTH   32.0f
/** Height of the game world in Box2d units */
#define DEFAULT_HEIGHT  18.0f
/** The default value of gravity (going down) */
#define DEFAULT_GRAVITY -0.0f

#define DEFAULT_TURN_RATE 0.05f

/** To automate the loading of crate files */
#define NUM_CRATES 100


// Since these appear only once, we do not care about the magic numbers.
// In an actual game, this information would go in a data file.
// IMPORTANT: Note that Box2D units do not equal drawing units
/** The wall vertices */
/** The positions of the crate pyramid */
float BOXES[] = { 14.5f, 14.25f,
                  13.0f, 12.00f, 16.0f, 12.00f,
                  11.5f,  9.75f, 14.5f,  9.75f, 17.5f, 9.75f,
                  13.0f,  7.50f, 16.0f,  7.50f,
                  11.5f,  5.25f, 14.5f,  5.25f, 17.5f, 5.25f,
                  10.0f,  3.00f, 13.0f,  3.00f, 16.0f, 3.00f, 19.0f, 3.0f};

/** The initial cannon position */
float CAN1_POS[] = { 2, 9 };
float CAN2_POS[] = { 30,9 };

#pragma mark Assset Constants
/** The key for the earth texture in the asset manager */
#define EARTH_TEXTURE       "earth"
/** The key for the cannon texture in the asset manager */
#define CANNON_TEXTURE        "rocket"
/** The key prefix for the multiple crate assets */
#define CRATE_PREFIX        "crate"
/** The key for the fire textures in the asset manager */
#define MAIN_FIRE_TEXTURE   "flames"
#define RGHT_FIRE_TEXTURE   "flames-right"
#define LEFT_FIRE_TEXTURE   "flames-left"

/** Color to outline the physics nodes */
#define STATIC_COLOR    Color4::WHITE
/** Opacity of the physics outlines */
#define DYNAMIC_COLOR   Color4::YELLOW

/** The key for collisions sounds */
#define COLLISION_SOUND     "bump"
/** The key for the main afterburner sound */
#define MAIN_FIRE_SOUND     "burn"
/** The key for the right afterburner sound */
#define RGHT_FIRE_SOUND     "right-burn"
/** The key for the left afterburner sound */
#define LEFT_FIRE_SOUND     "left-burn"

/** The key for the font reference */
#define PRIMARY_FONT        "retro"

#pragma mark Physics Constants

// Physics constants for initialization
/** Density of non-crate objects */
#define BASIC_DENSITY       0.0f
/** Density of the crate objects */
#define CRATE_DENSITY       1.0f
/** Friction of non-crate objects */
#define BASIC_FRICTION      0.1f
/** Friction of the crate objects */
#define CRATE_FRICTION      0.2f
/** Angular damping of the crate objects */
#define CRATE_DAMPING       1.0f
/** Collision restitution for all objects */
#define BASIC_RESTITUTION   0.1f
/** Threshold for generating sound on collision */
#define SOUND_THRESHOLD     3

#define FIXED_TIMESTEP_S 0.02f

/**
 * Generate a pair of Obstacle and SceneNode using the given parameters
 */
std::pair<std::shared_ptr<physics2::Obstacle>, std::shared_ptr<scene2::SceneNode>> CrateFactory::createObstacle(Vec2 pos, float scale) {
    //Choose randomly between wooden crates and iron crates.
    int indx = 1;
    std::string name = (CRATE_PREFIX "0") + std::to_string(indx);
    auto image = _assets->get<Texture>(name);
    Size boxSize(image->getSize() / scale / 2.f);
    
    // TODO: allocate a box obstacle at pos with boxSize, set its angleSnap to 0, debugColor to DYNAMIC_COLOR, density to CRATE_DENSITY, friction to CRATE_FRICTION, and restitution to BASIC_RESTITUTION, after everything is set, make the object shared by calling setShared(). Then allocate a PolygonNode from image, set its anchor to center, and scale to 0.5f. Lastly return the pair of Obstacle and sceneNode.
    
    // NOTE: When an Obstacle is shared, function calls that change its state are monitored and automatically synchronized. However, every client calling this method is going to run the code above setting the properties. We don't want to share them redundantly, so sharing is turned on afterwards.
    
#pragma mark BEGIN SOLUTION
    auto crate = physics2::BoxObstacle::alloc(pos, boxSize);
    
    crate->setDebugColor(DYNAMIC_COLOR);
    crate->setAngleSnap(0); // Snap to the nearest degree
    
    // Set the physics attributes
    crate->setDensity(CRATE_DENSITY);
    crate->setFriction(CRATE_FRICTION);
    crate->setAngularDamping(CRATE_DAMPING);
    crate->setRestitution(BASIC_RESTITUTION);

    crate->setShared(true);
    
    auto sprite = scene2::PolygonNode::allocWithTexture(image);
    sprite->setAnchor(Vec2::ANCHOR_CENTER);
    sprite->setScale(0.5f);
    return std::make_pair(crate, sprite);
#pragma mark END SOLUTION
}

/**
 * Helper method for converting normal parameters into byte vectors used for syncing.
 */
std::shared_ptr<std::vector<std::byte>> CrateFactory::serializeParams(Vec2 pos, float scale) {
    // TODO: Use _serializer to serialize pos and scale (remember to make a shared copy of the serializer reference, otherwise it will be lost if the serializer is reset).
#pragma mark BEGIN SOLUTION
    _serializer.reset();
    _serializer.writeFloat(pos.x);
    _serializer.writeFloat(pos.y);
    _serializer.writeFloat(scale);
    return std::make_shared<std::vector<std::byte>>(_serializer.serialize());
#pragma mark END SOLUTION
}

/**
 * Generate a pair of Obstacle and SceneNode using serialized parameters.
 */
std::pair<std::shared_ptr<physics2::Obstacle>, std::shared_ptr<scene2::SceneNode>> CrateFactory::createObstacle(const std::vector<std::byte>& params) {
    // TODO: Use _deserializer to deserialize byte vectors packed by {@link serializeParams()} and call the regular createObstacle() method with them.
#pragma mark BEGIN SOLUTION
    _deserializer.reset();
    _deserializer.receive(params);
    float x = _deserializer.readFloat();
    float y = _deserializer.readFloat();
    Vec2 pos = Vec2(x,y);
    float scale = _deserializer.readFloat();
    return createObstacle(pos, scale);
#pragma mark END SOLUTION
}

/**
 * Resets the status of the game so that we can play again.
 *
 * This method disposes of the world and creates a new one.
 */
void GameScene::reset() {
    _todoReset = false;
    removeAllChildren();
    _worldnode->removeAllChildren();
    _debugnode->removeAllChildren();
    
    _backgroundWrapper = std::make_shared<World>(Vec2(0, 0),_scale, _level->getTiles(), _level->getBoundaries(), _assets->get<cugl::Texture>("tile"));
    
    populate();
    overWorld.reset();
    overWorld.setRootNode(_worldnode, _debugnode, _world);
    
    _camera.init(overWorld.getDog()->getDogNode(), _worldnode, std::dynamic_pointer_cast<OrthographicCamera>(getCamera()), overWorld.getDog()->getUINode(), 1000.0f);

    addChild(_worldnode);
    addChild(_debugnode);
    addChild(overWorld.getDog()->getUINode());
    
    std::function<void(const std::shared_ptr<physics2::Obstacle>&,const std::shared_ptr<scene2::SceneNode>&)> linkSceneToObsFunc = [=](const std::shared_ptr<physics2::Obstacle>& obs, const std::shared_ptr<scene2::SceneNode>& node) {
        this->linkSceneToObs(obs,node);
    };
    _network->enablePhysics(_world, linkSceneToObsFunc);
    
    if(!_isHost){
        _network->getPhysController()->acquireObs(_cannon2, 0);
    }

    _factId = _network->getPhysController()->attachFactory(_crateFact);
    Application::get()->resetFixedRemainder();
}

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
_todoReset(false)
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
bool GameScene::init(const std::shared_ptr<AssetManager>& assets, const std::shared_ptr<cugl::physics2::net::NetEventController> network, bool isHost) {
    return init(assets, Rect(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT), Vec2(0, DEFAULT_GRAVITY), network, isHost);
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
bool GameScene::init(const std::shared_ptr<AssetManager>& assets, const Rect rect, const std::shared_ptr<NetEventController> network, bool isHost) {
    return init(assets,rect,Vec2(0,DEFAULT_GRAVITY),network,isHost);
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
bool GameScene::init(const std::shared_ptr<AssetManager>& assets, const Rect rect, const Vec2 gravity, const std::shared_ptr<NetEventController> network, bool isHost) {
    Size dimen = computeActiveSize();

    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }
    
    _isHost = isHost;

    _network = network;
    _todoReset = false;
    // Start up the input handler
    _level = assets->get<LevelModel>(LEVEL_ONE_KEY);
    if (_level == nullptr) {
        // Might need to change later if too many assets Copy Tiled Demo?
        CULog("Fail!");
        return false;
    }
    _assets = assets;
    _input.init();
    _input.update();
    
    _crateFact = CrateFactory::alloc(_assets);

    // IMPORTANT: SCALING MUST BE UNIFORM
    // This means that we cannot change the aspect ratio of the physics world
    // Shift to center if a bad fit
    _scale = dimen.width == SCENE_WIDTH ? dimen.width/rect.size.width : dimen.height/rect.size.height;
    Vec2 offset((dimen.width-SCENE_WIDTH)/2.0f,(dimen.height-SCENE_HEIGHT)/2.0f);
    
    _backgroundWrapper = std::make_shared<World>(Vec2(0, 0),_scale, _level->getTiles(), _level->getBoundaries(), assets->get<Texture>("tile"));
    
    _worldnode = scene2::SceneNode::alloc();
    _worldnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _worldnode->setPosition(offset);
    
    _debugnode = scene2::SceneNode::alloc();
    _debugnode->setScale(_scale); // Debug node draws in PHYSICS coordinates
    _debugnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _debugnode->setPosition(offset);
    
    addChild(_worldnode);
    addChild(_debugnode);
    
    populate();
    overWorld.init(assets, _level, _scale, computeActiveSize(),_network, isHost);
    overWorld.setRootNode(_worldnode, _debugnode, _world);
    
    addChild(overWorld.getDog()->getUINode());
    
    _camera.init(overWorld.getDog()->getDogNode(), _worldnode, std::dynamic_pointer_cast<OrthographicCamera>(getCamera()), overWorld.getDog()->getUINode(), 1000.0f);
    
    _active = true;
    setDebug(false);

    //Make a std::function reference of the linkSceneToObs function in game scene for network controller
    std::function<void(const std::shared_ptr<physics2::Obstacle>&,const std::shared_ptr<scene2::SceneNode>&)> linkSceneToObsFunc = [=](const std::shared_ptr<physics2::Obstacle>& obs, const std::shared_ptr<scene2::SceneNode>& node) {
        this->linkSceneToObs(obs,node);
    };
    
    /**
     * TODO: Call network controller to enable physics with linkSceneToObsFunc as the method to attach nodes to obstacles and attach the crate factory to the physics controller and set _factId to its return value.
     *
     * TODO: Acquire the ownership of _cannon2 if this machine is not the host.
     */
#pragma mark BEGIN SOLUTION
    _network->enablePhysics(_world, linkSceneToObsFunc);
    
    if(!isHost){
        _network->getPhysController()->acquireObs(_cannon2, 0);
    }

    _factId = _network->getPhysController()->attachFactory(_crateFact);
#pragma mark END SOLUTION

//TODO: For task 5, attach CrateEvent to the network controller
#pragma mark BEGIN SOLUTION
    _network->attachEventType<CrateEvent>();
    _network->attachEventType<ResetEvent>();
    _network->attachEventType<DecoyEvent>();
#pragma mark END SOLUTION
    
    // XNA nostalgia
    Application::get()->setClearColor(Color4f::CORNFLOWER);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void GameScene::dispose() {
    if (_active) {
        removeAllChildren();
//        _input.dispose();
        _world = nullptr;
        _worldnode = nullptr;
        _debugnode = nullptr;
        _debug = false;
        Scene2::dispose(); 
    }
}

#pragma mark -
#pragma mark Level Layout


std::vector<std::shared_ptr<physics2::BoxObstacle>> boxes;
std::vector<std::shared_ptr<scene2::PolygonNode>> nodes;

/**
 * This method adds a crate at the given position during the init process.
 */
std::shared_ptr<physics2::Obstacle> GameScene::addInitCrate(cugl::Vec2 pos) {
    auto pair =  _crateFact->createObstacle(pos, _scale);
    addInitObstacle(pair.first,pair.second);
    return pair.first;
}

/**
 * This method adds a crate that had been fired by the player's cannon amid the simulation.
 *
 * If this machine is host, the crate should be fire from the left cannon (_cannon1), vice versa.
 */
void GameScene::fireCrate() {
    //TODO: Add a new crate to the simulation using the addSharedObstacle() method from the physics controller, and set its velocity in the direction the cannon is aimed scaled by (50 * _input.getFirePower()).
    //HINT: You can use the serializedParams() method of the crate factory to help you serialize the parameters.
#pragma mark BEGIN SOLUTION
    auto cannon = _isHost ? _cannon1 : _cannon2;
    auto params = _crateFact->serializeParams(cannon->getPosition(), _scale);
    auto pair = _network->getPhysController()->addSharedObstacle(_factId, params);
    float angle = cannon->getAngle() + M_PI_2;
    Vec2 forward(SDL_cosf(angle), SDL_sinf(angle));
    pair.first->setLinearVelocity(forward * 50 );
    pair.first->setDebugScene(_debugnode);
#pragma mark END SOLUTION
}


/**
 * This method takes a crateEvent and processes it.
 */
void GameScene::processCrateEvent(const std::shared_ptr<CrateEvent>& event){
    //Choose randomly between wooden crates and iron crates.
    int indx = 1;
    std::string name = (CRATE_PREFIX "0") + std::to_string(indx);
    auto image = _assets->get<Texture>(name);
    Size boxSize(image->getSize() / _scale);
    auto crate = physics2::BoxObstacle::alloc(Vec2(event->getPos().x,event->getPos().y), boxSize);
    
    crate->setDebugColor(DYNAMIC_COLOR);
    crate->setAngleSnap(0); // Snap to the nearest degree
    
    // Set the physics attributes
    crate->setDensity(CRATE_DENSITY);
    crate->setFriction(CRATE_FRICTION);
    crate->setAngularDamping(CRATE_DAMPING);
    crate->setRestitution(BASIC_RESTITUTION);

    crate->setShared(true);
    
    auto sprite = scene2::PolygonNode::allocWithTexture(image);
    sprite->setAnchor(Vec2::ANCHOR_CENTER);
    sprite->setScale(1.0f);
    
    //TODO: add the crate and sprite to the simulation
    //NOTE: since both the host and client will receive a CrateEvent, we don't want to use addSharedObstacle() for it because it will create two separate crate. Instead you should use addInitObstacle(), which has the same top-bit id and if all clients called init obstacle the same amount of times, the same low-bit id. There is a potential race condition where multiple clients calling addInitObstacle() can cause id to be mixed up(clients send CrateEvent at the same time). In this lab, we will not address that race condition. But you could send along an obstacle id to ensure that all clients have that id for the obstacle.
#pragma mark BEGIN SOLUTION
    addInitObstacle(crate,sprite);
#pragma mark END SOLUTION
}

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
void GameScene::populate() {
    _world = physics2::net::NetWorld::alloc(Rect(0,0,DEFAULT_WIDTH,DEFAULT_HEIGHT),Vec2(0,DEFAULT_GRAVITY));
    _world->activateCollisionCallbacks(true);
    _world->onBeginContact = [this](b2Contact* contact) {
        beginContact(contact);
    };
    _world->beforeSolve = [this](b2Contact* contact, const b2Manifold* oldManifold) {
        beforeSolve(contact,oldManifold);
    };
    
    
    std::shared_ptr<Texture> image;
        
    
#pragma mark : Background
    addChildBackground();
#pragma mark : Cannon
    image  = _assets->get<Texture>(CANNON_TEXTURE);
    _cannon1Node = scene2::PolygonNode::allocWithTexture(image);
    Size canSize(image->getSize()/_scale);
         
    Vec2 canPos1 = ((Vec2)CAN1_POS);
    _cannon1 = cugl::physics2::BoxObstacle::alloc(canPos1,canSize);
    _cannon1->setAngle(-M_PI_2);
    _cannon1->setDebugColor(DYNAMIC_COLOR);
    _cannon1->setSensor(true);
        
    image  = _assets->get<Texture>(CANNON_TEXTURE);
    _cannon2Node = scene2::PolygonNode::allocWithTexture(image);
    
    Vec2 canPos2 = ((Vec2)CAN2_POS);
    _cannon2= cugl::physics2::BoxObstacle::alloc(canPos2,canSize);
    _cannon2->setAngle(M_PI_2);
    _cannon2->setDebugColor(DYNAMIC_COLOR);
    _cannon2->setSensor(true);

    addInitObstacle(_cannon1, _cannon1Node);
    addInitObstacle(_cannon2, _cannon2Node);
}

void GameScene::linkAllAnimationsToObject(const std::shared_ptr<physics2::Obstacle>& obj,
                                          const std::vector<std::shared_ptr<scene2::SceneNode>>& vecNodes) {
    
    for (const std::shared_ptr<scene2::SceneNode>& node: vecNodes){
        node->setPosition(obj->getPosition() * _scale);
        _worldnode->addChild(node);
    }

    // Dynamic objects need constant updating
    if (obj->getBodyType() == b2_dynamicBody) {
        obj->setListener([=](physics2::Obstacle* obs) {
            float leftover = Application::get()->getFixedRemainder() / 1000000.f;
            Vec2 pos = obs->getPosition() + leftover * obs->getLinearVelocity();
            float angle = obs->getAngle() + leftover * obs->getAngularVelocity();
            for (const std::shared_ptr<scene2::SceneNode>& node: vecNodes){
                scene2::SceneNode* weak = node.get();
                weak->setPosition(pos * _scale);
                weak->setAngle(angle);
            }
        });
    }
}
void GameScene::linkSceneToObs(const std::shared_ptr<physics2::Obstacle>& obj,
    const std::shared_ptr<scene2::SceneNode>& node) {
    
    node->setPosition(obj->getPosition() * _scale);
    _worldnode->addChild(node);

    // Dynamic objects need constant updating
    if (obj->getBodyType() == b2_dynamicBody) {
        scene2::SceneNode* weak = node.get(); // No need for smart pointer in callback
        obj->setListener([=](physics2::Obstacle* obs) {
            float leftover = Application::get()->getFixedRemainder() / 1000000.f;
            Vec2 pos = obs->getPosition() + leftover * obs->getLinearVelocity();
            float angle = obs->getAngle() + leftover * obs->getAngularVelocity();
            weak->setPosition(pos * _scale);
            weak->setAngle(angle);
        });
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
void GameScene::addInitObstacle(const std::shared_ptr<physics2::Obstacle>& obj,
    const std::shared_ptr<scene2::SceneNode>& node) {
    _world->initObstacle(obj);
    obj->setDebugScene(_debugnode);
    if(_isHost){
        _world->getOwnedObstacles().insert({obj,0});
    }
    linkSceneToObs(obj, node);
}

void GameScene::addInitObstacleLinkAnimation(const std::shared_ptr<physics2::Obstacle>& obj,
                                      const std::vector<std::shared_ptr<scene2::SceneNode>>& vecNodes) {
    _world->initObstacle(obj);
    obj->setDebugScene(_debugnode);
    if(_isHost){
        _world->getOwnedObstacles().insert({obj,0});
    }
    linkAllAnimationsToObject(obj, vecNodes);
}


#pragma mark
#pragma mark Physics Handling

void GameScene::preUpdate(float dt) {
    if (needToReset()){
         CULog("Reseting\n");
         reset();
    }
    if (_input.didPressExit()) {
        Application::get()->quit();
    }
    _input.update();

    // Process the toggled key commands
    if (_input.didPressDebug()) {
        setDebug(!isDebug());
    }

    if (_input.didPressFire()) {
        fireCrate();
    }

    _camera.update(dt);
    overWorld.update(_input, computeActiveSize(), dt);
#pragma mark BEGIN SOLUTION
    if (_input.didChangeMode()){
        CULog("BIG CRATE COMING");
        _network->pushOutEvent(CrateEvent::allocCrateEvent(Vec2(DEFAULT_WIDTH/2,DEFAULT_HEIGHT/2)));
    }
    if (_input.didPressReset()){
        CULog("RESET COMING");
        _network->pushOutEvent(ResetEvent::allocResetEvent());
    }
#pragma mark END SOLUTION
    
    float turnRate = _isHost ? DEFAULT_TURN_RATE : -DEFAULT_TURN_RATE;
    auto cannon = _isHost ? _cannon1 : _cannon2;
    cannon->setAngle(_input.getForward() * turnRate + cannon->getAngle());
}

void GameScene::postUpdate(float dt) {
    //Nothing to do now
    overWorld.postUpdate();
}

void GameScene::fixedUpdate() {
    //TODO: check for available incoming events from the network controller and call processCrateEvent if it is a CrateEvent.
    
    //Hint: You can check if ptr points to an object of class A using std::dynamic_pointer_cast<A>(ptr). You should always check isInAvailable() before popInEvent().
    
#pragma mark BEGIN SOLUTION
    if(_network->isInAvailable()){
        auto e = _network->popInEvent();
        if(auto crateEvent = std::dynamic_pointer_cast<CrateEvent>(e)){
//            CULog("BIG CRATE GOT");
            processCrateEvent(crateEvent);
        }
        if (auto resetEvent = std::dynamic_pointer_cast<ResetEvent>(e)){
//            CULog("RESET EVENT GOT");
            setToDoReset(true);
        }
        if (auto decoyEvent = std::dynamic_pointer_cast<DecoyEvent>(e)){
//            CULog("Decoy Event Got");
            overWorld.getDecoys()->addNewDecoy(Vec2(decoyEvent->getPos().x,decoyEvent->getPos().y));
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
void GameScene::update(float dt) {
    //deprecated
}

/**
 * Processes the start of a collision
 *
 * Since this game has no real need to determine collisions, right now this is left empty.
 *
 * @param  contact  The two bodies that collided
 */
void GameScene::beginContact(b2Contact* contact) {
}

/**
 * Handles any modifications necessary before collision resolution
 *
 * This method is called just before Box2D resolves a collision.  We use this method
 * to implement sound on contact, using the algorithms outlined in Ian Parberry's
 * "Introduction to Game Physics with Box2D".
 *
 * @param  contact  	The two bodies that collided
 * @param  oldManfold  	The collision manifold before contact
 */
void GameScene::beforeSolve(b2Contact* contact, const b2Manifold* oldManifold) {
    float speed = 0;

    // Use Ian Parberry's method to compute a speed threshold
    b2Body* body1 = contact->GetFixtureA()->GetBody();
    b2Body* body2 = contact->GetFixtureB()->GetBody();
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    b2PointState state1[2], state2[2];
    b2GetPointStates(state1, state2, oldManifold, contact->GetManifold());
    for(int ii =0; ii < 2; ii++) {
        if (state2[ii] == b2_addState) {
            b2Vec2 wp = worldManifold.points[0];
            b2Vec2 v1 = body1->GetLinearVelocityFromWorldPoint(wp);
            b2Vec2 v2 = body2->GetLinearVelocityFromWorldPoint(wp);
            b2Vec2 dv = v1-v2;
            speed = b2Dot(dv,worldManifold.normal);
        }
    }
    
    // Play a sound if above threshold
    if (speed > SOUND_THRESHOLD) {
        // These keys result in a low number of sounds.  Too many == distortion.
        physics2::Obstacle* data1 = reinterpret_cast<physics2::Obstacle*>(body1->GetUserData().pointer);
        physics2::Obstacle* data2 = reinterpret_cast<physics2::Obstacle*>(body2->GetUserData().pointer);

        if (data1 != nullptr && data2 != nullptr) {
            std::string key = (data1->getName()+data2->getName());
            auto source = _assets->get<Sound>(COLLISION_SOUND);
            if (!AudioEngine::get()->isActive(key)) {
                AudioEngine::get()->play(key, source, false, source->getVolume());
            }
        }
    }
}

/**
 * Returns the active screen size of this scene.
 *
 * This method is for graceful handling of different aspect
 * ratios
 */
Size GameScene::computeActiveSize() const {
    Size dimen = Application::get()->getDisplaySize();
    float ratio1 = dimen.width/dimen.height;
    float ratio2 = ((float)SCENE_WIDTH)/((float)SCENE_HEIGHT);
    if (ratio1 < ratio2) {
        dimen *= SCENE_WIDTH/dimen.width;
    } else {
        dimen *= SCENE_HEIGHT/dimen.height;
    }
    return dimen;
}


void GameScene::addChildBackground(){
    const std::vector<std::vector<std::shared_ptr<TileInfo>>>& currentBackground = _backgroundWrapper->getTileWorld();
    int originalRows = (int) currentBackground.size();
    int originalCols = (int) currentBackground.at(0).size();
    for (int j =0  ;j< originalCols; j++){
        for (int i = originalRows -1; i > -1; i--){
            std::shared_ptr<TileInfo> t = currentBackground.at(i).at(j);
            auto image  = t->texture;
            auto sprite = scene2::PolygonNode::allocWithTexture(image);
            if (i == 0 || j == 0 || i == originalRows -1 || j == originalCols - 1){
                t->setDebugColor(DYNAMIC_COLOR);
                addInitObstacle(t, sprite);
            }else{
                sprite->setPosition(t->getPosition() * _scale);
                _worldnode->addChild(sprite);
            }
        }
    }
}

//
//  Dog.cpp
//  Rocket Demo
//
//  This encapsulates all of the information for the rocket demo.  Note how this
//  class combines physics and animation.  This is a good template for models in
//  your game.
//
//  WARNING: There are a lot of shortcuts in this design that will do not adapt
//  well to data driven design.  This demo has a lot of simplifications to make
//  it a bit easier to see how everything fits together.  However, the model
//  classes and how they are initialized will need to be changed if you add
//  dynamic level loading.
//
//  Pay close attention to how this class designed. This class uses our standard
//  shared-pointer architecture which is common to the entire engine.
//
//  1. The constructor does not perform any initialization; it just sets all
//     attributes to their defaults.
//
//  2. All initialization takes place via init methods, which can fail if an
//     object is initialized more than once.
//
//  3. All allocation takes place via static constructors which return a shared
//     pointer.
//
//  Note that this object manages its own texture, but DOES NOT manage its own
//  scene graph node.  This is a very common way that we will approach complex
//  objects.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White
//  Version: 1/10/17
//
#include "NLDog.h"

using namespace cugl;

#pragma mark -
#pragma mark Animation and Physics Constants

/** This is adjusted by screen aspect ratio to get the height */
#define GAME_WIDTH 1024

/** The number of frames for the afterburner */
#define FIRE_FRAMES     4

// Default physics values
/** The density of this rocket */
#define DEFAULT_DENSITY 1.0f
/** The friction of this rocket */
#define DEFAULT_FRICTION 0.1f
/** The restitution of this rocket */
#define DEFAULT_RESTITUTION 0.4f

#pragma mark -
#pragma mark Constructors

/**
 * Initializes a new rocket with the given position and size.
 *
 * The rocket size is specified in world coordinates.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos      Initial position in world coordinates
 * @param  size       The dimensions of the box.
 *
 * @return  true if the obstacle is initialized properly, false otherwise.
 */
bool Dog::init(const Vec2 pos, const Size size) {
    physics2::BoxObstacle::init(pos,size);
    std::string name("rocket");
    setName(name);
    

    _mainSound  = "";
    _leftSound  = "";
    _rghtSound  = "";

    setDensity(DEFAULT_DENSITY);
    setFriction(DEFAULT_FRICTION);
    setRestitution(DEFAULT_RESTITUTION);
    setFixedRotation(true);
    
    return true;
}


/**
 * Disposes all resources and assets of this rocket
 *
 * Any assets owned by this object will be immediately released.  Once
 * disposed, a rocket may not be used until it is initialized again.
 */
void Dog::dispose() {
    runAnimationMedium = nullptr;
    biteAnimationMedium = nullptr;
    idleAnimationMedium = nullptr;
    shootAnimationMedium = nullptr;
}


#pragma mark -
#pragma mark Physics
/**
 * Applies the Velocity Movement for the Physics Engine
 *
 */
void Dog::moveOnInput(NetLabInput& _input){
    setVX(_input.getHorizontal()*getThrust());
    setVY(_input.getVertical()*getThrust());
    dir.x =_input.getHorizontal()*getThrust();
    dir.y = _input.getVertical()*getThrust();
    
}

void Dog::dogActions(){
    AnimationSceneNode::Directions direction = AnimationSceneNode::convertRadiansToDirections(dir.getAngle());
//    if(biteAnimationMedium->getFrame() == biteAnimationMedium->getSize() - 1){
//        // bite is finished
//    }
//    
//    if(shootAnimationMedium->getFrame() == shootAnimationMedium->getSize() - 1){
//        // shoot is finished
//    }
    if(dir.x == 0 && dir.y == 0){
        idleAnimationMedium->animate(prevDirection, true);
        
        runAnimationMedium->animate(prevDirection, false);
//        biteAnimationMedium->animate(prevDirection, false);
//        shootAnimationMedium->animate(prevDirection, false);
    }
    else{
        prevDirection = direction;
        
        runAnimationMedium->animate(direction, true);
        
        idleAnimationMedium->animate(direction, false);
//        biteAnimationMedium->animate(direction, false);
//        shootAnimationMedium->animate(direction, false);
        
    }
}
/**
 * Updates the object's physics state (NOT GAME LOGIC).
 *
 * This method is called AFTER the collision resolution state. Therefore, it
 * should not be used to process actions or any other gameplay information.
 * Its primary purpose is to adjust changes to the fixture, which have to
 * take place after collision.
 *
 * In other words, this is the method that updates the scene graph.  If you
 * forget to call it, it will not draw your changes.
 *
 * @param delta Timing values from parent loop
 */
void Dog::update(float delta) {
    Obstacle::update(delta);
    if (runAnimationMedium != nullptr) {
        runAnimationMedium->setPosition(getPosition()*_drawscale);
        idleAnimationMedium->setPosition(getPosition()*_drawscale);
        shootAnimationMedium->setPosition(getPosition()*_drawscale);
        biteAnimationMedium->setPosition(getPosition()*_drawscale);
        
        
        runAnimationMedium->setAngle(getAngle());
        
        dogActions();
    }
}


#pragma mark -
#pragma mark Animation
/**
 * Returns the key for the sound to accompany the given afterburner
 *
 * The key should either refer to a valid sound loaded in the AssetManager or
 * be empty ("").  If the key is "", then no sound will play.
 *
 * @param burner    The enumeration to identify the afterburner
 *
 * @return the key for the sound to accompany the given afterburner
 */
const std::string& Dog::getBurnerSound(Burner burner) const {
    switch (burner) {
        case Burner::MAIN:
            return _mainSound;
        case Burner::LEFT:
            return _leftSound;
        case Burner::RIGHT:
            return _rghtSound;
    }
    CUAssertLog(false, "Invalid burner enumeration");
    return _mainSound;
}

/**
 * Sets the key for the sound to accompany the given afterburner
 *
 * The key should either refer to a valid sound loaded in the AssetManager or
 * be empty ("").  If the key is "", then no sound will play.
 *
 * @param burner    The enumeration to identify the afterburner
 * @param key       The key for the sound to accompany the main afterburner
 */
void Dog::setBurnerSound(Burner burner, const std::string& key) {
    switch (burner) {
        case Burner::MAIN:
            _mainSound = key;
            break;
        case Burner::LEFT:
            _leftSound = key;
            break;
        case Burner::RIGHT:
            _rghtSound = key;
            break;
        default:
            CUAssertLog(false, "Invalid burner enumeration");
    }
}


/**
 * Sets the ratio of the ship sprite to the physics body
 *
 * The rocket needs this value to convert correctly between the physics
 * coordinates and the drawing screen coordinates.  Otherwise it will
 * interpret one Box2D unit as one pixel.
 *
 * All physics scaling must be uniform.  Rotation does weird things when
 * attempting to scale physics by a non-uniform factor.
 *
 * @param scale The ratio of the ship sprite to the physics body
 */
void Dog::setDrawScale(float scale) {
    _drawscale = scale;
    if (runAnimationMedium != nullptr) {
       runAnimationMedium->setPosition(getPosition()*_drawscale);
    }
}

void Dog::setMediumAnimation(std::shared_ptr<AnimationSceneNode> idle, std::shared_ptr<AnimationSceneNode> run, std::shared_ptr<AnimationSceneNode> bite, std::shared_ptr<AnimationSceneNode> shoot){
    runAnimationMedium = run;
    idleAnimationMedium = idle;
    shootAnimationMedium = shoot;
    biteAnimationMedium = bite;
}


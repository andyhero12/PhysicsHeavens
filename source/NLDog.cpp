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

#define FIRE_RATE 25
#define HEAL_RATE 50
#define HEALTH 100
#define MAX_ABSORB 30
#define BITE_RADIUS 220.0f
#define SHOOT_RADIUS 540.0f
#define EXPLOSION_RADIUS 90.0f

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
    setDensity(DEFAULT_DENSITY);
    setFriction(DEFAULT_FRICTION);
    setRestitution(DEFAULT_RESTITUTION);
    setFixedRotation(true);
    setDogSize(DogSize::MEDIUM);
    _mode = 0;
    _refire = 0;
    _absorbValue = 0;
    _maxAbsorb = MAX_ABSORB;
    _firerate = FIRE_RATE;
    _healCooldown = 0;
    _health = HEALTH;
    _maxHealth = HEALTH;
    _healRate = HEAL_RATE;
    _explosionRadius = EXPLOSION_RADIUS;
    _biteRadius = BITE_RADIUS;
    _shootRadius = SHOOT_RADIUS;
    prevDirection =AnimationSceneNode::Directions::EAST;
    _curDirection = AnimationSceneNode::Directions::EAST;
    
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

void Dog::updateUI(){
    _uiController->setHealthBarTexture(((float)getHealth())/getMaxHealth());
    _uiController->setSizeBarTexture(((float)getAbsorb())/getMaxAbsorb());
}
/**
 * Applies the Velocity Movement for the Physics Engine
 *
 */
void Dog::moveOnInputSetAction(InputController& _input){
    if (_refire <= _firerate) {
        _refire++;
    }
    if (_healCooldown <= _healRate) {
        _healCooldown++;
    }
    cugl::Vec2 _vel(0,0);
    if (_input.getControllerState()){
        _vel = _input.getVelocity();
    }else if (_input.getKeyboardState()){
        _vel = Vec2(_input.getTurn(), _input.getForward());
    }
    prevDirection =_curDirection;
    setVX(_vel.x*getThrust());
    setVY(_vel.y*getThrust());
    dir.x =_vel.x*getThrust();
    dir.y = _vel.y*getThrust();
    
    // keep same direction until movement
    if (_vel.x != 0 || _vel.y != 0){
        _ang = dir;
        _curDirection = AnimationSceneNode::convertRadiansToDirections(dir.getAngle());
    }
    
    if (_input.didChangeMode()){
        toggleMode();
    }
    if (action == Actions::BITE || action == Actions::SHOOT){ // wait for them to finish
        return;
    }
    if (_input.didPressFire() && canFireWeapon()){ // bite
        action = Actions::BITE;
    }else if (_input.didPressSpecial() && modes.at(_mode) == "SHOOT"){ // attack
        action = Actions::SHOOT;
    }else if (_input.didPressSpecial() && getMode() == "EXPLODE"){
        // maybe add explode animation later
    }else if (_vel.x != 0 || _vel.y != 0){ // walking
        action = Actions::RUN;
    }else{ // idle
        action = Actions::IDLE;
    }
    
}
void Dog::setDogSize(DogSize size){
    if(idleAnimation != nullptr && size != dogSize){
        idleAnimation->animate(AnimationSceneNode::Directions::SOUTH, false);
        runAnimation->animate(AnimationSceneNode::Directions::SOUTH, false);
        biteAnimation->animate(AnimationSceneNode::Directions::SOUTH, false);
        shootAnimation->animate(AnimationSceneNode::Directions::SOUTH, false);
    }
    switch (size) {

        case DogSize::SMALL:
            idleAnimation =  idleAnimationSmall;
            runAnimation =  runAnimationSmall;
            biteAnimation = biteAnimationSmall;
            shootAnimation = shootAnimationSmall;
            break;
        case DogSize::MEDIUM:
            idleAnimation =  idleAnimationMedium;
            runAnimation =  runAnimationMedium;
            biteAnimation = biteAnimationMedium;
            shootAnimation = shootAnimationMedium;
            break;
        case DogSize::LARGE:
            idleAnimation =  idleAnimationLarge;
            runAnimation =  runAnimationLarge;
            biteAnimation = biteAnimationLarge;
            shootAnimation = shootAnimationLarge;
            break;
        default:
            CULog("INVALID SETDOGSIZE INPUT");
            break;
    }
    
}
void Dog::dogActions(){
    AnimationSceneNode::Directions direction = getDirection();
    if(action == Actions::BITE && biteAnimation->getFrame() == biteAnimation->getSize() - 1){
        // bite is finished
        action = Actions::RUN;
    }
    else if(action == Actions::SHOOT && shootAnimation->getFrame() == shootAnimation->getSize() - 1){
        // shoot is finished
        action = Actions::RUN;
    }
    
    bool attack = action == Actions::BITE || action==Actions::SHOOT;
    
    if(dir.x == 0 && dir.y == 0 && !attack){
        action = Actions::IDLE;
        idleAnimation->animate(prevDirection, true);
        runAnimation->animate(prevDirection, false);
        biteAnimation->animate(prevDirection, false);
        shootAnimation->animate(prevDirection, false);
    }
    else{
        idleAnimation->animate(direction, false);
        runAnimation->animate(direction, action == Actions::RUN);
        biteAnimation->animate(direction, action == Actions::BITE);
        shootAnimation->animate(direction, action == Actions::SHOOT);
        
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
    // Decoupled so useless for now
    if (baseBlankNode){
        baseBlankNode->setPosition(getPosition()*_drawscale);
        baseBlankNode->setAngle(getAngle());
    }
    dogActions();
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
}

void Dog::setSmallAnimation(std::shared_ptr<AnimationSceneNode> idle, std::shared_ptr<AnimationSceneNode> run, std::shared_ptr<AnimationSceneNode> bite, std::shared_ptr<AnimationSceneNode> shoot){
    runAnimationSmall = run;
    idleAnimationSmall = idle;
    shootAnimationSmall = shoot;
    biteAnimationSmall = bite;
}

void Dog::setMediumAnimation(std::shared_ptr<AnimationSceneNode> idle, std::shared_ptr<AnimationSceneNode> run, std::shared_ptr<AnimationSceneNode> bite, std::shared_ptr<AnimationSceneNode> shoot){
    runAnimationMedium = run;
    idleAnimationMedium = idle;
    shootAnimationMedium = shoot;
    biteAnimationMedium = bite;
}

void Dog::setLargeAnimation(std::shared_ptr<AnimationSceneNode> idle, std::shared_ptr<AnimationSceneNode> run, std::shared_ptr<AnimationSceneNode> bite, std::shared_ptr<AnimationSceneNode> shoot){
    runAnimationLarge = run;
    idleAnimationLarge = idle;
    shootAnimationLarge = shoot;
    biteAnimationLarge = bite;
}
// Decoupled so useless for now
void Dog::setFinalDog(std::shared_ptr<cugl::scene2::SceneNode> baseNode){
    baseBlankNode = baseNode;
    resetCurrentAnimations(DogSize::MEDIUM);
}

void Dog::resetCurrentAnimations(DogSize size){
    baseBlankNode->removeAllChildren();
    setDogSize(size);
    idleAnimation->setPosition(baseBlankNode->getAnchor());
    baseBlankNode->addChild(idleAnimation);
    biteAnimation->setPosition(baseBlankNode->getAnchor());
    baseBlankNode->addChild(biteAnimation);
    shootAnimation->setPosition(baseBlankNode->getAnchor());
    baseBlankNode->addChild(shootAnimation);
    runAnimation->setPosition(baseBlankNode->getAnchor());
    baseBlankNode->addChild(runAnimation);
    baseBlankNode->setPosition(getPosition()*_drawscale);
}

void Dog::setHealth(int value){
    _health = value;
    _uiController->setHealthBarTexture(_health/_maxHealth);
}

void Dog::addAbsorb(int value) {
    _absorbValue += value;
    _absorbValue = fmin(_absorbValue, MAX_ABSORB);
}

void Dog::subAbsorb(int value) {
    _absorbValue -= value;
    if (_absorbValue < 0){
        _absorbValue = 0;
    }
}
void Dog::setAbsorbValue(int value){
    _absorbValue = value;
    _absorbValue = fmin(_absorbValue, MAX_ABSORB);
    _uiController->setSizeBarTexture(_absorbValue/MAX_ABSORB);
}

void Dog::setMode(int mode){
    _mode = mode;
    _uiController->setToggle(modes.at(_mode));
}

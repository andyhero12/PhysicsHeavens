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
#define BITE_RADIUS 3.5f
#define SHOOT_RADIUS 8.5f
#define EXPLOSION_RADIUS 1.4f
#define DASH_RATE 50
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
    if (physics2::BoxObstacle::init(pos,size)){
        std::string name("rocket");
        setName(name);
        setDensity(DEFAULT_DENSITY);
        setFriction(DEFAULT_FRICTION);
        setRestitution(DEFAULT_RESTITUTION);
        setFixedRotation(true);
        setDogSize(DogSize::SMALL);
        _startDash = false;
        _dashCounter = 0;
        _dashRate = DASH_RATE;
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
    
    return false;
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
    dashAnimationMedium = nullptr;
    
    runAnimationSmall = nullptr;
    biteAnimationSmall = nullptr;
    idleAnimationSmall = nullptr;
    shootAnimationSmall = nullptr;
    dashAnimationSmall = nullptr;
    
    runAnimationLarge = nullptr;
    biteAnimationLarge = nullptr;
    idleAnimationLarge = nullptr;
    shootAnimationLarge = nullptr;
    dashAnimationLarge = nullptr;
}


#pragma mark -
#pragma mark Physics

void Dog::updateUI(){
    _uiController->setHealthBarTexture(((float)getHealth())/getMaxHealth());
    _uiController->setSizeBarTexture(((float)getAbsorb())/getMaxAbsorb());
}

void Dog::updateClientAnimations(){
    prevDirection =_curDirection;
    if (getVX() != 0 || getVY() != 0){
        if (_startDash){
            _startDash = false;
            action = Actions::DASH;
        }
        _curDirection = AnimationSceneNode::convertRadiansToDirections(getLinearVelocity().getAngle());
    }
}
/**
 * Applies the Velocity Movement for the Physics Engine
 *
 */
void Dog::moveOnInputSetAction(InputController& _input){
    if (_refire <= _firerate) {
        _refire++;
    }
    if (_dashCounter <= _dashRate){
        _dashCounter++;
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
    bool startingDash = _startDash;
    if (action != Actions::DASH){
        if (_startDash){
            _startDash = false;
            setVX(_vel.x*getThrust()*2.5);
            setVY(_vel.y*getThrust()*2.5);
            dir.x =_vel.x*getThrust()*2.5;
            dir.y = _vel.y*getThrust()*2.5;
        }else{
            setVX(_vel.x*getThrust());
            setVY(_vel.y*getThrust());
            dir.x =_vel.x*getThrust();
            dir.y = _vel.y*getThrust();
        }
    }
    
    // keep same direction until movement
    if (_vel.x != 0 || _vel.y != 0 && (action != Actions::DASH)){
        _ang = dir;
        _curDirection = AnimationSceneNode::convertRadiansToDirections(dir.getAngle());
    }
    
    if (_input.didChangeMode()){
        toggleMode();
    }
    if (action == Actions::BITE || action == Actions::SHOOT || action == Actions::DASH){ // wait for them to finish
        return;
    }
    if (_input.didPressFire() && canFireWeapon()){ // bite
        action = Actions::BITE;
    }else if (_input.didPressSpecial() && modes.at(_mode) == "SHOOT" && getAbsorb() >= 5){ // attack
        action = Actions::SHOOT;
    }else if (_input.didPressSpecial() && getMode() == "EXPLODE" && getAbsorb() >= 5){
        // maybe add explode animation later
    }
    else if (startingDash){
        action = Actions::DASH;
    }
    else if (_vel.x != 0 || _vel.y != 0){ // walking
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
        dashAnimation->animate(AnimationSceneNode::Directions::SOUTH, false);
    }
    switch (size) {

        case DogSize::SMALL:
            idleAnimation =  idleAnimationSmall;
            runAnimation =  runAnimationSmall;
            biteAnimation = biteAnimationSmall;
            shootAnimation = shootAnimationSmall;
            dashAnimation = dashAnimationSmall;
            break;
        case DogSize::MEDIUM:
            idleAnimation =  idleAnimationMedium;
            runAnimation =  runAnimationMedium;
            biteAnimation = biteAnimationMedium;
            shootAnimation = shootAnimationMedium;
            dashAnimation = dashAnimationMedium;
            break;
        case DogSize::LARGE:
            idleAnimation =  idleAnimationLarge;
            runAnimation =  runAnimationLarge;
            biteAnimation = biteAnimationLarge;
            shootAnimation = shootAnimationLarge;
            dashAnimation = dashAnimationLarge;
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
    else if (action == Actions::DASH && dashAnimation->getFrame() == dashAnimation->getSize() - 1){
        action = Actions::RUN;
    }
    
    bool attack = action == Actions::BITE || action==Actions::SHOOT || action == Actions::DASH;
    
    if(dir.x == 0 && dir.y == 0 && !attack){
        action = Actions::IDLE;
        idleAnimation->animate(prevDirection, true);
        runAnimation->animate(prevDirection, false);
        biteAnimation->animate(prevDirection, false);
        shootAnimation->animate(prevDirection, false);
        dashAnimation->animate(prevDirection, false);
    }
    else{
        idleAnimation->animate(direction, false);
        runAnimation->animate(direction, action == Actions::RUN);
        biteAnimation->animate(direction, action == Actions::BITE);
        shootAnimation->animate(direction, action == Actions::SHOOT);
        dashAnimation->animate(direction, action == Actions::DASH);
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
        baseBlankNode->setPosition(getPosition());
        baseBlankNode->setAngle(getAngle());
    }
    dogActions();
}


void Dog::setSmallAnimation(std::shared_ptr<AnimationSceneNode> idle, std::shared_ptr<AnimationSceneNode> run, std::shared_ptr<AnimationSceneNode> bite, std::shared_ptr<AnimationSceneNode> shoot, std::shared_ptr<AnimationSceneNode> dash){
    runAnimationSmall = run;
    idleAnimationSmall = idle;
    shootAnimationSmall = shoot;
    biteAnimationSmall = bite;
    dashAnimationSmall = dash;
}

void Dog::setMediumAnimation(std::shared_ptr<AnimationSceneNode> idle, std::shared_ptr<AnimationSceneNode> run, std::shared_ptr<AnimationSceneNode> bite, std::shared_ptr<AnimationSceneNode> shoot,std::shared_ptr<AnimationSceneNode> dash){
    runAnimationMedium = run;
    idleAnimationMedium = idle;
    shootAnimationMedium = shoot;
    biteAnimationMedium = bite;
    dashAnimationMedium = dash;
}

void Dog::setLargeAnimation(std::shared_ptr<AnimationSceneNode> idle, std::shared_ptr<AnimationSceneNode> run, std::shared_ptr<AnimationSceneNode> bite, std::shared_ptr<AnimationSceneNode> shoot,std::shared_ptr<AnimationSceneNode> dash){
    runAnimationLarge = run;
    idleAnimationLarge = idle;
    shootAnimationLarge = shoot;
    biteAnimationLarge = bite;
    dashAnimationLarge = dash;
}
// Decoupled so useless for now
void Dog::setFinalDog(std::shared_ptr<cugl::scene2::SceneNode> baseNode){
    baseBlankNode = baseNode;
    resetCurrentAnimations(DogSize::SMALL);
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
    
    baseBlankNode->addChild(dashAnimation);
    dashAnimation->setPosition(baseBlankNode->getAnchor());
    
    baseBlankNode->addChild(runAnimation);
    baseBlankNode->setPosition(getPosition());
}

void Dog::setHealth(int value){
    _health = std::max(0,value);
    _uiController->setHealthBarTexture(float(_health)/_maxHealth);
}

void Dog::addAbsorb(int value) {
    setAbsorbValue(std::fmin(MAX_ABSORB,getAbsorb() + value));
}

void Dog::subAbsorb(int value) {
    setAbsorbValue(std::fmax(0.0f,getAbsorb() - value));
}
void Dog::setAbsorbValue(int value){
    if (_absorbValue > 10 && value <= 10){
        resetCurrentAnimations(DogSize::SMALL);
    }else if (_absorbValue > 10 && _absorbValue < 20 && value <= 20){
        resetCurrentAnimations(DogSize::MEDIUM);
    }else if (_absorbValue >= 20 &&  value <= 30){
        resetCurrentAnimations(DogSize::LARGE);
    }
    _absorbValue = value;
    _absorbValue = fmin(_absorbValue, MAX_ABSORB);
    _uiController->setSizeBarTexture(_absorbValue/MAX_ABSORB);
    float dim = (float) value / MAX_ABSORB + 1.5;
    cugl::Size nxtSize(dim,dim);
    setDimension(nxtSize);
    baseBlankNode->setScale(dim/64);
}

void Dog::setMode(int mode){
    _mode = mode;
    _uiController->setToggle(modes.at(_mode));
}

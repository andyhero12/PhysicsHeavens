//
//  AbstractEnemy.h
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#ifndef AbstractEnemy_h
#define AbstractEnemy_h
#include <cugl/cugl.h>
#include <random>
#include "OverWorld.h"
#include "AudioController.h"
#include "AnimationSceneNode.h"
#define MAGIC_NUMBER_ENEMY_ANIMATION_FREQUENECY 4
// Default physics values
/** The density of this rocket */
#define DEFAULT_DENSITY 5.0f
/** The friction of this rocket */
#define DEFAULT_FRICTION 0.0f
/** The restitution of this rocket */
#define DEFAULT_RESTITUTION 0.0f

/** The number of frames before a new goal can be set */
#define PATHFIND_COOLDOWN 20


/** Set these to 0 to disable */
#define CLOSE_DISTANCE 0
#define STRAY_DISTANCE 1000000
#define SAME_GOAL_DISTANCE 2

#define DAMAGED_DURATION 0.5f

#define KNOCKBACK_FORCE 45
#define LINEAR_DAMPING 15.0f
#define KNOCKBACK_TIME 0.25f
#define KNOCKBACK_LIMIT 14

#define MAX_ESCAPE_TIME 25

#include "stlastar.h"
#include "WorldSearchVertex.h"

class AbstractEnemy : public cugl::physics2::CapsuleObstacle {
    
public:
    
    enum class EnemyActions : int {
        SPAWN,
        WANDER,
        STAY,
        CHASE,
        LOWHEALTH,
        RUNAWAY,
        ATTACK
    };
    
    AbstractEnemy(){}
    // Virtual destructor
    virtual ~AbstractEnemy(){
        
        topLevelPlaceHolder = nullptr;
        runAnimations = nullptr;
        attackAnimations = nullptr;
        _pathfinder = nullptr;
        _healthBar = nullptr;
    }
    

    bool init(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex){
        bool result = physics2::CapsuleObstacle::init(m_pos,m_size);
        
        if (result){
            _counter = 0;
            updateRate = 5;
            clearSharingDirtyBits();
            setShared(true);
            setDensity(DEFAULT_DENSITY);
            setFriction(DEFAULT_FRICTION);
            setRestitution(DEFAULT_RESTITUTION);
            setFixedRotation(true);
            setLinearDamping(LINEAR_DAMPING);
            
            curAction = EnemyActions::SPAWN;
            _health = m_health;
            _maxHealth = m_health;
            targetIndex = m_targetIndex;
            _prevDirection =AnimationSceneNode::Directions::EAST;
            _curDirection = AnimationSceneNode::Directions::EAST;
            _pathfinder = std::make_shared<AStarSearch<WorldSearchVertex>>();
            _time = 0;
            _wanderAngle = 0.0f;
            timeSinceLastMajorChange = 0.0f;

            _damagedTimer = 0;
            _knockbackTimer = false;
            
            _inContact = false;
            movementDirection = Vec2(0,0);
            return true;
        }
        return false;
    }

    void setAudioController(std::shared_ptr<AudioController> m_audioController)
    {
        _audioController = m_audioController;
    }

    virtual void setVX(float value) override {
        if(!(_knockbackTimer > 0)) {
            CapsuleObstacle::setVX(value);
        }
    }

    virtual void setVY(float value) override {
        if(!(_knockbackTimer > 0)) {
            CapsuleObstacle::setVY(value);
        }
    }

    virtual void setLinearVelocity(Vec2 value) override {
        setLinearVelocity(value.x, value.y);
    }


    virtual void setLinearVelocity(float x, float y) override {
        if(!(_knockbackTimer > 0)) {
            CapsuleObstacle::setLinearVelocity(x, y);
        }
    }

    virtual void update(float delta) override {
        Obstacle::update(delta);
        
        _knockbackTimer -= delta;
        if(_knockbackTimer < 0) {
            _knockbackTimer = 0;
        }

        if(_damagedTimer > 0) {
            _damagedTimer -= delta;
            
        }
        if(_damagedTimer < 0) {
            _damagedTimer = 0;
            //tints may be expensive, so separating out this special case may be worthwhile
            topLevelPlaceHolder->setColor(cugl::Color4(255, 255, 255));
        }
        if(_damagedTimer > 0) {
            float ratio = (DAMAGED_DURATION - _damagedTimer) / DAMAGED_DURATION;
            float brightness = 255 * (0.8f + ratio * 0.2f);
            topLevelPlaceHolder->setColor(cugl::Color4(brightness, brightness * ratio, brightness * ratio));
        }
    }

    
    virtual void preUpdate(float dt, OverWorld& overWorld) = 0;

    void postUpdate(){
        _prevDirection =_curDirection;
        if(_knockbackTimer <= 0){
            Vec2 direction = movementDirection;
            _curDirection = AnimationSceneNode::convertRadiansToDirections(direction.getAngle());
            
            runAnimations->animate(_curDirection, curAction != EnemyActions::ATTACK);
            attackAnimations->animate(_curDirection, curAction == EnemyActions::ATTACK);
        }
    }
    
    void clientAnimationUpdate(OverWorld& overWorld, float dt){
        if (timeSinceLastMajorChange < 1.0){
            timeSinceLastMajorChange += dt;
        }
        Vec2 pos = getPosition();
        Vec2 clientDogPos = overWorld.getClientDog()->getPosition();
        Vec2 diff = clientDogPos - pos;
        float dogLen = diff.length();
        if (timeSinceLastMajorChange >= 1.0){
            timeSinceLastMajorChange = 0.0;
            Vec2 vel = getLinearVelocity();
            _prevDirection =_curDirection;
            if (dogLen > 3){
                _curDirection = AnimationSceneNode::convertRadiansToDirections(vel.getAngle());
            }else{
                _curDirection = AnimationSceneNode::convertRadiansToDirections(diff.getAngle());
            }
        }
        
        runAnimations->animate(_curDirection, dogLen > 3);
        attackAnimations->animate(_curDirection, dogLen <= 3);
    }
    bool isInContact() const { return _inContact; }
    void setInContact(bool value) { _inContact = value; }
    
    virtual int getDamage() = 0;
    virtual bool canAttack() const = 0;
    virtual void resetAttack() = 0;
    virtual int getAbsorbValue() const = 0;
    
    
    virtual void executeDeath(OverWorld& overWorld) {
        CULog("executing Death\n");
    }
    
    int getTargetIndex() const{
        return targetIndex;
    }
    void setTargetIndex(int index){
        targetIndex = index;
    }
    int getHealth() const {
        return _health;
    }
    void setHealth(int m_health) {
        // are there attacks that do no damage?
        if(m_health < _health) {
            _damagedTimer = DAMAGED_DURATION;
        }
        _health = fmax(0.0,m_health);
        if (_health > _maxHealth){
            _health = _maxHealth;
        }
        
        _healthBar->setProgress((float)_health/_maxHealth);
    }
    void applyDamage(int dmg, Vec2 direction) {
        direction.normalize();
        float velocity = KNOCKBACK_FORCE / getMass();
        velocity = velocity > KNOCKBACK_LIMIT ? KNOCKBACK_LIMIT : velocity;
        setLinearVelocity(direction.x * velocity, direction.y * velocity);
        setHealth(getHealth() - dmg);
        _knockbackTimer = KNOCKBACK_TIME;
    }


    
    
    
    cugl::Vec2 getTargetPositionFromIndex(OverWorld& overWorld){
        const std::shared_ptr<Dog>& curDog = overWorld.getDog();
        const std::shared_ptr<BaseSet> baseSet = overWorld.getBaseSet();
        const std::shared_ptr<DecoySet> decoySet = overWorld.getDecoys();
        cugl::Vec2 target_pos;
        int _targetIndex  = getTargetIndex();
        if (_targetIndex == 0){
            target_pos = curDog->getPosition();
        }else if (_targetIndex <= baseSet->_bases.size()){
            target_pos = baseSet->_bases.at(_targetIndex-1)->getPos();
        }else{
            target_pos = decoySet->getCurrentDecoys().at(_targetIndex-1-baseSet->_bases.size())->getPos();
        }
        return target_pos;
    }
    
    void setHealthBar(std::shared_ptr<cugl::scene2::ProgressBar> bar){
        _healthBar = bar;
        _healthBar->setScale(0.1);
        _healthBar->setAnchor(Vec2::ANCHOR_CENTER);
    }
    
    void setFinalEnemy(std::shared_ptr<cugl::scene2::SceneNode> baseNode){
        topLevelPlaceHolder = baseNode;
        topLevelPlaceHolder->removeAllChildren();
        runAnimations->setPosition(topLevelPlaceHolder->getAnchor());
        topLevelPlaceHolder->addChild(runAnimations);
        attackAnimations->setPosition(topLevelPlaceHolder->getAnchor());
        topLevelPlaceHolder->addChild(attackAnimations);
        _healthBar->setPosition(0, runAnimations->getHeight()/2);
        topLevelPlaceHolder->addChild(_healthBar);
    }
    void setWalkingSceneNode(std::shared_ptr<AnimationSceneNode> walkingNode){
        runAnimations = walkingNode;
    }
    
    void setAttackingSceneNode(std::shared_ptr<AnimationSceneNode> attackingNode){
        attackAnimations = attackingNode;
    }
    
    std::shared_ptr<cugl::scene2::SceneNode> getTopLevelNode(){
        return topLevelPlaceHolder;
    }
    
    void setCurAction(EnemyActions act){curAction = act;}
    
    
protected:
    std::shared_ptr<AudioController> _audioController;
    int _maxHealth;
    int _health;
    int targetIndex;
    int updateRate;
    Vec2 trueGoal = Vec2(-1, -1);
    
    // timers
    int _counter;
    float _knockbackTimer;
    float _damagedTimer;
    int _time;
    int _escapeBoundTimer;
    
    // used for wander
    float _wanderAngle;
    const float wanderStrength = 0.3f;
    const float directionChangeInterval = 5.0f;
    float timeSinceLastMajorChange;
    // update animations; remove jitter
    bool _inContact;
    Vec2 movementDirection;
    /** The next step along the enemy's path */
    Vec2 _nextStep = Vec2(-1, -1);
    
    EnemyActions curAction;
    AnimationSceneNode::Directions _prevDirection;
    AnimationSceneNode::Directions _curDirection;
    std::shared_ptr<cugl::scene2::SceneNode> topLevelPlaceHolder;
    std::shared_ptr<AnimationSceneNode> runAnimations;
    std::shared_ptr<AnimationSceneNode> attackAnimations;
    std::shared_ptr<AStarSearch<WorldSearchVertex>> _pathfinder;
    
    /** The health  bar */
    std::shared_ptr<cugl::scene2::ProgressBar>  _healthBar;
    
    /** Timers */
    
    /** Sets a new goal for this enemy to go to. This will rerun pathfinding every time it is called. USING THIS IS NOT RECOMMENDED, USE setGoal() INSTEADl */
    bool rawSetGoal(Vec2 goal, const World* world){

        // Garbage collect the nodes used for the previous path if they exist
        if(_pathfinder->GetSolutionEnd() && _pathfinder->SearchStep() == AStarSearch<WorldSearchVertex>::SEARCH_STATE_SUCCEEDED){
    //            CULog("Garbage Collecting Path...");
            _pathfinder->FreeSolutionNodes();
        }
        
        const cugl::Vec2& my_pos = getPosition();
        // Initialize start and end for the search Force truncation
        WorldSearchVertex start = WorldSearchVertex(static_cast<int>(my_pos.x + 0.5), static_cast<int>(my_pos.y + 0.5), world);
        WorldSearchVertex end = WorldSearchVertex(static_cast<int>(goal.x + 0.5), static_cast<int>(goal.y + 0.5), world);
        _pathfinder->SetStartAndGoalStates(start, end);
        
        // Perform the search
        unsigned int SearchState = _pathfinder->SearchStep();
        while( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_SEARCHING )
        {
            SearchState = _pathfinder->SearchStep();
        }
        
        // Check if the search was successful
        if( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_SUCCEEDED ){
            
    //            CULog("Found Solution from (%d, %d) to (%d, %d)", start.x, start.y, end.x, end.y);
            
            _pathfinder->GetSolutionStart();
            WorldSearchVertex* nextNode = _pathfinder->GetSolutionNext();
            
            if(nextNode){
                _nextStep = Vec2(nextNode->x, nextNode->y);
            } else{
                _nextStep = my_pos;
            }
            
            return true;
        }
        
        if( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_FAILED ){
            CULog("Search Failed");
        }
        
        if( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_OUT_OF_MEMORY ){
            CULog("Search Out of Memory");
        }
        
        return false;
    }
    
    /** Sets a new goal for this enemy to go to. Returns true if pathfinding to the goal was successful */
    bool setGoal(Vec2& goal, const World* world){
//        WorldSearchVertex* prevGoalVertex =_pathfinder->GetSolutionEnd();
        trueGoal = goal;

        
        // If the newly set goal is very close to the old goal, just keep the old goal
//        if(prevGoalVertex && trueGoal.x >= 0){
//            Vec2 prevGoal = Vec2(prevGoalVertex->x, prevGoalVertex->y);
//            if(prevGoal.distance(trueGoal) < SAME_GOAL_DISTANCE){
//                CULog("Goal too close, don't pathfind again");
//                return true;
//            }
//        }
//
//        CULogError("GOAL CHANGED! REDOING PATHFINDING");
        
        return rawSetGoal(goal, world);
    };

    void goToGoal(){
        if(atGoal()){
            return;
        }
        if(atTile(_nextStep)){
            WorldSearchVertex* nextNode = _pathfinder->GetSolutionNext();
            _nextStep.x = nextNode->x;
            _nextStep.y = nextNode->y;
        }
        Vec2& nextTile = _nextStep;
        cugl::Vec2 direction = nextTile - getPosition();
        setVX(direction.normalize().x * 1.5);
        setVY(direction.normalize().y * 1.5);
        setX(getX());
        setY(getY());
        _prevDirection =_curDirection;
        _curDirection = AnimationSceneNode::convertRadiansToDirections(direction.getAngle());
    }
    bool atGoal(){
        // Get the goal
        return atTile(trueGoal);
    };

    bool atTile(Vec2& tile){
        return getPosition().distanceSquared(tile) < 0.03*0.03;
    };
    
    /** Returns whether the last pathfind was successful */
    bool searchSuccess(){
        return _pathfinder->GetSolutionEnd() && _pathfinder->SearchStep() == AStarSearch<WorldSearchVertex>::SEARCH_STATE_SUCCEEDED;
    }
    
    bool canResetAction() const{
        bool attack = curAction == EnemyActions::ATTACK && attackAnimations->getFrame() == attackAnimations->getSpan() - 1;
        bool nonAttack = curAction != EnemyActions::ATTACK && runAnimations->getFrame() == runAnimations->getSpan() - 1;
        return nonAttack || attack;
    }
    // update state
    virtual void handleChase(OverWorld& overWorld) = 0;
    virtual void handleLowHealth(OverWorld& overWorld) = 0;
    virtual void handleAttack(OverWorld& overWorld) = 0;
    virtual void handleStay(OverWorld& overWorld) = 0;
    
    virtual void handleRunaway(OverWorld& overWorld){
        Vec2 dogPos = overWorld.getDog()->getPosition();
        Vec2 clientDogPos = Vec2(-10000, -1000);
        cugl::Vec2 myPos = getPosition();
        
        // Get the position of the client dog if there are two players
        if(overWorld.getNetwork()->getNumPlayers() == 2){
            clientDogPos = overWorld.getClientDog()->getPosition();
        };
        
        // Flee from the dog that is closer
        if(getPosition().distance(clientDogPos) < getPosition().distance(dogPos)){
            dogPos = clientDogPos;
        };
        
        
        cugl::Vec2 direction = myPos - dogPos;
        direction.normalize();
        float distance = myPos.distance(dogPos);
        
        
        if (distance < 10) {
           setVX(direction.x * 1.5f);
           setVY(direction.y * 1.5f);
           movementDirection.x = direction.x ;
           movementDirection.y = direction.y ;
        }
        else{
           movementDirection.x = 0;
           movementDirection.y = 0 ;
        }
    };
    
    virtual void handleSpawn() {
        setHealth(_maxHealth);
        _wanderAngle = 0.0f;
        timeSinceLastMajorChange = 0.0f;
        curAction = EnemyActions::CHASE;
        
    }
    
    Vec2 randomTile(OverWorld& overWorld){
        // Create a random device
        std::random_device rd;

        // Seed the generator
        std::mt19937 gen(rd());

        // Define the range for the x position
        std::uniform_int_distribution<> xdistr(0, overWorld.getWorld()->getCols());

        // Define the range for the y position
        std::uniform_int_distribution<> ydistr(0, overWorld.getWorld()->getRows());

        // Generate and output the random integers from different ranges
        Vec2 randomTile = Vec2(xdistr(gen), ydistr(gen));
        
        // If the tile generated is not passible, regenerate the tile
        while(overWorld.getWorld()->isPassable(randomTile.x, randomTile.y)){
            randomTile = Vec2(xdistr(gen), ydistr(gen));
        }

        return randomTile;
    }
    
    bool atDeadEnd(Vec2 direction, OverWorld& overWorld){
        direction.normalize();
        float newX = getPosition().x + std::round(direction.x);
        float newY = getPosition().y + std::round(direction.y);
        
        return !overWorld.getWorld()->isPassable(newX, newY);
    }
    
    void handleWander(float dt){
        // Update time since last major direction change
         timeSinceLastMajorChange += dt;

         // Random device and generator setup
         std::random_device rd;
         std::mt19937 gen(rd());
         std::uniform_real_distribution<> minorChange(-wanderStrength, wanderStrength);
         std::uniform_real_distribution<> majorChange(0.0f, 360.0f);

         // Apply a minor random adjustment regularly
         _wanderAngle += minorChange(gen);

         // Check to change directions
         if (timeSinceLastMajorChange >= directionChangeInterval) {
             timeSinceLastMajorChange = 0.0f; // Reset the timer
         }
        if(timeSinceLastMajorChange == 0.0f){
            _wanderAngle = majorChange(gen); // new random angle
        }
         // Normalize the angle
         _wanderAngle = std::fmod(_wanderAngle, 360.0f);
         if (_wanderAngle < 0) _wanderAngle += 360.0f;

         // Calculate new velocity based on the wander angle
         float radAngle = _wanderAngle * (M_PI / 180.0f);
         float vx = cos(radAngle);
         float vy = sin(radAngle);

         // Set the new velocity, scaled by a speed factor
         float speed = 0.5f;
         setVX(vx * speed);
         setVY(vy * speed);
        
        movementDirection.x = vx;
        movementDirection.y = vy;
    }

};
#endif /* AbstractEnemy_h */



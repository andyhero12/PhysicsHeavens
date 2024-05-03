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
#define STRAY_DISTANCE 2
#define SAME_GOAL_DISTANCE 1

#define DAMAGED_DURATION 0.5f

#define KNOCKBACK_FORCE 45
#define LINEAR_DAMPING 15.0f
#define KNOCKBACK_TIME 0.25f
#define KNOCKBACK_LIMIT 14

#define MAX_ESCAPE_TIME 25

#include "stlastar.h"
#include "WorldSearchVertex.h"

class AbstractEnemy : public cugl::physics2::BoxObstacle {
    
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
        bool result = physics2::BoxObstacle::init(m_pos,m_size);
        
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

    void setVX(float value) override {
        if(!(_knockbackTimer > 0)) {
            BoxObstacle::setVX(value);
        }
    }

    void setVY(float value) override {
        if(!(_knockbackTimer > 0)) {
            BoxObstacle::setVY(value);
        }
    }

    void setLinearVelocity(Vec2 value) override {
        setLinearVelocity(value.x, value.y);
    }


    void setLinearVelocity(float x, float y) override {
        if(!(_knockbackTimer > 0)) {
            BoxObstacle::setLinearVelocity(x, y);
        }
    }

    void update(float delta) override {
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
        _health = m_health;
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
        
        cugl::Vec2 my_pos = getPosition();
        
        // Initialize start and end for the search
        WorldSearchVertex start = WorldSearchVertex(static_cast<int>(my_pos.x), static_cast<int>(my_pos.y), world);
        WorldSearchVertex end = WorldSearchVertex(static_cast<int>(goal.x), static_cast<int>(goal.y), world);
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
                _nextStep = Vec2((int) nextNode->x, (int) nextNode->y);
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
    bool setGoal(Vec2 goal, const World* world){
//        if (world.expired()){
//            CULog("World Expired");
//            return false;
//        }
        
        WorldSearchVertex* prevGoalVertex =_pathfinder->GetSolutionEnd();
        trueGoal = goal;

        
        // If the newly set goal is very close to the old goal, just keep the old goal
        if(prevGoalVertex && trueGoal.x >= 0){
            Vec2 prevGoal = Vec2(prevGoalVertex->x + 0.5, prevGoalVertex->y + 0.5);
            if(prevGoal.distance(trueGoal) < SAME_GOAL_DISTANCE){
                CULog("Goal too close, don't pathfind again");
                return true;
            }
        }
        
        return rawSetGoal(goal, world);
    };


    void goToGoal(){
        
        // Get the goal
        WorldSearchVertex* goalNode = _pathfinder->GetSolutionEnd();
        cugl::Vec2 direction;
        
        // If there is no goal or we are already at the goal, do nothing
        if(atGoal() || _nextStep.x < 0){
            if(atGoal()){
                CULog("At goal already, do notihing");
            } else {
                CULog("No goal instantiated, do nothing");
            }
            return;
        }
        
        Vec2 goalTile = Vec2(trueGoal.x + 0.5, trueGoal.y + 0.5);
        
        // If we are very close to the goal, go directly to it instead of using pathfinding
        if(getPosition().distance(goalTile) <= CLOSE_DISTANCE){
            CULog("Close enough, going directly to the goal");
            direction = goalTile - getPosition();
        }
        else {
            
            // If we strayed too far from the pathfinding path, restart pathfinding
            Vec2 nextTile = Vec2(_nextStep.x + 0.5, _nextStep.y + 0.5);
            if(getPosition().distance(nextTile) > STRAY_DISTANCE){
                CULog("Recalculating Path...");
                rawSetGoal(Vec2(trueGoal.x, trueGoal.y), goalNode->_world);
            }
            
            // If we already reached the next tile, get the next node along the path and set it as the next tile
            if(atTile(_nextStep)){
                CULog("Reached Tile (%f, %f)", _nextStep.x, _nextStep.y);
                WorldSearchVertex* nextNode = _pathfinder->GetSolutionNext();
                
                if(nextNode){
                    _nextStep = Vec2((int) nextNode->x, (int) nextNode->y);
                } else{
                    rawSetGoal(trueGoal, goalNode->_world);
                    CULog("Can't find next tile, recalculating");
                }
                
                nextTile = Vec2(_nextStep.x + 0.5, _nextStep.y + 0.5);
            }
            
            direction = nextTile - getPosition();
        }
        
        //Move towards the next tile
        setVX(direction.normalize().x * 0.75);
        setVY(direction.normalize().y * 0.75);
        setX(getX());
        setY(getY());
        _prevDirection =_curDirection;
        _curDirection = AnimationSceneNode::convertRadiansToDirections(direction.getAngle());
        
        
        
        
        
        // If we have been stuck on the same tile for too long, move randomly and then restart pathfinding
        
    };


    bool atGoal(){
        // Get the goal
        return trueGoal.x < 0 || atTile(trueGoal);
    };

    bool atTile(Vec2 tile){
        
        // Get the center of the tile
        Vec2 tileCenter = Vec2(tile.x + 0.5, tile.y + 0.5);
        
        // Check if the enemy position is close to the center
        if(getPosition().distance(tileCenter) < 0.05 ){
            return true;
        };
        
        return false;
    };
    
    /** Returns whether the last pathfind was successful */
    bool searchSuccess(){
        return _pathfinder->GetSolutionEnd() && _pathfinder->SearchStep() == AStarSearch<WorldSearchVertex>::SEARCH_STATE_SUCCEEDED;
    }
    
    // update state
    virtual void handleChase(OverWorld& overWorld) = 0;
    virtual void handleLowHealth(OverWorld& overWorld) = 0;
    virtual void handleAttack(OverWorld& overWorld) = 0;
    virtual void handleStay(OverWorld& overWorld) = 0;
    virtual void handleRunaway(OverWorld& overWorld)= 0;
    
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

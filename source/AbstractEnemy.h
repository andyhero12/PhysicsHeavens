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


#define CLOSE_DISTANCE 2
#define STRAY_DISTANCE 3

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
    virtual ~AbstractEnemy() {}
    
    bool init(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex);
    void update(float delta) override;
    
    virtual void preUpdate(float dt, OverWorld& overWorld) = 0;
    void postUpdate();
    
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
        _health = m_health;
        _healthBar->setProgress((float)_health/_maxHealth);
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
    int _counter;
    int _time;
    // used for wander
    float _wanderAngle;
    const float wanderStrength = 0.3f;
    const float directionChangeInterval = 5.0f;
    float timeSinceLastMajorChange;
    // update animations; remove jitter
    bool _inContact;
    
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
    int _pathfindTimer = PATHFIND_COOLDOWN;
    
    /** Sets a new goal for this enemy to go to. Returns true if pathfinding to the goal was successful */
    bool setGoal(Vec2 goal, std::shared_ptr<World> world);
    
    /** Goes towards the goal. If there is no goal, do nothing. */
    void goToGoal();
    
    /** Returns whether the enemy is at its goal */
    bool atGoal();
    
    /** Returns whether the enemy is on the given tile , i. e. close  to its center. Tiles should have integer coordinates*/
    bool atTile(Vec2 tile);
    
    /** Returns whether the last pathfind was successful */
    bool searchSuccess(){
        return _pathfinder->GetSolutionEnd() && _pathfinder->SearchStep() == AStarSearch<WorldSearchVertex>::SEARCH_STATE_SUCCEEDED;
    }
    
    // update state
    virtual void handleChase(OverWorld& overWorld) = 0;
    virtual void handleLowHealth() = 0;
    virtual void handleAttack(OverWorld& overWorld) = 0;
    virtual void handleStay() = 0;
    virtual void handleRunaway(OverWorld& overWorld)= 0;
    
    virtual void handleSpawn();
    void handleWander(float dt);
};
#endif /* AbstractEnemy_h */

//
//  AbstractEnemy.h
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#ifndef AbstractEnemy_h
#define AbstractEnemy_h
#include <cugl/cugl.h>
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
class AbstractEnemy : public cugl::physics2::BoxObstacle {
public:
    
    enum class EnemyActions : int {
        RUN,
        ATTACK
    };
    AbstractEnemy(){}
    // Virtual destructor
    virtual ~AbstractEnemy() {}
    
    bool init(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex){
        bool result = physics2::BoxObstacle::init(m_pos,m_size);
        if (result){
            _counter = 0;
            updateRate = 10;
            setDensity(DEFAULT_DENSITY);
            setFriction(DEFAULT_FRICTION);
            setRestitution(DEFAULT_RESTITUTION);
            setFixedRotation(true);
            
            curAction = EnemyActions::RUN;
            _health = m_health;
            _maxHealth = m_health;
            targetIndex = m_targetIndex;
            _prevDirection =AnimationSceneNode::Directions::EAST;
            _curDirection = AnimationSceneNode::Directions::EAST;
            return true;
        }
        return false;
    }
    
    void update(float delta) override{
        Obstacle::update(delta);
        runAnimations->animate(_curDirection, curAction == EnemyActions::RUN);
        attackAnimations->animate(_curDirection, curAction == EnemyActions::ATTACK);
    }
    virtual void preUpdate(float dt, OverWorld& overWorld) = 0;
    
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
        
//        _healthBar->setPosition(getPosition() + Vec2(-5,16));
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
protected:
    int _maxHealth;
    int _health;
    int targetIndex;
    int updateRate;
    int _counter;
    EnemyActions curAction;
    AnimationSceneNode::Directions _prevDirection;
    AnimationSceneNode::Directions _curDirection;
    std::shared_ptr<cugl::scene2::SceneNode> topLevelPlaceHolder;
    std::shared_ptr<AnimationSceneNode> runAnimations;
    std::shared_ptr<AnimationSceneNode> attackAnimations;
    /** The health  bar */
    std::shared_ptr<cugl::scene2::ProgressBar>  _healthBar;
};
#endif /* AbstractEnemy_h */

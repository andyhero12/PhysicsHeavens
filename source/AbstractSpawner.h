//
//  AbstractSpawner.h
//  Heavan
//
//  Created by Andrew Cheng on 3/24/24.
//

#ifndef AbstractSpawner_h
#define AbstractSpawner_h
#include <cugl/cugl.h>
#include "MonsterController.h"
#include "OverWorld.h"

class AbstractSpawner{
protected:
    std::shared_ptr<cugl::scene2::SceneNode> sceneNode;
    std::shared_ptr<cugl::scene2::ProgressBar>  _healthBar;
    float _regularDelay;
    float _accumulatedDelay;
    int _health;
    int _maxHealth;
    float _delay;
    float _timeElapsed;
    cugl::Vec2 _position;
    
public:

    std::shared_ptr<cugl::scene2::SceneNode> getSpawnerNode(){
        return sceneNode;
    }
    void setSpawnerNode(std::shared_ptr<cugl::scene2::SceneNode>  inc){
        sceneNode = inc;
    }
    void setHealthBar(std::shared_ptr<cugl::scene2::ProgressBar> bar){
        _healthBar = bar;
        _healthBar->setScale(0.1);
        _healthBar->setAnchor(Vec2::ANCHOR_CENTER);
    }
    AbstractSpawner(float regularDelay, cugl::Vec2 pos, int health, float delay)
    : _regularDelay(regularDelay)
    , _position(pos)
    , _health(health)
    , _maxHealth(health)
    , _timeElapsed(0.0)
    , _accumulatedDelay(-delay + regularDelay)
    , _delay{delay}
    {
    }
    
    float getTimeElapsed() const{
        return _timeElapsed;
    }

    void updateTime(float dt){
        _accumulatedDelay += dt;
        _timeElapsed += dt;
    }

    bool canSpawn() const {
        return (_accumulatedDelay > _regularDelay);
    }
    void reloadSpawner() {
        _accumulatedDelay = 0;
    }
    const float getAccumulatedDelay() const {
        return _accumulatedDelay;
    }
    cugl::Vec2 getPos() const {
        return _position; }
    void subHealth(const int val) {
        _health -= val;
        cout << _health;
        _healthBar->setProgress((float)_health/_maxHealth);
    }
    bool dead(){
        return _health <= 0;
    }
    virtual void update(MonsterController& monsterController, OverWorld& overWorld, float timestep, float difficulty) = 0;
    
};
#endif /* AbstractSpawner_h */

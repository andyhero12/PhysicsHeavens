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
    std::shared_ptr<cugl::scene2::PolygonNode> polyNode;
    int _spawnRate;
    int _respawnCnt;
    int _health;
    int _delay;
    double _timeElapsed;
    cugl::Vec2 _position;
    
public:
    
    std::shared_ptr<cugl::scene2::SceneNode> getSpawnerNode(){
        return polyNode;
    }
    AbstractSpawner(int rate, cugl::Vec2 pos, int health, int delay)
    : _spawnRate(rate)
    , _position(pos)
    , _health(health)
    , _timeElapsed(0.0)
    , _delay{delay}
    {
    }
    
    double getTimeElapsed() const{
        return _timeElapsed;
    }
    bool canSpawn() const {
        return (_respawnCnt > _spawnRate);
    }
    void reloadSpawner() {
        _respawnCnt = 0;
    }
    void setSceneNode(std::shared_ptr<cugl::Texture> _texture){
        polyNode = cugl::scene2::PolygonNode::allocWithTexture(_texture);
    }
    const int getCnt() const {
        return _respawnCnt;
    }
    cugl::Vec2 getPos() const {
        return _position; }
    void subHealth(const int val);
    bool dead(){
        return _health <= 0;
    }
    virtual void update(MonsterController& monsterController, OverWorld& overWorld, float timestep) = 0;
    
};
#endif /* AbstractSpawner_h */

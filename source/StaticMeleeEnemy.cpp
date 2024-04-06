//
//  StaticMeleeEnemy.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/24/24.
//

#include "StaticMeleeEnemy.h"
#define DISTANCE_CUTOFF 5

#define DYNAMIC_COLOR   Color4::YELLOW
/**
 * Generate a pair of Obstacle and SceneNode using the given parameters
 */
std::pair<std::shared_ptr<physics2::Obstacle>, std::shared_ptr<scene2::SceneNode>> StaticMeleeFactory::createObstacle(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex) {
    std::vector<std::shared_ptr<cugl::Texture>>& _textures = staticEnemyStruct._walkTextures;
    int _framesize = staticEnemyStruct._framesize;
    int _framecols = staticEnemyStruct._framecols;
    int _freqAnims = staticEnemyStruct._freqAnimations;
    int rows = _framesize / _framecols;
    if (_framesize % _framecols != 0)
    {
        rows++;
    }
    std::shared_ptr<StaticMeleeEnemy> static_enemy = StaticMeleeEnemy::alloc(m_pos, m_size, m_health, m_targetIndex);
    std::shared_ptr<cugl::scene2::SceneNode> topLevel = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
    topLevel->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<AnimationSceneNode> runAnimations = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
    // Temp PlaceHolder
    runAnimations->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<AnimationSceneNode> attackAnimations = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
    attackAnimations->setAnchor(Vec2::ANCHOR_CENTER);
    topLevel->setPosition(m_pos * 44); // temp scale
    static_enemy->setWalkingSceneNode(runAnimations);
    static_enemy->setAttackingSceneNode(attackAnimations);
    static_enemy->setFinalEnemy(topLevel);
    static_enemy->setShared(true);
    
//        static_enemy->setHealthBar(_healthBar);
    return std::make_pair(static_enemy, topLevel);
#pragma mark END SOLUTION
}

/**
 * Helper method for converting normal parameters into byte vectors used for syncing.
 */
std::shared_ptr<std::vector<std::byte>> StaticMeleeFactory::serializeParams(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex) {
    // TODO: Use _serializer to serialize pos and scale (remember to make a shared copy of the serializer reference, otherwise it will be lost if the serializer is reset).
#pragma mark BEGIN SOLUTION
    _serializer.reset();
    _serializer.writeFloat(m_pos.x);
    _serializer.writeFloat(m_pos.y);
    _serializer.writeFloat(m_size.width);
    _serializer.writeFloat(m_size.height);
    _serializer.writeSint32(m_health);
    _serializer.writeSint32(m_targetIndex);
    return std::make_shared<std::vector<std::byte>>(_serializer.serialize());
#pragma mark END SOLUTION
}

/**
 * Generate a pair of Obstacle and SceneNode using serialized parameters.
 */
std::pair<std::shared_ptr<physics2::Obstacle>, std::shared_ptr<scene2::SceneNode>> StaticMeleeFactory::createObstacle(const std::vector<std::byte>& params) {
    // TODO: Use _deserializer to deserialize byte vectors packed by {@link serializeParams()} and call the regular createObstacle() method with them.
#pragma mark BEGIN SOLUTION
    _deserializer.reset();
    _deserializer.receive(params);
    float x = _deserializer.readFloat();
    float y = _deserializer.readFloat();
    Vec2 pos = Vec2(x,y);
    float w =_deserializer.readFloat();
    float h =_deserializer.readFloat();
    Size size = Size(w,h);
    int health = _deserializer.readSint32();
    int targetIndex = _deserializer.readSint32();
    return createObstacle(pos, size, health, targetIndex);
#pragma mark END SOLUTION
}

StaticMeleeEnemy::StaticMeleeEnemy(){}
bool StaticMeleeEnemy::init(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex){
    if (MeleeEnemy::init(m_pos, m_size, m_health, m_targetIndex)){
        std::string name("Static Melee Enemy");
        setName(name);
        original_pos = m_pos;
        return true;
    }
    return false;
}
void StaticMeleeEnemy::preUpdate(float dt, OverWorld& overWorld){
    if (_attackCooldown < 60){
        _attackCooldown++;
    }
    cugl::Vec2 dog_pos = overWorld.getDog()->getPosition();
    
    
    cugl::Vec2 org_dist = dog_pos - original_pos;
    float distance = org_dist.length();
    cugl::Vec2 direction;
//    CULog("distance %f %d", distance, DISTANCE_CUTOFF);
    if (distance > DISTANCE_CUTOFF){ // too far from origin return
        direction = original_pos - getPosition();
        if (overWorld._isHost){
            if (direction.lengthSquared() >= 1){
                setVX(direction.normalize().x);
                setVY(direction.normalize().y);
            }else{
                setVX(0);
                setVY(0);
            }
        }
    }else{ // chase dog
//        CULog("Chasing Dog");
        direction = dog_pos - getPosition();
        if (overWorld._isHost){
            setVX(direction.normalize().x);
            setVY(direction.normalize().y);
        }
    }
    // Animate
    _prevDirection =_curDirection;
    _curDirection = AnimationSceneNode::convertRadiansToDirections(direction.getAngle());
    runAnimations->animate(_curDirection, curAction == EnemyActions::RUN);
    attackAnimations->animate(_curDirection, curAction == EnemyActions::ATTACK);
}

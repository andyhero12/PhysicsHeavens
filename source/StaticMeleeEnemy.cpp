//
//  StaticMeleeEnemy.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/24/24.
//

#include "StaticMeleeEnemy.h"
#define DISTANCE_CUTOFF 4
#define SPRITE_SCALE 1.14f

#define DYNAMIC_COLOR   Color4::YELLOW
/**
 * Generate a pair of Obstacle and SceneNode using the given parameters
 */
std::pair<std::shared_ptr<physics2::Obstacle>, std::shared_ptr<scene2::SceneNode>> StaticMeleeFactory::createObstacle(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex) {
    std::vector<std::shared_ptr<cugl::Texture>>& _textures = staticEnemyStruct._walkTextures;
    if (_textures.size() == 0){
        CULog("EMPTY TEXTURES");
    }
    int _framesize = staticEnemyStruct._framesize;
    int _framecols = staticEnemyStruct._framecols;
    int _freqAnims = staticEnemyStruct._freqAnimations;
    int rows = _framesize / _framecols;
    if (_framesize % _framecols != 0)
    {
        rows++;
    }
    std::shared_ptr<StaticMeleeEnemy> static_enemy = StaticMeleeEnemy::alloc(m_pos, m_size, m_health, m_targetIndex);
    
    // used to create progress bars
    std::shared_ptr<cugl::Texture> barImage = _assets->get<Texture>("progress");
    
    float textureWidth = barImage->getWidth();
    float textureHeight = barImage->getHeight();
    
    std::shared_ptr<cugl::Texture> bg = barImage->getSubTexture(0/textureWidth, 320/textureWidth, 0/textureHeight, 45/textureHeight);
    std::shared_ptr<cugl::Texture> fg = barImage->getSubTexture(24/textureWidth, 296/textureWidth, 45/textureHeight, 90/textureHeight);
    std::shared_ptr<cugl::Texture> left_cap = barImage->getSubTexture(0/textureWidth, 24/textureWidth, 45/textureHeight, 90/textureHeight);
    std::shared_ptr<cugl::Texture> right_cap = barImage->getSubTexture(296/textureWidth, 320/textureWidth, 45/textureHeight, 90/textureHeight);
    
    std::shared_ptr<cugl::scene2::ProgressBar> _bar = cugl::scene2::ProgressBar::allocWithCaps(bg, fg, left_cap, right_cap);
    _bar->setProgress(1.0f);
    static_enemy->setHealthBar(_bar);
    static_enemy->setDebugColor(DYNAMIC_COLOR);
    static_enemy->setAngleSnap(0); // Snap to the nearest degree
    
    std::shared_ptr<cugl::scene2::SceneNode> topLevel = scene2::SceneNode::allocWithBounds(m_size);
    topLevel->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<AnimationSceneNode> runAnimations = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
    runAnimations->setContentSize(m_size);
    // Temp PlaceHolder
    runAnimations->setAnchor(Vec2::ANCHOR_CENTER);
    int _framesizeAttack = staticEnemyStruct._framesizeAttack;
    int _framecolsAttack = staticEnemyStruct._framecolsAttack;
    if (_framesizeAttack % _framecolsAttack != 0)
    {
        rows++;
    }
    std::vector<std::shared_ptr<cugl::Texture>>& _attacks = staticEnemyStruct._attackTextures;
    std::shared_ptr<AnimationSceneNode> attackAnimations = AnimationSceneNode::allocWithTextures(_attacks, rows,_framecolsAttack, _framesizeAttack, _freqAnims);
    attackAnimations->setAnchor(Vec2::ANCHOR_CENTER);
    attackAnimations->setContentSize(m_size);
    topLevel->setPosition(m_pos);
    static_enemy->setWalkingSceneNode(runAnimations);
    static_enemy->setAttackingSceneNode(attackAnimations);
    static_enemy->setFinalEnemy(topLevel);
    topLevel->setScale(m_size.height / _textures.at(0)->getHeight() * SPRITE_SCALE);
    static_enemy->setShared(true);
    static_enemy->setAudioController(_audioController);
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
    
    if (_counter < updateRate){
        _counter++;
    }
    // Determine the action based on the state; for now it's alway in atttack but should change
    cugl::Vec2 dog_pos = overWorld.getDog()->getPosition();
    cugl::Vec2 org_dist = dog_pos - original_pos;
    float distance = org_dist.length();
    if (distance > DISTANCE_CUTOFF){
        curAction = AbstractEnemy::EnemyActions::RUNAWAY;
        handleRunaway(overWorld);
    }
    else{
        curAction = AbstractEnemy::EnemyActions::ATTACK;
        handleAttack(overWorld);
    }
}


void StaticMeleeEnemy::handleChase(OverWorld& overWorld){}
void StaticMeleeEnemy::handleLowHealth(OverWorld& overWorld){}
void StaticMeleeEnemy::handleAttack(OverWorld& overWorld){
    cugl::Vec2 dog_pos = overWorld.getDog()->getPosition();
    cugl::Vec2 direction;
    direction = dog_pos - getPosition();
    if (overWorld._isHost && _counter >= updateRate){
        _counter = 0;
        _prevDirection =_curDirection;
        _curDirection = AnimationSceneNode::convertRadiansToDirections(direction.getAngle());
        movementDirection = direction;
    }
    setVX(direction.normalize().x * 1.5);
    setVY(direction.normalize().y * 1.5);
    setX(getX());
    setY(getY());
}
void StaticMeleeEnemy::handleStay(OverWorld& overWorld){}
void StaticMeleeEnemy::handleRunaway(OverWorld& overWorld){
    cugl::Vec2 org_dist = original_pos - getPosition();
    if (overWorld._isHost && _counter >= updateRate){
        _counter = 0;
        _prevDirection =_curDirection;
        _curDirection = AnimationSceneNode::convertRadiansToDirections(org_dist.getAngle());
        movementDirection = org_dist;
    }
    if (org_dist.lengthSquared() >= 1){
        setVX(org_dist.normalize().x * 1.5);
        setVY(org_dist.normalize().y * 1.5);
    }else{
        setVX(0);
        setVY(0);
    }
    setX(getX());
    setY(getY());
}

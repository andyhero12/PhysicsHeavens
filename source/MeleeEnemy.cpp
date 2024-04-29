//
//  MeleeEnemy.cpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#define SPRITE_SCALE 1.14f

#define MELEE_DAMAGE 5
#define FRAMES 200

#include "MeleeEnemy.h"
#define DYNAMIC_COLOR   Color4::YELLOW
std::pair<std::shared_ptr<physics2::Obstacle>, std::shared_ptr<scene2::SceneNode>> MeleeFactory::createObstacle(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex) {
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
    std::shared_ptr<MeleeEnemy> static_enemy = MeleeEnemy::alloc(m_pos, m_size, m_health, m_targetIndex);
    
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
    std::shared_ptr<AnimationSceneNode> attackAnimations = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
    attackAnimations->setAnchor(Vec2::ANCHOR_CENTER);
    attackAnimations->setContentSize(m_size);
    
    topLevel->setPosition(m_pos);
    static_enemy->setWalkingSceneNode(runAnimations);
    static_enemy->setAttackingSceneNode(attackAnimations);
    static_enemy->setFinalEnemy(topLevel);
    topLevel->setScale(m_size.height / _textures.at(0)->getHeight() * SPRITE_SCALE);
    static_enemy->setShared(true);

    return std::make_pair(static_enemy, topLevel);
#pragma mark END SOLUTION
}

/**
 * Helper method for converting normal parameters into byte vectors used for syncing.
 */
std::shared_ptr<std::vector<std::byte>> MeleeFactory::serializeParams(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex) {
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
std::pair<std::shared_ptr<physics2::Obstacle>, std::shared_ptr<scene2::SceneNode>> MeleeFactory::createObstacle(const std::vector<std::byte>& params) {
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

MeleeEnemy::MeleeEnemy(){}
bool MeleeEnemy::init(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex){
    if (AbstractEnemy::init(m_pos, m_size, m_health, m_targetIndex)){
        std::string name("Melee Enemy");
        setName(name);
        _contactDamage = MELEE_DAMAGE;
        _attackCooldown = 0;
        return true;
    }
    return false;
}
void MeleeEnemy::preUpdate(float dt, OverWorld& overWorld){
    // Update the counter for timed actions
    if (_attackCooldown < 60){
        _attackCooldown++;
    }
    
    _time += 1;
    
    if (_counter < updateRate){
        _counter++;
    }
    
    // if (!(overWorld._isHost && _counter >= updateRate)){
    //     return;
    // }
    
    if(_health < _maxHealth/3){
        curAction = AbstractEnemy::EnemyActions::LOWHEALTH;
    }
    
    // Determine the action based on the state
    runAnimations->setColor(cugl::Color4::WHITE);
    if (curAction == EnemyActions::SPAWN){
        handleSpawn();
    }
    else if (curAction == EnemyActions::WANDER){
        handleWander(dt);
        if(_time >= FRAMES){
            curAction = AbstractEnemy::EnemyActions::CHASE;
            _time = 0;
        }
    }
    else if(curAction == EnemyActions::CHASE){
        handleChase(overWorld);
        if(_time >= FRAMES) {
            curAction = AbstractEnemy::EnemyActions::WANDER;
            _time = 0;
        }
    }
    else if(curAction == EnemyActions::LOWHEALTH){
        handleLowHealth(overWorld);
    }
    else if(curAction == EnemyActions::STAY){
        handleStay(overWorld);
        if(_time >= FRAMES){
            curAction = AbstractEnemy::EnemyActions::WANDER;
            _time = 0;
        }
    }
    else if(curAction == EnemyActions::ATTACK){
        handleAttack(overWorld);
        if(_time >= FRAMES){
            curAction = AbstractEnemy::EnemyActions::STAY;
            _time = 0;
        }
    }
    
    _counter = 0;
}


void MeleeEnemy::handleChase(OverWorld& overWorld) {
    cugl::Vec2 target_pos = getTargetPositionFromIndex(overWorld);
    
    cugl::Vec2 dist = target_pos - getPosition();
    
    setGoal(target_pos, overWorld.getWorld());
    goToGoal();
    
    
    movementDirection = dist;
    
    if( dist.length() < 4 && curAction == AbstractEnemy::EnemyActions::CHASE){
        curAction = AbstractEnemy::EnemyActions::ATTACK;
        _time = 0;
    }
}

void MeleeEnemy::handleLowHealth(OverWorld& overWorld) {
    runAnimations->setColor(cugl::Color4::BLACK);
    handleRunaway(overWorld);
}

void MeleeEnemy::handleAttack(OverWorld& overWorld) {
    attackAnimations->setColor(Color4::GREEN);
    handleChase(overWorld);
}

void MeleeEnemy::handleStay(OverWorld& overWorld) {}

void MeleeEnemy::handleRunaway(OverWorld& overWorld){
    cugl::Vec2 dogPos = overWorld.getDog()->getPosition();
       cugl::Vec2 myPos = getPosition();
       cugl::Vec2 direction = myPos - dogPos;
       float distance = direction.length();

       if (distance < 4) {
           direction.normalize();
           setVX(-direction.x * 0.1f);
           setVY(-direction.y * 0.1f);
           movementDirection.x = -direction.x ;
           movementDirection.y = -direction.y ;
       }
       else{
           movementDirection.x = 0;
           movementDirection.y = 0 ;
       }
}

//
//  BombEnemy.cpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//
#include "BombEnemy.h"

#define EXPLOSION_RADIUS 0.5
#define CONTACT_DAMAGE 4
#define EXPLOSION_DAMAGE 11

BombEnemy::BombEnemy(){}
bool BombEnemy::init(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex){
    if (AbstractEnemy::init(m_pos, m_size, m_health, m_targetIndex)){
        std::string name("Bomb Enemy");
        setName(name);
        _contactDamage = CONTACT_DAMAGE;
        _baseExplosionDamage = EXPLOSION_DAMAGE;
        return true;
    }
    return false;
}

//void BombEnemy::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size,  std::shared_ptr<cugl::Font> font){
//    Vec2 pos = getPos();
//    Affine2 trans;
//    auto sprite = getSprite();
//    trans.scale(1 / sprite->getFrameSize().height);
//    trans.translate(pos);
//    sprite->draw(batch, trans);
//    
//    // Might need to change; this was specifically for the default health bar
//    Affine2 trans_bar;
//    trans_bar.scale(0.1);
//    Vec2 pos_bar = Vec2(pos.x + 5, pos.y + 35);
//    trans_bar.translate(pos_bar);
//    
//    _healthBar->setProgress(getHealth()/(float)_maxHealth);
//    _healthBar->render(batch, trans_bar, Color4::RED);
//}

void BombEnemy::preUpdate(float dt, OverWorld& overWorld){
    if (_attackCooldown < 60){
        _attackCooldown++;
    }
    
    cugl::Vec2 target_pos = getTargetPositionFromIndex(overWorld);
    cugl::Vec2 direction = target_pos - getPosition();
    if (overWorld._isHost){
        setVX(direction.normalize().x * 0.5);
        setVY(direction.normalize().y * 0.5);
    }
    
    _prevDirection =_curDirection;
    _curDirection = AnimationSceneNode::convertRadiansToDirections(direction.getAngle());
    runAnimations->animate(_curDirection, curAction == EnemyActions::RUN);
    attackAnimations->animate(_curDirection, curAction == EnemyActions::ATTACK);
    
}
void BombEnemy::executeDeath(OverWorld& overWorld){
    
    std::shared_ptr<Dog> curDog = overWorld.getDog();
    cugl::Vec2 dog_pos = curDog->getPosition();
    
    float distance = (dog_pos - getPosition()).length();
    if (distance > EXPLOSION_RADIUS){
        return;
    }
    curDog->setHealth(curDog->getHealth() - getExplosionDamage());
    CULog("Dog got exploded on\n");
}


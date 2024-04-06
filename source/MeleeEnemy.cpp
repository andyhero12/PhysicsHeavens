//
//  MeleeEnemy.cpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#define MELEE_DAMAGE 5
#include "MeleeEnemy.h"
MeleeEnemy::MeleeEnemy(){}
bool MeleeEnemy::init(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex){
    if (AbstractEnemy::init(m_pos, m_size, m_health, m_targetIndex)){
        std::string name("Melee Enemy");
        setName(name);
        _contactDamage = MELEE_DAMAGE;
        return true;
    }
    return false;
}
void MeleeEnemy::preUpdate(float dt, OverWorld& overWorld){
    if (_attackCooldown < 60){
        _attackCooldown++;
    }
    
    cugl::Vec2 target_pos = getTargetPositionFromIndex(overWorld);
    cugl::Vec2 direction = target_pos - getPosition();
    if (overWorld._isHost){
        setVX(direction.normalize().x * 0.5);
        setVY(direction.normalize().y * 0.5);
    }
    // Animate
    _prevDirection =_curDirection;
    _curDirection = AnimationSceneNode::convertRadiansToDirections(direction.getAngle());
    runAnimations->animate(_curDirection, curAction == EnemyActions::RUN);
    attackAnimations->animate(_curDirection, curAction == EnemyActions::ATTACK);
}

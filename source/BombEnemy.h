//
//  BombEnemy.hpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#ifndef BombEnemy_hpp
#define BombEnemy_hpp

#include "AbstractEnemy.h"
class BombEnemy : public AbstractEnemy {
public:
    
    static std::shared_ptr<BombEnemy> alloc(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex) {
        std::shared_ptr<BombEnemy> result = std::make_shared<BombEnemy>();
        return (result->init(m_pos, m_size, m_health, m_targetIndex) ? result : nullptr);
    }
    BombEnemy();
    
    bool init(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex);
    virtual ~BombEnemy() {
        
    }
    
    virtual void preUpdate(float dt, OverWorld& overWorld) override;
    
    virtual int getDamage() override{
        return _contactDamage;
    }
    int getExplosionDamage(){
        return _baseExplosionDamage;
    }
    bool canAttack() const override{
        if (_attackCooldown > 60){
            std::cout <<"WTF\n";
        }
        return _attackCooldown >= 60;
    }
    
    virtual void resetAttack() override{
        _attackCooldown = 0;
    }
    virtual void executeDeath(OverWorld& overWorld) override;
    
    virtual int getAbsorbValue() const override{
        CULog("TODO ABSORB BOMB\n");
        return 1;
    }
    
    
protected:
    int _contactDamage;
    int _attackCooldown;
    int _baseExplosionDamage;
};
#endif /* BombEnemy_hpp */

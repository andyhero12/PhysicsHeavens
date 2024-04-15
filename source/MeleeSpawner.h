//
//  MeleeSpawner.hpp
//  Heavan
//
//  Created by Andrew Cheng on 3/24/24.
//

#ifndef MeleeSpawner_hpp
#define MeleeSpawner_hpp
#include <cstdlib> 

#include "AbstractSpawner.h"
class MeleeSpawner: public AbstractSpawner{

private:
    std::string spawnType1;
    std::string spawnType2;
    std::string spawnType3;
    
public:
    MeleeSpawner(int rate, cugl::Vec2 pos, int health, int delay, std::string spawnType1, std::string spawnType2, std::string spawnType3);
    virtual void update(MonsterController& monsterController, OverWorld& overWorld, float timestep) override;
};

#endif /* MeleeSpawner_hpp */
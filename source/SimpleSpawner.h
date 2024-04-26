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
class SimpleSpawner: public AbstractSpawner{

private:
    std::string spawnType1;
    std::string spawnType2;
    std::string spawnType3;
    
public:
    SimpleSpawner(int rate, cugl::Vec2 pos, int health, int delay, std::string spawnType1, std::string spawnType2, std::string spawnType3);
    virtual void update(MonsterController& monsterController, OverWorld& overWorld, float timestep, float difficulty) override;

    std::string getSpawnType1() {
        return spawnType1;
    }

    std::string getSpawnType2() {
        return spawnType2;
    }

    std::string getSpawnType3() {
        return spawnType3;
    }
};

#endif /* MeleeSpawner_hpp */
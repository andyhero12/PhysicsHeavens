//
//  MeleeSpawner.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/24/24.
//

#include "MeleeSpawner.h"




MeleeSpawner::MeleeSpawner(int rate, cugl::Vec2 pos, int health, int delay)
:AbstractSpawner(rate, pos, health, delay)
{
    
}


void MeleeSpawner::update(MonsterController& monsterController, OverWorld& overWorld, float timestep){
    if (_respawnCnt <= _spawnRate) {
        _respawnCnt++;
    }
    _timeElapsed += timestep;
    if (canSpawn()){
        reloadSpawner();
        monsterController.spawnBasicEnemy(getPos(),overWorld);
    }
}

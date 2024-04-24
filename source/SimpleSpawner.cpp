//
//  MeleeSpawner.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/24/24.
//

#include "SimpleSpawner.h"
#include <cctype> // for tolower()
#include <cstdlib>



SimpleSpawner::SimpleSpawner(int rate, cugl::Vec2 pos, int health, int delay, std::string s1, std::string s2, std::string s3)
:AbstractSpawner(rate, pos, health, delay)
{
    spawnType1 = s1;
    if (s2.empty()) {
        s2 = s1;
    }
    spawnType2 = s2;
    if (s3.empty()) {
        s3 = s2;
    }
    spawnType3 = s3;
}


void SimpleSpawner::update(MonsterController& monsterController, OverWorld& overWorld, float timestep, float difficulty){
    updateTime(timestep);
    int r = rand() % 10;
    if (canSpawn()){
        reloadSpawner();

        std::string s;
        if (r < 5) {
            s = spawnType1;
        }
        else if (r < 8) {
            s = spawnType2;
        }
        else {
            s = spawnType3;
        }
        float power = difficulty;

        float r = rand() / (float)RAND_MAX;
        r = r * r * r;
        power *= 0.8f + r;

        monsterController.spawnEnemyFromString(s, getPos(), overWorld, power);
    }
}
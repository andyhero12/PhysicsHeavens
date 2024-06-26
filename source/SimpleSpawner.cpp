//
//  MeleeSpawner.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/24/24.
//

#include "SimpleSpawner.h"
#include <cctype> // for tolower()
#include <cstdlib>
#include <cmath>


int generateRandomInclusiveHighLowNew(int low, int high)
{
    // Static used for the seed to ensure it's only seeded once
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(low, high); // Range is 1 to 3, inclusive
    return dis(gen);
}
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
    updateTime(timestep * difficulty);
    int r = rand() % 10;
    sceneNode->update();
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
        int cluster = generateRandomInclusiveHighLowNew(1, 4);
        for (int i =0 ; i< cluster ;i++){
            Vec2 pos = getPos() + Vec2(generateRandomInclusiveHighLowNew(-30, 30) / 100.0f, generateRandomInclusiveHighLowNew(-30, 30) / 100.0f);
            monsterController.spawnEnemyFromString(s, pos, overWorld, difficulty);
        }
    }

    if(_damagedTimer > 0) {
        _damagedTimer -= timestep;
    }
    if(_damagedTimer < 0) {
        _damagedTimer = 0;
        //tints may be expensive, so separating out this special case may be worthwhile
        sceneNode->setColor(cugl::Color4(255, 255, 255));
    }
    if(_damagedTimer > 0) {
        float ratio = (DAMAGED_DURATION - _damagedTimer) / DAMAGED_DURATION;
        float brightness = 255 * (0.8f + ratio * 0.2f);
        sceneNode->setColor(cugl::Color4(brightness, brightness * ratio, brightness * ratio));
    }
}

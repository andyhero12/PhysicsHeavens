//
//  GLCollisionController.h
//  Programming Lab
//
//  Unless you are making a point-and-click adventure game, every single
//  game is going to need some sort of collision detection.  In a later
//  lab, we will see how to do this with a physics engine. For now, we use
//  custom physics.
//
//  You might ask why we need this file when we have Box2d. That is because
//  we are trying to make this code as close to that of 3152 as possible. At
//  this point in the semester of 3152, we had not covered Box2d.
//
//  However, you will notice that this is NOT A CLASS.  The collision
//  controller in 3152 did not have any state to speak of (it had some cache
//  objects that are completely unnecessary in C++.  So we can just do this
//  as a collection of functions.  But if you do that, we recommend that
//  you put the functions together in a namespace, like we have done here.
//
//  Author: Walker M. White
//  Based on original GameX Ship Demo by Rama C. Hoetzlein, 2002
//  Version: 2/21/21
//
#include "CollisionController.h"

/** Impulse for giving collisions a slight bounce. */
#define COLLISION_COEFF     0.1f
#define DEFAULT_RADIUS  0.13f
#define BUFFER  0.05f

using namespace cugl;
void CollisionController::intraOverWorldCollisions(OverWorld& overWorld){
    if (healFromBaseCollsion(*overWorld.getBaseSet(), overWorld.getDog())){
//        CULog("HEAL DETECTED\n");
    }
}

void CollisionController::overWorldMonsterControllerCollisions(OverWorld& overWorld, MonsterController& monsterController){
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& monsterEnemies = monsterController.getEnemies();
    std::unordered_set<std::shared_ptr<AbsorbEnemy>>& absorbEnemies = monsterController.getAbsorbEnemies();
    if (monsterDogCollision(overWorld.getDog(), monsterEnemies)){
//         CULog("MONSTER DOG COLLISION DETECTED\n");
    }
    if (monsterDecoyCollision(overWorld.getDecoys(), monsterEnemies)){
//         CULog("MONSTER DECOY COLLISION DETECTED\n");
    }
    if (monsterBaseCollsion(overWorld, overWorld.getBaseSet(), monsterController)){
//        CULog("Monster Base COLLISION DETECTED\n");
    }
    if (absorbEnemDogCollision(overWorld.getDog(), absorbEnemies)){
//        CULog("Absorb Enemy vs. Dog COLLISION DETECTED\n");
    }
    if (absorbEnemMonsterCollision(monsterController, absorbEnemies)){
        CULog("Absorb Enemy vs. Other enemies COLLISION DETECTED\n");
    }

}
void CollisionController::attackCollisions(OverWorld& overWorld, MonsterController& monsterController, SpawnerController& spawnerController){
    AttackPolygons& attacks = overWorld.getAttackPolygons();
    std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners = spawnerController._spawners;
    std::shared_ptr<Dog> dog = overWorld.getDog();
    for (const std::shared_ptr<ActionPolygon>& action: attacks.currentAttacks){
        switch (action->getAction()){
            case (Action::SHOOT):
                hugeBlastCollision(action->getPolygon(), monsterController); // Play blast sound
                break;
            case (Action::EXPLODE):
                resolveBlowup(action->getPolygon(),monsterController, spawners); // play boom sound
                break;
            case (Action::BITE):
                resolveBiteAttack(action->getPolygon(), monsterController, overWorld);
                break;
            default:
                CULog("Action not used in Collisions\n");
        };
    }
}
bool CollisionController::monsterBaseCollsion(OverWorld& overWorld, std::shared_ptr<BaseSet> curBases, MonsterController& monsterController){
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies = monsterController.getEnemies();
    bool collision = false;
    float baseRadius = DEFAULT_RADIUS;
    auto itP = curBases->_bases.begin();
    while (itP != curBases->_bases.end()){
        std::shared_ptr<Base> base = *itP;
        auto itA = curEnemies.begin();
        bool hitSomething = false;
        while ( itA != curEnemies.end()){
            const std::shared_ptr<AbstractEnemy>& enemy = *itA;
            float enemyRadius = fmin(enemy->getWidth(), enemy->getHeight())/2;
            Vec2 norm = base->getPos() - enemy->getPosition();
            float distance = norm.length();
            float impactDistance = baseRadius + enemyRadius;
            auto curA = itA;
            itA++;
            if (distance < impactDistance) {
                hitSomething = true;
                collision = true;
                monsterController.removeEnemy(enemy);
                enemy->executeDeath(overWorld);
                curEnemies.erase(curA);
            }
        }
        auto curP = itP++;
        if (hitSomething){
            CULog("Base Health %d", (*curP)->getHealth());
            (*curP)->reduceHealth(5);
        }
    }
    return collision;
}
bool CollisionController::monsterDecoyCollision(std::shared_ptr<DecoySet> decoySet, std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies){
    bool collide = false;
    std::vector<std::shared_ptr<Decoy>> decoys = decoySet->getCurrentDecoys();
    for (std::shared_ptr<Decoy> curDecoy: decoys){
        float decoyRadius = DEFAULT_RADIUS;
        for (std::shared_ptr<AbstractEnemy> enemy: curEnemies){
            Vec2 norm = curDecoy->getPos() - enemy->getPosition();
            float enemyRadius = fmin(enemy->getWidth(), enemy->getHeight())/2;
            float distance = norm.length();
            float impactDistance = decoyRadius + enemyRadius;
            if (distance < impactDistance){ // need noise
                if (enemy->canAttack()){
                    collide = true;
                    enemy->resetAttack();
                    curDecoy->subHealth(enemy->getDamage());
                }
            }
        }
    }
    return collide;
}
bool CollisionController::monsterDogCollision(std::shared_ptr<Dog> curDog, std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies){
    float dogRadius = fmin(curDog->getWidth(), curDog->getHeight())/2;
    bool collision = false;
    auto it = curEnemies.begin();
    while (it != curEnemies.end()){
        std::shared_ptr<AbstractEnemy> enemy = *it;
        float enemyRadius = fmin(enemy->getWidth(), enemy->getHeight())/2;
        Vec2 norm = curDog->getPosition() - enemy->getPosition();
        float distance = norm.length();
        float impactDistance = dogRadius + enemyRadius + BUFFER;
        it++;
        if (distance < impactDistance) {
            if (enemy->canAttack()){
                collision = true;
                enemy->resetAttack();
                curDog->setHealth(curDog->getHealth()-enemy->getDamage());
            }
        }
    }
    return collision;
    
    
    return false;
}

// IS THIS NECESSARY ???
bool CollisionController::absorbEnemDogCollision(std::shared_ptr<Dog> curDog, std::unordered_set<std::shared_ptr<AbsorbEnemy>>& absorbCurEnemies){
    bool collision = false;
    auto it = absorbCurEnemies.begin();
    while (it != absorbCurEnemies.end()){
        std::shared_ptr<AbsorbEnemy> enemy = *it;
        Vec2 norm = curDog->getPosition() - enemy->getPosition();
        float distance = norm.length();
        float impactDistance = 1.2;
        it++;
        if (distance < impactDistance) {
            if (enemy->canAttack()){
                collision = true;
                enemy->resetAttack();
                curDog->setHealth(curDog->getHealth()-enemy->getDamage());
            }
        }
    }
    return collision;
}
bool CollisionController::absorbEnemMonsterCollision(MonsterController& monsterController, std::unordered_set<std::shared_ptr<AbsorbEnemy>>& absorbCurEnemies){
    bool collision = false;
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& monsterEnemies = monsterController.getEnemies();
    for (std::shared_ptr<AbsorbEnemy> curAbsorbEnemy: absorbCurEnemies){
        for (std::shared_ptr<AbstractEnemy> enemy: monsterEnemies){
            Vec2 norm = curAbsorbEnemy->getPosition() - enemy->getPosition();
            int curDamage = curAbsorbEnemy->getDamage();
            float distance = norm.length();
            float impactDistance = 1.2;
            if (distance < impactDistance){ // need noise
                if (curAbsorbEnemy->canAttack() && curAbsorbEnemy != enemy){
                    CULog("ABSORPTION IN PROGRESS");
                    collision = true;
                    curAbsorbEnemy->resetAttack();
                    // INCREASE ABSORB ENEMY STRENGTH
                    CULog("Prev damage: %d", curDamage);
                    curAbsorbEnemy->setDamage(curDamage + 1/64);
                    CULog("New damage: %d", curAbsorbEnemy->getDamage());
                    // SCALE ABSORB ENEMY
//                    float newWidth = curAbsorbEnemy->getDimension().width + 0.02;
//                    float newHeight = curAbsorbEnemy->getDimension().height + 0.02;
//                    cugl::Size newSize(newWidth,newHeight);
//                    curAbsorbEnemy->setDimension(newSize);
                    // ERASE CURRENT ENEMY
                    monsterController.removeEnemy(enemy);
//                    monsterEnemies.erase(enemy);
                }
            }
        }
    }
    return collision;
}

void CollisionController::resolveBiteAttack(const cugl::Poly2& bitePolygon, MonsterController& monsterController, OverWorld& overWorld){
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& monsterEnemies = monsterController.getEnemies();
    auto itA = monsterEnemies.begin();
    bool hitSomething = false;
    while ( itA != monsterEnemies.end()){
        const std::shared_ptr<AbstractEnemy>& enemy = *itA;
        auto curA = itA;
        itA++;
        if (bitePolygon.contains(enemy->getPosition())){
            hitSomething = true;
            enemy->setHealth(enemy->getHealth() - 1);
            if(enemy->getHealth() <= 0){
                monsterController.removeEnemy(enemy);
                enemy->executeDeath(overWorld);
                overWorld.getDog()->addAbsorb((*curA)->getAbsorbValue());
                monsterEnemies.erase(curA);
            }
        }
    }
}
bool CollisionController::healFromBaseCollsion( BaseSet& bset, std::shared_ptr<Dog> ship){
    if (!ship->canHeal()|| ship->getHealth() >= ship->getMaxHealth()){
        return false;
    }
    auto itP = bset._bases.begin();
    float healDistanceCutoff = 1;
    
    while (itP != bset._bases.end()){
        std::shared_ptr<Base> base = *itP;
        Vec2 norm = base->getPos() - ship->getPosition();
        float distance = norm.length();
        itP++;
        if (distance < healDistanceCutoff){
            ship->resetHeal();
            ship->setHealth(std::min(ship->getMaxHealth(), ship->getHealth()+base->getHealValue()));
            return true;
        }
    }
    return false;
}
void CollisionController::hugeBlastCollision(const cugl::Poly2& blastRectangle, MonsterController& monsterController){
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& enemies = monsterController.getEnemies();
    auto itA = enemies.begin();
    while (itA != enemies.end()){
        const std::shared_ptr<AbstractEnemy>& enemy = *itA;
        Vec2 enemyPos = enemy->getPosition();
        auto curA = itA;
        itA++;
        if (blastRectangle.contains(enemyPos)){
            monsterController.removeEnemy(enemy);
            enemies.erase(curA);
        }
    }
}
void CollisionController::resolveBlowup(const cugl::Poly2& blastCircle, MonsterController& monsterController, std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners){
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& monsterEnemies = monsterController.getEnemies();
    auto itA = monsterEnemies.begin();
    while (itA != monsterEnemies.end()){
        const std::shared_ptr<AbstractEnemy>& enemy = *itA;
        auto curA = itA++;
        if (blastCircle.contains(enemy->getPosition())){
            monsterController.removeEnemy(enemy);
            monsterEnemies.erase(curA);
        }
    }
    auto itS = spawners.begin();
    while (itS != spawners.end()){
        const std::shared_ptr<AbstractSpawner>& spawn = *itS;
        auto curS = itS;
        itS++;
        if (blastCircle.contains(spawn->getPos())){
            spawn->getSpawnerNode()->removeFromParent();
            spawners.erase(curS);
        }
    }
}

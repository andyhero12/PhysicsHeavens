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
#define DEFAULT_RADIUS_COLLIDE  0.13f
#define BUFFER  0.1f

using namespace cugl;


static inline double angle_1to360(float angle){
    if (angle >= 360.0f){
        angle -= 360.0f;
    }
    if (angle <= 0.0f){
        angle += 360;
    }
    return angle;
}
bool withinAngle(float ang1, float ang2, float degrees){
    float N = angle_1to360(ang2); //normalize angles to be 1-360 degrees
    float a = angle_1to360(ang1);
    float b = angle_1to360(ang1 + degrees);
    
    if (a < b){
        return a <= N && N <= b;
    }
    return a <= N || N <= b;
}

void CollisionController::clientIntraWorld(OverWorld& overWorld){
    if (healFromBaseCollsion(*overWorld.getBaseSet(), overWorld.getClientDog())){
        CULog("Client Heal");
    }
}

void CollisionController::clienOverMonster(OverWorld& overWorld, MonsterController& monsterController){
    
}

void CollisionController::clientAttackCollide(OverWorld& overWorld, MonsterController& monsterController, SpawnerController& spawnerController){
    AttackPolygons& attacks = overWorld.getAttackPolygons();
    std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners = spawnerController._spawners;
    std::shared_ptr<Dog> dog = overWorld.getDog();
    for (const std::shared_ptr<ActionPolygon>& action: attacks.currentAttacks){
        switch (action->getAction()){
            case (Action::SHOOT):
                hugeBlastCollisionClient(action, monsterController, spawners); // Play blast sound
                break;
            case (Action::EXPLODE):
                resolveBlowupClient(action,monsterController, spawners); // play boom sound
                break;
            case (Action::BITE):
                resolveBiteAttackClient(action, monsterController, overWorld, spawners, true);
                break;
            default:
                CULog("Action not used in Collisions\n");
        };
    }
    AttackPolygons& attacksClient = overWorld.getAttackPolygonsClient();
    for (const std::shared_ptr<ActionPolygon>& action: attacksClient.currentAttacks){
        switch (action->getAction()){
            case (Action::SHOOT):
                hugeBlastCollisionClient(action, monsterController, spawners); // Play blast sound
                break;
            case (Action::EXPLODE):
                resolveBlowupClient(action,monsterController, spawners); // play boom sound
                break;
            case (Action::BITE):
                resolveBiteAttackClient(action, monsterController, overWorld, spawners, false);
                break;
            default:
                CULog("Action not used in Collisions\n");
        };
    }
}
void CollisionController::intraOverWorldCollisions(OverWorld& overWorld){
    if (healFromBaseCollsion(*overWorld.getBaseSet(), overWorld.getDog())){
//        CULog("HEAL DETECTED\n");
    }
}
void CollisionController::overWorldMonsterControllerCollisions(OverWorld& overWorld, MonsterController& monsterController){
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& monsterEnemies = monsterController.getEnemies();
    if (monsterDogCollision(overWorld.getDog(), monsterController, monsterEnemies)){
//         CULog("MONSTER DOG COLLISION DETECTED\n");
    }
    if (_network->getNumPlayers() == 2){
        if (clientDogMonsterCollision(overWorld.getClientDog(), monsterController, monsterEnemies)){
        }
    }
    if (monsterDecoyCollision(overWorld.getDecoys(), monsterEnemies)){
//         CULog("MONSTER DECOY COLLISION DETECTED\n");
    }
    if (monsterDecoyExplosionCollision(overWorld.getDecoys(), monsterController)){
//         CULog("MONSTER DECOY COLLISION DETECTED\n");
    }
    if (monsterBaseCollsion(overWorld, overWorld.getBaseSet(), monsterController)){
//        CULog("Monster Base COLLISION DETECTED\n");
    }
    if (absorbEnemMonsterCollision(monsterController, monsterController.getAbsorbEnemies())){
        CULog("Absorbed");
    }
}
void CollisionController::attackCollisions(OverWorld& overWorld, MonsterController& monsterController, SpawnerController& spawnerController){
    AttackPolygons& attacks = overWorld.getAttackPolygons();
    std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners = spawnerController._spawners;
    std::shared_ptr<Dog> dog = overWorld.getDog();
    for (const std::shared_ptr<ActionPolygon>& action: attacks.currentAttacks){
        switch (action->getAction()){
            case (Action::SHOOT):
                hugeBlastCollision(action, monsterController, spawners); // Play blast sound
                break;
            case (Action::EXPLODE):
                resolveBlowup(action,monsterController, spawners); // play boom sound
                break;
            case (Action::BITE):
                resolveBiteAttack(action, monsterController, overWorld, spawners, true);
                break;
            default:
                CULog("Action not used in Collisions\n");
        };
    }
    AttackPolygons& attacksClient = overWorld.getAttackPolygonsClient();
    for (const std::shared_ptr<ActionPolygon>& action: attacksClient.currentAttacks){
        switch (action->getAction()){
            case (Action::SHOOT):
                hugeBlastCollision(action, monsterController, spawners); // Play blast sound
                break;
            case (Action::EXPLODE):
                resolveBlowup(action,monsterController, spawners); // play boom sound
                break;
            case (Action::BITE):
                resolveBiteAttack(action, monsterController, overWorld, spawners, false);
                break;
            default:
                CULog("Action not used in Collisions\n");
        };
    }
}
bool CollisionController::monsterBaseCollsion(OverWorld& overWorld, std::shared_ptr<BaseSet> curBases, MonsterController& monsterController){
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies = monsterController.getEnemies();
    bool collision = false;
    float baseRadius = DEFAULT_RADIUS_COLLIDE;
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
            _network->pushOutEvent(BaseHealthEvent::allocBaseHealthEvent(base->getPos(), 10));
        }
    }
    return collision;
}
bool CollisionController::monsterDecoyCollision(std::shared_ptr<DecoySet> decoySet, std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies){
    bool collide = false;
    std::vector<std::shared_ptr<Decoy>> decoys = decoySet->getCurrentDecoys();
//    std::vector<std::shared_ptr<Decoy>> removedDecoys = decoySet->getRemovedDecoys();
    for (std::shared_ptr<Decoy> curDecoy: decoys){
        float decoyRadius = DEFAULT_RADIUS_COLLIDE;
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
//    auto itDec = removedDecoys.begin();
//    auto itMon = curEnemies.begin();
//    float decoyRadius = 0.26f;
//    while (itDec != removedDecoys.end()){
//        const std::shared_ptr<Decoy>& decoy= *itDec;
//        auto curDec = itDec;
//        itDec++;
//        while(itMon != curEnemies.end()){
//            const std::shared_ptr<AbstractEnemy>& enemy = *itMon;
//            auto curMon = itMon;
//            itMon++;
//            Vec2 norm = decoy->getPos() - enemy->getPosition();
//            float enemyRadius = fmin(enemy->getWidth(), enemy->getHeight())/2;
//            float distance = norm.length();
//            float impactDistance = decoyRadius + enemyRadius;
//            if (distance < impactDistance){ // need noise
//                collide = true;
//                curEnemies.erase(enemy);
//            }
//        }
//    }
    return collide;
}
bool CollisionController::monsterDecoyExplosionCollision(std::shared_ptr<DecoySet> decoySet, MonsterController& monsterController){
    bool collide = false;
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies = monsterController.getEnemies();
    std::vector<std::shared_ptr<Decoy>> removedDecoys = decoySet->getRemovedDecoys();
    
    auto itDec = removedDecoys.begin();
    auto itMon = curEnemies.begin();
    float decoyRadius = 2.00f;
    while (itDec != removedDecoys.end()){
        const std::shared_ptr<Decoy>& decoy= *itDec;
//        auto curDec = itDec;
        itDec++;
        while(itMon != curEnemies.end()){
            const std::shared_ptr<AbstractEnemy>& enemy = *itMon;
            auto curMon = itMon;
            itMon++;
            Vec2 norm = decoy->getPos() - enemy->getPosition();
            float enemyRadius = fmin(enemy->getWidth(), enemy->getHeight())/2;
            float distance = norm.length();
            float impactDistance = decoyRadius + enemyRadius;
            if (distance < impactDistance){ // need noise
                collide = true;
                monsterController.removeEnemy(enemy);
                curEnemies.erase(curMon);
            }
        }
    }
    return collide;
}

bool CollisionController::clientDogMonsterCollision(std::shared_ptr<Dog> curDog, MonsterController& monsterController, std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies){
    float dogRadius = fmax(curDog->getWidth(), curDog->getHeight())/2;
    bool collision = false;
    auto it = curEnemies.begin();
    while (it != curEnemies.end()){
        std::shared_ptr<AbstractEnemy> enemy = *it;
        float enemyRadius = fmax(enemy->getWidth(), enemy->getHeight())/2;
        Vec2 norm = curDog->getPosition() - enemy->getPosition();
        float distance = norm.length();
        float impactDistance = dogRadius + enemyRadius + BUFFER;
        if (distance < impactDistance) {
            if (enemy->canAttack()){
                if (std::dynamic_pointer_cast<BombEnemy>(enemy)) {
                    monsterController.removeEnemy(enemy);
                    collision = true;
                    it = curEnemies.erase(it); // Correctly update the iterator here
                    continue;
                }
                else {
                    collision = true;
                    enemy->resetAttack();
                    _network->pushOutEvent(ClientHealthEvent::allocClientHealthEvent(enemy->getDamage(),false));
                }
//                curDog->setHealth(curDog->getHealth()-enemy->getDamage());
            }else{
            }
        }
        ++it;
    }
    return collision;
}
bool CollisionController::monsterDogCollision(std::shared_ptr<Dog> curDog, MonsterController& monsterController, std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies){
    float dogRadius = fmax(curDog->getWidth(), curDog->getHeight())/2;
    bool collision = false;
    auto it = curEnemies.begin();
    while (it != curEnemies.end()){
        std::shared_ptr<AbstractEnemy> enemy = *it;
        float enemyRadius = fmax(enemy->getWidth(), enemy->getHeight())/2;
        Vec2 norm = curDog->getPosition() - enemy->getPosition();
        float distance = norm.length();
        float impactDistance = dogRadius + enemyRadius + BUFFER;
        if (distance < impactDistance) {
            if (enemy->canAttack()){
                if (std::dynamic_pointer_cast<BombEnemy>(enemy)) {
                    monsterController.removeEnemy(enemy);
                    collision = true;
                    it = curEnemies.erase(it);
                    continue;
                }
                else {
                    collision = true;
                    enemy->resetAttack();
                    _network->pushOutEvent(ClientHealthEvent::allocClientHealthEvent(enemy->getDamage(),true));
                }
//                curDog->setHealth(curDog->getHealth()-enemy->getDamage());
            }else{
            }
        }
        ++it;
    }
    return collision;
}

void CollisionController::resolveBiteAttack(const std::shared_ptr<ActionPolygon>& action, MonsterController& monsterController, OverWorld& overWorld, std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners,bool isHostAttack){
    if (!action->dealDamage())
        return;
    bool collided = false;
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& monsterEnemies = monsterController.getEnemies();
    auto itA = monsterEnemies.begin();
    int addedAbsorb = 0;
    while ( itA != monsterEnemies.end()){
        const std::shared_ptr<AbstractEnemy>& enemy = *itA;
        auto curA = itA;
        itA++;
        
        Vec2 diff = enemy->getPosition() - action->getCenter();
        float ang = diff.getAngle();
        float result = ((ang > 0 ? ang : (2*M_PI +ang)) * 360 / (2*M_PI));
        if (result < 0.0f){
            result += 360.0f;
        }
        float dist = diff.length();
        if (withinAngle(action->getAngle()-90.0f, result, 180.0f) && dist <= 3 * action->getScale()){
            int damage = (int)(action->getScale() * action->getScale() * 3);
            Uint64 objNum = _network->getPhysController()->getPhysicsWorld()->getObstacleId(enemy);
            if (objNum == -1){
                CULog("Somehow object doesn't exist");
            }else{
                _network->pushOutEvent(MonsterHealthEvent::allocMonsterHealthEvent(damage, objNum));
            }
            enemy->applyDamage(damage, diff);
            collided = true;
            if(enemy->getHealth() <= 0){
                monsterController.removeEnemy(enemy);
                enemy->executeDeath(overWorld);
//                overWorld.getDog()->addAbsorb((*curA)->getAbsorbValue());
                addedAbsorb += (*curA)->getAbsorbValue();
                
                monsterEnemies.erase(curA);
            }
        }
    }
    if (addedAbsorb != 0){
        _network->pushOutEvent(SizeEvent::allocSizeEvent(addedAbsorb, isHostAttack));
    }

    for (auto& spawner : spawners){
        Vec2 diff = spawner->getPos() - action->getCenter();
        float ang = diff.getAngle();
        float result = ((ang > 0 ? ang : (2*M_PI +ang)) * 360 / (2*M_PI));
        if (result < 0.0f){
            result += 360.0f;
        }
        float dist = diff.length();
        if (withinAngle(action->getAngle()-90.0f, result, 180.0f) && dist <= 3 * action->getScale()){
            collided = true;
            spawner->subHealth(4);
        }
    }
    if (collided){
        action->resetAttack();
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
void CollisionController::hugeBlastCollisionClient(const std::shared_ptr<ActionPolygon>& action, MonsterController& monsterController, std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners){
    if (!action->dealDamage())
        return;
    Poly2 blastRectangle = action->getPolygon();
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& enemies = monsterController.getEnemies();
    bool hitSomething = false;
    for (auto& spawner : spawners){
        Vec2 diff = spawner->getPos() - action->getCenter();
        float ang = diff.getAngle();
        float result = ((ang > 0 ? ang : (2*M_PI +ang)) * 360 / (2*M_PI));
        if (result < 0.0f){
            result += 360.0f;
        }
        float dist = diff.length();
        if (withinAngle(action->getAngle()-45.0f, result, 90.0f) && dist <= 5.5f * action->getScale()){
            hitSomething = true;
            spawner->subHealth(60);
        }
    }
    if (hitSomething){
        action->resetAttack();
    }
}
void CollisionController::resolveBlowupClient(const std::shared_ptr<ActionPolygon>& action, MonsterController& monsterController, std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners){
    if (!action->dealDamage())
        return;
    Poly2 blastCircle = action->getPolygon();
    auto itS = spawners.begin();
    while (itS != spawners.end()){
        const std::shared_ptr<AbstractSpawner>& spawn = *itS;
        itS++;
        Vec2 diff = spawn->getPos() - action->getCenter();
        float dist = diff.length();
        if (dist <= 2.2f * action->getScale()){
            spawn->subHealth(999);
        }
    }
}

void CollisionController::resolveBiteAttackClient(const std::shared_ptr<ActionPolygon>& action, MonsterController& monsterController, OverWorld& overWorld, std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners,bool isHostAttack){
    if (!action->dealDamage())
        return;
    bool collided = false;
    for (auto& spawner : spawners){
        Vec2 diff = spawner->getPos() - action->getCenter();
        float ang = diff.getAngle();
        float result = ((ang > 0 ? ang : (2*M_PI +ang)) * 360 / (2*M_PI));
        if (result < 0.0f){
            result += 360.0f;
        }
        float dist = diff.length();
        if (withinAngle(action->getAngle()-90.0f, result, 180.0f) && dist <= 3 * action->getScale()){
            collided = true;
            spawner->subHealth(4);
        }
    }
    if (collided){
        action->resetAttack();
    }
}

void CollisionController::hugeBlastCollision(const std::shared_ptr<ActionPolygon>& action, MonsterController& monsterController, std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners){
    if (!action->dealDamage())
        return;
    Poly2 blastRectangle = action->getPolygon();
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& enemies = monsterController.getEnemies();
    bool hitSomething = false;
    auto itA = enemies.begin();
    while (itA != enemies.end()){
        const std::shared_ptr<AbstractEnemy>& enemy = *itA;
        auto curA = itA;
        itA++;
        Vec2 diff = enemy->getPosition() - action->getCenter();
        float ang = diff.getAngle();
        float result = ((ang > 0 ? ang : (2*M_PI +ang)) * 360 / (2*M_PI));
        if (result < 0.0f){
            result += 360.0f;
        }
        float dist = diff.length();
//        CULog("Distance %f Scale %f", dist, action->getScale());
        if (withinAngle(action->getAngle()-45.0f, result, 90.0f) && dist <= 5.5f * action->getScale()){
            hitSomething = true;
            monsterController.removeEnemy(enemy);
            enemies.erase(curA);
        }
    }
    for (auto& spawner : spawners){
        Vec2 diff = spawner->getPos() - action->getCenter();
        float ang = diff.getAngle();
        float result = ((ang > 0 ? ang : (2*M_PI +ang)) * 360 / (2*M_PI));
        if (result < 0.0f){
            result += 360.0f;
        }
        float dist = diff.length();
        if (withinAngle(action->getAngle()-45.0f, result, 90.0f) && dist <= 5.5f * action->getScale()){
            hitSomething = true;
            spawner->subHealth(60);
        }
    }
    if (hitSomething){
        action->resetAttack();
    }
}
void CollisionController::resolveBlowup(const std::shared_ptr<ActionPolygon>& action, MonsterController& monsterController, std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners){
    if (!action->dealDamage())
        return;
    Poly2 blastCircle = action->getPolygon();
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& monsterEnemies = monsterController.getEnemies();
    auto itA = monsterEnemies.begin();
    while (itA != monsterEnemies.end()){
        const std::shared_ptr<AbstractEnemy>& enemy = *itA;
        auto curA = itA++;
        Vec2 diff = enemy->getPosition() - action->getCenter();
        float dist = diff.length();
        if (dist <= 2.2f * action->getScale()){
            monsterController.removeEnemy(enemy);
            monsterEnemies.erase(curA);
        }
    }
    auto itS = spawners.begin();
    while (itS != spawners.end()){
        const std::shared_ptr<AbstractSpawner>& spawn = *itS;
//        auto curS = itS;
        itS++;
        Vec2 diff = spawn->getPos() - action->getCenter();
        float dist = diff.length();
        if (dist <= 2.2f * action->getScale()){
            spawn->subHealth(999);
        }
    }
}

bool CollisionController::absorbEnemMonsterCollision(MonsterController& monsterController, std::unordered_set<std::shared_ptr<AbsorbEnemy>>& absorbCurEnemies){
    bool collision = false;
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& monsterEnemies = monsterController.getEnemies();
    
    auto itAbs = absorbCurEnemies.begin();
    auto itMon = monsterEnemies.begin();
    while (itAbs != absorbCurEnemies.end()){
        bool absorbAte = false;
        const std::shared_ptr<AbsorbEnemy>& absEnemy = *itAbs;
//        auto curAbs = itAbs;
        itAbs++;
        while(itMon != monsterEnemies.end()){
            const std::shared_ptr<AbstractEnemy>& curEnemy = *itMon;
            auto curMon = itMon;
            itMon++;
            Vec2 norm = (absEnemy)->getPosition() - (curEnemy)->getPosition();
            float distance = norm.length();
            float impactDistance = 3.5;
            if (distance < impactDistance){
                std::shared_ptr<AbsorbEnemy> isAbsorb = std::dynamic_pointer_cast<AbsorbEnemy>(curEnemy);
                if (isAbsorb == nullptr && absEnemy->canAttack()){
                    collision = true;
                    absorbAte = true;
                    absEnemy->increaseHealth(curEnemy->getHealth());
                    // SCALE ABSORB ENEMY
                    monsterController.removeEnemy(curEnemy);
                    monsterEnemies.erase(curMon);
                }
            }
        }
        if (absorbAte){
            absEnemy->resetAttack();
        }
    }
    return collision;
}

bool CollisionController::enemyExplodedCollision(Vec2 pos, float radius, std::shared_ptr<Dog> curDog, MonsterController& monsterController, bool isHost){
    bool collide = false;
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies = monsterController.getEnemies();
    
    auto itMon = curEnemies.begin();
    while(itMon != curEnemies.end()){
        const std::shared_ptr<AbstractEnemy>& enemy = *itMon;
        auto curMon = itMon;
        itMon++;
        Vec2 norm = pos - enemy->getPosition();
        float enemyRadius = fmin(enemy->getWidth(), enemy->getHeight())/2;
        float distance = norm.length();
        float impactDistance = radius + enemyRadius;
        if (distance < impactDistance){ // need noise
            collide = true;
            monsterController.removeEnemy(enemy);
            curEnemies.erase(curMon);
        }
    }

    Vec2 norm = pos - curDog->getPosition();
    float dogRadius = fmax(curDog->getWidth(), curDog->getHeight())/2;
    float distance = norm.length();
    float impactDistance = radius + dogRadius;
    if (distance < impactDistance){ // need noise
        collide = true;
        _network->pushOutEvent(ClientHealthEvent::allocClientHealthEvent(8,isHost));
    }

    return collide;
}

void CollisionController::dispose(){
    
    _network = nullptr;
}

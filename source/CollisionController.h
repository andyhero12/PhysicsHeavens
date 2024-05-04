//
//  SLCollisionController.h
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
//  As you work on this class, there is an interesting thing to note.  The
//  constructor and the initializer are SEPARATE.  That is because the
//  constructor is called as soon as the object is created and that happens
//  BEFORE we know the window size.
//
//  Author: Walker M. White
//  Based on original GameX Ship Demo by Rama C. Hoetzlein, 2002
//  Version: 1/20/22
//
#ifndef __SL_COLLISION_CONTROLLER_H__
#define __SL_COLLISION_CONTROLLER_H__
#include <cugl/cugl.h>
#include "NLDog.h"
#include "BaseSet.h"
#include "SpawnerController.h"
#include "AttackPolygons.h"
#include "OverWorld.h"
#include "MonsterController.h"
#include "NLBaseHealthEvent.h"
#include "NLClientHealthEvent.h"
#include "NLMonsterHealthEvent.h"
#include "NLSizeEvent.h"
/**
 * Namespace of functions implementing simple game physics.
 *
 * This is the simplest of physics engines. In reality, you will probably use
 * box2d just like you did in 3152.
 */
class CollisionController {
private:

public:
    /**
     * Creates a new collision controller.
     *
     * You will notice this constructor does nothing.  That is because the
     * object is constructed the instance the game starts (main.cpp immediately
     * constructs DogApp, which immediately constructs GameScene, which then
     * immediately constructs this class), before we know the window size.
     */
    CollisionController() {}
    
    /**
     * Deletes the collision controller.
     *
     * Not much to do here since there was no dynamic allocation.
     */
    ~CollisionController() {}
    
    /**
     * Initializes the collision controller with the given size.
     *
     * This initializer is where we can finally set the window size. This size
     * is used to manage screen wrap for collisions.
     *
     * The pattern we use in this class is that all initializers should return
     * a bool indicating whether initialization was successful (even for
     * initializers that always return true).
     *
     * @param size  The window size
     *
     * @return true if initialization was successful
     */
    bool init(std::shared_ptr<NetEventController> net) {
        _network = net;
        return true;
    }
    
    void resolveBlowup(const std::shared_ptr<ActionPolygon>& action, MonsterController& monsterController, std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners);
    
    void hugeBlastCollision(const std::shared_ptr<ActionPolygon>& action, MonsterController& monsterController, std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners);
    void resolveBiteAttack(const std::shared_ptr<ActionPolygon>& action, MonsterController& monsterController,
                           OverWorld& overWorld, std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners,bool isHostAttack);
    
    void resolveBlowupClient(const std::shared_ptr<ActionPolygon>& action, MonsterController& monsterController, std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners);
    
    void hugeBlastCollisionClient(const std::shared_ptr<ActionPolygon>& action, MonsterController& monsterController, std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners);
    void resolveBiteAttackClient(const std::shared_ptr<ActionPolygon>& action, MonsterController& monsterController,
                           OverWorld& overWorld, std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners,bool isHostAttack);
    bool healFromBaseCollsion( BaseSet& bset, std::shared_ptr<Dog> ship);
    
    // Post Update Functions
    void intraOverWorldCollisions( OverWorld& overWorld);
    
    void overWorldMonsterControllerCollisions(OverWorld& overWorld, MonsterController& monsterController);
    
    bool monsterDogCollision(std::shared_ptr<Dog> curDog, std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies);
    bool monsterDecoyCollision(std::shared_ptr<DecoySet> decoySet, std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies);
    bool monsterDecoyExplosionCollision(std::shared_ptr<DecoySet> decoySet, MonsterController& monsterController);
    bool monsterBaseCollsion(OverWorld& overWorld, std::shared_ptr<BaseSet> curBases, MonsterController& monsterController);
    bool absorbEnemMonsterCollision(MonsterController& monsterController, std::unordered_set<std::shared_ptr<AbsorbEnemy>>& absorbCurEnemies);
    
    void clientIntraWorld(OverWorld& overWorld);
    void clienOverMonster(OverWorld& overWorld, MonsterController& monsterController);
    void clientAttackCollide(OverWorld& overWorld, MonsterController& monsterController, SpawnerController& spawnerController);
    
    
    void attackCollisions(OverWorld& overWorld, MonsterController& monsterController, SpawnerController& spawnerController);
    
    bool clientDogMonsterCollision(std::shared_ptr<Dog> curDog, std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies);
    void dispose();
    std::shared_ptr<NetEventController> _network;
};

#endif /* __SL_COLLISION_CONTROLLER_H__ */


//
//  MonsterController.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/21/24.
//
#include "MonsterController.h"

#define DYNAMIC_COLOR   Color4::YELLOW
int generateRandomInclusiveHighLow(int low, int high)
{
    // Static used for the seed to ensure it's only seeded once
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(low, high); // Range is 1 to 3, inclusive
    return dis(gen);
}

bool MonsterController::init(OverWorld& overWorld,
                             std::shared_ptr<cugl::scene2::SceneNode> worldNode,
     std::shared_ptr<cugl::scene2::SceneNode> debugNode){
    _current.clear();
    _pending.clear();
    monsterControllerSceneNode = cugl::scene2::SceneNode::alloc();
    _debugNode = debugNode;
    _worldnode = worldNode;
    for (const cugl::Vec3& cluster : overWorld.getLevelModel()->preSpawnLocs()){
        float cx = cluster.x;
        float cy = cluster.y;
        int count = round(cluster.z);
//        for(int i = 0; i < count; i++) {
//            spawnStaticBasicEnemy(Vec2(cx,cy), overWorld);
            spawnBombEnemy(Vec2(cx,cy), overWorld);
//        }
    }
    return true;
}
void MonsterController::postUpdate(){
    for (std::shared_ptr<AbstractEnemy> curEnemy: _pending){
        _current.insert(curEnemy);
    }
    _pending.clear();
}

void MonsterController::retargetToDecoy( OverWorld& overWorld){
    int totalTargets = overWorld.getTotalTargets(); // really brittle be careful
    for (std::shared_ptr<AbstractEnemy> enemy : getEnemies()){
        enemy->setTargetIndex(totalTargets-1); // added pending decoy to this iteration
    }
}
void MonsterController::retargetCloset( OverWorld& overWorld){
    cugl::Vec2 dogPos = overWorld.getDog()->getPosition();
    int baseSize = (int) overWorld.getBaseSet()->_bases.size();
    int decoySize = (int) overWorld.getDecoys()->getCurrentDecoys().size();
    for (std::shared_ptr<AbstractEnemy> enemy : getEnemies()){
        Vec2 enemyPos = enemy->getPosition();
        int index = 0;
        float dist = (enemyPos - dogPos).length();
        int sizeBases = baseSize;
        for (int i = 0 ;i < sizeBases;i++){
            Vec2 basePos = overWorld.getBaseSet()->_bases[i]->getPos();
            float curDist = (enemyPos -  basePos).length();
            if (curDist < dist){
                dist = curDist;
                index = i + 1;
            }
        }
        int sizeDecoys = decoySize;
        for (int i = 0 ;i < sizeDecoys; i++){
            Vec2 decoyPos = overWorld.getDecoys()->getCurrentDecoys().at(i)->getPos();
            float curDist = (enemyPos -  decoyPos).length();
            if (curDist < dist){
                dist = curDist;
                index = i + 1 + sizeBases;
            }
        }
        enemy->setTargetIndex(index);
    }
}
void MonsterController::update(float timestep, OverWorld& overWorld){
    if (!overWorld._isHost){
        return;
    }
    std::shared_ptr<DecoySet> decoySet = overWorld.getDecoys();
    if (decoySet->addedNewDecoy()){
        retargetToDecoy(overWorld);
        return;
    }
    if (decoySet->removedDecoy()){
        retargetCloset(overWorld);
        return;
    }
    for (std::shared_ptr<AbstractEnemy> curEnemy: _current){
        curEnemy->preUpdate(timestep, overWorld);
    }
}
void MonsterController::setMeleeAnimationData(std::shared_ptr<cugl::JsonValue> data,
                           std::shared_ptr<cugl::AssetManager> _assets){
    _staticMeleeFactory = StaticMeleeFactory::alloc(data, _assets);
    _staticMeleeFactID = _network->getPhysController()->attachFactory(_staticMeleeFactory);
    _meleeFactory = MeleeFactory::alloc(data, _assets);
    _meleeFactID = _network->getPhysController()->attachFactory(_meleeFactory);
}

void MonsterController::setBombAnimationData(std::shared_ptr<cugl::JsonValue> data,
                        std::shared_ptr<cugl::AssetManager> _assets){
    _bombEnemyFactory = BombFactory::alloc(data, _assets);
    _bombEnemyFactID = _network->getPhysController()->attachFactory(_bombEnemyFactory);
}

void MonsterController::setHealthBar(std::shared_ptr<cugl::scene2::ProgressBar> bar){
    _healthBar = bar;
}
void MonsterController::spawnBasicEnemy(cugl::Vec2 pos, OverWorld& overWorld){
    if (!overWorld._isHost){
        return;
    }
    int numTargets =  overWorld.getTotalTargets();
    int chosenTarget = generateRandomInclusiveHighLow(0, numTargets-1);
    Size mySize(0.5,0.5);
    auto params = _meleeFactory->serializeParams(pos, mySize, 3, chosenTarget);
    auto pair = _network->getPhysController()->addSharedObstacle(_meleeFactID, params);
//        static_enemy->setHealthBar(_healthBar);
    pair.first->setDebugScene(_debugNode);
    if (auto static_enemy = std::dynamic_pointer_cast<AbstractEnemy>(pair.first)){
        _pending.emplace(static_enemy);
    }
}

void MonsterController::spawnStaticBasicEnemy(cugl::Vec2 pos, OverWorld& overWorld){
    if (!overWorld._isHost){
        return;
    }
    Size mySize(0.5,0.5);
    auto params = _staticMeleeFactory->serializeParams(pos, mySize, 3, 0);
    auto pair = _network->getPhysController()->addSharedObstacle(_staticMeleeFactID, params);
//        static_enemy->setHealthBar(_healthBar);
    pair.first->setDebugScene(_debugNode);
    if (auto static_enemy = std::dynamic_pointer_cast<AbstractEnemy>(pair.first)){
        _pending.emplace(static_enemy);
    }
}

void MonsterController::spawnBombEnemy(cugl::Vec2 pos, OverWorld& overWorld){
    if (!overWorld._isHost){
        return;
    }
    Size mySize(1,1);
    auto params = _bombEnemyFactory->serializeParams(pos, mySize, 3, 0);
    auto pair = _network->getPhysController()->addSharedObstacle(_bombEnemyFactID, params);
//        static_enemy->setHealthBar(_healthBar);
    pair.first->setDebugScene(_debugNode);
    if (auto static_enemy = std::dynamic_pointer_cast<AbstractEnemy>(pair.first)){
        _pending.emplace(static_enemy);
    }
}
void MonsterController::removeEnemy(std::shared_ptr<AbstractEnemy> enemy){
    getNetwork()->getPhysController()->removeSharedObstacle(enemy);
    enemy->getTopLevelNode()->removeFromParent();
}

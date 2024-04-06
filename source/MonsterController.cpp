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

bool MonsterController::init(std::shared_ptr<cugl::JsonValue> data, OverWorld& overWorld, std::shared_ptr<cugl::physics2::net::NetWorld> physicsWorld,
     std::shared_ptr<cugl::scene2::SceneNode> debugNode){
    if (data){
        _current.clear(); 
        _pending.clear();
        monsterControllerSceneNode = cugl::scene2::SceneNode::alloc();
        _physicsWorld = physicsWorld;
        _debugNode = debugNode;
        if (data->get("start")){
            auto initEnemies = data->get("start")->children();
            for (auto it = initEnemies.begin(); it != initEnemies.end(); it++){
                std::shared_ptr<JsonValue> entry = (*it);
                Vec2 pos;
                // TEMPORARY MAGIC NUMBERS
                pos.x = entry->get(0)->get(0)->asFloat(0) / 64;
                pos.y = entry->get(0)->get(1)->asFloat(0) / 64;
                spawnStaticBasicEnemy(pos, overWorld);
               pos += Vec2(2,2);
               spawnBasicEnemy(pos,overWorld);
            }
        }
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
        enemy->setTargetIndex(totalTargets); // will add the pending decoy to real next iteration
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
                           const std::shared_ptr<cugl::AssetManager> _assets){
    int _framecols = data->getFloat("sprite cols", 0);
    int _framesize = data->getFloat("sprite size", 0);
    std::vector<std::shared_ptr<cugl::Texture>> textures;
    textures.push_back(_assets->get<Texture>("basicEnemy1"));
    textures.push_back(_assets->get<Texture>("basicEnemy1"));
    textures.push_back(_assets->get<Texture>("basicEnemy0"));
    textures.push_back(_assets->get<Texture>("basicEnemy0"));
    textures.push_back(_assets->get<Texture>("basicEnemy0"));
    textures.push_back(_assets->get<Texture>("basicEnemy0"));
    textures.push_back(_assets->get<Texture>("basicEnemy1"));
    textures.push_back(_assets->get<Texture>("basicEnemy1"));
    meleeAnimationData._textures  = textures;
    meleeAnimationData._framesize = _framesize;
    meleeAnimationData._framecols = _framecols;
    meleeAnimationData._freqAnimations = 5;
}

void MonsterController::setBombAnimationData(std::shared_ptr<cugl::JsonValue> data,
                           const std::shared_ptr<cugl::AssetManager> _assets){
    int _framecols = data->getFloat("sprite cols", 0);
    int _framesize = data->getFloat("sprite size", 0);
    std::vector<std::shared_ptr<cugl::Texture>> textures;
    textures.push_back(_assets->get<Texture>("bombEnemyIdle"));
    textures.push_back(_assets->get<Texture>("bombEnemyIdle"));
    textures.push_back(_assets->get<Texture>("bombEnemyIdle"));
    textures.push_back(_assets->get<Texture>("bombEnemyIdle"));
    textures.push_back(_assets->get<Texture>("bombEnemyIdle"));
    textures.push_back(_assets->get<Texture>("bombEnemyIdle"));
    textures.push_back(_assets->get<Texture>("bombEnemyIdle"));
    textures.push_back(_assets->get<Texture>("bombEnemyIdle"));
    bombAnimationData._textures = textures;
    bombAnimationData._framesize = _framesize;
    bombAnimationData._framecols = _framecols;
    bombAnimationData._freqAnimations = 5;
}

void MonsterController::setHealthBar(std::shared_ptr<cugl::scene2::ProgressBar> bar){
    _healthBar = bar;
}
void MonsterController::spawnBasicEnemy(cugl::Vec2 pos, OverWorld& overWorld){
    std::vector<std::shared_ptr<cugl::Texture>>& _textures = meleeAnimationData._textures;
    int _framesize = meleeAnimationData._framesize;
    int _framecols = meleeAnimationData._framecols;
    int _freqAnims = meleeAnimationData._freqAnimations;
    if (_textures.size() > 0)
    {
        int rows = _framesize / _framecols;
        if (_framesize % _framecols != 0)
        {
            rows++;
        }
        Size mySize(32,32);
        mySize /= 44;
        std::shared_ptr<MeleeEnemy> static_enemy = MeleeEnemy::alloc(pos, mySize, 3, 0);
        static_enemy->setShared(true);
        std::shared_ptr<cugl::scene2::SceneNode> topLevel = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
        topLevel->setAnchor(Vec2::ANCHOR_CENTER);
        std::shared_ptr<AnimationSceneNode> runAnimations = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
        // Temp PlaceHolder
        runAnimations->setAnchor(Vec2::ANCHOR_CENTER);
        std::shared_ptr<AnimationSceneNode> attackAnimations = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
        attackAnimations->setAnchor(Vec2::ANCHOR_CENTER);
        topLevel->setPosition(pos * 44); // temp scale
        static_enemy->setWalkingSceneNode(runAnimations);
        static_enemy->setAttackingSceneNode(attackAnimations);
        static_enemy->setFinalEnemy(topLevel);
        
        monsterControllerSceneNode->addChild(topLevel);
        _physicsWorld->initObstacle(static_enemy);
        static_enemy->setDebugColor(DYNAMIC_COLOR);
        static_enemy->setDebugScene(_debugNode);
        if (overWorld._isHost){
            _physicsWorld->getOwnedObstacles().insert({static_enemy,0});
        }
        //        static_enemy->setHealthBar(_healthBar);
        _pending.emplace(static_enemy);
    }
}

void MonsterController::spawnStaticBasicEnemy(cugl::Vec2 pos, OverWorld& overWorld){
    std::vector<std::shared_ptr<cugl::Texture>>& _textures = meleeAnimationData._textures;
    int _framesize = meleeAnimationData._framesize;
    int _framecols = meleeAnimationData._framecols;
    int _freqAnims = meleeAnimationData._freqAnimations;
    if (_textures.size() > 0)
    {
        int rows = _framesize / _framecols;
        if (_framesize % _framecols != 0)
        {
            rows++;
        }
        Size mySize(32,32);
        mySize /= 44;
        std::shared_ptr<StaticMeleeEnemy> static_enemy = StaticMeleeEnemy::alloc(pos, mySize, 3, 0);
        static_enemy->setShared(true);
        std::shared_ptr<cugl::scene2::SceneNode> topLevel = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
        topLevel->setAnchor(Vec2::ANCHOR_CENTER);
        std::shared_ptr<AnimationSceneNode> runAnimations = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
        // Temp PlaceHolder
        runAnimations->setAnchor(Vec2::ANCHOR_CENTER);
        std::shared_ptr<AnimationSceneNode> attackAnimations = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
        attackAnimations->setAnchor(Vec2::ANCHOR_CENTER);
        topLevel->setPosition(pos * 44); // temp scale
        static_enemy->setWalkingSceneNode(runAnimations);
        static_enemy->setAttackingSceneNode(attackAnimations);
        static_enemy->setFinalEnemy(topLevel);
                                            
        monsterControllerSceneNode->addChild(topLevel);
        _physicsWorld->initObstacle(static_enemy);
        static_enemy->setDebugColor(DYNAMIC_COLOR);
        static_enemy->setDebugScene(_debugNode);
        if (overWorld._isHost){
            _physicsWorld->getOwnedObstacles().insert({static_enemy,0});
        }
//        static_enemy->setHealthBar(_healthBar);
        _pending.emplace(static_enemy);
    }
}

void MonsterController::spawnBombEnemy(cugl::Vec2 pos, OverWorld& overWorld){
    std::vector<std::shared_ptr<cugl::Texture>>& _textures = bombAnimationData._textures;
    int _framesize = bombAnimationData._framesize;
    int _framecols = bombAnimationData._framecols;
    int _freqAnims = bombAnimationData._freqAnimations;
    if (_textures.size() > 0)
    {
        int rows = _framesize / _framecols;
        if (_framesize % _framecols != 0)
        {
            rows++;
        }
        Size mySize(32,32);
        mySize /= 44;
        std::shared_ptr<BombEnemy> bomb_enemy = BombEnemy::alloc(pos, mySize, 3, 0);
        bomb_enemy->setShared(true);
        std::shared_ptr<cugl::scene2::SceneNode> topLevel = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
        topLevel->setAnchor(Vec2::ANCHOR_CENTER);
        std::shared_ptr<AnimationSceneNode> runAnimations = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
        // Temp PlaceHolder
        runAnimations->setAnchor(Vec2::ANCHOR_CENTER);
        std::shared_ptr<AnimationSceneNode> attackAnimations = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
        attackAnimations->setAnchor(Vec2::ANCHOR_CENTER);
        topLevel->setPosition(pos * 44); // temp scale
        bomb_enemy->setWalkingSceneNode(runAnimations);
        bomb_enemy->setAttackingSceneNode(attackAnimations);
        bomb_enemy->setFinalEnemy(topLevel);
        
        monsterControllerSceneNode->addChild(topLevel);
        _physicsWorld->initObstacle(bomb_enemy);
        bomb_enemy->setDebugColor(DYNAMIC_COLOR);
        bomb_enemy->setDebugScene(_debugNode);
        if (overWorld._isHost){
            _physicsWorld->getOwnedObstacles().insert({bomb_enemy,0});
        }
        //        static_enemy->setHealthBar(_healthBar);
        _pending.emplace(bomb_enemy);
    }
}

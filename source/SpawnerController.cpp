//
//  SpawnerController.cpp
//
//  Created by Henry Gao on 2/23/24.
//
#include "SpawnerController.h"
#include <random>
#include <cmath>

SpawnerController::SpawnerController(){
    
}
SpawnerController::~SpawnerController(){

}

float generateRandomFloat(float low, float high) {
    // Static used for the seed to ensure it's only seeded once
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(low, high);
    return (float)dis(gen);
}

void SpawnerController::drawFlames(){
    auto itAnim = _curAnimations.begin();
    while(itAnim != _curAnimations.end()){
        std::shared_ptr<SpriteAnimationNode> curAnim = *itAnim;
        auto curA = itAnim;
        curAnim->update();
        itAnim++;
        if (curAnim->getFrame() == curAnim->getSpan()-1){
            _curAnimations.erase(curA);
            curAnim->removeFromParent();
        }
    }
}

void SpawnerController::update(MonsterController& monsterController, OverWorld& overWorld, float timestep){
    //_difficulty *= 1.00077046f;
   // _difficulty *= 1.00003851f;
    accumulatedTime += timestep;
    //cout << (std::to_string(difficulty));
    float timeDifficulty = (accumulatedTime / 180.0f) * (accumulatedTime / 180.0f) / (1 + accumulatedTime / 180.0f);
    float power = 1 + timeDifficulty + difficulty;
    float r = rand() / (float)RAND_MAX;
    r = r * r * r;
    power *= 0.8f + r;
    power = (float)sqrt(power);
    // Jank Change
    for(auto& spawner : _spawners) {
        spawner->update(monsterController, overWorld, timestep, power);
        if (spawner->canGenerateFlame()){
            spawner->reloadFlame();
            std::shared_ptr<SpriteAnimationNode> spawnAnim = SpriteAnimationNode::allocWithSheet(_spawnTexture, 2, 5, 10, 2);
            spawnAnim->setAnchor(Vec2::ANCHOR_CENTER);
            spawnAnim->setScale(power * cugl::Size(1,1)/32.0);
            spawnAnim->setPosition(spawner->getPos());
            _curAnimations.emplace(spawnAnim);
            animSpawnerNode->addChild(spawnAnim);
        }
    }
    for (auto& spawner : animationNodes){
        spawner->update();
    }
    drawFlames();
    
    auto it = _spawners.begin();
    while (it != _spawners.end()){
        std::shared_ptr<AbstractSpawner> spawner = *it;
        
        if (spawner->dead()){
            if (_isHost){
                _network->pushOutEvent(SpawnerDeathEvent::allocSpawnerDeathEvent(spawner->getPos()));
            }
            it = _spawners.erase(it);
            spawner->getSpawnerNode()->removeFromParent();
            difficulty += 0.1;

            std::shared_ptr<SimpleSpawner> simpleSpawner = std::dynamic_pointer_cast<SimpleSpawner>(spawner);

            if (simpleSpawner) {
                monsterController.spawnEnemyFromString(simpleSpawner->getSpawnType1(), 
                spawner->getPos().add(Vec2(generateRandomFloat(-0.1, 0.1), generateRandomFloat(-0.1, 0.1))),
                overWorld, power * 1.5f);

                monsterController.spawnEnemyFromString(simpleSpawner->getSpawnType2(), 
                spawner->getPos().add(Vec2(generateRandomFloat(-0.1, 0.1), generateRandomFloat(-0.1, 0.1))),
                overWorld, power * 1.5f);

                monsterController.spawnEnemyFromString(simpleSpawner->getSpawnType3(), 
                spawner->getPos().add(Vec2(generateRandomFloat(-0.1, 0.1), generateRandomFloat(-0.1, 0.1))),
                overWorld, power * 1.5f);
            }
        }
        else{
            ++it;
        }
    }
    
}

/** monster death event*/
void SpawnerController::processDeathEvent(const std::shared_ptr<DeathEvent>& deathEvent){
    std::shared_ptr<SpriteAnimationNode> deathAnim;
    if(deathEvent->isBomb()) {
        deathAnim = SpriteAnimationNode::allocWithSheet(_explodeTexture, 4, 5, 20, 1);
        deathAnim->setFrame(2);
        deathAnim->setAnchor(Vec2::ANCHOR_CENTER);
        deathAnim->setScale(deathEvent->getSize()/64.0);
    }
    else {
        deathAnim = SpriteAnimationNode::allocWithSheet(_deathTexture, 2, 5, 10, 6);
        deathAnim->setAnchor(Vec2::ANCHOR_CENTER);
        //spawnAnim->setScale(power * cugl::Size(1,1)/32.0);
        deathAnim->setScale(deathEvent->getSize()/32.0);
    }
    deathAnim->setPosition(deathEvent->getPos());
    _curAnimations.emplace(deathAnim);
    animSpawnerNode->addChild(deathAnim);
}

void SpawnerController::processSpawnerDeathEvent(const std::shared_ptr<SpawnerDeathEvent>& spawnerDeathEvent){
    Vec2 pos = spawnerDeathEvent->getPos();
    auto deathNode = SpriteAnimationNode::allocWithSheet(_deathSpawner, 1, 1, 1, 1);
    float scale = 1 / 48.0f;
    deathNode->setAnchor(cugl::Vec2::ANCHOR_CENTER);
    deathNode->setScale(scale);
    deathNode->setPosition(pos);
    baseSpawnerNode->addChild(deathNode);
    _audioController->playSFX(SPAWNER_DEATH, SPAWNER_DEATH);
}
bool SpawnerController::init(const std::vector<LevelModel::Spawner>& startLocs, std::shared_ptr<cugl::AssetManager> assets, std::shared_ptr<NetEventController> net, std::shared_ptr<AudioController> audioController) {
    _audioController = audioController;
    _spawners.clear();
    animationNodes.clear();
    _curAnimations.clear();
    baseSpawnerNode = cugl::scene2::SceneNode::alloc();
    animSpawnerNode = cugl::scene2::SceneNode::alloc();
    _spawnTexture = assets->get<cugl::Texture>("enemySpawn");
    _deathTexture = assets->get<cugl::Texture>("enemyDeath");
    _deathSpawner = assets->get<cugl::Texture>("spawnerDeath");
    _explodeTexture = assets->get<cugl::Texture>("explodingGate");
    _network = net;
    bool twoPlayer = net->getNumPlayers();
    for (int i =0; i< startLocs.size(); i++){
        LevelModel::Spawner spawner = startLocs.at(i);
        cugl::Vec2 pos = Vec2(spawner.spawnerX, spawner.spawnerY);
        int health = spawner.hp;
        int regDelay = twoPlayer ? spawner.regularDelay/2:spawner.regularDelay;
        std::shared_ptr<SimpleSpawner> curSpawner = std::make_shared<SimpleSpawner>(spawner.regularDelay,pos,health,spawner.initDelay,spawner.primaryEnemy, spawner.secondaryEnemy, spawner.tertiaryEnemy);
        auto drawNode = SpriteAnimationNode::allocWithSheet(assets->get<cugl::Texture>("spawner"), 1, 4, 4, 6);
        float scale = 1 / 48.0f;
        drawNode->setAnchor(cugl::Vec2::ANCHOR_CENTER);
        drawNode->setScale(scale);
        drawNode->setPosition(pos);

        std::shared_ptr<cugl::Texture> barImage = assets->get<cugl::Texture>("progress");
    
        float textureWidth = barImage->getWidth();
        float textureHeight = barImage->getHeight();
        
        std::shared_ptr<cugl::Texture> bg = barImage->getSubTexture(0/textureWidth, 320/textureWidth, 0/textureHeight, 45/textureHeight);
        std::shared_ptr<cugl::Texture> fg = barImage->getSubTexture(24/textureWidth, 296/textureWidth, 45/textureHeight, 90/textureHeight);
        std::shared_ptr<cugl::Texture> left_cap = barImage->getSubTexture(0/textureWidth, 24/textureWidth, 45/textureHeight, 90/textureHeight);
        std::shared_ptr<cugl::Texture> right_cap = barImage->getSubTexture(296/textureWidth, 320/textureWidth, 45/textureHeight, 90/textureHeight);
        
        std::shared_ptr<cugl::scene2::ProgressBar> _bar = cugl::scene2::ProgressBar::allocWithCaps(bg, fg, left_cap, right_cap);
        _bar->setProgress(1.0f);
        _bar->setPosition(drawNode->getWidth() * (1 / scale)/2, drawNode->getHeight() * (1 / scale));
        curSpawner->setHealthBar(_bar);
        drawNode->addChild(_bar);
        baseSpawnerNode->addChild(drawNode);
        curSpawner->setSpawnerNode(drawNode);
        _spawners.insert(curSpawner);
    }
    difficulty = 0;
    accumulatedTime = 0;
    return true;
}
void SpawnerController::setRootNode(const std::shared_ptr<scene2::SceneNode>& _worldNode, bool isHost){
    
    _isHost = isHost;
    // Add the Base Spawner Node
    _worldNode->addChild(baseSpawnerNode);
    
    
}

void SpawnerController::setAnimNode(const std::shared_ptr<scene2::SceneNode>& _worldNode){
    _worldNode->addChild(animSpawnerNode);
}
void SpawnerController::dispose(){
    _spawners.clear();
    animationNodes.clear();
    _curAnimations.clear();
    baseSpawnerNode = nullptr;
    animSpawnerNode = nullptr;
    _network = nullptr;
    _deathTexture = nullptr;
    _spawnTexture = nullptr;
    _deathSpawner = nullptr;
    _curAnimations.clear();
    _audioController = nullptr;
}

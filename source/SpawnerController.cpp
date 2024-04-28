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

void SpawnerController::drawFlames(float timestep){
    auto itAnim = _curAnimations.begin();
    while(itAnim != _curAnimations.end()){
        std::shared_ptr<SpriteAnimationNode> curAnim = *itAnim;
        auto curA = itAnim;
        curAnim->update();
        itAnim++;
        if (curAnim->getFrame() == curAnim->getSpan()-1){
            _curAnimations.erase(curA);
            animSpawnerNode->removeChild(curAnim);
        }
    }
}

void SpawnerController::update(MonsterController& monsterController, OverWorld& overWorld, float timestep){
    //_difficulty *= 1.00077046f;
   // _difficulty *= 1.00003851f;
    accumulatedTime += timestep;
    
    //cout << (std::to_string(difficulty));

    float timeDifficulty = (accumulatedTime / 90.0f) * (accumulatedTime / 90.0f) / (1 + accumulatedTime / 90.0f);
    float power = 1 + timeDifficulty + difficulty;
    for(auto& spawner : _spawners) {
        spawner->update(monsterController, overWorld, timestep, power);
        
    }

    for (auto& spawner : animationNodes){
        spawner->update();
    }
    
    auto it = _spawners.begin();
    while (it != _spawners.end()){
        std::shared_ptr<AbstractSpawner> spawner = *it;
        if(spawner->getSpawnCount() > 0){
            spawner->setSpawnCount(0);
            std::shared_ptr<SpriteAnimationNode> explodeAnim = SpriteAnimationNode::allocWithSheet(_spawnTexture, 2, 5, 10, 2);
            explodeAnim->setScale(cugl::Size(2,2)/64);
            explodeAnim->setAnchor(cugl::Vec2(0.5,0.3));
//            explodeAnim->setAnchor(Vec2::ANCHOR_CENTER);
            explodeAnim->setPosition(spawner->getPos());
            
            _curAnimations.emplace(explodeAnim);
            animSpawnerNode->addChild(explodeAnim);
        }
        else if (spawner->dead()){
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
        spawner->update(monsterController, overWorld, timestep, (float)std::pow(1.00231316, accumulatedTime) + difficulty);
        spawner->updateTime(timestep);
    }
    
    auto itAnim = _curAnimations.begin();
    while(itAnim != _curAnimations.end()){
        std::shared_ptr<SpriteAnimationNode> curAnim = *itAnim;
        auto curA = itAnim;
        curAnim->update();
        itAnim++;
        if (curAnim->getFrame() == curAnim->getSpan()-1){
            _curAnimations.erase(curA);
            animSpawnerNode->removeChild(curAnim);
        }
    }
    
}

bool SpawnerController::init(const std::vector<LevelModel::Spawner>& startLocs, std::shared_ptr<cugl::AssetManager> assets) {
    _spawners.clear();
    animationNodes.clear();
    _curAnimations.clear();
    baseSpawnerNode = cugl::scene2::SceneNode::alloc();
    animSpawnerNode = cugl::scene2::SceneNode::alloc();
    for (int i =0; i< startLocs.size(); i++){
        LevelModel::Spawner spawner = startLocs.at(i);
        cugl::Vec2 pos = Vec2(spawner.spawnerX, spawner.spawnerY);
        int health = spawner.hp;
        std::shared_ptr<SimpleSpawner> curSpawner = std::make_shared<SimpleSpawner>(spawner.regularDelay,pos,health,spawner.initDelay,spawner.primaryEnemy, spawner.secondaryEnemy, spawner.tertiaryEnemy);
//        auto drawNode = cugl::scene2::PolygonNode::allocWithTexture(assets->get<cugl::Texture>("spawner"));
        auto drawNode = SpriteAnimationNode::allocWithSheet(assets->get<cugl::Texture>("spawner"), 1,1,1);
        float scale = 1 / 48.0f;
        drawNode->setScale(scale);
//        drawNode->setContentSize(cugl::Size(1,1));
        drawNode->setPosition(pos);
        drawNode->setAnchor(cugl::Vec2::ANCHOR_CENTER);

        std::shared_ptr<cugl::Texture> barImage = assets->get<cugl::Texture>("progress");
    
        float textureWidth = barImage->getWidth();
        float textureHeight = barImage->getHeight();
        
        std::shared_ptr<cugl::Texture> bg = barImage->getSubTexture(0/textureWidth, 320/textureWidth, 0/textureHeight, 45/textureHeight);
        std::shared_ptr<cugl::Texture> fg = barImage->getSubTexture(24/textureWidth, 296/textureWidth, 45/textureHeight, 90/textureHeight);
        std::shared_ptr<cugl::Texture> left_cap = barImage->getSubTexture(0/textureWidth, 24/textureWidth, 45/textureHeight, 90/textureHeight);
        std::shared_ptr<cugl::Texture> right_cap = barImage->getSubTexture(296/textureWidth, 320/textureWidth, 45/textureHeight, 90/textureHeight);
        
        std::shared_ptr<cugl::scene2::ProgressBar> _bar = cugl::scene2::ProgressBar::allocWithCaps(bg, fg, left_cap, right_cap);
        _bar->setProgress(1.0f);
        _bar->setPosition(0, drawNode->getHeight() * (1 / scale));
        curSpawner->setHealthBar(_bar);
        drawNode->addChild(_bar);

//        animationNodes.push_back(drawNode);
        baseSpawnerNode->addChild(drawNode);
        curSpawner->setSpawnerNode(drawNode);
        _spawners.insert(curSpawner);
    }
    difficulty = 0;
    accumulatedTime = 0;
    updateCalls = 0;
    return true;
}
void SpawnerController::setRootNode(const std::shared_ptr<scene2::SceneNode>& _worldNode, bool isHost){
    
    _isHost = isHost;
    // Add the Base Spawner Node
    _worldNode->addChild(baseSpawnerNode);
    
    
}

void SpawnerController::setAnimNode(const std::shared_ptr<scene2::SceneNode>& _worldNode, bool isHost){
    
    _isHost = isHost;
    // Add the Anim Spawner Node
    _worldNode->addChild(animSpawnerNode);
    
    
}
void SpawnerController::dispose(){
    _spawners.clear();
    animationNodes.clear();
    _curAnimations.clear();
    baseSpawnerNode = nullptr;
    animSpawnerNode = nullptr;
    _network = nullptr;
}

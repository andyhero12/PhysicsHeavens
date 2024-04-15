//
//  StaticMeleeEnemy.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/24/24.
//

#include "StaticMeleeEnemy.h"
#define DISTANCE_CUTOFF 5

#define DYNAMIC_COLOR   Color4::YELLOW
/**
 * Generate a pair of Obstacle and SceneNode using the given parameters
 */
std::pair<std::shared_ptr<physics2::Obstacle>, std::shared_ptr<scene2::SceneNode>> StaticMeleeFactory::createObstacle(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex) {
    std::vector<std::shared_ptr<cugl::Texture>>& _textures = staticEnemyStruct._walkTextures;
    if (_textures.size() == 0){
        CULog("EMPTY TEXTURES");
    }
    int _framesize = staticEnemyStruct._framesize;
    int _framecols = staticEnemyStruct._framecols;
    int _freqAnims = staticEnemyStruct._freqAnimations;
    int rows = _framesize / _framecols;
    if (_framesize % _framecols != 0)
    {
        rows++;
    }
    std::shared_ptr<StaticMeleeEnemy> static_enemy = StaticMeleeEnemy::alloc(m_pos, m_size, m_health, m_targetIndex);
    
    // used to create progress bars
    std::shared_ptr<cugl::Texture> barImage = _assets->get<Texture>("progress");
    
    float textureWidth = barImage->getWidth();
    float textureHeight = barImage->getHeight();
    
    std::shared_ptr<cugl::Texture> bg = barImage->getSubTexture(0/textureWidth, 320/textureWidth, 0/textureHeight, 45/textureHeight);
    std::shared_ptr<cugl::Texture> fg = barImage->getSubTexture(24/textureWidth, 296/textureWidth, 45/textureHeight, 90/textureHeight);
    std::shared_ptr<cugl::Texture> left_cap = barImage->getSubTexture(0/textureWidth, 24/textureWidth, 45/textureHeight, 90/textureHeight);
    std::shared_ptr<cugl::Texture> right_cap = barImage->getSubTexture(296/textureWidth, 320/textureWidth, 45/textureHeight, 90/textureHeight);
    
    std::shared_ptr<cugl::scene2::ProgressBar> _bar = cugl::scene2::ProgressBar::allocWithCaps(bg, fg, left_cap, right_cap);
    _bar->setProgress(1.0f);
    static_enemy->setHealthBar(_bar);
    static_enemy->setDebugColor(DYNAMIC_COLOR);
    static_enemy->setAngleSnap(0); // Snap to the nearest degree
    
    std::shared_ptr<cugl::scene2::SceneNode> topLevel = scene2::SceneNode::allocWithBounds(m_size);
    topLevel->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<AnimationSceneNode> runAnimations = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
    runAnimations->setContentSize(m_size);
    // Temp PlaceHolder
    runAnimations->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<AnimationSceneNode> attackAnimations = AnimationSceneNode::allocWithTextures(_textures, rows,_framecols, _framesize, _freqAnims);
    attackAnimations->setAnchor(Vec2::ANCHOR_CENTER);
    attackAnimations->setContentSize(m_size);
    topLevel->setPosition(m_pos);
    static_enemy->setWalkingSceneNode(runAnimations);
    static_enemy->setAttackingSceneNode(attackAnimations);
    static_enemy->setFinalEnemy(topLevel);
    topLevel->setScale(m_size.height / _textures.at(0)->getHeight());
    static_enemy->setShared(true);
    
//        static_enemy->setHealthBar(_healthBar);
    return std::make_pair(static_enemy, topLevel);
#pragma mark END SOLUTION
}

/**
 * Helper method for converting normal parameters into byte vectors used for syncing.
 */
std::shared_ptr<std::vector<std::byte>> StaticMeleeFactory::serializeParams(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex) {
    // TODO: Use _serializer to serialize pos and scale (remember to make a shared copy of the serializer reference, otherwise it will be lost if the serializer is reset).
#pragma mark BEGIN SOLUTION
    _serializer.reset();
    _serializer.writeFloat(m_pos.x);
    _serializer.writeFloat(m_pos.y);
    _serializer.writeFloat(m_size.width);
    _serializer.writeFloat(m_size.height);
    _serializer.writeSint32(m_health);
    _serializer.writeSint32(m_targetIndex);
    return std::make_shared<std::vector<std::byte>>(_serializer.serialize());
#pragma mark END SOLUTION
}

/**
 * Generate a pair of Obstacle and SceneNode using serialized parameters.
 */
std::pair<std::shared_ptr<physics2::Obstacle>, std::shared_ptr<scene2::SceneNode>> StaticMeleeFactory::createObstacle(const std::vector<std::byte>& params) {
    // TODO: Use _deserializer to deserialize byte vectors packed by {@link serializeParams()} and call the regular createObstacle() method with them.
#pragma mark BEGIN SOLUTION
    _deserializer.reset();
    _deserializer.receive(params);
    float x = _deserializer.readFloat();
    float y = _deserializer.readFloat();
    Vec2 pos = Vec2(x,y);
    float w =_deserializer.readFloat();
    float h =_deserializer.readFloat();
    Size size = Size(w,h);
    int health = _deserializer.readSint32();
    int targetIndex = _deserializer.readSint32();
    return createObstacle(pos, size, health, targetIndex);
#pragma mark END SOLUTION
}

StaticMeleeEnemy::StaticMeleeEnemy(){}
bool StaticMeleeEnemy::init(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex){
    if (MeleeEnemy::init(m_pos, m_size, m_health, m_targetIndex)){
        std::string name("Static Melee Enemy");
        setName(name);
        original_pos = m_pos;
        return true;
    }
    return false;
}

void StaticMeleeEnemy::preUpdate(float dt, OverWorld& overWorld){
    if (_attackCooldown < 60){
        _attackCooldown++;
    }
    
    if (_counter < updateRate){
        _counter++;
    }
    //CULog("Boundary World Size in Melee Enemy Preupdate: %zu", overWorld.getWorld()->getBoundaryWorld().size());
    
    cugl::Vec2 dog_pos = overWorld.getDog()->getPosition();
    cugl::Vec2 my_pos = getPosition();
    
//    for (const auto &row : overWorld.getWorld()->getBoundaryWorld()) {
//        // Iterate over columns
//        for (std::shared_ptr<TileInfo> elem : row) {
//            std::cout << elem->type << " ";
//        }
//        std::cout << std::endl; // Newline after each row for better formatting
//    }
    
//    CULog("World Size is %d by %d", overWorld.getWorld()->getRows(), overWorld.getWorld()->getCols());
//    CULog("Search for path from (%d, %d) to (%d, %d)", start.x, start.y, end.x, end.y);
    
    // Check if there is a goal and we haven't reached it
    if((_goal.x == -1 && _goal.y == -1) || _goal.distance(getPosition()) < 0.02){
        
        // Initialize start and end for the search
        WorldSearchVertex start = WorldSearchVertex(static_cast<int>(my_pos.x), static_cast<int>(my_pos.y), overWorld.getWorld());
        WorldSearchVertex end = WorldSearchVertex(static_cast<int>(dog_pos.x), static_cast<int>(dog_pos.y), overWorld.getWorld());
        
        _pathfinder->SetStartAndGoalStates(start, end);
        
        unsigned int SearchState = _pathfinder->SearchStep();
        
        // Perform the search
        while( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_SEARCHING )
        {
            SearchState = _pathfinder->SearchStep();
        }
        
        if( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_FAILED ){
            CULog("Search Failed");
        }
        
        if( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_OUT_OF_MEMORY ){
            CULog("Search Out of Memory");
        }
        
        Vec2 next_pos = getPosition();
        
        // Check if the search was successful
        if( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_SUCCEEDED ){
            
            CULog("Found Solution from (%d, %d) to (%d, %d)", start.x, start.y, end.x, end.y);
            
            WorldSearchVertex* node = _pathfinder->GetSolutionStart();
            
            WorldSearchVertex* next_node = _pathfinder->GetSolutionNext();
            
            if(next_node){
                next_pos = Vec2((int) next_node->x + 0.5, (int) next_node->y + 0.5);
            }
            
                    for( ;; ){
                        
                        WorldSearchVertex* prev_node = _pathfinder->GetSolutionPrev();
                        _pathfinder->GetSolutionNext();
                        node = _pathfinder->GetSolutionNext();
            
                        if( !node ){
                            break;
                        }
                        
                        CULog("Prev Node: (%d, %d)", static_cast<int>(prev_node->x), static_cast<int>(prev_node->y));
                        CULog("Next Node: (%d, %d)", static_cast<int>(node->x), static_cast<int>(node->y));
            
                    };
            
            _pathfinder->FreeSolutionNodes();
            CULog("Found Solution from (%d, %d) to (%d, %d) test", _pathfinder->GetSolutionStart()->x, _pathfinder->GetSolutionStart()->y, _pathfinder->GetSolutionEnd()->x, _pathfinder->GetSolutionEnd()->y);
            CULog("Found Solution from (%d, %d) to (%d, %d) test2", _pathfinder->GetSolutionStart()->x, _pathfinder->GetSolutionStart()->y, _pathfinder->GetSolutionEnd()->x, _pathfinder->GetSolutionEnd()->y);
            _pathfinder->GetSolutionNext();
            CULog("%d", _pathfinder->SearchStep());
        }
        
        setGoal(next_pos);
    }
    
    
    
    cugl::Vec2 org_dist = dog_pos - original_pos;
    float distance = org_dist.length();
    
    cugl::Vec2 direction;
//    CULog("distance %f %d", distance, DISTANCE_CUTOFF);
//    if (distance > DISTANCE_CUTOFF){ // too far from origin return
//        direction = original_pos - getPosition();
//        if (overWorld._isHost && _counter >= updateRate){
//            _counter = 0;
//            if (direction.lengthSquared() >= 1){
//                setVX(direction.normalize().x);
//                setVY(direction.normalize().y);
//            }else{
//                setVX(0);
//                setVY(0);
//            }
//        }
//    }else{ // chase dog
        //CULog("Chasing Dog at (%f, %f)", dog_pos.x, dog_pos.y);
    
    CULog("Going to Goal: (%f, %f)", _goal.x, _goal.y);
    CULog("Dog Position: (%f, %f)", dog_pos.x, dog_pos.y);
    CULog("Enemy Position: (%f, %f)", getPosition().x, getPosition().y);
    CULog("Enemy Position rounded: (%d, %d)", (int) getPosition().x, (int) getPosition().y);
    CULog("-----------------------");
//        if(_goal.distance(getPosition()) < 0.4){
//            return;
//        }

    
    direction = _goal - getPosition(); //Vec2((int) getPosition().x, (int) getPosition().y);
    CULog("Direction: (%f, %f)", direction.normalize().x, direction.normalize().y);
    
        if (overWorld._isHost){
//=======
//    if (distance > DISTANCE_CUTOFF){ // too far from origin return
//        direction = original_pos - getPosition();
//        if (overWorld._isHost && _counter >= updateRate){
//            _counter = 0;
//            if (direction.lengthSquared() >= 1){
//                setVX(direction.normalize().x);
//                setVY(direction.normalize().y);
//            }else{
//                setVX(0);
//                setVY(0);
//            }
//            _prevDirection =_curDirection;
//            _curDirection = AnimationSceneNode::convertRadiansToDirections(direction.getAngle());
//        }
//    }else{ // chase dog
//        direction = dog_pos - getPosition();
//        if (overWorld._isHost && _counter >= updateRate){
//            _counter = 0;
//>>>>>>> main
            setVX(direction.normalize().x);
            setVY(direction.normalize().y);
            _prevDirection =_curDirection;
            _curDirection = AnimationSceneNode::convertRadiansToDirections(direction.getAngle());
        }
}

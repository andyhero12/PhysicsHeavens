//
//  AbstractEnemy.cpp
//  Heaven
//
//  Created by Henry Gao on 4/27/24.
//

#include <stdio.h>
#include "AbstractEnemy.h"


bool AbstractEnemy::init(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex){
    bool result = physics2::BoxObstacle::init(m_pos,m_size);
    
    if (result){
        _counter = 0;
        updateRate = 15;
        clearSharingDirtyBits();
        setShared(true);
        setDensity(DEFAULT_DENSITY);
        setFriction(DEFAULT_FRICTION);
        setRestitution(DEFAULT_RESTITUTION);
        setFixedRotation(true);
        
        curAction = EnemyActions::SPAWN;
        _health = m_health;
        _maxHealth = m_health;
        targetIndex = m_targetIndex;
        _prevDirection =AnimationSceneNode::Directions::EAST;
        _curDirection = AnimationSceneNode::Directions::EAST;
        _pathfinder = std::make_shared<AStarSearch<WorldSearchVertex>>();
        _time = 0;
        _wanderAngle = 0.0f;
        timeSinceLastMajorChange = 0.0f;
        
        _inContact = false;
        return true;
    }
    return false;
}

void AbstractEnemy::update(float delta) {
    Obstacle::update(delta);
}

void AbstractEnemy::postUpdate(){
    _prevDirection =_curDirection;
    if(!_inContact){
        Vec2 direction = getLinearVelocity();
        _curDirection = AnimationSceneNode::convertRadiansToDirections(direction.getAngle());
    }

    runAnimations->animate(_curDirection, curAction != EnemyActions::ATTACK);
    attackAnimations->animate(_curDirection, curAction == EnemyActions::ATTACK);
    
}

#pragma mark -
#pragma mark PATH FINDING

/** Sets a new goal for this enemy to go to. Returns true if pathfinding to the goal was successful */
bool AbstractEnemy::setGoal(Vec2 goal, std::shared_ptr<World> world){
    
    // If we already pathfound recently, don't path find again
    if(_pathfindTimer < PATHFIND_COOLDOWN){
        return searchSuccess();
    }
    
    // Garbage collect the nodes used for the previous path if they exist
    if(_pathfinder->GetSolutionEnd() && _pathfinder->SearchStep() == AStarSearch<WorldSearchVertex>::SEARCH_STATE_SUCCEEDED){
//            CULog("Garbage Collecting Path...");
        _pathfinder->FreeSolutionNodes();
    }
    
    cugl::Vec2 my_pos = getPosition();
    
    // Initialize start and end for the search
    WorldSearchVertex start = WorldSearchVertex(static_cast<int>(my_pos.x), static_cast<int>(my_pos.y), world);
    WorldSearchVertex end = WorldSearchVertex(static_cast<int>(goal.x), static_cast<int>(goal.y), world);
    _pathfinder->SetStartAndGoalStates(start, end);
    
    // Perform the search
    unsigned int SearchState = _pathfinder->SearchStep();
    while( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_SEARCHING )
    {
        SearchState = _pathfinder->SearchStep();
    }
    
    // Check if the search was successful
    if( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_SUCCEEDED ){
        
//            CULog("Found Solution from (%d, %d) to (%d, %d)", start.x, start.y, end.x, end.y);
        
        _pathfinder->GetSolutionStart();
        WorldSearchVertex* nextNode = _pathfinder->GetSolutionNext();
        
        if(nextNode){
            _nextStep = Vec2((int) nextNode->x, (int) nextNode->y);
        } else{
            _nextStep = my_pos;
        }
        
        return true;
    }
    
    if( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_FAILED ){
        CULog("Search Failed");
    }
    
    if( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_OUT_OF_MEMORY ){
        CULog("Search Out of Memory");
    }
    
    return false;
};


void AbstractEnemy::goToGoal(){
    
    // Get the goal
    WorldSearchVertex* goalNode = _pathfinder->GetSolutionEnd();
    cugl::Vec2 direction;
    
    // If there is no goal or we are already at the goal, do nothing
    if(!goalNode || atGoal() || _nextStep.x == -1){
        return;
    }
    
    Vec2 goalTile = Vec2(goalNode->x + 0.5, goalNode->y + 0.5);
    
    // If we are very close to the goal, go directly to it instead of using pathfinding
    if(getPosition().distance(goalTile) < CLOSE_DISTANCE){
        direction = goalTile - getPosition();
    }
    else {
        
        // If we strayed too far from the pathfinding path, restart pathfinding
        Vec2 nextTile = Vec2(_nextStep.x + 0.5, _nextStep.y + 0.5);
        if(getPosition().distance(nextTile) > STRAY_DISTANCE){
            CULog("Recalculating Path...");
            setGoal(Vec2(goalNode->x, goalNode->y), goalNode->_world);
        }
        
        // If we already reached the next tile, get the next node along the path and set it as the next tile
        if(atTile(_nextStep)){
            
            WorldSearchVertex* nextNode = _pathfinder->GetSolutionNext();
            
            if(nextNode){
                _nextStep = Vec2((int) nextNode->x, (int) nextNode->y);
            } else{
                return;
            }
        
        }
        
        direction = _nextStep - getPosition();
    }
    
    //Move towards the next tile
    setVX(direction.normalize().x * 0.75);
    setVY(direction.normalize().y * 0.75);
    setX(getX());
    setY(getY());
    _prevDirection =_curDirection;
    _curDirection = AnimationSceneNode::convertRadiansToDirections(direction.getAngle());
    
    
    
    
    
    // If we have been stuck on the same tile for too long, move randomly and then restart pathfinding
    
};


bool AbstractEnemy::atGoal(){
    // Get the goal
    WorldSearchVertex* goalNode = _pathfinder->GetSolutionEnd();
    
    Vec2 goal = Vec2(goalNode->x, goalNode->y);
    
    return atTile(goal);
};

bool AbstractEnemy::atTile(Vec2 tile){
    
    // Get the center of the tile
    Vec2 tileCenter = Vec2(tile.x + 0.5, tile.y + 0.5);
    
    // Check if the enemy position is close to the center
    if(getPosition().distance(tileCenter) < 0.05 ){
        return true;
    };
    
    return false;
};


#pragma mark -
#pragma mark STATE UPDATES

void AbstractEnemy::handleSpawn() {
    setHealth(_maxHealth);
    _wanderAngle = 0.0f;
    timeSinceLastMajorChange = 0.0f;
    curAction = EnemyActions::WANDER;
    
}

void AbstractEnemy::handleWander(float dt){
    // Update time since last major direction change
     timeSinceLastMajorChange += dt;

     // Random device and generator setup
     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_real_distribution<> minorChange(-wanderStrength, wanderStrength);
     std::uniform_real_distribution<> majorChange(0.0f, 360.0f);

     // Apply a minor random adjustment regularly
     _wanderAngle += minorChange(gen);

     // Check to change directions
     if (timeSinceLastMajorChange >= directionChangeInterval) {
         timeSinceLastMajorChange = 0.0f; // Reset the timer
     }
    if(timeSinceLastMajorChange == 0.0f){
        _wanderAngle = majorChange(gen); // new random angle
    }
     // Normalize the angle
     _wanderAngle = std::fmod(_wanderAngle, 360.0f);
     if (_wanderAngle < 0) _wanderAngle += 360.0f;

     // Calculate new velocity based on the wander angle
     float radAngle = _wanderAngle * (M_PI / 180.0f);
     float vx = cos(radAngle);
     float vy = sin(radAngle);

     // Set the new velocity, scaled by a speed factor
     float speed = 0.5f;
     setVX(vx * speed);
     setVY(vy * speed);
}

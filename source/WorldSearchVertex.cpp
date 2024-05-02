//
//  WorldSearchVertex.cpp
//  Heaven
//
//  Created by Colin Wu on 4/9/24.
//

#include "WorldSearchVertex.h"

bool WorldSearchVertex::IsSameState( WorldSearchVertex &rhs )
{

    return x == rhs.x && y == rhs.y;
}

size_t WorldSearchVertex::Hash()
{
    size_t h1 = std::hash<float>{}(x);
    size_t h2 = std::hash<float>{}(y);
    return h1 ^ (h2 << 1);
}

float WorldSearchVertex::GoalDistanceEstimate( WorldSearchVertex &nodeGoal )
{
    return abs(x - nodeGoal.x) + abs(y - nodeGoal.y);
}

bool WorldSearchVertex::IsGoal( WorldSearchVertex &nodeGoal )
{
    return x == nodeGoal.x && y == nodeGoal.y;
}

// This generates the successors to the given Node. It uses a helper function called
// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
// is done for each node internally, so here you just set the state information that
// is specific to the application
bool WorldSearchVertex::GetSuccessors( AStarSearch<WorldSearchVertex> *astarsearch, WorldSearchVertex *parent_node )
{
    int parent_x = -1;
    int parent_y = -1;

    if( parent_node )
    {
        parent_x = parent_node->x;
        parent_y = parent_node->y;
    }
    const std::shared_ptr<World> curWorld = _world.lock();
    WorldSearchVertex NewNode(0, 0, _world);
    // push each possible move except allowing the search to go backwards
    for (auto& diff : direction_list) {
        NewNode = WorldSearchVertex( x - diff.first, y - diff.second, _world);
        if(curWorld->isPassable(NewNode.x, NewNode.y) && !((parent_x == NewNode.x) && (parent_y == NewNode.y))){
            astarsearch->AddSuccessor( NewNode );
        }
    }
    return true;
}

float WorldSearchVertex::GetCost( WorldSearchVertex &successor )
{
    if(closeToEdge()){
        return 5;
    }else {
        return 1;
    }
}

bool WorldSearchVertex::closeToEdge(){
    //Create list of all possible next tiles
    const std::shared_ptr<World> curWorld = _world.lock();
    return curWorld->isPassable(x,y+1) && curWorld->isPassable(x,y-1) && curWorld->isPassable(x+1,y) && curWorld->isPassable(x-1,y);
}

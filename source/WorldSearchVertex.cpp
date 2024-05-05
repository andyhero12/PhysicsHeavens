//
//  WorldSearchVertex.cpp
//  Heaven
//
//  Created by Colin Wu on 4/9/24.
//

#include "WorldSearchVertex.h"
static inline int generateRandomCost(int low, int high)
{
    // Static used for the seed to ensure it's only seeded once
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(low, high); // Range is 1 to 3, inclusive
    return dis(gen);
}
bool WorldSearchVertex::IsSameState( WorldSearchVertex &rhs ) const
{
    return (x == rhs.x) && (y == rhs.y);
}

size_t WorldSearchVertex::Hash()
{
    size_t h1 = std::hash<float>{}(x);
    size_t h2 = std::hash<float>{}(y);
    return h1 ^ (h2 << 1);
}

float WorldSearchVertex::GoalDistanceEstimate( WorldSearchVertex &nodeGoal ) const
{
    return (abs(x - nodeGoal.x) + abs(y - nodeGoal.y)) * 20 ;
}

bool WorldSearchVertex::IsGoal( WorldSearchVertex &nodeGoal ) const
{
    return (x == nodeGoal.x) && (y == nodeGoal.y);
}

// This generates the successors to the given Node. It uses a helper function called
// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
// is done for each node internally, so here you just set the state information that
// is specific to the application
bool WorldSearchVertex::GetSuccessors( AStarSearch<WorldSearchVertex> *astarsearch, WorldSearchVertex *parent_node) const
{
    
    int parent_x = -1;
    int parent_y = -1;

    if( parent_node )
    {
        parent_x = parent_node->x;
        parent_y = parent_node->y;
    }
    
    // push each possible move except allowing the search to go backwards
    for (const auto& diff : adj_list) {
        WorldSearchVertex NewNode = WorldSearchVertex( x - diff.first, y - diff.second, _world);
        if(_world->isPassable(NewNode.x, NewNode.y) && !((parent_x == NewNode.x) && (parent_y == NewNode.y))){
            astarsearch->AddSuccessor( NewNode );
        }
    }
    
    return true;
}

float WorldSearchVertex::GetCost( WorldSearchVertex &successor ) const
{
    int costOrg = successor.closeToEdge() ? 100 : 10;
    
    int noise = generateRandomCost(-10,10);
    
    return costOrg + noise;
}

bool WorldSearchVertex::closeToEdge() const {

    for (const auto& diff : adj_list) {
        if(!(_world->isPassable(x - diff.first, y - diff.second))){
            return true;
        }
    }

    return false;
}

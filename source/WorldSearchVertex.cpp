//
//  WorldSearchVertex.cpp
//  Heaven
//
//  Created by Colin Wu on 4/9/24.
//

#include "WorldSearchVertex.h"

bool WorldSearchVertex::IsSameState( WorldSearchVertex &rhs )
{

    // same state in a maze search is simply when (x,y) are the same
    if( (x == rhs.x) &&
        (y == rhs.y) )
    {
        return true;
    }
    else
    {
        return false;
    }

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

    if( (x == nodeGoal.x) &&
        (y == nodeGoal.y) )
    {
        return true;
    }

    return false;
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
    
    
    WorldSearchVertex NewNode(0, 0, _world);

    // push each possible move except allowing the search to go backwards

    if( _world->isPassable(x - 1, y)
        && !((parent_x == x-1) && (parent_y == y))
      )
    {
        NewNode = WorldSearchVertex( x-1, y, _world);
        astarsearch->AddSuccessor( NewNode );
    }

    if( _world->isPassable(x, y - 1)
        && !((parent_x == x) && (parent_y == y-1))
      )
    {
        NewNode = WorldSearchVertex( x, y-1, _world);
        astarsearch->AddSuccessor( NewNode );
    }

    if( _world->isPassable(x + 1, y)
        && !((parent_x == x+1) && (parent_y == y))
      )
    {
        NewNode = WorldSearchVertex( x+1, y, _world );
        astarsearch->AddSuccessor( NewNode );
    }

        
    if( _world->isPassable(x, y + 1)
        && !((parent_x == x) && (parent_y == y+1))
        )
    {
        NewNode = WorldSearchVertex( x, y + 1, _world);
        astarsearch->AddSuccessor( NewNode );
    }
    
    return true;
}

float WorldSearchVertex::GetCost( WorldSearchVertex &successor )
{
    return 0;
}

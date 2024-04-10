//
//  WorldSearchVertex.hpp
//  Heaven
//
//  Created by Colin Wu on 4/9/24.
//

#ifndef WorldSearchVertex_h
#define WorldSearchVertex_h

#include <stdio.h>
#include "stlastar.h"

class WorldSearchVertex{
private:
public:
    
    int x;
    int y;
    
    WorldSearchVertex() { x = y = 0; }
    WorldSearchVertex( int px, int py ) { x=px; y=py; }
    
    float GoalDistanceEstimate( WorldSearchVertex &nodeGoal );
    bool IsGoal( WorldSearchVertex &nodeGoal );
    bool GetSuccessors( AStarSearch<WorldSearchVertex> *astarsearch, WorldSearchVertex *parent_node );
    float GetCost( WorldSearchVertex &successor );
    bool IsSameState( WorldSearchVertex &rhs );
    size_t Hash();
};

#endif /* WorldSearchVertex_hpp */

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
#include "World.h"

class WorldSearchVertex{
private:
    // Returns whether this node is close to an impassible corner
    bool closeToEdge() const;
    
public:
    
    // the world this node is in
    const World* _world;
    
    // x coord of this node
    int x;
    
    // y coord of this node
    int y;
    
    WorldSearchVertex( int px, int py,const World* world) { x=px; y=py;
        _world = world;
    }
    
    // WARNING: This default consrtuctor is needed by the astarstl class for some reason, this implementation might be incorrect
    WorldSearchVertex(){ 
        x = y = 0;
        _world = nullptr;
    }
    
    ~WorldSearchVertex(){
        _world = nullptr;
    }
    
    // The heurstic used to determine the distance to the goal
    float GoalDistanceEstimate( WorldSearchVertex &nodeGoal ) const;
    
    // Whether this node is the goal
    bool IsGoal( WorldSearchVertex &nodeGoal );
    
    // Adds the nodes you can move to from this node to the pathfinder
    bool GetSuccessors( AStarSearch<WorldSearchVertex> *astarsearch, WorldSearchVertex *parent_node );
    
    //The cost of moving to a successor
    float GetCost( WorldSearchVertex &successor ) const;
    
    // Whether rhs is the current node
    bool IsSameState( WorldSearchVertex &rhs );
    
    // The hash of this node
    size_t Hash();
};

#endif /* WorldSearchVertex_hpp */

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
    static constexpr std::array<std::pair<int, int>, 8> adj_list = {
        std::make_pair(0, 1),
        std::make_pair(0, -1),
        std::make_pair(1, 0),
        std::make_pair(-1, 0),
        std::make_pair(-1, -1),
        std::make_pair(1, 1),
        std::make_pair(-1, 1),
        std::make_pair(1, -1)
    };
public:
    
    // the world this node is in
    const World* _world;
    
    // x coord of this node
    int x;
    
    // y coord of this node
    int y;
    
    WorldSearchVertex( int px, int py, const World* world)
    : x{px}
    , y{py}
    , _world{world}
    {
    }
    
    // WARNING: This default consrtuctor is needed by the astarstl class for some reason, this implementation might be incorrect
    WorldSearchVertex()
    : x{0}
    , y{0}
    , _world{0}
    {
    }
    
    // The heurstic used to determine the distance to the goal
    float GoalDistanceEstimate( WorldSearchVertex &nodeGoal ) const;
    
    // Whether this node is the goal
    bool IsGoal( WorldSearchVertex &nodeGoal ) const;
    
    // Adds the nodes you can move to from this node to the pathfinder
    bool GetSuccessors( AStarSearch<WorldSearchVertex> *astarsearch, WorldSearchVertex *parent_node ) const;
    
    //The cost of moving to a successor
    float GetCost( WorldSearchVertex &successor ) const;
    
    bool IsSameState( WorldSearchVertex &rhs ) const
    {
        return (x == rhs.x) && (y == rhs.y);
    }
    // The hash of this node
    size_t Hash();
};

#endif /* WorldSearchVertex_hpp */

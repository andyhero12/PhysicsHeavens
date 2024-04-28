//
//  CollisionListener.hpp
//  Heaven
//
//  Created by Henry Gao on 4/27/24.
//

#ifndef CollisionListener_h
#define CollisionListener_h

#include <stdio.h>
#include <cugl/cugl.h>
#include <box2d/box2d.h>
#include "AbstractEnemy.h"

class CollisionListener : public b2ContactListener {
public:
    CollisionListener() = default;
    virtual ~CollisionListener() = default;

    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
    
    static std::shared_ptr<CollisionListener> alloc() {
        return std::make_shared<CollisionListener>();
    }

};

#endif /* CollisionListener_hpp */

//
//  EnemyContactListener.hpp
//  Heaven
//
//  Created by Henry Gao on 4/27/24.
//

#ifndef ContactListener_h
#define ContactListener_h

#include <stdio.h>
#include<cugl/cugl.h>
#include "AbstractEnemy.h"
#include <Box2D/Box2D.h>


class ContactListener : public b2ContactListener {
public:
    ContactListener() = default;
    virtual ~ContactListener() = default;

    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
};



#endif /* EnemyContactListener_hpp */

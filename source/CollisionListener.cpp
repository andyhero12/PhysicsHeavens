//
//  CollisionListener.cpp
//  Heaven
//
//  Created by Henry Gao on 4/27/24.
//

#include "CollisionListener.h"


void CollisionListener::BeginContact(b2Contact* contact) {
    auto* bodyA = contact->GetFixtureA()->GetBody();
    auto* bodyB = contact->GetFixtureB()->GetBody();

    // Check if either body is the correct type and then cast
    auto* enemyA = dynamic_cast<AbstractEnemy*>(reinterpret_cast<AbstractEnemy*>(bodyA->GetUserData().pointer));
    auto* enemyB = dynamic_cast<AbstractEnemy*>(reinterpret_cast<AbstractEnemy*>(bodyB->GetUserData().pointer));

    if (enemyA) {
        enemyA->setInContact(true);
    }
    if (enemyB) {
        enemyB->setInContact(true);
    }
}

void CollisionListener::EndContact(b2Contact* contact) {
    auto* bodyA = contact->GetFixtureA()->GetBody();
    auto* bodyB = contact->GetFixtureB()->GetBody();

    // Check if either body is the correct type and then cast
    auto* enemyA = dynamic_cast<AbstractEnemy*>(reinterpret_cast<AbstractEnemy*>(bodyA->GetUserData().pointer));
    auto* enemyB = dynamic_cast<AbstractEnemy*>(reinterpret_cast<AbstractEnemy*>(bodyB->GetUserData().pointer));

    if (enemyA) {
        enemyA->setInContact(false);
    }
    if (enemyB) {
        enemyB->setInContact(false);
    }
}

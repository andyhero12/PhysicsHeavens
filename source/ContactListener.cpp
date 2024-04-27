//
//  EnemyContactListener.cpp
//  Heaven
//
//  Created by Henry Gao on 4/27/24.
//

#include "ContactListener.h"

void ContactListener::BeginContact(b2Contact* contact) {
    auto* bodyA = contact->GetFixtureA()->GetBody();
    auto* bodyB = contact->GetFixtureB()->GetBody();

    // Check if either body is the correct type and then cast
    auto* enemyA = dynamic_cast<AbstractEnemy*>(reinterpret_cast<AbstractEnemy*>(bodyA->GetUserData().pointer));
    auto* enemyB = dynamic_cast<AbstractEnemy*>(reinterpret_cast<AbstractEnemy*>(bodyB->GetUserData().pointer));

    // Set inContact to true if the object is an AbstractEnemy
    if (enemyA) {
        enemyA->setInContact(true);
    }
    if (enemyB) {
        enemyB->setInContact(true);
    }
}

void ContactListener::EndContact(b2Contact* contact) {
    auto* bodyA = contact->GetFixtureA()->GetBody();
    auto* bodyB = contact->GetFixtureB()->GetBody();

    // Check if either body is the correct type and then cast
    auto* enemyA = dynamic_cast<AbstractEnemy*>(reinterpret_cast<AbstractEnemy*>(bodyA->GetUserData().pointer));
    auto* enemyB = dynamic_cast<AbstractEnemy*>(reinterpret_cast<AbstractEnemy*>(bodyB->GetUserData().pointer));

    // Set inContact to false if the object is an AbstractEnemy
    if (enemyA) {
        enemyA->setInContact(false);
    }
    if (enemyB) {
        enemyB->setInContact(false);
    }
}


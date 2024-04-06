//
//  AttackPolygons.cpp
//
//  Created by Andrew Cheng on 3/12/24.
//

#include "AttackPolygons.h"
#define BITE_AGE 2

#include "NLDog.h"
// used to translate attacks to originate from the dog's head
#define BITE_HEAD_OFFSET_RATIO 0.8f
#define SHOOT_HEAD_OFFSET_RATIO 1.5f

void ActionPolygon::update(){
    _age++;
    // update animation when needed
}

ActionPolygon::ActionPolygon(Action curAction, Poly2& mintPoly, int mx)
: polygonAction{curAction}
, internalPolygon{mintPoly}
, _age{0}
, _maxage{mx}
, _scale{1.0}
{
    actionNode = cugl::scene2::PolygonNode::allocWithPoly(mintPoly);
}
void ActionPolygon::drawShoot(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
    batch->setTexture(nullptr);
    batch->setColor(Color4::RED);
    batch->fill(internalPolygon);
}
void ActionPolygon::drawExplode(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
    batch->setTexture(nullptr);
    batch->setColor(Color4::BLUE);
    batch->fill(internalPolygon);
}
void ActionPolygon::drawBite(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
    batch->setTexture(nullptr);
    batch->setColor(Color4::GREEN);
    batch->fill(internalPolygon);
}
void ActionPolygon::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
    switch (polygonAction){
        case Action::BITE:
            drawBite(batch,size);
            break;
        case Action::SHOOT:
            drawShoot(batch,size);
            break;
        case Action::EXPLODE:
            drawExplode(batch,size);
            break;
        default:
            CULog("action not used %d\n", polygonAction);
            break;
    };
}


AttackPolygons::AttackPolygons()
: max_age{25}
{
    
}

bool AttackPolygons::init(){
    currentAttacks.clear();
    attackPolygonNode = cugl::scene2::SceneNode::alloc();
    return true;
}
void AttackPolygons::update(){
    auto it =currentAttacks.begin();
    while (it != currentAttacks.end()){
        std::shared_ptr<ActionPolygon> curAction = *it;
        curAction->update();
        auto curIt = it++;
        if (curAction->expired()){
            attackPolygonNode->removeChild(curAction->getActionNode());
            currentAttacks.erase(curIt);
        }
    }
}

void AttackPolygons::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
    for (const std::shared_ptr<ActionPolygon>& action : currentAttacks){
        action->draw(batch, size);
    }
}

void AttackPolygons::addShoot(Vec2 center, float angle, float shootRadius){
    float degree = 60;
    PolyFactory curFactory;
    Poly2 resultingPolygon_shoot = curFactory.makeArc(center, shootRadius, angle + degree, degree);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(Action::SHOOT, resultingPolygon_shoot, max_age);
    attackPolygonNode->addChild(curPtr->getActionNode());
    Vec2 offset = Vec2(SDL_cosf((angle + 90) * 3.14f / 180), SDL_sinf((angle + 90) * 3.14f / 180)) * DOG_SIZE.x * SHOOT_HEAD_OFFSET_RATIO;
    curPtr->getActionNode()->setPosition(center.add(offset));
    curPtr->getActionNode()->setPosition(center);
    currentAttacks.insert(curPtr);
}

void AttackPolygons::addExplode(Vec2 center, float explosionRad){
    PolyFactory curFactory;
    Poly2 resultingPolygon = curFactory.makeCircle(center, explosionRad);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(Action::EXPLODE, resultingPolygon,max_age);
    attackPolygonNode->addChild(curPtr->getActionNode());
    curPtr->getActionNode()->setPosition(center);
    currentAttacks.insert(curPtr);
}

void AttackPolygons::addBite(Vec2 center, float angle, float explosionRad){
    PolyFactory curFactory;
    Poly2 resultingPolygon = curFactory.makeArc(center, explosionRad, angle, 180);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(Action::BITE, resultingPolygon,BITE_AGE);
    attackPolygonNode->addChild(curPtr->getActionNode());
    Vec2 offset = Vec2(SDL_cosf((angle + 90) * 3.14f / 180), SDL_sinf((angle + 90) * 3.14f / 180)) * DOG_SIZE.x * BITE_HEAD_OFFSET_RATIO;
    curPtr->getActionNode()->setPosition(center.add(offset));
    currentAttacks.insert(curPtr);
}

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
#define ANIM_FREQ 1
#define BITE_SCALE 0.02f

void ActionPolygon::update(){
    if(!_polygon){
        if(_freq == ANIM_FREQ){
            _expired = (spriteActionNode->getFrame() + 1) == spriteActionNode->getSpan();
            if(_expired){
                return;
            }
            _freq = 0;
            spriteActionNode->setFrame((spriteActionNode->getFrame() + 1) % spriteActionNode->getSpan());
        }
        else{
            _freq += 1;
        }
    }
    else{
        _age++;
        _expired = _age >= _maxage;
    }
    // update animation when needed
}


std::shared_ptr<cugl::scene2::SceneNode> ActionPolygon::getActionNode(){
    if(_polygon){
        return polyActionNode;
    }
    else{
        return spriteActionNode;
    }
}


ActionPolygon::ActionPolygon(std::shared_ptr<cugl::scene2::SpriteNode> actionSprite, Action curAction, Poly2& mintPoly, int mx, float scale)
: ActionPolygon(curAction, mintPoly, mx, scale)
{
    _expired = false;
    _polygon = false;
    spriteActionNode = actionSprite;
    _scale = BITE_SCALE * scale;
    spriteActionNode->setScale(_scale);
    internalPolygon = spriteActionNode->getPolygon() * _scale;
}

ActionPolygon::ActionPolygon(Action curAction, Poly2& mintPoly, int mx, float scale)
: polygonAction{curAction}
, internalPolygon{mintPoly}
, _age{0}
, _maxage{mx}
, _scale{1.0}
, _polygon(true)
{
    polyActionNode = cugl::scene2::SpriteNode::allocWithPoly(mintPoly);
}

void ActionPolygon::draw(const std::shared_ptr<SpriteBatch>& batch, const Affine2& transform, Color4 tint){
    if(_polygon){
        polyActionNode->draw(batch, transform, tint);
    }
    else{
        spriteActionNode->draw(batch, transform, tint);
    }
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

void AttackPolygons::addShoot(Vec2 center, float angle, float shootRadius){
    float degree = 60;
    PolyFactory curFactory;
    Poly2 resultingPolygon_shoot = curFactory.makeArc(center, shootRadius, angle + degree, degree);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(Action::SHOOT, resultingPolygon_shoot, max_age, shootRadius);
    attackPolygonNode->addChild(curPtr->getActionNode());
    Vec2 offset = Vec2(SDL_cosf((angle + 90) * 3.14f / 180), SDL_sinf((angle + 90) * 3.14f / 180)) * DOG_SIZE.x * SHOOT_HEAD_OFFSET_RATIO;
    curPtr->getActionNode()->setPosition(center.add(offset));
    curPtr->getActionNode()->setPosition(center);
    currentAttacks.insert(curPtr);
}

void AttackPolygons::addExplode(Vec2 center, float explosionRad){
    PolyFactory curFactory;
    Poly2 resultingPolygon = curFactory.makeCircle(center, explosionRad);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(Action::EXPLODE, resultingPolygon, max_age, explosionRad);
    attackPolygonNode->addChild(curPtr->getActionNode());
    curPtr->getActionNode()->setPosition(center);
    currentAttacks.insert(curPtr);
}

void AttackPolygons::addBite(Vec2 center, float angle, float explosionRad, float scale){
    std::shared_ptr<cugl::scene2::SpriteNode> biteSprite = cugl::scene2::SpriteNode::allocWithSheet(biteTexture, 1, 13);
    PolyFactory curFactory;
    Poly2 resultingPolygon = curFactory.makeArc(center, explosionRad, angle, 180);
    
    
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(biteSprite, Action::BITE, resultingPolygon, BITE_AGE, 1 + scale);
    attackPolygonNode->addChild(curPtr->getActionNode());
    Vec2 offset = Vec2(SDL_cosf((angle + 90) * 3.14f / 180), SDL_sinf((angle + 90) * 3.14f / 180)) * DOG_SIZE.x * BITE_HEAD_OFFSET_RATIO;
    curPtr->getActionNode()->setPosition(center.add(offset));
    currentAttacks.insert(curPtr);
}

bool AttackPolygons::setTexture(const std::shared_ptr<cugl::Texture> &bite){
    biteTexture = bite;
    return true;
}

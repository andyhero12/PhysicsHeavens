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
#define BITE_SCALE 1
#define BITE_FRAME 4
#define OFFSET_SCALE 1/0.0234375f


bool ActionPolygon::dealDamage(){
    if(!_polygon && polygonAction == Action::BITE){
        return (spriteActionNode->getFrame() == BITE_FRAME) && _freq == 0;
    }
    else{
        return true;
    }
}

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


ActionPolygon::ActionPolygon( std::shared_ptr<cugl::scene2::SpriteNode> actionSprite, Action curAction, Poly2& mintPoly, int mx, float scale)
: ActionPolygon(curAction, mintPoly, mx, scale)
{
    _expired = false;
    _polygon = false;
    spriteActionNode = actionSprite;
    _scale = scale * BITE_SCALE;
    spriteActionNode->setScale(_scale);
//    internalPolygon = spriteActionNode->getPolygon() * _scale;
//    polyActionNode = cugl::scene2::SpriteNode::allocWithPoly(internalPolygon);
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
    backAttackPolygonNode = cugl::scene2::SceneNode::alloc();
    frontAttackPolygonNode = cugl::scene2::SceneNode::alloc();
    return true;
}
void AttackPolygons::update(){
    auto it =currentAttacks.begin();
    while (it != currentAttacks.end()){
        std::shared_ptr<ActionPolygon> curAction = *it;
        curAction->update();
        auto curIt = it++;
        if (curAction->expired()){
            curAction->getActionNode()->removeFromParent();
            currentAttacks.erase(curIt);
        }
    }
}

void AttackPolygons::addShoot(Vec2 center, float angle, float shootRadius){
    float degree = 60;
    PolyFactory curFactory;
    Poly2 resultingPolygon_shoot = curFactory.makeArc(center, shootRadius, angle + degree, degree);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(Action::SHOOT, resultingPolygon_shoot, max_age, shootRadius);
    frontAttackPolygonNode->addChild(curPtr->getActionNode());
    Vec2 offset = Vec2(SDL_cosf((angle + 90) * 3.14f / 180), SDL_sinf((angle + 90) * 3.14f / 180)) * DOG_SIZE.x * SHOOT_HEAD_OFFSET_RATIO;
    curPtr->getActionNode()->setScale(OFFSET_SCALE);
    curPtr->getActionNode()->setPosition(center.add(offset));
    curPtr->getActionNode()->setPosition(center);
    currentAttacks.insert(curPtr);
}

void AttackPolygons::addExplode(Vec2 center, float explosionRad){
    PolyFactory curFactory;
    Poly2 resultingPolygon = curFactory.makeCircle(center, explosionRad);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(Action::EXPLODE, resultingPolygon, max_age, explosionRad);
    frontAttackPolygonNode->addChild(curPtr->getActionNode());
    curPtr->getActionNode()->setScale(OFFSET_SCALE);
    curPtr->getActionNode()->setPosition(center);
    currentAttacks.insert(curPtr);
}

void AttackPolygons::addBite(Vec2 center, float angle, float explosionRad, float scale){
    std::shared_ptr<cugl::Texture> bite;
    
    
    float ang = std::fmod(angle, 2*M_PI);
    
    if (ang < 0) {
        ang += 2*M_PI;
    }
    
    
    bool front = true;
    if (ang >= 0 && ang < M_PI_2) {
        bite = biteBackTexture;
        front = false;
     } else if (ang >= M_PI_2 && ang < M_PI) {
         bite = biteLeftTexture;
     } else if (ang >= M_PI && ang < 3 * M_PI_2) {
         bite = biteFrontTexture;
     } else if (ang >= 3 * M_PI_2 && ang < 2 * M_PI) {
         bite = biteRightTexture;
     }
    
    std::shared_ptr<cugl::scene2::SpriteNode> biteSprite = cugl::scene2::SpriteNode::allocWithSheet(bite, 3, 5);
    PolyFactory curFactory;
    Poly2 resultingPolygon = curFactory.makeArc(center, explosionRad, angle, 180);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(biteSprite, Action::BITE, resultingPolygon, BITE_AGE, 1 + scale);
    if(front){
        frontAttackPolygonNode->addChild(curPtr->getActionNode());
    }
    else{
        backAttackPolygonNode->addChild(curPtr->getActionNode());
    }
    Vec2 offset = Vec2(SDL_cosf((angle + 90) * 3.14f / 180), SDL_sinf((angle + 90) * 3.14f / 180)) * DOG_SIZE.x * BITE_HEAD_OFFSET_RATIO;
    curPtr->getActionNode()->setPosition(offset * OFFSET_SCALE);
    currentAttacks.insert(curPtr);
}

bool AttackPolygons::setTexture(const std::shared_ptr<cugl::Texture> &biteL, const std::shared_ptr<cugl::Texture> &biteR, const std::shared_ptr<cugl::Texture> &biteF, const std::shared_ptr<cugl::Texture> &biteB){
    biteRightTexture = biteR;
    biteLeftTexture = biteL;
    biteFrontTexture = biteF;
    biteBackTexture = biteF;
    return true;
}

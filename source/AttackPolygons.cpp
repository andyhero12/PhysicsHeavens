//
//  AttackPolygons.cpp
//
//  Created by Andrew Cheng on 3/12/24.
//

#include "AttackPolygons.h"
#define BITE_AGE 2
#define EXPLODE_AGE 2
#include "NLDog.h"
// used to translate attacks to originate from the dog's head
#define BITE_HEAD_OFFSET_RATIO 0.8f
#define SHOOT_HEAD_OFFSET_RATIO 1.5f
#define ANIM_FREQ 1
#define BITE_SCALE 1
#define BITE_FRAME 5
#define EXPLODE_TIME 5
#define SHOOT_TIME 1
#define OFFSET_SCALE 1/0.0234375f


bool ActionPolygon::dealDamage(){
    if(!_polygon && polygonAction == Action::BITE){
        return spriteActionNode->getFrame() == BITE_FRAME;
    }
    else if (polygonAction == Action::EXPLODE) {
        return _age == EXPLODE_TIME;
    }
    else if (polygonAction == Action::SHOOT) {
        return _age == SHOOT_TIME;
    }
    else{
        return true;
    }
}

void ActionPolygon::update(){
    _age++;
    if(!_polygon){
        _expired = (spriteActionNode->getFrame() + 1) == spriteActionNode->getSpan();
        if(_expired){
            return;
        }
        spriteActionNode->update();
    }
    else{
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


ActionPolygon::ActionPolygon( std::shared_ptr<SpriteAnimationNode> actionSprite, Action curAction, Poly2& mintPoly, int mx, float scale, float ang, Vec2 center)
: ActionPolygon(curAction, mintPoly, mx, scale, ang, center)
{
    _expired = false;
    _polygon = false;
    spriteActionNode = actionSprite;
    _scale = scale;
    spriteActionNode->setScale(_scale);
}

ActionPolygon::ActionPolygon(Action curAction, Poly2& mintPoly, int mx, float scale, float ang, Vec2 center)
: polygonAction{curAction}
, internalPolygon{mintPoly}
, _age{0}
, _maxage{mx}
, _scale{scale}
, _polygon(true)
, _freq{0}
, _ang{ang}
, _center(center)
{
    polyActionNode = cugl::scene2::SpriteNode::allocWithPoly(mintPoly);
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
float convertArcTanToAngAttack(float ang){
    if (ang >= 0){
        return ang;
    }
    return ang + 360.0f;
}
void AttackPolygons::addShoot(Vec2 center, float angle, float shootRadius){
    float degree = 60;
    float ang = convertArcTanToAngAttack(angle);
    PolyFactory curFactory;
    Poly2 resultingPolygon_shoot = curFactory.makeArc(center, shootRadius, ang - degree/2, degree);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(Action::SHOOT, resultingPolygon_shoot, max_age, 1.0f, ang, center);
    backAttackPolygonNode->addChild(curPtr->getActionNode());
    Vec2 offset = Vec2(cosf((angle) * 3.14f / 180), sinf((angle) * 3.14f / 180)) * DOG_SIZE.x * SHOOT_HEAD_OFFSET_RATIO;
    curPtr->getActionNode()->setScale(OFFSET_SCALE);
    curPtr->getActionNode()->setPosition(offset * OFFSET_SCALE);
    currentAttacks.insert(curPtr);
}

void AttackPolygons::addExplode(Vec2 center, float explosionRad){
    std::shared_ptr<SpriteAnimationNode> explodeSprite = SpriteAnimationNode::allocWithSheet(bombTexture, 4, 5, 20, 3);
    PolyFactory curFactory;
    Poly2 resultingPolygon = curFactory.makeCircle(center, explosionRad);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(explodeSprite, Action::EXPLODE, resultingPolygon, EXPLODE_AGE, explosionRad, 0, center);
    frontAttackPolygonNode->addChild(curPtr->getActionNode());
    Vec2 offset = Vec2(0,0);
    curPtr->getActionNode()->setAnchor(Vec2::ANCHOR_CENTER);
    curPtr->getActionNode()->setScale(0.6);
    curPtr->getActionNode()->setPosition(offset * OFFSET_SCALE);
    currentAttacks.insert(curPtr);
}

void AttackPolygons::addBite(Vec2 center, float angle, float explosionRad, float scale){
    std::shared_ptr<cugl::Texture> bite;
    float ang = convertArcTanToAngAttack(angle);
    bool front = true;
    if (ang > 45 && ang < 135) {
         bite = biteBackTexture;
         front = false;
     } else if (ang >= 135 && ang <= 225) {
         bite = biteLeftTexture;
     } else if (ang > 225 && ang < 315) {
         bite = biteFrontTexture;
     } else{
         bite = biteRightTexture;
     }
    
    std::shared_ptr<SpriteAnimationNode> biteSprite = SpriteAnimationNode::allocWithSheet(bite, 3, 5, 15, ANIM_FREQ);
//    CULog("Ang %f" ,ang);
    float rads = M_PI/180.0f * ang;
    if (ang <= 45){
        biteSprite->setAngle(rads);
    }else if (ang > 45 && ang < 135) {
        
    } else if (ang >= 135 && ang <= 225) {
        biteSprite->setAngle(rads - M_PI);
    } else if (ang > 225 && ang < 315) {
        
    } else{
        biteSprite->setAngle(rads);
    }
    PolyFactory curFactory;
    Poly2 resultingPolygon = curFactory.makeArc(center, explosionRad * (1 + scale), angle, 180);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(biteSprite, Action::BITE, resultingPolygon, BITE_AGE, 1 + scale, angle, center);
    if(front){
        frontAttackPolygonNode->addChild(curPtr->getActionNode());
    }
    else{
        backAttackPolygonNode->addChild(curPtr->getActionNode());
    }
    Vec2 offset = Vec2(cosf((angle) * 3.14f / 180), sinf((angle) * 3.14f / 180)) * DOG_SIZE.x * BITE_HEAD_OFFSET_RATIO;
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

bool AttackPolygons::setBombTexture(const std::shared_ptr<cugl::Texture> &bomb){
    bombTexture = bomb;
    return true;
}
void AttackPolygons::dispose(){
    frontAttackPolygonNode = nullptr;
    backAttackPolygonNode = nullptr;
    biteRightTexture = nullptr;
    biteLeftTexture = nullptr;
    biteFrontTexture = nullptr;
    biteBackTexture = nullptr;
    bombTexture = nullptr;
    currentAttacks.clear();
}

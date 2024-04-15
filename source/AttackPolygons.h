//
//  AttackPolygons.hpp
//  Dog
//
//  Created by Andrew Cheng on 3/12/24.
//
#pragma once
#ifndef AttackPolygons_hpp
#define AttackPolygons_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <unordered_set>
#include "NLDog.h"
using namespace cugl;
enum class Action : unsigned int {
    BITE = 0,
    SHOOT = 1,
    EXPLODE = 2
};

class ActionPolygon{
public:
    Action polygonAction;
    Poly2 internalPolygon;
    int _age;
    int _maxage;
    float _scale;
    bool _polygon;
    bool _expired;
    
    int _frame;
    int _freq;
    
    ActionPolygon(Action curAction, Poly2& mintPoly, int mx, float scale);
    ActionPolygon(std::shared_ptr<cugl::scene2::SpriteNode> actionSprite, Action curAction, Poly2& mintPoly, int mx, float scale);
    
    float getScale() const { return _scale; }
    
    int getAge() const { return _age; }
    Action getAction() const {return polygonAction;}
    const Poly2& getPolygon() const {return internalPolygon;}
//    const Poly2& getPolygon() const {return actionNode->getPolygon();}
    bool expired() const { return _expired;}
    void update();
    std::shared_ptr<cugl::scene2::SceneNode> getActionNode();
    void draw(const std::shared_ptr<SpriteBatch>& batch, const Affine2& transform, Color4 tint);
    
private:
    std::shared_ptr<cugl::scene2::SpriteNode> spriteActionNode;
    std::shared_ptr<cugl::scene2::PolygonNode> polyActionNode;
};

class AttackPolygons{
    
private:
    std::shared_ptr<cugl::scene2::SceneNode> attackPolygonNode;
    std::shared_ptr<cugl::Texture> biteTexture;
public:
    // Have AttackPolygon Set hold all possible textures for bite, shoot, explode
    int max_age;
    std::unordered_set<std::shared_ptr<ActionPolygon>> currentAttacks;
    AttackPolygons();
    bool init();
    bool setTexture(const std::shared_ptr<cugl::Texture> &bite);
    bool isEmpty() const { return currentAttacks.empty(); }
    void update();
    void addShoot(Vec2 center, float angle, float shootRadius);
    void addExplode(cugl::Vec2 center, float explosionRadius);
    void addBite(cugl::Vec2 center, float angle, float explosionRad, float scale);
    std::shared_ptr<cugl::scene2::SceneNode> getAttackPolygonNode(){
        return attackPolygonNode;
    }
};
#endif /* AttackPolygons_hpp */

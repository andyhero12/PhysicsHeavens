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
    
    ActionPolygon(Action curAction, Poly2& mintPoly, int mx);
    
    float getScale() const { return _scale; }
    
    int getAge() const { return _age; }
    Action getAction() const {return polygonAction;}
    const Poly2& getPolygon() const {return internalPolygon;}
    bool expired() const { return _age >= _maxage;}
    
    void update();
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
    std::shared_ptr<cugl::scene2::SceneNode> getActionNode(){
        return actionNode;
    }
private:
    std::shared_ptr<cugl::scene2::SceneNode> actionNode;
    void drawShoot(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
    void drawExplode(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
    void drawBite(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
};

class AttackPolygons{
    
private:
    std::shared_ptr<cugl::scene2::SceneNode> attackPolygonNode;
public:
    // Have AttackPolygon Set hold all possible textures for bite, shoot, explode
    int max_age;
    std::unordered_set<std::shared_ptr<ActionPolygon>> currentAttacks;
    AttackPolygons();
    bool init();
    bool isEmpty() const { return currentAttacks.empty(); }
    void update();
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
    void addShoot(Vec2 center, float angle, float shootRadius);
    void addExplode(cugl::Vec2 center, float explosionRadius);
    void addBite(cugl::Vec2 center, float angle, float explosionRad);
    std::shared_ptr<cugl::scene2::SceneNode> getAttackPolygonNode(){
        return attackPolygonNode;
    }
};
#endif /* AttackPolygons_hpp */

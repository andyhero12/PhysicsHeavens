//
//  Base.h
//  Ship
//
//  Created by Andrew Cheng on 2/26/24.
//
#pragma once
#ifndef Base_h
#define Base_h

#include <stdio.h>
#include <cugl/cugl.h>
#include <unordered_set>
#include <vector>
#include "SpriteAnimationNode.h"

class Base{
    
private:
    int _health;
    int _maxHealth;
    int _healVal;
    cugl::Vec2 _position;
    std::shared_ptr<SpriteAnimationNode> _drawNode;
    float _damagedTimer;
    
public:
    Base(int health, cugl::Vec2 pos, std::shared_ptr<SpriteAnimationNode> drawNode);
    void update();
    void reduceHealth(int val);
    
    bool lost(){
        return _health <= 0;
    }

    std::shared_ptr<SpriteAnimationNode> getDrawNode() {
        return _drawNode;
    }
    
    const cugl::Vec2& getPos() const {
        return _position;
    }
    const int getHealth() const{
        return _health;
    }
    const int getMaxHealth() const{
        return _maxHealth;
    }
    const int getHealValue() const{
        return _healVal;
    }
};

#endif /* Base_h */

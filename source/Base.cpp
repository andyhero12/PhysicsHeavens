//
//  Base.cpp
//
//  Created by Andrew Cheng on 2/26/24.
//

#include <stdio.h>
#include "Base.h"

#define DAMAGED_DURATION 0.4f

Base::Base(int health, cugl::Vec2 pos, std::shared_ptr<GateUIController> uiController, std::shared_ptr<SpriteAnimationNode> drawNode)
:_health{health},
_maxHealth{health}
,_position{pos}
, _gateUIController{uiController}
,_healVal{5}
,_drawNode{drawNode}
,_damagedTimer{0}
{
}

void Base::update(){
    if(_damagedTimer > 0) {
        // magic magic
        _damagedTimer -= 0.016f;
    }
    if(_damagedTimer < 0) {
        _damagedTimer = 0;
        //tints may be expensive, so separating out this special case may be worthwhile
        _drawNode->setColor(cugl::Color4(255, 255, 255));
    }
    if(_damagedTimer > 0) {
        float ratio = (DAMAGED_DURATION - _damagedTimer) / DAMAGED_DURATION;
        float brightness = 255 * (0.8f + ratio * 0.2f);
        _drawNode->setColor(cugl::Color4(brightness, brightness * (0.1f + ratio * 0.9f), brightness * (0.1f + ratio * 0.9f)));
    }
}

void Base::reduceHealth(int val){
    _health = _health - val;
//    CULog("%f", static_cast<double>(_health)/_maxHealth);
    _gateUIController->setGateBarTexture(static_cast<double>(_health)/_maxHealth);
    _damagedTimer = DAMAGED_DURATION;
}


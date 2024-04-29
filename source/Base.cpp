//
//  Base.cpp
//
//  Created by Andrew Cheng on 2/26/24.
//

#include <stdio.h>
#include "Base.h"
Base::Base(int health, cugl::Vec2 pos, std::shared_ptr<GateUIController> uiController)
:_health{health},
_maxHealth{health}
,_position{pos}
, _gateUIController{uiController}
,_healVal{5}
{
}

void Base::update(){
}

void Base::reduceHealth(int val){
    _health = _health - val;
    CULog("%f", static_cast<double>(_health)/_maxHealth);
    _gateUIController->setGateBarTexture(static_cast<double>(_health)/_maxHealth);
}


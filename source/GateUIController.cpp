//
//  GateUIController.cpp
//  Heaven
//
//  Created by Colin Wu on 4/28/24.
//

#include "GateUIController.h"
#include "BaseSet.h"

using namespace cugl;
#define ANIM_FREQ 5

bool GateUIController::init(std::shared_ptr<cugl::scene2::SceneNode> node, const std::shared_ptr<cugl::AssetManager>& assets, cugl::Size screenSize, std::shared_ptr<BaseSet> gates){
    
//    _childOffset = -1;
//    // Get gameplay ui elements
    _screenSize = screenSize;
    UInode = node;
    _gates = gates;
    
    _gateframe = cugl::scene2::PolygonNode::allocWithTexture(assets->get<Texture>("gateframe"));
    _gatefill = SubTextureNode::allocWithTexture(assets->get<Texture>("gatefill"));
    
    
    // set the scale
    _gateframe->setScale(UI_SCALE);
    _gatefill->setScale(UI_SCALE);
    
    // set the position
    _gateframe->setAnchor(Vec2::ANCHOR_CENTER);
    _gatefill->setAnchor(Vec2::ANCHOR_CENTER);
    
    x = 0;
    y = 0;
    
    float gatex = x + screenSize.width - UI_SCALE * _gateframe->getTexture()->getWidth()/2;
    float gatey = y + screenSize.height - UI_SCALE * (_gateframe->getTexture()->getHeight()/2 + 2);
    
    _gateframe->setPosition(gatex, gatey);
    
    float gatefillx = x + screenSize.width - UI_SCALE * _gatefill->getTexture()->getWidth()/2;
    
    _gatefill->setPosition(gatefillx, gatey);
    
    node->addChild(_gatefill);
    node->addChild(_gateframe);
    
    return true;
}

void GateUIController::setGateBarTexture(float percentage){
    CUAssert(0 <= percentage <= 1);
    // The percentage of the size bar that is empty space, needed to adjust how fast the health bar decreases
    GLfloat emptyPercent = 0;
    
    GLfloat minS = emptyPercent + (1.0 - emptyPercent) * (1.0-percentage);
    GLfloat maxS = 1;
    GLfloat minT = 0;
    GLfloat maxT = 1;

    _gatefill->setSubtexture(minS, maxS, minT, maxT);
    
    float gatefillx = x + _screenSize.width - UI_SCALE * _gatefill->getTexture()->getWidth()/2;
    float gatey = y + _screenSize.height - UI_SCALE * (_gateframe->getTexture()->getHeight()/2 + 2);

    _gatefill->setAnchor(Vec2::ANCHOR_CENTER);
    _gatefill->setPosition(gatefillx, gatey);
    
}

void GateUIController::updateHealthTexture(){
    std::shared_ptr<Base> firstGate = _gates->getBases().at(0);
    setGateBarTexture(fmax(0.0,static_cast<double>(firstGate->getHealth())/firstGate->getMaxHealth()));
};


//
//  GateUIController.cpp
//  Heaven
//
//  Created by Colin Wu on 4/28/24.
//

#include "GateUIController.h"
#include "BaseSet.h"
#include "cmath"

using namespace cugl;
#define ANIM_FREQ 5
#define WARNING_THRESHOLD 0.35
#define WARNING_FREQ 3

bool GateUIController::init(std::shared_ptr<cugl::scene2::SceneNode> node, const std::shared_ptr<cugl::AssetManager>& assets, cugl::Size screenSize, std::shared_ptr<BaseSet> gates){

    _screenSize = screenSize;
    UInode = node;
    _gates = gates;
    
    if(gates->getBases().size() == 1){
        _gateframe = cugl::scene2::PolygonNode::allocWithTexture(assets->get<Texture>("gateframe"));
        _gatefill = SubTextureNode::allocWithTexture(assets->get<Texture>("gatefill"));
    } else {
        _gateframe = cugl::scene2::PolygonNode::allocWithTexture(assets->get<Texture>("gateframe2"));
        _gatefill = SubTextureNode::allocWithTexture(assets->get<Texture>("gatefillup"));
        _gatefill2 = SubTextureNode::allocWithTexture(assets->get<Texture>("gatefilldown"));
    }
    
    
    // set the scale
    _gateframe->setScale(UI_SCALE_GATE);
    _gatefill->setScale(UI_SCALE_GATE);
    
    // set the position
    _gateframe->setAnchor(Vec2::ANCHOR_CENTER);
    _gatefill->setAnchor(Vec2::ANCHOR_CENTER);
    
    float gatex = screenSize.width - UI_SCALE_GATE * _gateframe->getTexture()->getWidth()/2;
    float gatey = screenSize.height - UI_SCALE_GATE * (_gateframe->getTexture()->getHeight()/2 + 2);
    
    _gateframe->setPosition(gatex, gatey);
    
    float gatefillx = screenSize.width - UI_SCALE_GATE * _gatefill->getTexture()->getWidth()/2;
    
    _gatefill->setPosition(gatefillx, gatey);
    
    node->addChild(_gatefill);
    
    if(_gatefill2){
        _gatefill2->setScale(UI_SCALE_GATE);
        _gatefill2->setAnchor(Vec2::ANCHOR_CENTER);
        _gatefill2->setPosition(gatefillx, gatey);
        node->addChild(_gatefill2);
    }
    
    node->addChild(_gateframe);
    
    return true;
}

void GateUIController::setGateBarTexture(std::shared_ptr<SubTextureNode> fill, float percentage){
    CUAssert(0 <= percentage <= 1);
    // im changing this
    GLfloat emptyPercent = 0.2;
    GLfloat minS = emptyPercent + (1.0 - emptyPercent) * (1.0 - percentage);
    GLfloat maxS = 1;
    GLfloat minT = 0;
    GLfloat maxT = 1;

    fill->setSubtexture(minS, maxS, minT, maxT);
    
    float gatefillx = _screenSize.width - UI_SCALE_GATE * fill->getTexture()->getWidth()/2;
    float gatey = _screenSize.height - UI_SCALE_GATE * (_gateframe->getTexture()->getHeight()/2 + 2);

    fill->setAnchor(Vec2::ANCHOR_CENTER);
    fill->setPosition(gatefillx, gatey);

    if(percentage < WARNING_THRESHOLD) {
        fill->setColor(calculateTint());
    }
    else {
        fill->setColor(Color4::WHITE);
    }
}

void GateUIController::updateHealthTexture(){
    
    
    std::shared_ptr<Base> firstGate = _gates->getBases().at(0);
    setGateBarTexture(_gatefill, fmax(0.0,static_cast<double>(firstGate->getHealth())/firstGate->getMaxHealth()));

    bool low = static_cast<double>(firstGate->getHealth())/firstGate->getMaxHealth() < WARNING_THRESHOLD;
    
    if(_gates->getBases().size() == 2){
        std::shared_ptr<Base> secondGate = _gates->getBases().at(1);
        setGateBarTexture(_gatefill2, fmax(0.0,static_cast<double>(secondGate->getHealth())/secondGate->getMaxHealth()));
        low |= static_cast<double>(secondGate->getHealth())/secondGate->getMaxHealth() < WARNING_THRESHOLD;
    }

    if(low) {
        _gateframe->setColor(calculateTint());
    }
    else {
        _gateframe->setColor(Color4::WHITE);
    }
};

Color4 GateUIController::calculateTint() {
    float ratio = (std::sin(timer * WARNING_FREQ) + 1) / 2 * 0.65 + 0.35;
    return Color4(255, ratio * 255, ratio * 255);
}

void GateUIController::update(float dt) {
    timer += dt;
    updateHealthTexture();
}


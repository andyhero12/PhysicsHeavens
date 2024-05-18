//
//  pauseScene.cpp
//  Heaven
//
//  Created by Henry Gao on 4/9/24.
//

#include "WinScene.h"
#include "NLInput.h"

using namespace cugl;

bool WinScene::init(std::shared_ptr<cugl::AssetManager> &assets, cugl::Size screenSize){
    _assets = assets;
    _screenSize = screenSize;
    status = Choice::NONE;
    return init();
}

bool WinScene::init(){
    if (!cugl::scene2::SceneNode::init()){
        return false;
    }
    std::shared_ptr<cugl::scene2::SceneNode> main =cugl::scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("resume"));

    std::shared_ptr<cugl::scene2::SceneNode> next =cugl::scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("mainmenu"));
    
    mainButton = cugl::scene2::Button::alloc(main, Color4::GRAY);
    nextButton = cugl::scene2::Button::alloc(next, Color4::GRAY);
    
    mainButton->setScale(PAUSE_SCALE);
    nextButton->setScale(PAUSE_SCALE);
    
    mainButton->addListener([=](const std::string& name, bool down){
        if(isActive()){
            status = Choice::MAIN;
        }
    });
    
    nextButton->addListener([=](const std::string& name, bool down) {
        if(isActive()){
            status = Choice::NEXT;
        }
    });
    
    mainButton->activate();
    nextButton->activate();
    
    addChild(mainButton);
    addChild(nextButton);
    
    float centerX = _screenSize.width/2;
    float centerY = _screenSize.height/2;

    mainButton->setAnchor(Vec2::ANCHOR_CENTER);
    nextButton->setAnchor(Vec2::ANCHOR_CENTER);
    
    mainButton->setPosition(centerX + 100, centerY);
    nextButton->setPosition(centerX - 100,centerY);

    setActive(false);
    status = Choice::NONE;
    return true;
}


void WinScene::setActive(bool value) {
    _active = value; 
    if(value){
        status = Choice::NONE;
    }
    setVisible(isActive());
}

void WinScene::dispose(){
    nextButton->removeFromParent();
    mainButton->removeFromParent();
    nextButton->deactivate();
    mainButton->deactivate();
    nextButton = nullptr;
    mainButton = nullptr;
}

void WinScene::exitToMain(){
    mainButton->setDown(true);
    mainButton->setDown(false);
}

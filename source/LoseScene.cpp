//
//  pauseScene.cpp
//  Heaven
//
//  Created by Henry Gao on 4/9/24.
//

#include "LoseScene.h"
#include "NLInput.h"

using namespace cugl;

bool LoseScene::init(std::shared_ptr<cugl::AssetManager> &assets, cugl::Size screenSize){
    _assets = assets;
    _screenSize = screenSize;
    status = Choice::NONE;
    return init();
}

bool LoseScene::init(){
    if (!cugl::scene2::SceneNode::init()){
        return false;
    }
    std::shared_ptr<cugl::scene2::SceneNode> main =cugl::scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("resume"));

    std::shared_ptr<cugl::scene2::SceneNode> retry =cugl::scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("mainmenu"));
    
    mainButton = cugl::scene2::Button::alloc(main, Color4::GRAY);
    retryButton = cugl::scene2::Button::alloc(retry, Color4::GRAY);
    
    mainButton->setScale(PAUSE_SCALE);
    retryButton->setScale(PAUSE_SCALE);
    
    mainButton->addListener([=](const std::string& name, bool down){
        if(isActive()){
            status = Choice::MAIN;
        }
    });
    
    retryButton->addListener([=](const std::string& name, bool down) {
        if(isActive()){
            status = Choice::RETRY;
        }
    });
    
    mainButton->activate();
    retryButton->activate();
    
    addChild(mainButton);
    addChild(retryButton);
    
    float centerX = _screenSize.width/2;
    float centerY = _screenSize.height/2;

    mainButton->setAnchor(Vec2::ANCHOR_CENTER);
    retryButton->setAnchor(Vec2::ANCHOR_CENTER);
    
    mainButton->setPosition(centerX + 100, centerY);
    retryButton->setPosition(centerX - 100,centerY);

    setActive(false);
    status = Choice::NONE;
    return true;
}


void LoseScene::setActive(bool value) {
    _active = value; 
    if(value){
        status = Choice::NONE;
    }
    setVisible(isActive());
}

void LoseScene::dispose(){
    retryButton->removeFromParent();
    mainButton->removeFromParent();
    retryButton->deactivate();
    mainButton->deactivate();
    retryButton = nullptr;
    mainButton = nullptr;
}

void LoseScene::exitToMain(){
    mainButton->setDown(true);
    mainButton->setDown(false);
}

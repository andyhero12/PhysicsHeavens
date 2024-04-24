//
//  pauseScene.cpp
//  Heaven
//
//  Created by Henry Gao on 4/9/24.
//

#include "PauseScene.h"
#include "NLInput.h"

using namespace cugl;

bool PauseScene::init(std::shared_ptr<cugl::AssetManager> &assets, cugl::Size screenSize){
    _assets = assets;
    _screenSize = screenSize;
    return init();
}

bool PauseScene::init(){
<<<<<<< HEAD
    cugl::scene2::SceneNode::init();
    
    status = Choice::GAME;
    
=======
    if (!cugl::scene2::SceneNode::init()){
        return false;
    }
    CULog("Init Pause");
    _paused = false;
>>>>>>> main
    std::shared_ptr<cugl::scene2::SceneNode> resume =cugl::scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("resume"));

    std::shared_ptr<cugl::scene2::SceneNode> exit =cugl::scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("mainmenu"));
    
    resumeButton = cugl::scene2::Button::alloc(resume, Color4::GRAY);
    exitButton = cugl::scene2::Button::alloc(exit, Color4::GRAY);
    
    resumeButton->setScale(PAUSE_SCALE);
    exitButton->setScale(PAUSE_SCALE);
    
    resumeButton->addListener([=](const std::string& name, bool down){
        if(getPause()){
            setPause(false);
        }
    });
    
    exitButton->addListener([=](const std::string& name, bool down) {
        if(getPause()){
            status = Choice::EXIT;
        }
    });
    
    resumeButton->activate();
    exitButton->activate();
    
    addChild(resumeButton);
    addChild(exitButton);
    
    float centerX = _screenSize.width/2;
    float centerY = _screenSize.height/2;

    resumeButton->setAnchor(Vec2::ANCHOR_CENTER);
    exitButton->setAnchor(Vec2::ANCHOR_CENTER);
    
    resumeButton->setPosition(centerX - 100, centerY);
    exitButton->setPosition(centerX + 100,centerY);

    setPause(false);
    status = Choice::GAME;
    return true;
}


void PauseScene::setPause(bool value) {
    _paused = value; 
    if(value){
        status = Choice::GAME;
    }
    setVisible(getPause());
}
void PauseScene::togglePause() {
    _paused = !_paused;
    if(_paused){
        status = Choice::GAME;
    }
    setVisible(_paused);
}

void PauseScene::dispose(){
<<<<<<< HEAD
    status = Choice::GAME;
    resumeButton->dispose();
    exitButton->dispose();
=======
    resumeButton->deactivate();
    exitButton->deactivate();
    resumeButton = nullptr;
    exitButton = nullptr;
>>>>>>> main
}

void PauseScene::exitToMain(){
    exitButton->setDown(true);
    exitButton->setDown(false);
}

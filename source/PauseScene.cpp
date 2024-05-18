//
//  pauseScene.cpp
//  Heaven
//
//  Created by Henry Gao on 4/9/24.
//

#include "PauseScene.h"
#include "NLInput.h"

using namespace cugl;

bool PauseScene::init(std::shared_ptr<cugl::AssetManager> &assets, cugl::Size screenSize, InputController input){
    _input = input;
    _assets = assets;
    _screenSize = screenSize;
    return init();
}

bool PauseScene::init(){
    if (!cugl::scene2::SceneNode::init()){
        return false;
    }

    background =cugl::scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("pause"));
    background->setScale(_screenSize.width/background->getTexture()->getWidth());
    background->setPosition(0.5*_screenSize);
//    _input.init();

    std::shared_ptr<cugl::scene2::SceneNode> resume =cugl::scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("resume"));

    std::shared_ptr<cugl::scene2::SceneNode> exit =cugl::scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("mainmenu"));
    
    resumeButton = cugl::scene2::Button::alloc(resume, Color4::GRAY);
    exitButton = cugl::scene2::Button::alloc(exit, Color4::GRAY);
    _buttonset.push_back(resumeButton);
    _buttonset.push_back(exitButton);
    resumeButton->setScale(PAUSE_SCALE);
    exitButton->setScale(PAUSE_SCALE);
    
    resumeButton->addListener([=](const std::string& name, bool down){
        if(down){
            if(_input.getState()==InputController::State::CONTROLLER){
                if(getPause()){
                    constatus = ContorllerChoice::isGAME;
                }
            }else if(getPause()){
                setPause(false);
            }
        }
    });
    
    exitButton->addListener([=](const std::string& name, bool down) {
        if(down){
            if(_input.getState()==InputController::State::CONTROLLER){
                if(getPause()){
                    constatus = ContorllerChoice::isEXIT;
                }
            }else if(getPause()){
                status = Choice::EXIT;
            }
        }
    });
    
    resumeButton->activate();
    exitButton->activate();
    
    addChild(background);
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
    constatus = ContorllerChoice::isNONE;
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
    resumeButton->removeFromParent();
    exitButton->removeFromParent();
    resumeButton->deactivate();
    exitButton->deactivate();
    resumeButton = nullptr;
    exitButton = nullptr;
}

void PauseScene::exitToMain(){
    exitButton->setDown(true);
    exitButton->setDown(false);
    status = Choice::EXIT;
}

void PauseScene::update(float timestep, int leftright, bool confirm){
    
    timeSinceLastSwitch += timestep;

    if (timeSinceLastSwitch >= switchFreq) {
        //std::cout<<_input._updown<<std::endl;
        if (leftright != 0) {
            if (leftright== -1 && _counter > 0) {
                _buttonset.at(_counter)->setDown(false);
                _counter--;
                _buttonset.at(_counter)->setDown(true);
            }
            else if (leftright == 1 && _counter < _buttonset.size() - 1) {
                _buttonset.at(_counter)->setDown(false);
                _counter++;
                _buttonset.at(_counter)->setDown(true);
            }
            timeSinceLastSwitch = 0;

        }
    }

    if(getControlStatus()==ContorllerChoice::isGAME && confirm){
        setPause(false);
    }else if(getControlStatus()==ContorllerChoice::isEXIT && confirm){
        status = Choice::EXIT;
    }


}

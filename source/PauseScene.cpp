//
//  pauseScene.cpp
//  Heaven
//
//  Created by Henry Gao on 4/9/24.
//

#include "PauseScene.h"

using namespace cugl;

bool PauseScene::init(std::shared_ptr<cugl::AssetManager> &assets){
    _assets = assets;
    return init();
}

bool PauseScene::init(){
    _childOffset = -1;
    
    std::shared_ptr<cugl::scene2::SceneNode> resume =cugl::scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("resume"));
    resume->setScale(PAUSE_SCALE);
    
    
    std::shared_ptr<cugl::scene2::SceneNode> exit =cugl::scene2::PolygonNode::allocWithTexture(_assets->get<Texture>("mainmenu"));

    exit->setScale(PAUSE_SCALE);
    
    
    resumeButton = cugl::scene2::Button::alloc(resume, Color4::GRAY);
    exitButton = cugl::scene2::Button::alloc(exit, Color4::GRAY);
    
    resumeButton->addListener([=](const std::string& name, bool down){
        std::cout << "resume : "<< resumeButton->isActive() << " exit " << exitButton->isActive() << std::endl;
        std::cout << "resume ??? \n";
        if(getPause()){
            setPause(false);
        }
    });
    
    exitButton->addListener([=](const std::string& name, bool down) {
        std::cout << "resume : "<< resumeButton->isActive() << " exit " << exitButton->isActive() << std::endl;
        std::cout << "exit ??? \n";
        if(getPause()){
            status = Choice::EXIT;
        }
    });
    
    resumeButton->activate();
    exitButton->activate();
    
    addChild(resumeButton);
    addChild(exitButton);
    
    resumeButton->setPosition(-50,0);
    exitButton->setPosition(100,0);
    
    resumeButton->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    
    // need to fix offset for exit i.e getPosition
    Vec2 exitPos = exitButton->getPosition()- Vec2(exitButton->getWidth(), exitButton->getHeight()).scale(PAUSE_SCALE/2);
    Vec2 resumePos = resumeButton->getPosition()- Vec2(resumeButton->getWidth(), resumeButton->getHeight()).scale(PAUSE_SCALE/2);
    
//    std::cout << "pre translate vec : " <<  exitButton->getPosition().x << " " << exitButton->getPosition().y << std::endl;
    std::cout << "exit translate vec : " << exitPos.x << " " << exitPos.y << std::endl;
    
    std::cout << "resume translate vec : " << resumePos.x << " " << resumePos.y << std::endl;
    
    exitButton->setPushable(Path2(Rect(exitPos, PAUSE_SCALE * Size(exitButton->getWidth(), exitButton->getHeight()))));
    
    
    resumeButton->setPushable(Path2(Rect(resumePos, PAUSE_SCALE * Size(resumeButton->getWidth(), resumeButton->getHeight()))));
    
//    resumeButton->setPushable(Path2(Rect(Vec2(0,0), PAUSE_SCALE * Size(resumeButton->getWidth(), resumeButton->getHeight()))));
    
    
    auto r = cugl::scene2::PolygonNode::allocWithPoly(Rect(exitButton->getPosition(), PAUSE_SCALE * Size(exitButton->getWidth(), exitButton->getHeight())));
    r->setPosition(exitPos);
    addChild(r);
//    
//    r->setPosition(resumeButton->getPosition());
    return true;
}


void PauseScene::setPause(bool value) {
    paused = value; 
    setVisible(getPause());
}

void PauseScene::dispose(){
    resumeButton->dispose();
    exitButton->dispose();
}

void PauseScene::exitToMain(){
    resumeButton->setDown(true);
    resumeButton->setDown(false);
}

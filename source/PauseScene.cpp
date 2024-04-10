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
    // create the EXIT button
    std::shared_ptr<cugl::scene2::SceneNode> up = cugl::scene2::PolygonNode::allocWithPoly(Rect(0,0,100,100));
    up->setColor(Color4::RED);
    
    
    std::shared_ptr<cugl::scene2::SceneNode> down = cugl::scene2::PolygonNode::allocWithPoly(Rect(0,0,100,100));
    down->setColor(Color4::BLUE);
    
    
    paused = false;
    
    _button = cugl::scene2::Button::alloc(up, down);
    _button->addListener([=](const std::string& name, bool down) {
        if(getPause()){
            status = Choice::EXIT;
        }
    });
    
    _button->setPushable(Path2(Rect(-50,-50,200,200)));
    
    _button->setVisible(false);
    _button->activate();
    addChild(_button);
    _button->setAnchor(Vec2::ANCHOR_CENTER);
    return true;
}



void PauseScene::updateMenu(float x, float y){
    _button->setPosition(x,y);
    _button->setVisible(getPause());
}

void PauseScene::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, const cugl::Affine2& transform, cugl::Color4 tint){
    if(paused){
        SceneNode::draw(batch,transform,tint);
    }
}

void PauseScene::dispose(){
    _button->dispose();
}

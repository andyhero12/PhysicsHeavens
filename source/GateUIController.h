//
//  GateUIController.hpp
//  Heaven
//
//  Created by Colin Wu on 4/28/24.
//

#ifndef GateUIController_h
#define GateUIController_h

#include <stdio.h>
#include <cugl/cugl.h>
#include "SubTextureNode.h"
#include "SpriteAnimationNode.h"
#include "GateUIController.h"
#include "BaseSet.h"

#define UI_SCALE 6.0f

using namespace cugl;

class GateUIController{
    
private:
    std::shared_ptr<cugl::scene2::SceneNode> UInode;
    cugl::Size _screenSize;
    std::shared_ptr<BaseSet> _gates;
    
    // represents the bottom left corner of the screen
    float x;
    float y;
    
    const Vec2 origin = Vec2(0, 0);
    std::shared_ptr<cugl::scene2::PolygonNode> _gateframe;
    std::shared_ptr<SubTextureNode> _gatefill;
    std::shared_ptr<SubTextureNode> _gatefill2;
    
public:
        
    std::shared_ptr<cugl::scene2::SceneNode> getUINode(){
        return UInode;
    }
    GateUIController(){}
    
    ~GateUIController(){}
    
    bool init(std::shared_ptr<cugl::scene2::SceneNode> node, const std::shared_ptr<cugl::AssetManager>& assets, cugl::Size screenSize, std::shared_ptr<BaseSet> gates);
    
    void setGateBarTexture(std::shared_ptr<SubTextureNode> fill, float percentage);
    
    void updateHealthTexture();
  
};

#endif /* GateUIController_h */

//
//  UIController.hpp
//  Heavan
//
//  Created by Colin Wu on 3/28/24.
//

#ifndef UIController_h
#define UIController_h

#include <cugl/cugl.h>
#include <stdio.h>
#include "SubTextureNode.h"

#define UI_SCALE 6

using namespace cugl;

class UIController{
    
private:
    std::shared_ptr<cugl::scene2::SceneNode> UInode;
    cugl::Size _screenSize;
    
    // represents the bottom left corner of the screen
    float x;
    float y;
    
    
    const Vec2 origin = Vec2(0, 0);
    
    std::shared_ptr<cugl::scene2::PolygonNode> _healthframe;
    std::shared_ptr<SubTextureNode> _healthfill;
    std::shared_ptr<cugl::scene2::PolygonNode> _sizeframe;
    std::shared_ptr<SubTextureNode> _sizefill;
    std::shared_ptr<cugl::scene2::PolygonNode> _bombtoggle;
    std::shared_ptr<cugl::scene2::PolygonNode> _shoottoggle;
    std::shared_ptr<cugl::scene2::PolygonNode> _baittoggle;
    
public:
        
    UIController(){}
    
    ~UIController(){}
    
    bool init(std::shared_ptr<cugl::scene2::SceneNode> node, const std::shared_ptr<cugl::AssetManager>& assets, cugl::Size screenSize);
    
    void setHealthBarTexture(float percentage);
    void setSizeBarTexture(float percentage);
    void setToggle(std::string mode);
  
};

#endif /* UIController_hpp */

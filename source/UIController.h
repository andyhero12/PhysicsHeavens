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
#include "NLDog.h"
#include "SubTextureNode.h"

#define UI_SCALE 6

using namespace cugl;

class UIController{
    
private:
    std::shared_ptr<cugl::scene2::SceneNode> UInode;
    
    const Vec2 origin = Vec2(0, 0);
    
    /** Game UI Elements **/
    std::shared_ptr<Dog> _dog;
    std::shared_ptr<cugl::scene2::PolygonNode> _healthframe;
    std::shared_ptr<SubTextureNode> _healthfill;
    std::shared_ptr<cugl::scene2::PolygonNode> _sizeframe;
    std::shared_ptr<SubTextureNode> _sizefill;
    std::shared_ptr<cugl::scene2::PolygonNode> _bombtoggle;
    std::shared_ptr<cugl::scene2::PolygonNode> _shoottoggle;
    std::shared_ptr<cugl::scene2::PolygonNode> _baittoggle;
    
    void setHealthBarTexture(float health);
    void setSizeBarTexture(float size);
    void setToggle();
    
public:
        
    UIController(){}
    
    ~UIController(){}
    
    bool init(std::shared_ptr<cugl::scene2::SceneNode> node, const std::shared_ptr<cugl::AssetManager>& assets, cugl::Size screenSize, std::shared_ptr<Dog> dog);
  
};

#endif /* UIController_hpp */

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

#define UI_SCALE 6

using namespace cugl;

class UIController : public cugl::scene2::PolygonNode{
    
private:
    
    const Vec2 origin = Vec2(0, 0);
    
    /** Game UI Elements **/
    cugl::Size _screenSize;
    std::shared_ptr<Dog> _dog;
    std::shared_ptr<cugl::Texture> _healthframe;
    std::shared_ptr<cugl::Texture> _healthfill;
    std::shared_ptr<cugl::Texture> _sizeframe;
    std::shared_ptr<cugl::Texture> _sizefill;
    std::shared_ptr<cugl::Texture> _bombtoggle;
    std::shared_ptr<cugl::Texture> _shoottoggle;
    std::shared_ptr<cugl::Texture> _baittoggle;
    
    std::shared_ptr<cugl::Texture> getHealthBarTexture(float health);
    std::shared_ptr<cugl::Texture> getSizeBarTexture(float size);
    
public:
        
    UIController(){}
    
    ~UIController(){}
    
    bool init(const std::shared_ptr<cugl::AssetManager>& assets, cugl::Size screenSize, std::shared_ptr<Dog> dog);
    
    virtual void draw(const std::shared_ptr<SpriteBatch>& batch, const Affine2& transform, Color4 tint) override;
    
};

#endif /* UIController_hpp */

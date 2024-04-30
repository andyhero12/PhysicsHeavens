//
//  BaseSet.h
//
//  Created by Andrew Cheng on 2/26/24.
//
#pragma once
#ifndef BaseSet_h
#define BaseSet_h

#include <cugl/cugl.h>
#include <unordered_set>
#include <vector>
#include "Base.h"
#include "SpriteAnimationNode.h"


class BaseSet{
private:
    
public:
    std::shared_ptr<cugl::scene2::SceneNode> baseSetNode;
    std::vector<std::shared_ptr<SpriteAnimationNode>> animationNodes;
    //    /* Base Textures */
    std::shared_ptr<cugl::Texture> _texture;
    //    /* Set of bases */
    std::vector<std::shared_ptr<Base>> _bases;
    std::shared_ptr<GateUIController> _uiController;
    
    BaseSet();
    
    ~BaseSet();
    
    void update();
    
    const std::vector<std::shared_ptr<Base>>& getBases(){
        return _bases;
    }
    std::shared_ptr<cugl::scene2::SceneNode> getBaseSetNode(){
        return baseSetNode;
    }
    bool init(const std::vector<cugl::Vec3>& basePoses, std::shared_ptr<cugl::AssetManager> assets, std::shared_ptr<GateUIController> uiController);
    
    bool baseLost();
    
    int getFirstHealth();
    
    const std::shared_ptr<cugl::Texture> getTexture() const { return _texture; }
    std::shared_ptr<scene2::SceneNode> getUINode();
private:
    

};

#endif /* BaseSet_h */

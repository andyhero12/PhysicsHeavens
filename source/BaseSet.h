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
#include "NLDog.h"


class BaseSet{
private:
    
public:
//    /* Base Textures */
    std::shared_ptr<cugl::Texture> _texture;
//    /* Set of bases */
    std::vector<std::shared_ptr<Base>> _bases;
    BaseSet();
    
    ~BaseSet();

    void setTexture(const std::shared_ptr<cugl::Texture>& value);
    
    void update();
    
    bool init(std::shared_ptr<cugl::JsonValue> data);
    
    bool init(const std::vector<cugl::Vec3>& basePoses);
    bool baseLost();
    
    int getFirstHealth();
    
    const std::shared_ptr<cugl::Texture> getTexture() const { return _texture; }
    
    Vec2 recallBase(std::shared_ptr<Dog> dog);
    
private:
    

};

#endif /* BaseSet_h */

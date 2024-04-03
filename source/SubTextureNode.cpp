//
//  SubTextureNode.cpp
//  Heaven
//
//  Created by Henry Gao on 4/3/24.
//
#include "SubTextureNode.h"
using namespace cugl::scene2;


bool SubTextureNode::initWithTexture(const std::shared_ptr<cugl::Texture>& texture){
    original = texture;
    PolygonNode::initWithTexture(texture);
    return true;
}

void SubTextureNode::setSubtexture(GLfloat minS, GLfloat maxS, GLfloat minT, GLfloat maxT){
    _texture = original->getSubTexture(minS, maxS, minT, maxT);
}

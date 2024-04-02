//
//  AnimationScene.cpp
//  Heaven
//
//  Created by Henry Gao on 4/1/24.
//

#include "AnimationSceneNode.h"

#pragma mark -
#pragma mark Constructors

/**
 * Constructs a ActionNode with no filmstrip
 *
 * You must initialize this object before use.
 *
 * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate an object on
 * the heap, use one of the static constructors instead.
 */
AnimationSceneNode::AnimationSceneNode() :
_cols(0),
_size(0),
_frame(0),
_bounds(cugl::Rect::ZERO) {
    _classname = "AnimationNode";
}

void AnimationSceneNode::dispose(){
    
}

void AnimationSceneNode::setFrame(int frame){
    
}

#pragma mark -
#pragma mark Helper Functions
/**
 initializes the animationSprites vector
 */
bool AnimationSceneNode::createSpriteNodes(const std::vector<std::shared_ptr<cugl::Texture>>& textures){
    bool ret = true;
    for (const std::shared_ptr<cugl::Texture>& texture : textures) {
        _animationSprites.emplace_back();
        ret = ret && _animationSprites.at(_animationSprites.size() - 1)->initWithSheet(texture, _rows, _cols);
    }
    return ret;
}
/**
 * Initializes the film strip with the given texture.
 *
 * This initializer assumes that the filmstrip is rectangular, and that
 * there are no unused frames.
 *
 * The size of the node is equal to the size of a single frame in the
 * filmstrip. To resize the node, scale it up or down.  Do NOT change the
 * polygon, as that will interfere with the animation.
 *
 * @param texture   The texture image to use
 * @param rows      The number of rows in the filmstrip
 * @param cols      The number of columns in the filmstrip
 *
 * @return  true if the filmstrip is initialized properly, false otherwise.
 */
bool AnimationSceneNode::initWithTextures(const std::vector<std::shared_ptr<cugl::Texture>>& textures,
                               int rows, int cols, int size) {
    CUAssertLog(size <= rows*cols, "Invalid strip size for %dx%d",rows,cols);
    
    this->_rows = rows;
    this->_cols = cols;
    this->_size = size;
    _bounds.size = textures.at(0)->getSize();
    _bounds.size.width /= cols;
    _bounds.size.height /= rows;
    return createSpriteNodes(textures);
}



//
//  pauseScene.hpp
//  Heaven
//
//  Created by Henry Gao on 4/9/24.
//

#ifndef PauseScene_h
#define PauseScene_h
#include <cugl/cugl.h>


class PauseScene:  public cugl::scene2::SceneNode 
{
    
public:
    /**
     * The menu choice.
     *
     * This state allows the top level application to know what the user
     * chose.
     */
    enum Choice {
        /** User wants to host a game */
        GAME,
        /** User wants to join a game */
        EXIT
    };
private:
    
    Choice status;
    
    bool paused;

    /** The "pause" button */
    std::shared_ptr<cugl::scene2::Button>  _button;
    
public:
    
    PauseScene(){};
    
    bool init() override;
    
    /**
     * Returns true if the gameplay controller is currently active
     *
     * @return true if the gameplay controller is currently active
     */
    bool getPause() const { return paused; }

    /**
     * Returns true if the gameplay controller is currently active
     *
     * @return true if the gameplay controller is currently active
     */
    void setPause(bool value) { paused = value; }
    
    void updateMenu(float x, float y);
    
    Choice getStatus(){return status;}
    
    
#pragma mark -
#pragma mark Rendering
    /**
     * Draws this polygon node via the given SpriteBatch.
     *
     * This method only worries about drawing the current node.  It does not
     * attempt to render the children.
     *
     * @param batch     The SpriteBatch to draw with.
     * @param transform The global transformation matrix.
     * @param tint      The tint to blend with the Node color.
     */
    virtual void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, const cugl::Affine2& transform, cugl::Color4 tint) override;
    
};
#include <stdio.h>

#endif /* pauseScene_hpp */

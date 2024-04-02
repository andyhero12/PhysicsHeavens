//
//  AnimationScene.hpp
//  Heaven
//
//  Created by Henry Gao on 4/1/24.
//

#ifndef AnimationScene_h
#define AnimationScene_h

#include <stdio.h>
#include <cugl/cugl.h>



class AnimationSceneNode : public cugl::scene2::SceneNode {
public:
#pragma mark -
#pragma mark Directions
    /**
     * Enumeration to identify the Animation directions
     * From top to bottom correponds to index 0 to 7
     */
    enum class Directions : int {
        EAST,
        NORTHEAST,
        NORTH,
        NORTHWEST,
        WEST,
        SOUTHWEST,
        SOUTH,
        SOUTHEAST
    };
#pragma mark -
#pragma mark Helper Functions
private:
    bool createSpriteNodes(const std::vector<std::shared_ptr<cugl::Texture>>& textures);
#pragma mark -
#pragma mark Class Attributes
protected:
    /** The number of columns in this filmstrip */
    int _cols;
    /** The number of rows in this filmstrip */
    int _rows;
    /** The number of frames in this filmstrip */
    int _size;
    /** The active animation frame */
    int _frame;
    /** The size of a single animation frame (different from active polygon) */
    cugl::Rect _bounds;
    /** All the different sprite directions*/
    std::vector<std::shared_ptr<cugl::scene2::SpriteNode>> _animationSprites;
    /** current direction animation is facing*/
    Directions _direction;
    /** Whether draw will draw animation or not*/
    bool _on;
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates an empty scene graph node with the degenerate texture.
     *
     * This constructor should never be called directly, as this is an abstract
     * class.
     */
    AnimationSceneNode();
    
    /**
     * Deletes this node, releasing all resources.
     */
    ~AnimationSceneNode() { dispose(); }
    
    /**
     * Disposes all of the resources used by this node.
     *
     * A disposed Node can be safely reinitialized. Any children owned by this
     * node will be released.  They will be deleted if no other object owns them.
     *
     * It is unsafe to call this on a Node that is still currently inside of
     * a scene graph.
     */
    virtual void dispose() override;
    
    
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
    bool initWithTextures(const std::vector<std::shared_ptr<cugl::Texture>>& textures, int rows, int cols) {
        return initWithTextures(textures,rows,cols,rows*cols);
    }
    
    
    /**
     * Initializes the film strip with the given texture.
     *
     * The parameter size is to indicate that there are unused frames in the
     * filmstrip.  The value size must be less than or equal to rows*cols, or
     * this constructor will raise an error.
     *
     * The size of the node is equal to the size of a single frame in the
     * filmstrip. To resize the node, scale it up or down.  Do NOT change the
     * polygon, as that will interfere with the animation.
     *
     * @param texture   The texture image to use
     * @param rows      The number of rows in the filmstrip
     * @param cols      The number of columns in the filmstrip
     * @param size      The number of frames in the filmstrip
     *
     * @return  true if the filmstrip is initialized properly, false otherwise.
     */
    bool initWithTextures(const std::vector<std::shared_ptr<cugl::Texture>>& textures, int rows, int cols, int size);
#pragma mark -
#pragma mark Attribute Accessors
    /**
     * Returns the number of frames in this filmstrip.
     *
     * @return the number of frames in this filmstrip.
     */
    int getSpan() const { return _size; }
    
    /**
     * Returns the current active frame.
     *
     * @return the current active frame.
     */
    unsigned int getFrame() const { return _frame; }
    
    /**
     * Sets the active frame as the given index.
     *
     * If the frame index is invalid, an error is raised.
     *
     * @param frame the index to make the active frame
     */
    void setFrame(int frame);

    /**
     * Returns the animation node for the given animation direction
     *
     * @param  direction   The enumeration to identify the afterburner
     *
     * @return the animation node for the given direction
     */
    const std::shared_ptr<cugl::scene2::SpriteNode>& getAnimation(Directions direction) const;


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
    
    /**
     * Animates the given direction.
     *
     * If the animation is not active, it will reset to the initial animation frame.
     *
     * @param direction    The reference to the animation direction
     * @param on        Whether the animation is active
     */
    void animate(Directions direction, bool on);
    };


#endif /* AnimationScene_h */

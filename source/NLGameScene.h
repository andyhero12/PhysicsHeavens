//
//  NLGameScene.h
//  Networked Crate Demo
//
//  This is a demo based on the original Rocket Demo, this demo is made in
//  addition to the Rocket Demo as a tutorial to the networked physics library.
//
//  The majority of this Demo is similar to the rocket demo, except for the
//  physics-related methods.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White
//  Version: 1/10/17
//
#ifndef __NL_GAME_SCENE_H__
#define __NL_GAME_SCENE_H__
#include <cugl/cugl.h>
#include <box2d/b2_world_callbacks.h>
#include <vector>
#include <format>
#include <string>
#include <random>
#include "NLInput.h"
#include "NLDecoyEvent.h"
#include "NLExplodeEvent.h"
#include "NLShootEvent.h"
#include "NLGameResEvent.h"
#include "NLDashEvent.h"
#include "NLRecallEvent.h"
#include "NLSizeEvent.h"
#include "NLDeathEvent.h"
#include "NLWinEvent.h"
#include "NLLoseEvent.h"
#include "NLSpawnerDeathEvent.h"
#include "NLBaseHealthEvent.h"
#include "NLMonsterHealthEvent.h"
#include "NLCameraController.h"
#include "NLLevelModel.h"
#include "World.h"
#include "AnimationSceneNode.h"
#include "UIController.h"
#include "CollisionController.h"
#include "SpawnerController.h"
#include "OverWorld.h"
#include "MonsterController.h"
#include "GlobalConstants.h"
#include "PauseScene.h"
#include "MiniMap.h"
#include "Tutorial.h"
#include "SpriteAnimationNode.h"
#include "NLAbsorbEvent.h"
#include "AudioController.h"

using namespace cugl::physics2::net;
using namespace cugl;

/**
 * This class is the primary gameplay constroller for the demo.
 *
 * A world has its own objects, assets, and input controller.  Thus this is
 * really a mini-GameEngine in its own right.  As in 3152, we separate it out
 * so that we can have a separate mode for the loading screen.
 */
class GameScene : public cugl::Scene2
{
protected:
    std::shared_ptr<cugl::AssetManager> _assets;

    // CONTROLLERS
    /** Controller for abstracting out input across multiple platforms */
    InputController _input;

    /** Reference to the root of the scene graph */
    std::shared_ptr<cugl::scene2::ScrollPane> _rootnode;

    /** Reference to the physics of the scene graph */
    std::shared_ptr<cugl::scene2::SceneNode> _worldnode;
    std::shared_ptr<cugl::scene2::SceneNode> _monsterSceneNode;
    OverWorld overWorld;
    CollisionController _collisionController;
    SpawnerController _spawnerController;
    MonsterController _monsterController;

    std::shared_ptr<cugl::JsonValue> _constants;
    /** Reference to the debug root of the scene graph */
    std::shared_ptr<cugl::scene2::SceneNode> _debugnode;

    std::shared_ptr<cugl::scene2::SceneNode> _uinode;
    std::shared_ptr<cugl::scene2::SceneNode> _tutorialnode;
    

    std::shared_ptr<cugl::physics2::net::NetWorld> _world;
    
    std::shared_ptr<SpriteAnimationNode> loseNode;
    std::shared_ptr<SpriteAnimationNode> winNode;
    std::shared_ptr<SpriteAnimationNode> repeatWinNode;
    /** The level model */
    std::shared_ptr<LevelModel> _level;

    std::shared_ptr<PauseScene> _pause;

    std::shared_ptr<World> _backgroundWrapper;
    
    std::vector<cugl::Rect> _transparentRects;
    std::set<std::shared_ptr<scene2::SceneNode>> _decorToHide;
    
    std::shared_ptr<Minimap> _minimap;
    
    std::shared_ptr<AudioController> _audioController;
    
    /** Host is by default the left cannon */
    bool _isHost;
    bool gameOverWin;
    bool gameOverLoss;
    int gameOverDelay;
    bool _todoReset;
    
    /** Whether or not debug mode is active */
    bool _debug;
    Vec2 olddogPos;
    std::shared_ptr<NetEventController> _network;
    cugl::Affine2 _transform;

    
    /** The current level */
    int _levelNum;
    
    float _zoom;
    std::vector<std::shared_ptr<Tutorial>> tutorialTiles;
    std::shared_ptr<SpriteAnimationNode> tutorialArrow;
    std::shared_ptr<SpriteAnimationNode> newTrick;
    
    
    std::shared_ptr<cugl::scene2::PolygonNode> textBox;
    std::shared_ptr<SpriteAnimationNode> devilNormal;
    std::shared_ptr<SpriteAnimationNode> devilLaugh;
    int tutorialIndex;
    
    Vec2 previousPan;
    float shakeMagnitude;
    /**
     * Lays out the game geography.
     *
     * Pay close attention to how we attach physics objects to a scene graph.
     * The simplest way is to make a subclass. However,
     * for simple objects you can just use a callback function to lightly couple
     * them.  This is what we do with the crates.
     *
     * This method is really, really long.  In practice, you would replace this
     * with your serialization loader, which would process a level file.
     */
    void populate();

    /**
     * Adds the physics object to the physics world and loosely couples it to the scene graph
     *
     * There are two ways to link a physics object to a scene graph node on the
     * screen.  One way is to make a subclass of a physics object.
     * The other is to use callback functions to loosely couple
     * the two.  This function is an example of the latter.
     * the two.  This function is an example of the latter.
     *
     * param obj    The physics object to add
     * param node   The scene graph node to attach it to
     */
    void addInitObstacle(const std::shared_ptr<cugl::physics2::Obstacle> &obj,
                         const std::shared_ptr<cugl::scene2::SceneNode> &node);
    /**
     * This method links a scene node to the obstacle.
     *
     * This method adds a listener so that the sceneNode will move along with the obstacle.
     */
    void linkSceneToObs(const std::shared_ptr<cugl::physics2::Obstacle> &obj,
                        const std::shared_ptr<cugl::scene2::SceneNode> &node);

    /**
     * Returns the active screen size of this scene.
     *
     * This method is for graceful handling of different aspect
     * ratios
     */
    cugl::Size computeActiveSize() const;
    
    /** Updates input controller */
    void updateInputController();
    
    
    /** Initialize tutorial */
    void initTutorial();
    
    void initTutorialOne();
    void initTutorialTwo();
    void initTutorialThree();

public:
#pragma mark -
#pragma mark Constructors

    PauseScene::Choice getStatus() { return _pause->getStatus(); }
    /**
     * Creates a new game world with the default values.
     *
     * This constructor does not allocate any objects or start the controller.
     * This allows us to use a controller without a heap pointer.
     */
    GameScene();

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~GameScene() { dispose(); }

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose();

    /**
     * Initializes the controller contents, and starts the game
     *
     * The constructor does not allocate any objects or memory.  This allows
     * us to have a non-pointer reference to this controller, reducing our
     * memory allocation.  Instead, allocation happens in this method.
     *
     * The game world is scaled so that the screen coordinates do not agree
     * with the Box2d coordinates.  This initializer uses the default scale.
     *
     * @param assets    The (loaded) assets for this game mode
     *
     * @return true if the controller is initialized properly, false otherwise.
     */
    bool init(const std::shared_ptr<cugl::AssetManager> &assets, const std::shared_ptr<NetEventController> network, bool isHost, std::string level, int levelNum);

    /**
     * Initializes the controller contents, and starts the game
     *
     * The constructor does not allocate any objects or memory.  This allows
     * us to have a non-pointer reference to this controller, reducing our
     * memory allocation.  Instead, allocation happens in this method.
     *
     * The game world is scaled so that the screen coordinates do not agree
     * with the Box2d coordinates.  The bounds are in terms of the Box2d
     * world, not the screen.
     *
     * @param assets    The (loaded) assets for this game mode
     * @param rect      The game bounds in Box2d coordinates
     *
     * @return  true if the controller is initialized properly, false otherwise.
     */
    bool init(const std::shared_ptr<cugl::AssetManager> &assets, const cugl::Rect rect, const std::shared_ptr<NetEventController> network, bool isHost, std::string level, int levelNum);

    /**
     * Initializes the controller contents, and starts the game
     *
     * The constructor does not allocate any objects or memory.  This allows
     * us to have a non-pointer reference to this controller, reducing our
     * memory allocation.  Instead, allocation happens in this method.
     *
     * The game world is scaled so that the screen coordinates do not agree
     * with the Box2d coordinates.  The bounds are in terms of the Box2d
     * world, not the screen.
     *
     * @param assets    The (loaded) assets for this game mode
     * @param rect      The game bounds in Box2d coordinates
     * @param gravity   The gravitational force on this Box2d world
     *
     * @return  true if the controller is initialized properly, false otherwise.
     */

    bool init(const std::shared_ptr<cugl::AssetManager> &assets, const cugl::Rect rect, const cugl::Vec2 gravity, const std::shared_ptr<NetEventController> network, bool isHost, std::string level_string, int levelNum);

    void setAudioController(std::shared_ptr<AudioController> audioController)
    {
        _audioController = audioController;
    }

#pragma mark -
#pragma mark State Access
    /**
     * Returns true if the gameplay controller is currently active
     *
     * @return true if the gameplay controller is currently active
     */
    bool isActive() const { return _active; }
    /**
     * Returns true if debug mode is active.
     *
     * If true, all objects will display their physics bodies.
     *
     * @return true if debug mode is active.
     */
    bool isDebug() const { return _debug; }

    /**
     * Sets whether debug mode is active.
     *
     * If true, all objects will display their physics bodies.
     *
     * @param value whether debug mode is active.
     */
    void setDebug(bool value)
    {
        _debug = value;
        _debugnode->setVisible(value);
    }

#pragma mark -
#pragma mark Gameplay Handling

    virtual void preUpdate(float timestep);
    virtual void postUpdate(float timestep);
    virtual void fixedUpdate();
    /**
     * The method called to update the game mode.
     *
     * This method contains any gameplay code that is not an OpenGL call.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    void update(float timestep);

#pragma mark -
#pragma mark Collision Handling
    /**
     * Processes the start of a collision
     *
     * This method is called when we first get a collision between two objects.
     * We use this method to test if it is the "right" kind of collision.  In
     * particular, we use it to test if we make it to the win door.
     *
     * @param  contact  The two bodies that collided
     */
    void beginContact(b2Contact *contact);
    void endContact(b2Contact *contact);
    /**
     * Handles any modifications necessary before collision resolution
     *
     * This method is called just before Box2D resolves a collision.  We use
     * this method to implement sound on contact, using the algorithms outlined
     * in Ian Parberry's "Introduction to Game Physics with Box2D".
     *
     * @param  contact  The two bodies that collided
     * @param  contact  The collision manifold before contact
     */
    void beforeSolve(b2Contact *contact, const b2Manifold *oldManifold);

    /**
     Adds the Scenenode for the background tiles
     */
    void addChildBackground();
    void addChildForeground();
    void executeSlidingWindow(Vec2 dest);
    void resetDraw();
    void clientMonsterUpdate(float dt);
    
    void clientProcessMonsterHealth(std::shared_ptr<MonsterHealthEvent> monsterHealthEvent);
    void processAbsorbEvent(std::shared_ptr<AbsorbEvent> absorbEvent);
};

#endif /* __NL_GAME_SCENE_H__ */

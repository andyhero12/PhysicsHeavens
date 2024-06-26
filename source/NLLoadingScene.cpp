//
//  NLLoadingScene.cpp
//  Networked Physics Demo
//
//  This module provides a very barebones loading screen.  Most of the time you
//  will not need a loading screen, because the assets will load so fast.  But
//  just in case, this is a simple example you can use in your games.
//
//  We know from 3152 that you all like to customize this screen.  Therefore,
//  we have kept it as simple as possible so that it is easy to modify. In
//  fact, this loading screen uses the new modular JSON format for defining
//  scenes.  See the file "loading.json" for how to change this scene.
//
//  Author: Walker White
//  Version: 1/10/17
//
#include "NLLoadingScene.h"

using namespace cugl;

/** Regardless of logo, lock the height to this */
#define SCENE_HEIGHT  800

#pragma mark -
#pragma mark Constructors

/**
 * Initializes the controller contents, making it ready for loading
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool LoadingScene::init(const std::shared_ptr<AssetManager>& assets) {
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_HEIGHT/dimen.height;
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }
    
    // IMMEDIATELY load the splash screen assets
    _assets = assets;
    _assets->loadDirectory("json/loading.json");
    _brand = cugl::scene2::PolygonNode::allocWithTexture(_assets->get<cugl::Texture>("logo"));
    background = SpriteAnimationNode::allocWithSheet(_assets->get<cugl::Texture>("backgroundl"), 1, 4, 4, 20);
    background->setScale(SCENE_HEIGHT/background->getTexture()->getHeight());
    background->setPosition(0.5 * background->getSize());
    
    _brand->setScale(3.25);
    _brand->setPosition(0.5f*background->getSize().width, 0.75f*background->getSize().height);
    addChild(background);
    addChild(_brand);
//    layer->setColor(Color4(0,0,0,1));
    Application::get()->setClearColor(Color4(192,192,192,255));
//    addChild(layer);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void LoadingScene::dispose() {
//    _bar->removeFromParent();
    _bar = nullptr;
    _assets = nullptr;
    _progress = 0.0f;
}


#pragma mark -
#pragma mark Progress Monitoring
/**
 * The method called to update the game mode.
 *
 * This method updates the progress bar amount.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void LoadingScene::update(float progress) {
    background->update();
    if (_progress < 1) {
        _progress = _assets->progress();
        CULog("Progress: %f", _progress);
//        if (_progress >= 1) {
//            _progress = 1.0f;
//            _bar->setVisible(false);
//        }
//        _bar->setProgress(_progress);
    }
}


bool LoadingScene::doneLoading() const {
//    CULog("Done Loading: %d", _progress >= 1.0f);
//    CULog("Loading: %f", _progress);
    return _progress >= 1.0f;
}

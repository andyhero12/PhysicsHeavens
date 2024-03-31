//
//  NLInput.cpp
//  Networked Physics Demo
//
//  This input controller is primarily designed for keyboard control.  On mobile
//  you will notice that we use gestures to emulate keyboard commands. They even
//  use the same variables (though we need other variables for internal keyboard
//  emulation).  This simplifies our design quite a bit.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White
//  Version: 1/10/17
//
#include "NLInput.h"

using namespace cugl;

#pragma mark Input Constants

/** The key to use for sending a big crate in the game */
#define RESET_KEY KeyCode::B
/** The key for toggling the debug display */
#define DEBUG_KEY KeyCode::D
/** The key for exitting the game */
#define EXIT_KEY  KeyCode::ESCAPE
/** The fire key for firing a crate */
#define FIRE_KEY  KeyCode::SPACE
/** The max charge time of a fire in milliseconds */
#define FIRE_CHARGE_TIME 2000.f

/** How fast a double click must be in milliseconds */
#define EVENT_DOUBLE_CLICK  400
/** How far we must swipe left or right for a gesture */
#define EVENT_SWIPE_LENGTH  100
/** How fast we must swipe left or right for a gesture */
#define EVENT_SWIPE_TIME   1000
/** How far we must turn the tablet for the accelerometer to register */
#define EVENT_ACCEL_THRESH  M_PI/10.0f
/** The key for the event handlers */
#define LISTENER_KEY        1


#pragma mark -
#pragma mark Input Controller
/**
 * Creates a new input controller.
 *
 * This constructor does NOT do any initialzation.  It simply allocates the
 * object. This makes it safe to use this class without a pointer.
 */
NetLabInput::NetLabInput() :
_active(false),
_resetPressed(false),
_debugPressed(false),
_exitPressed(false),
_keyUp(false),
_keyDown(false),
_keyReset(false),
_keyDebug(false),
_keyExit(false),
_fired(false),
_firePower(0.0f),
_horizontal(0.0f),
_vertical(0.0f) {
}

/**
 * Deactivates this input controller, releasing all listeners.
 *
 * This method will not dispose of the input controller. It can be reused
 * once it is reinitialized.
 */
void NetLabInput::dispose() {
    if (_active) {
#ifndef CU_TOUCH_SCREEN
        Input::deactivate<Keyboard>();
#else
        Input::deactivate<Accelerometer>();
        Touchscreen* touch = Input::get<Touchscreen>();
        touch->removeBeginListener(LISTENER_KEY);
        touch->removeEndListener(LISTENER_KEY);
#endif
        _active = false;
    }
}

/**
 * Initializes the input control for the given drawing scale.
 *
 * This method works like a proper constructor, initializing the input
 * controller and allocating memory.  However, it still does not activate
 * the listeners.  You must call start() do that.
 *
 * @return true if the controller was initialized successfully
 */
bool NetLabInput::init() {
    _timestamp.mark();
    bool success = true;
    
    success = Input::activate<Keyboard>();
    _active = success;
    return success;
}


/**
 * Processes the currently cached inputs.
 *
 * This method is used to to poll the current input state.  This will poll the
 * keyboad and accelerometer.
 *
 * This method also gathers the delta difference in the touches. Depending on
 * the OS, we may see multiple updates of the same touch in a single animation
 * frame, so we need to accumulate all of the data together.
 */
void NetLabInput::update(float dt) {
    int left = false;
    int rght = false;
    int up   = false;
    int down = false;
    // DESKTOP CONTROLS
    Keyboard* keys = Input::get<Keyboard>();

    // Map "keyboard" events to the current frame boundary
    _keyReset  = keys->keyPressed(RESET_KEY);
    _keyDebug  = keys->keyPressed(DEBUG_KEY);
    _keyExit   = keys->keyPressed(EXIT_KEY);
    
    if(keys->keyPressed(FIRE_KEY)){
        _timestamp.mark();
        _keyFired = false;
    }
    
    if (keys->keyReleased(FIRE_KEY)){
        cugl::Timestamp curr;
        _keyFired = true;
    }
    else if(keys->keyDown(FIRE_KEY)){
        Timestamp curr;
        _firePower = SDL_min(1.0f,curr.ellapsedMillis(_timestamp)/FIRE_CHARGE_TIME);
    }
    else{
        _firePower = 0.f;
    }
    
    left = keys->keyDown(KeyCode::ARROW_LEFT);
    rght = keys->keyDown(KeyCode::ARROW_RIGHT);
    up   = keys->keyDown(KeyCode::ARROW_UP);
    down = keys->keyDown(KeyCode::ARROW_DOWN);
    
    _resetPressed = _keyReset;
    _debugPressed = _keyDebug;
    _exitPressed  = _keyExit;
    _fired        = _keyFired;
    
    // Directional controls
    _horizontal = 0.0f;
    if (rght) {
        _horizontal += 1.0f;
    }
    if (left) {
        _horizontal -= 1.0f;
    }

    _vertical = 0.0f;
    if (up) {
        _vertical += 1.0f;
    }
    if (down) {
        _vertical -= 1.0f;
    }
    
    _keyFired = false;

}

/**
 * Clears any buffered inputs so that we may start fresh.
 */
void NetLabInput::clear() {
    _resetPressed = false;
    _debugPressed = false;
    _exitPressed  = false;
    _fired = false;
    
    _horizontal = 0.0f;
    _vertical   = 0.0f;
    _timestamp.mark();
}

//
//  JSLevelConstants.h
//  JSONDemo
//
//  This file contains all of the details that define the JSON structure of a LevelModel.
//  Created by Walker White on 1/20/16.
//
//  Author: Walker White
//  Version: 1/20/16
//
#ifndef __TD_LEVEL_CONSTANTS_H__
#define __TD_LEVEL_CONSTANTS_H__

/** The global fields of the level model */
#define WIDTH_FIELD         "width"
#define HEIGHT_FIELD        "height"
#define GRAVITY_FIELD       "gravity"
#define ROCKET_FIELD        "rocket"
#define GOALDOOR_FIELD      "exit"
#define CRATES_FIELD        "crate"
#define WALLS_FIELD         "wall"

/** The physics fields for each object */
#define POSITION_FIELD      "pos"
#define SIZE_FIELD          "size"
#define BODYTYPE_FIELD      "bodytype"
#define DENSITY_FIELD       "density"
#define FRICTION_FIELD      "friction"
#define RESTITUTION_FIELD   "restitution"
#define DAMPING_FIELD       "damping"
#define ROTATION_FIELD      "rotation"
#define STATIC_VALUE        "static"

/** The drawing fields for each object */
#define TEXTURE_FIELD       "texture"
#define DEBUG_COLOR_FIELD   "debugcolor"
#define DEBUG_OPACITY_FIELD "debugopacity"

/** The rocket specific fields */
#define THRUST_FIELD        "thrust"
#define MAIN_FLAMES_FIELD   "flames"
#define LEFT_FLAMES_FIELD   "flames-left"
#define RIGHT_FLAMES_FIELD  "flames-right"
#define MAIN_SOUND_FIELD    "main-sound"
#define LEFT_SOUND_FIELD    "left-sound"
#define RIGHT_SOUND_FIELD   "right-sound"

/** The wall specific fields */
#define VERTICES_FIELD      "polygon"
#define BOUNDARY_FIELD      "boundary"

/** The source for our level file */
#define LEVEL_ONE_FILE      "json/levels/tutorialOne.json"
/** The key for our loaded level */
#define LEVEL_ONE_KEY       "level1"

/** The source for our level file */
#define LEVEL_TWO_FILE      "json/levels/levelTwo.json"
/** The key for our loaded level */
#define LEVEL_TWO_KEY       "level2"

/** The source for our level file */
#define LEVEL_THREE_FILE      "json/levels/levelThree.json"
/** The key for our loaded level */
#define LEVEL_THREE_KEY       "level3"

/** The source for our level file */
#define LEVEL_FOUR_FILE      "json/levels/level_May4.json"
/** The key for our loaded level */
#define LEVEL_FOUR_KEY       "level4"


#endif /* defined(__TD_LEVEL_CONSTANTS_H__) */

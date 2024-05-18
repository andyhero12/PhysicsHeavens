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
#define LEVEL_TWO_FILE      "json/levels/level_May4.json"
/** The key for our loaded level */
#define LEVEL_TWO_KEY       "level2"

/** The source for our level file */
#define LEVEL_THREE_FILE      "json/levels/levelThree.json"
/** The key for our loaded level */
#define LEVEL_THREE_KEY       "level3"

/** The source for our level file */
#define LEVEL_FOUR_FILE      "json/levels/levelTwo.json"
/** The key for our loaded level */
#define LEVEL_FOUR_KEY       "level4"

/** The source for our level file */
#define LEVEL_FIVE_FILE      "json/levels/levelTwo.json"
/** The key for our loaded level */
#define LEVEL_FIVE_KEY       "level5"

/** The source for our level file */
#define LEVEL_SIX_FILE      "json/levels/xShapedLevel.json"
/** The key for our loaded level */
#define LEVEL_SIX_KEY       "level6"

/** The source for our level file */
#define LEVEL_SEVEN_FILE      "json/levels/level3.json"
/** The key for our loaded level */
#define LEVEL_SEVEN_KEY       "level7"

/** The source for our level file */
#define LEVEL_EIGHT_FILE      "json/levels/levelNine.json"
/** The key for our loaded level */
#define LEVEL_EIGHT_KEY       "level8"

/** The source for our level file */
#define LEVEL_NINE_FILE      "json/levels/level_May4.json"
/** The key for our loaded level */
#define LEVEL_NINE_KEY       "level9"

/** The source for our level file */
#define LEVEL_TEN_FILE      "json/levels/levelTwo_TwoGates.json"
/** The key for our loaded level */
#define LEVEL_TEN_KEY       "level10"

/** The source for our level file */
#define LEVEL_ELEVEN_FILE      "json/levels/crossHard.json"
/** The key for our loaded level */
#define LEVEL_ELEVEN_KEY       "level11"

/** The source for our level file */
#define LEVEL_TWELVE_FILE      "json/levels/crossMed.json"
/** The key for our loaded level */
#define LEVEL_TWELVE_KEY       "level12"

/** The source for our level file */
#define LEVEL_THIRTEEN_FILE      "json/levels/level15.json"
/** The key for our loaded level */
#define LEVEL_THIRTEEN_KEY       "level13"

/** The source for our level file */
#define LEVEL_FOURTEEN_FILE      "json/levels/levelTen.json"
/** The key for our loaded level */
#define LEVEL_FOURTEEN_KEY       "level14"

/** The source for our level file */
#define LEVEL_FIFTEEN_FILE      "json/levels/levelTwo_TwoGates.json"
/** The key for our loaded level */
#define LEVEL_FIFTEEN_KEY       "level15"

#endif /* defined(__TD_LEVEL_CONSTANTS_H__) */

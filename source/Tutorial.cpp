//
//  Tutorial.cpp
//  Heaven
//
//  Created by Henry Gao on 4/23/24.
//

#include "Tutorial.h"

bool Tutorial::init(int tile, MODE m_progress, std::string m_text, bool val)
{
    laugh = val;
    tileX = tile;
    passed = false;
    progress = m_progress;
    enter = false;
    text = m_text;
    sprite = nullptr;
    spriteRepeat = nullptr;
    pressA = nullptr;
    return true;
}


bool Tutorial::atArea(int tile)
{
    if(progress == DEFENDGATE){
        enter = enter || (tileX <= tile && tile <= tileX + 5);
    }
    else{
        enter = enter || tileX <= tile || progress == GREETING || progress == MOVEMENT;
    }
    return enter;
}

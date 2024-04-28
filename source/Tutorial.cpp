//
//  Tutorial.cpp
//  Heaven
//
//  Created by Henry Gao on 4/23/24.
//

#include "Tutorial.h"

bool Tutorial::init(int tile, MODE m_progress){
    tileX = tile;
    passed = false;
    progress = m_progress;
    return true;
}

bool Tutorial::atArea(int tile){
    return tileX == tile;
}

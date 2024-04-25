//
//  Tutorial.cpp
//  Heaven
//
//  Created by Henry Gao on 4/23/24.
//

#include "Tutorial.h"

bool Tutorial::init(int tile, PROGRESS progress){
    this->tileX = tile;
    this->passed = false;
    this->progress = progress;
    return true;
}

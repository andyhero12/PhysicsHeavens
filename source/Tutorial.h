//
//  Tutorial.hpp
//  Heaven
//
//  Created by Henry Gao on 4/23/24.
//

#ifndef Tutorial_h
#define Tutorial_h

#include <stdio.h>
#include <functional>

class Tutorial{
public:
    enum PROGRESS {
        MOVEMENT,
        BITE,
        CHANGEABILITY,
        SPECIALS
    };
    
protected:
    int tileX;
    bool passed;
    PROGRESS progress;
    
public:
    Tutorial(){}
    bool init(int tile, PROGRESS progress);
    static std::shared_ptr<Tutorial> alloc(int tile, PROGRESS progress) {
        std::shared_ptr<Tutorial> node = std::make_shared<Tutorial>();
        return (node->init(tile, progress) ? node : nullptr);
    }
    int getX() const {return tileX;}
    bool didPass() const {return passed;}
    PROGRESS getProgress() const {return progress;}
    void setPass(bool change){ passed = change; }
    
};



#endif /* tutorial_hpp */

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
#include <stdexcept>
#include <string>
#include <memory>

class Tutorial{
public:
   
    enum MODE {
        MOVEMENT,
        BITE,
        CHANGEABILITY,
        SPECIALS,
        DEFENDGATE,
        DESTROYSPAWNER
    };

    static std::string toString(MODE progress) {
        switch (progress) {
            case MOVEMENT:
                return "MOVEMENT";
            case BITE:
                return "BITE";
            case CHANGEABILITY:
                return "CHANGEABILITY";
            case SPECIALS:
                return "SPECIALS";
            case DEFENDGATE:
                return "DEFENDGATE";
            case DESTROYSPAWNER:
                return "DESTORYSPAWNER";
            default:
                return "Unknown Progress";
        }
    }

    static MODE fromString(const std::string& progressString) {
        if (progressString == "MOVEMENT") {
            return MOVEMENT;
        } else if (progressString == "BITE") {
            return BITE;
        } else if (progressString == "CHANGEABILITY") {
            return CHANGEABILITY;
        } else if (progressString == "SPECIALS") {
            return SPECIALS;
        } else if (progressString == "DEFENDGATE"){
            return DEFENDGATE;
        } else if(progressString == "DESTROYSPAWNER"){
            return DESTROYSPAWNER;
        }else {
            throw std::invalid_argument("Invalid progress string");
        }
    }
    
protected:
    int tileX;
    bool passed;
    MODE progress;
    
public:
    Tutorial(){}
    bool init(int tile, MODE progress);
    static std::shared_ptr<Tutorial> alloc(int tile, MODE progress) {
        std::shared_ptr<Tutorial> node = std::make_shared<Tutorial>();
        return (node->init(tile, progress) ? node : nullptr);
    }
    int getX() const {return tileX;}
    bool didPass() const {return passed;}
    MODE getProgress() const {return progress;}
    void setPass(bool change){ passed = change; }
    // returns true if your x coordinate is at the right spot
    bool atArea(int tile);
    
};



#endif /* tutorial_hpp */

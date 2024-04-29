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
#include <cugl/cugl.h>
class Tutorial{
public:
    
    enum MODE {
        MOVEMENT = 0,
        BITE = 1,
        DEFENDGATE = 2,
        DASH = 3,
        SPECIALSONE = 4,
        DESTROYSPAWNER = 5,
        CHANGEABILITYTWO = 6,
        SPECIALSTWO = 7,
        CHANGEABILITYTHREE = 8,
        SPECIALSTHREE = 9,
        CHANGEABILITYFOUR = 10,
        SPECIALSFOUR = 11
    };

    static std::string toString(MODE mode) {
        switch (mode) {
            case MOVEMENT:
                return "MOVEMENT";
            case BITE:
                return "BITE";
            case DEFENDGATE:
                return "DEFENDGATE";
            case DASH:
                return "DASH";
            case SPECIALSONE:
                return "SPECIALSONE";
            case DESTROYSPAWNER:
                return "DESTROYSPAWNER";
            case CHANGEABILITYTWO:
                return "CHANGEABILITYTWO";
            case SPECIALSTWO:
                return "SPECIALSTWO";
            case CHANGEABILITYTHREE:
                return "CHANGEABILITYTHREE";
            case SPECIALSTHREE:
                return "SPECIALSTHREE";
            case CHANGEABILITYFOUR:
                return "CHANGEABILITYFOUR";
            case SPECIALSFOUR:
                return "SPECIALSFOUR";
            default:
                return "Unknown Progress";
        }
    }

    static MODE fromString(const std::string& progressString) {
        if (progressString == "MOVEMENT") {
            return MOVEMENT;
        } else if (progressString == "BITE") {
            return BITE;
        } else if (progressString == "DEFENDGATE") {
            return DEFENDGATE;
        } else if (progressString == "DASH") {
            return DASH;
        } else if (progressString == "SPECIALS ONE") {
            return SPECIALSONE;
        } else if (progressString == "DESTROY SPAWNER") {
            return DESTROYSPAWNER;
        } else if (progressString == "CHANGEABILITY TWO") {
            return CHANGEABILITYTWO;
        } else if (progressString == "SPECIALS TWO") {
            return SPECIALSTWO;
        } else if (progressString == "CHANGEABILITY THREE") {
            return CHANGEABILITYTHREE;
        } else if (progressString == "SPECIALS THREE") {
            return SPECIALSTHREE;
        } else if (progressString == "CHANGEABILITY FOUR") {
            return CHANGEABILITYFOUR;
        } else if (progressString == "SPECIALS FOUR") {
            return SPECIALSFOUR;
        } else {
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

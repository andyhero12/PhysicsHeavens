//
//  NLResetEvent.cpp
//  Heaven
//
//  Created by Andrew Cheng on 3/31/24.
//

#include "NLResetEvent.h"


/**
 * This method is used by the NetEventController to create a new event of using a
 * reference of the same type.
 *
 * Not that this method is not static, it differs from the static alloc() method
 * and all methods must implement this method.
 */
std::shared_ptr<NetEvent> ResetEvent::newEvent(){
    return std::make_shared<ResetEvent>();
}

std::shared_ptr<NetEvent> ResetEvent::allocResetEvent(){
    auto event = std::make_shared<ResetEvent>();
    return event;
}

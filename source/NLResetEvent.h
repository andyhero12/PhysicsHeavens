//
//  NLResetEvent.h
//  Heaven
//
//  Created by Andrew Cheng on 3/31/24.
//

#ifndef NLResetEvent_h
#define NLResetEvent_h


#include <cugl/cugl.h>
using namespace cugl::physics2::net;
using namespace cugl;

class ResetEvent : public NetEvent {
    
protected:
    
    
public:
    /**
     * This method is used by the NetEventController to create a new event of using a
     * reference of the same type.
     *
     * Not that this method is not static, it differs from the static alloc() method
     * and all methods must implement this method.
     */
    std::shared_ptr<NetEvent> newEvent() override;
    
    static std::shared_ptr<NetEvent> allocResetEvent();
};
#endif /* NLResetEvent_h */

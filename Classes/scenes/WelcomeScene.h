//
//  WelcomeScene.h
//  CatchPanda
//
//  Created by AIRIA on 14-8-13.
//
//

#ifndef __CatchPanda__WelcomeScene__
#define __CatchPanda__WelcomeScene__

#include "base/BaseLayer.h"

class WelcomeScene:public BaseLayer
{
public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void onTexturesLoaded();
    CREATE_FUNC(WelcomeScene);
};


#endif /* defined(__CatchPanda__WelcomeScene__) */

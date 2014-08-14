//
//  BaseSprite.h
//  CatchPanda
//
//  Created by AIRIA on 14-8-13.
//
//

#ifndef __CatchPanda__BaseSprite__
#define __CatchPanda__BaseSprite__

#include "common/CommonHeaders.h"

class BaseSprite:public Sprite
{
public:
    static BaseSprite *create(std::string fileName);
    void setTouchHandler(std::function<void(Ref *pSender)> callback);
protected:
    void addEventListener();
    std::function<void(Ref *pSender)> touchHandler = nullptr;
};

#endif /* defined(__CatchPanda__BaseSprite__) */

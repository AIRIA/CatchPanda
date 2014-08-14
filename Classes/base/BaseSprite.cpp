//
//  BaseSprite.cpp
//  CatchPanda
//
//  Created by AIRIA on 14-8-13.
//
//

#include "BaseSprite.h"

BaseSprite *BaseSprite::create(std::string fileName)
{
    auto sprite = new BaseSprite();
    if(sprite&&sprite->initWithSpriteFrameName(fileName))
    {
        sprite->autorelease();
        sprite->addEventListener();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void BaseSprite::setTouchHandler(std::function<void(Ref *pSender)> callback)
{
    this->touchHandler = callback;
}

void BaseSprite::addEventListener()
{
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = [](Touch *touch,Event *event)->bool
    {
        auto target = static_cast<BaseSprite*>(event->getCurrentTarget());
        auto locationNode = target->convertToNodeSpace(touch->getLocation());
        auto size = target->getContentSize();
        auto rect = Rect(0, 0, size.width, size.height);
        if (rect.containsPoint(locationNode)) {
            return true;
        }
        return false;
    };
    
    eventListener->onTouchEnded = [](Touch *touch,Event *event)->void
    {
        auto target = static_cast<BaseSprite*>(event->getCurrentTarget());
        if(target->touchHandler)
        {
            target->touchHandler(target);
        }
        
    };
    
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
}
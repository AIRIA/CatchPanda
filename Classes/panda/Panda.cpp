//
//  Panda.cpp
//  CatchPanda
//
//  Created by AIRIA on 14-8-13.
//
//

#include "Panda.h"

Panda *Panda::create()
{
    auto panda = new Panda();
    if(panda&&panda->initWithSpriteFrameName("taopaoxiongmao00000.png"))
    {
        panda->autorelease();
        panda->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
        panda->initAnimations();
        panda->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("taopaoxiongmao"))));
        panda->setDisplayFrameWithAnimationName("taopaoxiongmao", 1);
        return panda;
    }
    CC_SAFE_FREE(panda);
    return nullptr;
}

void Panda::initAnimations()
{
    
    auto createAnimation = [](std::string prefix,int frameNums)->void{
        Vector<SpriteFrame*> framesVec;
        for (auto i=0; i<=frameNums; i++) {
            auto frameName = __String::createWithFormat("%s000%02d.png",prefix.c_str(),i)->getCString();
            auto frame = Sprite::createWithSpriteFrameName(frameName)->getSpriteFrame();
            framesVec.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(framesVec,0.1f);
        AnimationCache::getInstance()->addAnimation(animation, prefix);
    };
    
    createAnimation("taopaoxiongmao",15);
    createAnimation("weizhuxiongmao",14);
    
}

void Panda::beCatch()
{
    stopAllActions();
    runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("weizhuxiongmao"))));
}



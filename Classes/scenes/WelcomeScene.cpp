//
//  WelcomeScene.cpp
//  CatchPanda
//
//  Created by AIRIA on 14-8-13.
//
//

#include "WelcomeScene.h"
#include "base/BaseSprite.h"
#include "scenes/GameScene.h"

void WelcomeScene::onEnter()
{
    BaseLayer::onEnter();
}

void WelcomeScene::onExit()
{
    BaseLayer::onExit();
}

bool WelcomeScene::init()
{
    if (!BaseLayer::init()) {
        return false;
    }
    textureFiles.push_back("CatchPanda");
    return true;
}

void WelcomeScene::onTexturesLoaded()
{
    auto bg = Sprite::createWithSpriteFrameName("640_1136btn_start.png");
    auto play = BaseSprite::create("341_157play.png");
    bg->setPosition(DESIGN_CENTER);
    play->setPosition(Point(DESIGN_WIDTH/2,200));
    m_pBody->addChild(bg);
    m_pBody->addChild(play);
    
    play->setTouchHandler([](Ref *pSender)->void{
        GameScene::create()->run();
    });
}
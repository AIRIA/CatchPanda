//
//  ResultPanel.cpp
//  CatchPanda
//
//  Created by AIRIA on 14-8-18.
//
//

#include "ResultPanel.h"
#include "scenes/GameScene.h"
#include "base/BaseSprite.h"
#include "util/Util.h"

void ResultPanel::onEnter()
{
    LayerColor::onEnter();
    Util::showSpotAds();
}

ResultPanel *ResultPanel::create(ResultPanel::ResultType type)
{
    auto panel = new ResultPanel();
    if (panel&&panel->init()) {
        panel->autorelease();
        panel->addEventListener();
        panel->setResultType(type);
        panel->initUI();
        return panel;
    }
    CC_SAFE_FREE(panel);
    return nullptr;
}

void ResultPanel::addEventListener()
{
    auto event = EventListenerTouchOneByOne::create();
    event->setSwallowTouches(true);
    event->onTouchBegan = [](Touch *touch, Event *event)->bool
    {
        return true;
    };
    
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(event, this);
}

void ResultPanel::initUI()
{
    setColor(Color3B::BLACK);
    setOpacity(128);
    
    auto wrapper = Node::create();
    char frameName[50];
    char tip[100];
    if (getResultType()==ResultType::win)
    {
        sprintf(frameName, "%s","590_475victory.png");
        sprintf(tip, "%s","小伙子,牛逼呀!你就是未来的饲养员了!");
    }
    else
    {
        sprintf(frameName, "%s","590_475failed.png");
        sprintf(tip, "%s","屌丝,连只猫都抓不住,还想不想找妹子了..");
    }
    auto m_winSize = Director::getInstance()->getWinSize();
    auto m_fScaleWidth = m_winSize.width/DESIGN_WIDTH;
    auto m_fScaleHeight = m_winSize.height/DESIGN_HEIGHT;
    auto m_fScaleFactor = m_fScaleWidth>m_fScaleHeight?m_fScaleWidth:m_fScaleHeight;
    auto backPanel = Sprite::createWithSpriteFrameName(frameName);
    backPanel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wrapper->setPosition(VisibleRect::center());
    wrapper->setScale(m_fScaleFactor);
    wrapper->addChild(backPanel);
    addChild(wrapper);
    
    auto label = Label::createWithBMFont("tip.fnt", tip);
    label->setDimensions(DESIGN_WIDTH-100, 150);
    label->setAlignment(TextHAlignment::LEFT);
    label->setPositionY(-30);
    wrapper->addChild(label);
    
    auto retry = BaseSprite::create("265_136replay.png");
    retry->setPositionY(-140);
    wrapper->addChild(retry);
    retry->setTouchHandler([](Ref *pSender)->void{
        GameScene::create()->run();
    });
}
//
//  BaseLayer.cpp
//  CreazyBomber
//
//  Created by 完美计划 on 14-5-18.
//
//

#include "BaseLayer.h"

#define EVENT_ASSET_LOADED "event_asset_loaded"

enum Tags
{
    kTagWrapper,
    kTagLoading
};

void BaseLayer::onEnter()
{
    Director::getInstance()->getTextureCache()->removeAllTextures();

    log("%s\n", Director::getInstance()->getTextureCache()->getCachedTextureInfo().c_str());
    Layer::onEnter();
    m_pLeft = __createScaleLayer(Point(0.0f,0.5f), VisibleRect::left());
    m_pBody = __createScaleLayer(Point(0.5f,0.5f), VisibleRect::center());
    m_pRight = __createScaleLayer(Point(1.0f,0.5f), VisibleRect::right());
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseLayer::__loadedNotificationHander), EVENT_ASSET_LOADED, nullptr);
    __loadAssets();
}

void BaseLayer::onExit()
{
    Layer::onExit();
    NotificationCenter::getInstance()->removeAllObservers(this);
}

void BaseLayer::__loadedNotificationHander(cocos2d::Ref *pObj)
{
    loadedNum++;
    if(loadedNum==textureFiles.size())
    {
        auto it = textureFiles.begin();
        while (it!=textureFiles.end()) {
            std::string fileName = *it;
            log("load texture %s",fileName.c_str());
            Texture2D *texture = Director::getInstance()->getTextureCache()->getTextureForKey(fileName+".pvr.ccz");
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile(fileName+".plist",texture);
            it++;
        }
        runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([&]()->void{
            onTexturesLoaded();
        }), nullptr));
        return;
    }
    
}

void BaseLayer::onTexturesLoaded()
{
    textureFiles.clear();
    log("%s\n", Director::getInstance()->getTextureCache()->getCachedTextureInfo().c_str());
}

void BaseLayer::__loadAssets()
{
    if (textureFiles.size()==0) {
        this->onTexturesLoaded();
    }
    auto iterator = textureFiles.begin();
    while (iterator!=textureFiles.end()) {
        std::string fileName = *iterator+".pvr.ccz";
        Director::getInstance()->getTextureCache()->addImageAsync(fileName.c_str(), &BaseLayer::__loadedHandler);
        iterator++;
    }
}

void BaseLayer::__loadedHandler(cocos2d::Texture2D *texture)
{
    NotificationCenter::getInstance()->postNotification(EVENT_ASSET_LOADED);
}

bool BaseLayer::init()
{
    if (!Layer::init()) {
        return false;
    }

    m_winSize = Director::getInstance()->getWinSize();
    auto m_fScaleWidth = m_winSize.width/DESIGN_WIDTH;
    auto m_fScaleHeight = m_winSize.height/DESIGN_HEIGHT;
    m_fScaleFactor = m_fScaleWidth>m_fScaleHeight?m_fScaleWidth:m_fScaleHeight;
    if(textureFiles.size()==0)
    {
        
    }
    auto wrapper = Layer::create();
    wrapper->ignoreAnchorPointForPosition(false);
    wrapper->setContentSize(Size(DESIGN_WIDTH,DESIGN_HEIGHT));
    wrapper->setPosition(VisibleRect::center());
    wrapper->setAnchorPoint(Point(0.5f,0.5f));
    wrapper->setScale(m_fScaleFactor);
    wrapper->setTag(kTagWrapper);
    addChild(wrapper);
    return true;
}

void BaseLayer::run()
{
    auto scene = Scene::create();
    scene->addChild(this);
    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene==nullptr) {
        Director::getInstance()->runWithScene(scene);
        return;
    }
    Director::getInstance()->replaceScene(scene);
}

auto BaseLayer::__createScaleLayer(const cocos2d::Point &anchorPoint, const cocos2d::Point &position) -> Layer*
{
    auto layer = Layer::create();
    layer->setContentSize(Size(DESIGN_WIDTH,DESIGN_HEIGHT));
    layer->setScale(m_fScaleFactor);
    layer->ignoreAnchorPointForPosition(false);
    layer->setAnchorPoint(anchorPoint);
    layer->setPosition(position);
    this->addChild(layer);
    return layer;
}
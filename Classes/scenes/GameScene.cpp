//
//  GameScene.cpp
//  CatchPanda
//
//  Created by AIRIA on 14-8-13.
//
//

#include "GameScene.h"
#include "panda/Panda.h"
#include "panda/Dot.h"

#define DOTS_COUNT 81

bool GameScene::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    wrapper = Node::create();
    return true;
}

void GameScene::onTexturesLoaded()
{
    auto bg = Sprite::createWithSpriteFrameName("bg640_1136xin.png");
    bg->setPosition(DESIGN_CENTER);
    m_pBody->addChild(bg);
    m_pBody->addChild(wrapper);
    initDots();
}

void GameScene::initDots()
{
    auto startY = 0,startX = 0;
    auto size = Size::ZERO;
    for (auto i=0; i<DOTS_COUNT; i++) {
        auto fileName = "pot62h.png";
        auto enable = true;
        if (rand()%5==0&&i!=40) {
            fileName = "pot62r.png";
            enable = false;
        }
        auto dot = Dot::create(fileName);
        size = dot->getContentSize();
        auto row = i/9,col = i%9;
        startY = row*size.height;
        startX = col*size.width;
        if(row%2!=0)
        {
            startX += size.width/2;
        }
        dot->setRow(row+1);
        dot->setCol(col+1);
        dot->setPosition(Point(startX,startY));
        dot->setTag(i+1000);
        if(enable==false)
        {
            dot->getEventDispatcher()->removeEventListenersForTarget(dot);
            dot->setIsEnable(enable);
        }
        wrapper->addChild(dot);
        dot->setTouchHandler([this](Ref *pSender)->void{
            this->__dotTouchHandler(pSender);
        });
        
    }
    wrapper->setPosition(Point((DESIGN_WIDTH-size.width*8.5)/2,130));
    
    panda = Panda::create();
    auto dot = static_cast<Dot*>(wrapper->getChildByTag(1040));
    auto position = dot->getPosition();
    panda->setRow(dot->getRow());
    panda->setCol(dot->getCol());
    panda->setPosition(position);
    wrapper->addChild(panda);
}

void GameScene::__dotTouchHandler(Ref *pSender)
{
    auto target = static_cast<Dot*>(pSender);
    target->getEventDispatcher()->removeEventListenersForTarget(this);
    target->setDisplayFrame(Sprite::createWithSpriteFrameName("pot62r.png")->getSpriteFrame());
    target->setIsEnable(false);
    
    auto dot = static_cast<Dot*>(wrapper->getChildByTag(1040));
    
    /* 检测熊猫所有可以行走的点 */
    __getNeighbor(dot);
    log("road:%ld",roadDotsVec.size());
    if(__isCatch())
    {
        panda->beCatch();
    }
    roadDotsVec.clear();
}


void GameScene::__getNeighbor(Dot *dot)
{
    auto row = dot->getRow();
    auto col = dot->getCol();
    auto offset = 1;
    if (row%2) {
        offset=-1;
    }
    
    auto d1 = row*9+col+1;
    auto d2 = row*9+col-1;
    auto d3 = (row-1)*9+col;
    auto d4 = (row-1)*9+col+offset;
    auto d5 = (row+1)*9+col;
    auto d6 = (row+1)*9+col+offset;
    std::vector<int> idxVec= {d1,d2,d3,d4,d5,d6};
    for (auto i=idxVec.begin(); i!=idxVec.end(); i++) {
        auto node = wrapper->getChildByTag(*i+1000-10);
        if(node==nullptr)
        {
            continue;
        }
        auto dot = static_cast<Dot*>(node);
        if (roadDotsVec.contains(dot)||dot->getIsEnable()==false) {
            continue;
        }
        roadDotsVec.pushBack(dot);
        __getNeighbor(dot);
    }
}

bool GameScene::__isCatch()
{
    for (auto i=roadDotsVec.begin(); i!=roadDotsVec.end(); i++) {
        auto dot = *i;
        auto row = dot->getRow(),col = dot->getCol();
        if(row==1||row==9||col==1||col==9)
        {
            return false;
        }
    }
    return true;
}

Dot *GameScene::__getNextDot()
{
    
}

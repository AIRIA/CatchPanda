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
        dotsVec.pushBack(dot);
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
    __findShortPath();
    if (bestPath.size()==0)
    {
        panda->beCatch();
    }
    else
    {
        auto idx = 1;
        if (bestPath.size()==1) {
            idx = 0;
        }
        auto nextDot = bestPath.at(idx);
        panda->setCol(nextDot->getCol());
        panda->setRow(nextDot->getRow());
        panda->setPosition(nextDot->getPosition());
    }
    
    /* 检测熊猫所有可以行走的点 */
    __getNeighbor(dot);
    log("road:%ld",roadDotsVec.size());
    if(__isCatch())
    {
        
    }
    roadDotsVec.clear();
    
}


void GameScene::__getNeighbor(Dot *dot)
{
    auto surroundDotsVec = __getSurroundDots(dot);
    for (auto it=surroundDotsVec.begin(); it!=surroundDotsVec.end(); it++) {
        if (roadDotsVec.contains(*it)) {
            continue;
        }
        roadDotsVec.pushBack(*it);
        __getNeighbor(*it);
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

Vector<Dot*> GameScene::__getSurroundDots(Dot *dot)
{
    Vector<Dot *> surroundDotsVec;
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
        if (dot->getIsEnable()==false) {
            continue;
        }
        surroundDotsVec.pushBack(dot);
    }
    return surroundDotsVec;
}

Vector<Dot*> GameScene::__findShortestStep(Dot *endDot)
{
    Vector<Dot*> openVec; //寻路待考察集合
    Vector<Dot*> closeVec;//寻路已考察集合
    auto startDot = __getPandaDot();
    auto dot = startDot;
    while (dot!=endDot) {
        auto surroundDots = __getSurroundDots(dot);
        /* 计算出来周围的节点代价 */
        for (auto it = surroundDots.begin(); it!=surroundDots.end(); it++)
        {
            auto surroundDot = *it;
            auto cost = surroundDot->getPosition().getDistance(endDot->getPosition());
            if (openVec.contains(surroundDot) || closeVec.contains(surroundDot))
            {
                if (surroundDot->getCost()>cost)
                {
                    surroundDot->setCost(cost);
                }
            }
            else
            {
                surroundDot->setCost(cost);
                openVec.pushBack(surroundDot);
            }
        }
        /* 对代价进行排序 选择最优秀的点 */
        auto minCost = -1;
        Dot *nextDot = nullptr;
        for(auto it = openVec.begin();it!=openVec.end();it++)
        {
            auto cost = (*it)->getCost();
            if (minCost==-1 || cost<minCost)
            {
                minCost = cost;
                nextDot = *it;
            }
        }
        /* 当前开始的节点已经考察过 不再考察 加入到已考察列表*/
        closeVec.pushBack(dot);
//        dot->setOpacity(128);
        if(openVec.size()==0)
        {
            log("%s","路径无效");
            closeVec.clear();
            return closeVec;
        }
        dot = nextDot;
        openVec.eraseObject(nextDot);
    }
    return closeVec;
}

void GameScene::__findShortPath()
{
    Vector<Dot*> borderDotVec;
    for (auto it = dotsVec.begin(); it!=dotsVec.end(); it++) {
        auto dot = *it;
        auto row = dot->getRow(),col = dot->getCol();
        if ((row==1||row==9||col==1||col==9)&&dot->getIsEnable()) {
            borderDotVec.pushBack(dot);
        }
    }
    
    auto step = -1;
    for (auto it = borderDotVec.begin(); it!=borderDotVec.end(); it++) {
       
        auto path = __findShortestStep(*it);
        if (step==-1 || path.size()<step) {
            step = path.size();
            bestPath.clear();
            bestPath.pushBack(path);
        }
    }
    
}

int GameScene::__getShortestDistance(Dot *dot)
{
    auto surroundDots = __getSurroundDots(dot);
    auto step = 0;
    for (auto it=surroundDots.begin(); it!=surroundDots.end(); it++) {
        auto dot = *it;
        auto row = dot->getRow();
        auto col = dot->getRow();
        
    }
}

Dot *GameScene::__getPandaDot()
{
    auto pandaRow = panda->getRow();
    auto pandaCol = panda->getCol();
    auto idx = pandaRow*9+pandaCol;
    return static_cast<Dot*>(wrapper->getChildByTag(idx+1000-10));
}

Dot *GameScene::__getNextDot()
{
    
}

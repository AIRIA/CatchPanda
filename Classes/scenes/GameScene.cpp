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
#include "panda/ResultPanel.h"

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
    target->getEventDispatcher()->removeEventListenersForTarget(target);
    
    __findShortPath();

    Dot *nextDot = nullptr;
    if (bestPath.size()==0)
    {
        panda->beCatch();
        /* 被围住以后随机移动 */
        auto surroundDots = __getSurroundDots(__getPandaDot());
        auto count = surroundDots.size();
        if (count==0) {
            auto result = ResultPanel::create(ResultPanel::win);
            addChild(result);
            return;
        }
        else
        {
            auto idx = rand()%count;
            nextDot = surroundDots.at(idx);
        }
    }
    else
    {
        nextDot = bestPath.at(bestPath.size()-1);
    }
    panda->setCol(nextDot->getCol());
    panda->setRow(nextDot->getRow());
    panda->setPosition(nextDot->getPosition());
    
    /* 判断是不是游戏结束 */
    auto pandaRow = panda->getRow(),pandaCol = panda->getCol();
    if(pandaRow==1||pandaRow==9||pandaCol==1||pandaCol==9)
    {
        auto result = ResultPanel::create(ResultPanel::failed);
        addChild(result);
    }
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
    // temp
    auto dotIt = dotsVec.begin();
    while (dotIt!=dotsVec.end()) {
        (*dotIt)->setOpacity(255);
        dotIt++;
    }
    
    
    while (dot!=endDot) {
        auto surroundDots = __getSurroundDots(dot);
        /* 计算出来周围的节点代价 */
        for (auto it = surroundDots.begin(); it!=surroundDots.end(); it++)
        {
            auto surroundDot = *it;
            auto costG = 1+dot->getCostG();
            auto costH = surroundDot->getPosition().getDistance(endDot->getPosition());
            auto cost = costG+costH;
            if (openVec.contains(surroundDot) || closeVec.contains(surroundDot))
            {
                if (surroundDot->getCost()>cost)
                {
                    surroundDot->setCost(cost);
                    surroundDot->setCostG(costG);
                    surroundDot->setCostH(costH);
                }
            }
            else
            {
                surroundDot->setCost(cost);
                surroundDot->setCostG(costG);
                surroundDot->setCostH(costH);
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
        
        if(openVec.size()==0)
        {
            if(dot==startDot)
            {
                log("%s","路径无效");
                closeVec.clear();
                return closeVec;
            }
            else
            {
                dot = dot->parentDot;
                continue;
            }
        }
        nextDot->parentDot = dot;
        dot = nextDot;
        openVec.eraseObject(nextDot);
    }

    closeVec.pushBack(endDot);
    
    /* 恢复Dot的代价 */
    auto it = dotsVec.begin();
    while (it!=dotsVec.end()) {
        auto dot = *it;
        dot->setCostH(0);
        dot->setCost(0);
        dot->setCostG(0);
        it++;
    }
    
    Vector<Dot*> pathVec;
    auto path = endDot;
    while (path!=startDot) {
        pathVec.pushBack(path);
        path = path->parentDot;
    }
    
    return pathVec;
}

void GameScene::__findShortPath()
{
    bestPath.clear();
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
        if(path.size()==0)
        {
            continue;
        }
        if (step==-1 || path.size()<step) {
            step = path.size();
            bestPath.clear();
            bestPath.pushBack(path);
        }
    }
    
//    auto path = bestPath.begin();
//    while (path!=bestPath.end()) {
//        (*path)->setOpacity(128);
//        path++;
//    }
    
}

Dot *GameScene::__getPandaDot()
{
    auto pandaRow = panda->getRow();
    auto pandaCol = panda->getCol();
    auto idx = pandaRow*9+pandaCol;
    return static_cast<Dot*>(wrapper->getChildByTag(idx+1000-10));
}


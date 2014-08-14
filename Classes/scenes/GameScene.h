//
//  GameScene.h
//  CatchPanda
//
//  Created by AIRIA on 14-8-13.
//
//

#ifndef __CatchPanda__GameScene__
#define __CatchPanda__GameScene__

#include "base/BaseLayer.h"
#include "base/BaseSprite.h"
#include "panda/Dot.h"

class Panda;

class GameScene:public BaseLayer
{
public:
    virtual bool init();
    virtual void onTexturesLoaded();
    CREATE_FUNC(GameScene);
private:
    Panda *panda;
    Node *wrapper;
    void initDots();
    Vector<Dot*> dotsVec;
    Vector<Dot*> roadDotsVec; //可以行走的dot的集合
    void __dotTouchHandler(Ref *pSender);
    /**
     * 获取到所有可以通过的路点 
     */
    void __getNeighbor(Dot* dot);
    /**
     * 检测是不是被大体包围住了 
     */
    bool __isCatch();
    /** 
     * 获取下一个要行走到的点 
     */
    Dot *__getNextDot();
    /* 获取给定的点 周围可以通过的点 */
    Vector<Dot*> __getSurroundDots(Dot *dot);
    /**
     * 获取熊猫与指定的点的最近的距离 
     */
    int __getShortestDistance(Dot *dot);
    /**
     * 获取熊猫所在的点 
     */
    Dot *__getPandaDot();

};

#endif /* defined(__CatchPanda__GameScene__) */
//
//  Dot.h
//  CatchPanda
//
//  Created by AIRIA on 14-8-14.
//
//

#ifndef __CatchPanda__Dot__
#define __CatchPanda__Dot__

#include "base/BaseSprite.h"

class Dot:public BaseSprite
{
public:
    static Dot *create(std::string fileName);
    
    /**
     * 寻路过程中的父节点
     */
    Dot *parentDot = nullptr;
    
    CC_SYNTHESIZE(int, m_iRow, Row);
    CC_SYNTHESIZE(int, m_iCol, Col);
    CC_SYNTHESIZE(bool, m_bIsEnable, IsEnable);
    CC_SYNTHESIZE(float, m_fCost, Cost);
    CC_SYNTHESIZE(float, m_fCostG, CostG);
    CC_SYNTHESIZE(float, m_fCostH, CostH);
    
};

#endif /* defined(__CatchPanda__Dot__) */

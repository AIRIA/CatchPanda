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
    CC_SYNTHESIZE(int, m_iRow, Row);
    CC_SYNTHESIZE(int, m_iCol, Col);
    CC_SYNTHESIZE(bool, m_bIsEnable, IsEnable);
    
};

#endif /* defined(__CatchPanda__Dot__) */

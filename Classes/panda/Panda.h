//
//  Panda.h
//  CatchPanda
//
//  Created by AIRIA on 14-8-13.
//
//

#ifndef __CatchPanda__Panda__
#define __CatchPanda__Panda__

#include "panda/Dot.h"

class Panda:public Dot
{
public:
    static Panda *create();
    void initAnimations();
    void beCatch();
};

#endif /* defined(__CatchPanda__Panda__) */

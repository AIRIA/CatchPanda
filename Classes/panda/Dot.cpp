//
//  Dot.cpp
//  CatchPanda
//
//  Created by AIRIA on 14-8-14.
//
//

#include "Dot.h"

Dot *Dot::create(std::string fileName)
{
    auto dot = new Dot();
    if (dot&&dot->initWithSpriteFrameName(fileName)) {
        dot->autorelease();
        dot->addEventListener();
        dot->setIsEnable(true);
        dot->setCost(0);
        dot->setCostG(0);
        dot->setCostH(0);
        return dot;
    }
    
    return nullptr;

}
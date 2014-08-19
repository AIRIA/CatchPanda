//
//  ResultPanel.h
//  CatchPanda
//
//  Created by AIRIA on 14-8-18.
//
//

#ifndef __CatchPanda__ResultPanel__
#define __CatchPanda__ResultPanel__

#include "common/CommonHeaders.h"

class ResultPanel:public LayerColor
{
public:
    enum ResultType{
        win,
        failed
    };
    static ResultPanel *create(ResultType type);
    CC_SYNTHESIZE(ResultType, m_eResultType, ResultType);
private:
    void addEventListener();
    void initUI();
    virtual void onEnter();
};

#endif /* defined(__CatchPanda__ResultPanel__) */

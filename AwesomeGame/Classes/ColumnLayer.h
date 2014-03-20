#ifndef __COLOUMN_LAYER_H__
#define __COLOUMN_LAYER_H__

#include "cocos2d.h"

using namespace cocos2d;

class ColumnLayer : public CCLayerColor
{
public:
    int currentPosition;
    int desiredPosition;
    
    static ColumnLayer* create(const ccColor4B& color, GLfloat width, GLfloat height);
};


#endif // __COLOUMN_LAYER_H__

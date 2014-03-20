#include "ColumnLayer.h"

ColumnLayer* ColumnLayer::create(const ccColor4B& color, GLfloat width, GLfloat height)
{
    ColumnLayer *pSprite = new ColumnLayer();
    
    if (pSprite && pSprite->initWithColor(color, width, height)) {
        // Set to autorelease
        pSprite->autorelease();
        
        return pSprite;
    }
    
    CC_SAFE_DELETE(pSprite);
    return NULL;
}
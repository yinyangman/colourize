#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class GameScene : public CCLayerColor
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    void initalisePlayer();
    void initaliseStrips();

    void drawStrip(int x, int y, float width, float height, const ccColor4B& color);
    void updatePlayerPosition();
    
    void registerWithTouchDispatcher();
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
            
    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);
};


#endif // __GAME_SCENE_H__

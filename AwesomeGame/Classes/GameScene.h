#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ColumnLayer.h"

using namespace cocos2d;

class GameScene : public CCLayerColor
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void update(float delta);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    void initaliseTurrets();
    void initaliseStrips();
    
    void createSmallStrip(int x, float width, const ccColor4B& color);
    void createLargeStrip(int x, float width, const ccColor4B& color);
    void updateStripsPosition();
    void swapStrips(ColumnLayer* strip1, ColumnLayer* strip2);
    bool allCorrect();
    
    ColumnLayer* stripInPosition(int position);
    ColumnLayer* leftStrip(ColumnLayer* strip);
    ColumnLayer* rightStrip(ColumnLayer* strip);

    void startLevel();
    
    void spawnMover();
    void moverReachesBottom(CCSprite* mover);

    void registerWithTouchDispatcher();
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    
    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);
};


#endif // __GAME_SCENE_H__

#include "GameScene.h"
#include "CCControlUtils.h"

USING_NS_CC;

int columnsCount = 6;
int rowsCount = 1;
int stripBlocksCount = 8;
int playerWidth = 30;
int playerHeight = 30;

CCArray* stripsArray;

CCLayerColor* player;
int position = 0;

float stripHeight;
float stripWidth;

CCScene* GameScene::scene()
{
	CCScene * scene = NULL;
	do
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);
        
		// 'layer' is an autorelease object
		GameScene *layer = GameScene::create();
		CC_BREAK_IF(! layer);
        
		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);
    
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	bool bRet = false;
	do
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////
        
		CC_BREAK_IF(! CCLayerColor::initWithColor( ccc4(255,255,255,255) ) );
        
//        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

                
        this->setTouchEnabled(true);
        
        GameScene::initalisePlayer();
        GameScene::initaliseStrips();
        
		bRet = true;
	} while (0);
    
	return bRet;
}

void GameScene::initalisePlayer() {
    player = CCLayerColor::create(ccc4(255, 255, 255, 255), playerWidth, playerHeight);
    player->setZOrder(2);
    this->addChild(player);
    
    GameScene::updatePlayerPosition();
}

void GameScene::initaliseStrips() {
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    stripWidth = visibleSize.width / columnsCount;
    stripHeight = visibleSize.height / rowsCount;
    
    stripsArray = new CCArray;
    
    std::vector<ccColor3B> baseColours;
    baseColours.push_back(ccc3(255, 0, 0)); //Red
    baseColours.push_back(ccc3(0, 255, 0)); //Green
    baseColours.push_back(ccc3(0, 0, 255)); //Blue
    baseColours.push_back(ccc3(255, 255, 0)); //Yellow
    baseColours.push_back(ccc3(255, 0, 255)); //Purple
    baseColours.push_back(ccc3(0, 255, 255)); //Turquiose
    
    int randomColourIndex = arc4random_uniform(baseColours.size());
    ccColor3B randomColour = baseColours[randomColourIndex];
    
    for (int i = 0; i < columnsCount; i++) {
        for (int j = 0; j < rowsCount; j++) {
            
            int red = randomColour.r - (i*30);
            int blue = randomColour.b - (i*30);
            int green = randomColour.g - (i*30);
            
            if (red < 0) {
                red = 0;
            }
            
            if (green < 0) {
                green = 0;
            }

            if (blue < 0) {
                blue = 0;
            }
            
            GameScene::drawStrip(i, j, stripWidth, stripHeight, ccc4(red, green, blue, 255));
        }
    }
}

void GameScene::drawStrip(int x, int y, float width, float height, const ccColor4B& color)
{
    CCLayerColor* strip = CCLayerColor::create(color, width, height);
    
    strip->setPosition(ccp(width*x, height*y));
    strip->setZOrder(1);
    this->addChild(strip);
    
    stripsArray->addObject(strip);
    
//    for (int i = 0; i < kStripBlocksCount; i++) {
//        CCLayerColor* square = CCLayerColor::create(ccc4(arc4random_uniform(256), arc4random_uniform(256), arc4random_uniform(256), 255), (width/kStripBlocksCount), (width/kStripBlocksCount));
//        square->setPosition(ccp((width/kStripBlocksCount)*i, (height - (width/kStripBlocksCount))));
//        square->setZOrder(3);
//        strip->addChild(square);
//    }
}

void GameScene::updatePlayerPosition() {
    player->setPosition(ccp((stripWidth*position) + (stripWidth/2) - (playerWidth/2), 0.0));
}

void GameScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    if (location.x > visibleSize.width/2) {
        position++;
    } else {
        position--;
    }
    
    if (position < 0) {
        position = 0;
    } else if (position > (columnsCount-1)) {
        position = (columnsCount-1);
    }
    
    GameScene::updatePlayerPosition();
}

void GameScene::registerWithTouchDispatcher()
{
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
    
}

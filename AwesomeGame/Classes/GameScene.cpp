#include "GameScene.h"
#include "CCControlUtils.h"

USING_NS_CC;

int columnsCount = 6;
int intialDelay = 1;
int timeBetweenMoverSpawns = 2;
int moverSpeed = 150.0;
int bulletSpeed = 600.0;

CCArray* smallStripsArray;
CCArray* largeStripsArray;
CCArray* bulletsArray;
CCArray* moversArray;
CCLayerColor* player;

float stripLargeHeight;
float stripSmallHeight;
float stripWidth;

CCSize visibleSize;

CCTimer* spawnMoverTimer;

int z_SmallStrip = 0;
int z_LargeStrip = 0;
int z_Bullet = 1;
int z_Mover = 1;
int z_Turret = 1;
int z_WhiteLine = 2;

bool randomColours = false;

//int myrandom (int i) { return std::rand()%i;}

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
        
        this->setTouchEnabled(true);
        
        visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        stripWidth = visibleSize.width / columnsCount;
        stripLargeHeight = visibleSize.height * (5.0/6.0);
        stripSmallHeight = visibleSize.height * (1.0/6.0);
        
        GameScene::initaliseTurrets();
        
        CCLayerColor* whiteLine = CCLayerColor::create(ccc4(255, 255, 255, 255), visibleSize.width, 10);
        whiteLine->setPosition(0.0, stripSmallHeight-5);
        whiteLine->setZOrder(z_WhiteLine);
        this->addChild(whiteLine);
        
        GameScene::startLevel();
        
        this->schedule(schedule_selector(GameScene::spawnMover), timeBetweenMoverSpawns, MAXFLOAT, intialDelay);
        
        this->scheduleUpdate();
        
		bRet = true;
	} while (0);
    
	return bRet;
}

void GameScene::update(float delta)
{
    //Update bullets
    int count = bulletsArray->count();
    int start = count-1;
    
    if (count > 0) {
        for (int i = start; i >= 0; i--) {
            CCSprite* bullet = (CCSprite*)bulletsArray->objectAtIndex(i);
            CCPoint currentPos = bullet->getPosition();
            
            if (currentPos.y > visibleSize.height) {
                bullet->removeFromParent();
                bulletsArray->removeObject(bullet);
            } else {
                bullet->setPosition(CCPoint(currentPos.x, currentPos.y+(delta*bulletSpeed)));
            }
        }
    }
    
    //Update movers
    count = moversArray->count();
    start = count-1;
    
    if (count > 0) {
        for (int i = start; i >= 0; i--) {
            CCSprite* mover = (CCSprite*)moversArray->objectAtIndex(i);
            CCPoint currentPos = mover->getPosition();
            
            if (currentPos.y < stripSmallHeight) {
                GameScene::moverReachesBottom(mover);
                mover->removeFromParent();
                moversArray->removeObject(mover);
            } else {
                mover->setPosition(CCPoint(currentPos.x, currentPos.y-(delta*moverSpeed)));
            }
        }
    }
    
    //Check for collisions
    
    CCArray *bulletsToDelete = new CCArray;
    CCArray* moversToDelete =new CCArray;
    CCObject* it = NULL;
    CCObject* jt = NULL;
    
    CCARRAY_FOREACH(bulletsArray, it)
    {
        CCSprite *bullet = (CCSprite*)it;
        CCRect bulletRect = CCRectMake(
                                           bullet->getPosition().x - (bullet->getContentSize().width/2),
                                           bullet->getPosition().y - (bullet->getContentSize().height/2),
                                           bullet->getContentSize().width,
                                           bullet->getContentSize().height);
        
        
        CCARRAY_FOREACH(moversArray, jt)
        {
            CCSprite *mover = (CCSprite*)jt;
            CCRect moverRect = CCRectMake(
                                           mover->getPosition().x - (mover->getContentSize().width/2),
                                           mover->getPosition().y - (mover->getContentSize().height/2),
                                           mover->getContentSize().width,
                                           mover->getContentSize().height);
            
            if (bulletRect.intersectsRect(moverRect))
            {
                bulletsToDelete->addObject(bullet);
                moversToDelete->addObject(mover);
//                GameScene::collisionBetweenBulletAndMover(bullet, mover);
            }
        }
    }
    
    CCARRAY_FOREACH(bulletsToDelete, jt)
    {
        CCSprite *bullet = (CCSprite*)jt;
        bulletsArray->removeObject(bullet);
        this->removeChild(bullet, true);
    }
    
    CCARRAY_FOREACH(moversToDelete, it)
    {
        CCSprite* mover = (CCSprite*)it;
        moversArray->removeObject(mover);
        this->removeChild(mover, true);
    }
    
    
    moversToDelete->release();
    bulletsToDelete->release();
}

void GameScene::startLevel()
{
    if (bulletsArray) {
        for (int i = 0; i < bulletsArray->count(); i++) {
            CCSprite* bullet = (CCSprite*)bulletsArray->objectAtIndex(i);
            bullet->removeFromParent();
        }
    }
    
    if (moversArray) {
        for (int i = 0; i < moversArray->count(); i++) {
            CCSprite* mover = (CCSprite*)moversArray->objectAtIndex(i);
            mover->removeFromParent();
        }
    }
    
    if (smallStripsArray) {
        for (int i = 0; i < smallStripsArray->count(); i++) {
            ColumnLayer* strip = (ColumnLayer*)smallStripsArray->objectAtIndex(i);
            strip->removeFromParent();
        }
    }
    
    if (largeStripsArray) {
        for (int i = 0; i < largeStripsArray->count(); i++) {
            ColumnLayer* strip = (ColumnLayer*)largeStripsArray->objectAtIndex(i);
            strip->removeFromParent();
        }
    }
    
    bulletsArray = new CCArray;
    moversArray = new CCArray;
    smallStripsArray = new CCArray;
    largeStripsArray = new CCArray;
    
    GameScene::initaliseStrips();
}

void GameScene::moverReachesBottom(CCSprite* mover)
{
	CCPoint location = mover->getPosition();
    int stripLocation = location.x / stripWidth;

    ColumnLayer* selectedColumnLayer = GameScene::stripInPosition(stripLocation);
    ColumnLayer* adjacentStrip;
    
    int moverDirection = mover->getTag();
    
    if (moverDirection == 1) {
        adjacentStrip = GameScene::rightStrip(selectedColumnLayer);
    } else {
        adjacentStrip = GameScene::leftStrip(selectedColumnLayer);
    }
    
    GameScene::swapStrips(selectedColumnLayer, adjacentStrip);
    GameScene::updateStripsPosition();
}

void GameScene::initaliseTurrets() {
    for (int i = 0; i < columnsCount; i++) {
        CCSprite* turret = CCSprite::create("ship.png");
        turret->setPosition(CCPoint((i*stripWidth) + (stripWidth/2), 20));
        turret->setZOrder(z_Turret);
        this->addChild(turret);
    }
}

void GameScene::initaliseStrips() {
    //Set all possible starting colours
    
    std::vector<ccColor3B> stripColours;
    
    if (randomColours) {
        for (int i = 0; i < columnsCount; i++) {
            stripColours.push_back(ccc3(arc4random_uniform(255), arc4random_uniform(255), arc4random_uniform(255)));
        }
    } else {
    
        std::vector<ccColor3B> baseColours;
        baseColours.push_back(ccc3(255, 0, 0)); //Red
        baseColours.push_back(ccc3(0, 255, 0)); //Green
        baseColours.push_back(ccc3(0, 0, 255)); //Blue
        baseColours.push_back(ccc3(255, 255, 0)); //Yellow
        baseColours.push_back(ccc3(255, 0, 255)); //Purple
        baseColours.push_back(ccc3(0, 255, 255)); //Turquiose
        
        //Pick a starting colour
        int randomColourIndex = arc4random_uniform(baseColours.size());
        ccColor3B randomColour = baseColours[randomColourIndex];
        
        //Create all the different colour shades
        for (int i = 0; i < columnsCount; i++) {
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
            
            stripColours.push_back(ccc3(red, green, blue));
        }
    }
    
    //Draw the strips
    for (int i = 0; i < columnsCount; i++) {
        ccColor3B colour = stripColours[i];
        GameScene::createLargeStrip(i, stripWidth, ccc4(colour.r, colour.b, colour.g, 255));
        GameScene::createSmallStrip(i, stripWidth, ccc4(colour.r, colour.b, colour.g, 255));
    }
    
    //Randomise the colour shades:
    
    int count = largeStripsArray->count();
    for (int i = 0; i < count; ++i) {
        // Select a random element between i and end of array to swap with.
        int nElements = count - i;
        int n = arc4random_uniform(nElements) + i;
        largeStripsArray->exchangeObjectAtIndex(i, n);
    }

    //shuffle Strips
    for (int i = 0; i < largeStripsArray->count(); i++) {
        ColumnLayer* columnStrip = (ColumnLayer*)largeStripsArray->objectAtIndex(i);
        columnStrip->currentPosition = i;
        columnStrip->setPosition(stripWidth*i, stripSmallHeight);
    }
    
    for (int i = 0; i < smallStripsArray->count(); i++) {
        ColumnLayer* columnStrip = (ColumnLayer*)smallStripsArray->objectAtIndex(i);
        columnStrip->currentPosition = i;
        columnStrip->setPosition(stripWidth*i, 0.0);
    }
}

void GameScene::createSmallStrip(int x, float width, const ccColor4B& color)
{    
    ColumnLayer* strip = ColumnLayer::create(color, width, stripSmallHeight);
    strip->desiredPosition = x;
    strip->setZOrder(z_SmallStrip);
    this->addChild(strip);
    
    smallStripsArray->addObject(strip);
}

void GameScene::createLargeStrip(int x, float width, const ccColor4B& color)
{
    ColumnLayer* strip = ColumnLayer::create(color, width, stripLargeHeight);
    strip->desiredPosition = x;
    strip->setZOrder(z_LargeStrip);
    this->addChild(strip);
    
    largeStripsArray->addObject(strip);
}

void GameScene::updateStripsPosition()
{
    int count = largeStripsArray->count();
    for (int i = 0; i < count; i++) {
        ColumnLayer* columnStrip = (ColumnLayer*)largeStripsArray->objectAtIndex(i);
        columnStrip->setPosition(stripWidth*columnStrip->currentPosition, stripSmallHeight);
    }
}

void GameScene::spawnMover()
{
    int randomColumn = arc4random_uniform(columnsCount);
    
    CCSprite* mover = CCSprite::create("ship.png");
    mover->setPosition(CCPoint((randomColumn*stripWidth) + (stripWidth/2), visibleSize.height));
    mover->setZOrder(z_Mover);
    
    int randomDirection = arc4random_uniform(2);
    if (randomDirection == 1) {
        mover->setRotation(90);
    } else {
        mover->setRotation(-90);
    }
    
    mover->setTag(randomDirection);
    
    this->addChild(mover);
    moversArray->addObject(mover);
}

void GameScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();
    
    int stripLocation = location.x / stripWidth;
    
    CCSprite* bullet = CCSprite::create("ship.png");
    bullet->setPosition(CCPoint((stripLocation*stripWidth) + (stripWidth/2), 20));
    bullet->setZOrder(z_Bullet);
    bullet->setTag(stripLocation);
    this->addChild(bullet);
    bulletsArray->addObject(bullet);
}

ColumnLayer* GameScene::stripInPosition(int position)
{
    int count = largeStripsArray->count();
    for (int i = 0; i < count; i++) {
        ColumnLayer* columnStrip = (ColumnLayer*)largeStripsArray->objectAtIndex(i);
        if (columnStrip->currentPosition == position) {
            return columnStrip;
        }
    }
    
    return NULL;
}

ColumnLayer* GameScene::leftStrip(ColumnLayer* strip)
{
    int leftPosition = (strip->currentPosition - 1);
    if (leftPosition < 0) {
        leftPosition = (largeStripsArray->count() - 1);
    }
    
    return GameScene::stripInPosition(leftPosition);
}

ColumnLayer* GameScene::rightStrip(ColumnLayer* strip)
{
    int rightPosition = strip->currentPosition + 1;
    if (rightPosition == largeStripsArray->count()) {
        rightPosition = 0;
    }
    
    return GameScene::stripInPosition(rightPosition);
}

void GameScene::swapStrips(ColumnLayer* strip1, ColumnLayer* strip2)
{
    int tempPosition = strip1->currentPosition;
    strip1->currentPosition = strip2->currentPosition;
    strip2->currentPosition = tempPosition;
}

bool GameScene::allCorrect()
{
    int count = largeStripsArray->count();
    for (int i = 0; i < count; i++) {
        ColumnLayer* columnStrip = (ColumnLayer*)largeStripsArray->objectAtIndex(i);
        if (columnStrip->currentPosition != columnStrip->desiredPosition) {
            return false;
        }
    }
    
    return true;
}

void GameScene::registerWithTouchDispatcher()
{
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
    
}

#include "HelloWorldScene.h"
#include "Box2D\Common\b2Math.h"
#include "PhysicsScene\PhysicsScene.h"

USING_NS_CC;

#define  PTM_RATIO 32.0  

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
	auto scene = PhysicsScene::create();//Scene::createWithPhysics();//Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
//	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();

	auto listen = EventListenerTouchOneByOne::create();
	listen->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listen->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listen->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	listen->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
	eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);


    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);


	//_MarblesSpr = Sprite::create("CloseNormal.png");
	//_MarblesSpr->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	//addChild(_MarblesSpr);

	//auto marBlesBoby = PhysicsBody::createCircle(_MarblesSpr->getContentSize().width / 2, PHYSICSBODY_MATERIAL_DEFAULT, Vec2(1, 1));
	//marBlesBoby->setGravityEnable(false);
	//_MarblesSpr->setPhysicsBody(marBlesBoby);
	//_mouseJoint = nullptr;
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
	Vec2 pos = touch->getLocationInView();
	//b2Vec2 posInWorld(pos.x / PTM_RATIO, pos.y / PTM_RATIO);
	if (_MarblesSpr->getBoundingBox().containsPoint(pos))
	{
		b2MouseJointDef *mjd;
		int i = 1111;
	}
	return true;
}

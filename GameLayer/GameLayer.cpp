#include "GameLayer.h"
#include "PhysicsScene\PhysicsScene.h"
#include "Box2D\Dynamics\b2Body.h"
#include "Box2D\Collision\Shapes\b2PolygonShape.h"
#include "Box2D\Collision\Shapes\b2CircleShape.h"
#include "Box2D\Dynamics\b2Fixture.h"
#include "VisibleRect.h"
#define PTM_RATIO    32
GameLayer::GameLayer()
{
}

GameLayer::~GameLayer()
{
}

GameLayer * GameLayer::create(int level)
{
	GameLayer *gamelayer = new(std::nothrow) GameLayer;
	if (gamelayer && gamelayer->init())
	{
		gamelayer->autorelease();
		return gamelayer;
	}
	else
	{
		delete gamelayer;
		gamelayer = nullptr;
		return nullptr;
	}
}

void GameLayer::initPhysicsWorld()
{
	b2Vec2 gravity = b2Vec2(0, 0);
	_physicsWorld = new(std::nothrow) b2World(gravity);
	_physicsWorld->SetAllowSleeping(true);
	_physicsWorld->SetContinuousPhysics(true);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0); // bottom-left corner

									  // Call the body factory which allocates memory for the ground body
									  // from a pool and creates the ground box shape (also from a pool).
									  // The body is also added to the world.
	_limiGroundBody = _physicsWorld->CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2EdgeShape groundBox;

	// bottom
	groundBox.Set(b2Vec2(VisibleRect::leftBottom().x / PTM_RATIO, VisibleRect::leftBottom().y / PTM_RATIO), b2Vec2(VisibleRect::rightBottom().x / PTM_RATIO, VisibleRect::rightBottom().y / PTM_RATIO));
	_limiGroundBody->CreateFixture(&groundBox, 0);

	// top
	groundBox.Set(b2Vec2(VisibleRect::leftTop().x / PTM_RATIO, VisibleRect::leftTop().y / PTM_RATIO), b2Vec2(VisibleRect::rightTop().x / PTM_RATIO, VisibleRect::rightTop().y / PTM_RATIO));
	_limiGroundBody->CreateFixture(&groundBox, 0);

	// left
	groundBox.Set(b2Vec2(VisibleRect::leftTop().x / PTM_RATIO, VisibleRect::leftTop().y / PTM_RATIO), b2Vec2(VisibleRect::leftBottom().x / PTM_RATIO, VisibleRect::leftBottom().y / PTM_RATIO));
	_limiGroundBody->CreateFixture(&groundBox, 0);

	// right
	groundBox.Set(b2Vec2(VisibleRect::rightBottom().x / PTM_RATIO, VisibleRect::rightBottom().y / PTM_RATIO), b2Vec2(VisibleRect::rightTop().x / PTM_RATIO, VisibleRect::rightTop().y / PTM_RATIO));
	_limiGroundBody->CreateFixture(&groundBox, 0);

	_debugDraw = new GLESDebugDraw(PTM_RATIO);   //这里新建一个 debug渲染模块                
	_physicsWorld->SetDebugDraw(_debugDraw);    //设置                 
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_centerOfMassBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_centerOfMassBit;
	flags += b2Draw::e_jointBit;
	//b2Draw::e_centerOfMassBit;   //获取需要显示debugdraw的块                 
	//b2Draw::e_aabbBit;  //AABB块                 
	//b2Draw::e_centerOfMassBit; 物体质心                 
	//b2Draw::e_jointBit;  //关节                
	//b2Draw::e_shapeBit;   形状                 
	_debugDraw->SetFlags(flags);   //需要显示那些东西   
}

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	_mouseJoint = nullptr;
	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	listener->onTouchCancelled = CC_CALLBACK_2(GameLayer::onTouchCancelled, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, -10);
	//Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	scheduleUpdate();
	initPhysicsWorld();
	initMarbles();
	return true;
}

void GameLayer::initMarbles()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	_ballSprite = Sprite::create("CloseNormal.png");
	//_ballSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(_ballSprite);
	b2BodyDef ballSpriteDef;
	ballSpriteDef.type = b2_dynamicBody;
	ballSpriteDef.bullet = true;
	ballSpriteDef.userData = _ballSprite;
	ballSpriteDef.position.Set((visibleSize.width / 2 + origin.x) / PTM_RATIO, (visibleSize.height / 2 + origin.y) / PTM_RATIO);//_ballSprite->getPosition().x / PTM_RATIO, _ballSprite->getPosition().y / PTM_RATIO);
																																 //ballSpriteDef->userData = _ballSprite;
	_ballSpriteBody = _physicsWorld->CreateBody(&ballSpriteDef);

	b2CircleShape ballSpriteShape;
	ballSpriteShape.m_radius = _ballSprite->getContentSize().width / PTM_RATIO / 2;
	b2FixtureDef ballSpriteFixtureDef;
	ballSpriteFixtureDef.shape = &ballSpriteShape;
	ballSpriteFixtureDef.density = 1.0f;
	ballSpriteFixtureDef.friction = 0.5f;
	ballSpriteFixtureDef.restitution = 0.8f;
	_ballSpriteBody->CreateFixture(&ballSpriteFixtureDef);
}

Scene * GameLayer::createScene()
{
	auto scene = Scene::create();

	auto layer = GameLayer::create(0);

	scene->addChild(layer);

	return scene;
}


bool GameLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (nullptr != _mouseJoint)
	{
		return false;
	}
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	b2Vec2 locationWorld = b2Vec2(location.x / PTM_RATIO, location.y / PTM_RATIO);
	if (locationWorld.x < _ballSpriteBody->GetWorldCenter().x + 150.0 / PTM_RATIO)
	{
		b2MouseJointDef mouseDef;
		mouseDef.bodyA = _limiGroundBody;
		mouseDef.bodyB = _ballSpriteBody;
		mouseDef.target = locationWorld;
		mouseDef.collideConnected = true;
		mouseDef.maxForce = 100.0f;//* _ballSpriteBody->GetMass();
		_mouseJoint = (b2MouseJoint *)_physicsWorld->CreateJoint(&mouseDef);
		_ballSpriteBody->SetAwake(true);
		return true;
	}	
	return false;
}

void GameLayer::onTouchEnded(Touch *touch, Event *unused_event) 
{
	if (_mouseJoint != nullptr)
	{
		_physicsWorld->DestroyJoint(_mouseJoint);
		_mouseJoint = nullptr;
	}
}

void GameLayer::onTouchCancelled(Touch *touch, Event *unused_event)
{

}

void GameLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (_mouseJoint == nullptr)
	{
		return;
	}
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	b2Vec2 locationWorld = b2Vec2(location.x / PTM_RATIO, location.y / PTM_RATIO);
	_mouseJoint->SetTarget(locationWorld);
}

void GameLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	/*glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/
	_physicsWorld->DrawDebugData();   //这个一定要写  
	/*glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);*/
}

void GameLayer::update(float delta)
{
	/*
	Step方法需要三个参数。
	第一个是timeStep，它会告诉Box2D自从上次更新以后已经过去多长时间了，直接影响着刚体会在这一步移动多长距离。不建议使用delta time来作为timeStep的值，因为delta time会上下浮动，刚体就不能以相同的速度移动了。
	第二和第三个参数是迭代次数。它们被用于决定物理模拟的精确程度，也决定着计算刚体移动所需要的时间。
	*/
	_physicsWorld->Step(delta, 8, 3);
	for (b2Body *b = _physicsWorld->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != nullptr)
		{
			auto mBallSprite = (Sprite *)(b->GetUserData());
			mBallSprite->setPosition(Vec2(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
		}
	}
}

#include "GameLayer.h"
#include "PhysicsScene\PhysicsScene.h"
#include "Box2D\Dynamics\b2Body.h"
#include "Box2D\Collision\Shapes\b2PolygonShape.h"
#include "Box2D\Collision\Shapes\b2CircleShape.h"
#include "Box2D\Dynamics\b2Fixture.h"
#define PTM_RATIO    32
GameLayer::GameLayer()
{
}

GameLayer::~GameLayer()
{
}

GameLayer * GameLayer::create(b2World *world)
{
	GameLayer *gamelayer = new(std::nothrow) GameLayer;
	if (gamelayer && gamelayer->initPhysicsWorld(world))
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

bool GameLayer::initPhysicsWorld(b2World *world)
{
	if (!Layer::init())
	{
		return false;
	}
	_mouseJoint = nullptr;
	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(GameLayer::onTouchesBegan, this);
	listener->onTouchesCancelled = CC_CALLBACK_2(GameLayer::onTouchesCancelled, this);
	listener->onTouchesEnded = CC_CALLBACK_2(GameLayer::onTouchesEnded, this);
	listener->onTouchesMoved = CC_CALLBACK_2(GameLayer::onTouchesMoved, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

	_physicsWorld = world;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	b2BodyDef *groundDef = new(std::nothrow) b2BodyDef;
	groundDef->position.Set(0, 0);
	_limiGroundBody = _physicsWorld->CreateBody(groundDef);

	_ballSprite = Sprite::create("CloseNormal.png");
	//_ballSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(_ballSprite);
	b2BodyDef *ballSpriteDef = new(std::nothrow) b2BodyDef;
	ballSpriteDef->type = b2_dynamicBody;
	ballSpriteDef->linearDamping = 1;
	ballSpriteDef->angularDamping = 1;
	ballSpriteDef->position.Set((visibleSize.width / 2 + origin.x )/ PTM_RATIO, (visibleSize.height / 2 + origin.y )/ PTM_RATIO);//(_ballSprite->getPosition().x / PTM_RATIO, _ballSprite->getPosition().y / PTM_RATIO);
	ballSpriteDef->userData = _ballSprite;
	_ballSpriteBody = _physicsWorld->CreateBody(ballSpriteDef);
	//delete ballSpriteDef;

	b2CircleShape *ballSpriteShape = new(std::nothrow) b2CircleShape;
	ballSpriteShape->m_radius = _ballSprite->getContentSize().width / PTM_RATIO / 2;
	b2FixtureDef *ballSpriteFixtureDef = new(std::nothrow) b2FixtureDef;
	ballSpriteFixtureDef->shape = ballSpriteShape;
	ballSpriteFixtureDef->density = 1.0f;
	ballSpriteFixtureDef->friction = 1.0f;
	ballSpriteFixtureDef->restitution = 0.8f;
	_ballSpriteBody->CreateFixture(ballSpriteFixtureDef);
	//delete ballSpriteShape;
	//delete ballSpriteFixtureDef;
	return true;
}

Scene * GameLayer::createScene()
{
	auto scene = PhysicsScene::create();

	auto layer = GameLayer::create(scene->getWorld());

	scene->addChild(layer);

	return scene;
}

void GameLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	if (nullptr != _mouseJoint)
	{
		return;
	}
	for (auto &touch : touches)
	{
		Vec2 touchPoint = touch->getLocationInView();
		Vec2 glPoint = touch->getLocation();
		b2Vec2 localPoint = b2Vec2(glPoint.x / PTM_RATIO, glPoint.y / PTM_RATIO);
		//if (_ballSprite->getBoundingBox().containsPoint(touchPoint))
		if (_ballSpriteBody->GetFixtureList()->TestPoint(localPoint))
		{
			b2MouseJointDef *mouseDef = new(std::nothrow) b2MouseJointDef;
			mouseDef->bodyA = _limiGroundBody;
			mouseDef->bodyB = _ballSpriteBody;
			mouseDef->target = localPoint;
			mouseDef->collideConnected = true;
			mouseDef->maxForce = 100.0f;//* _ballSpriteBody->GetMass();
			_mouseJoint = (b2MouseJoint *)_physicsWorld->CreateJoint(mouseDef);
			_ballSpriteBody->SetAwake(true);
			//delete mouseDef;
		}
	}
	
}

void GameLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
	if (_mouseJoint != nullptr)
	{
		_mouseJoint = nullptr;
	}
}

void GameLayer::onTouchesCancelled(const std::vector<Touch*>& touches, Event *unused_event)
{
}

void GameLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
	if (_mouseJoint == nullptr)
	{
		return;
	}
	for (auto &touch : touches)
	{
		//b2Vec2 vecMouse;
		//vecMouse.Set((touch->getLocation().x) / PTM_RATIO, (touch->getLocation().y) / PTM_RATIO);
		///*Vec2 touchPoint = touch->getLocation();
		//b2Vec2 localPoint = b2Vec2(touchPoint.x, touchPoint.y);/// PTM_RATIO, touchPoint.y / PTM_RATIO);*/
		//_mouseJoint->SetTarget(vecMouse);
		auto touchLocation = touch->getLocation();
		auto nodePosition = convertToNodeSpace(touchLocation);
		b2Vec2 localPoint = b2Vec2(nodePosition.x, nodePosition.y);
		_mouseJoint->SetTarget(localPoint);
	}
}

#pragma once
#include "cocos2d.h"
#include "Box2D\Dynamics\b2World.h"
#include "Box2D\Dynamics\Joints\b2MouseJoint.h"
USING_NS_CC;

class GameLayer : public Layer
{
public:
	GameLayer();
	virtual ~GameLayer();
	//CREATE_FUNC(GameLayer);
	static GameLayer *create(b2World *world);
	virtual bool initPhysicsWorld(b2World *world);
	static Scene *createScene();
	b2World *getWorld()
	{
		return _physicsWorld;
	}
public:
	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)override;
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event) override;
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event) override;
	virtual void onTouchesCancelled(const std::vector<Touch*>& touches, Event *unused_event) override;
private:
	b2World *_physicsWorld;
	Sprite *_ballSprite;
	b2MouseJoint *_mouseJoint;
	b2Body  *_limiGroundBody;
	b2Body  *_ballSpriteBody;
};
#pragma once
#include "cocos2d.h"
#include "Box2D\Dynamics\b2World.h"
#include "Box2D\Dynamics\Joints\b2MouseJoint.h"
#include "GLES-Render.h"
USING_NS_CC;

class GameLayer : public Layer
{
public:
	GameLayer();
	virtual ~GameLayer();
	//CREATE_FUNC(GameLayer);
	static GameLayer *create(int level);
	virtual void initPhysicsWorld();
	virtual bool init() override;
	void initMarbles();
	static Scene *createScene();

	b2World *getWorld()
	{
		return _physicsWorld;
	}
public:
	virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded(Touch *touch, Event *unused_event) override;
	virtual void onTouchCancelled(Touch *touch, Event *unused_event) override;	
	virtual void onTouchMoved(Touch *touch, Event *unused_event) override;
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	virtual void update(float delta);
private:
	b2World *_physicsWorld;
	Sprite *_ballSprite;
	b2MouseJoint *_mouseJoint;
	b2Body  *_limiGroundBody;
	b2Body  *_ballSpriteBody;
	GLESDebugDraw *_debugDraw;
};
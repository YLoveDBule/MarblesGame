#pragma once 
#include "cocos2d.h"
#include "Box2D\Dynamics\b2World.h"

USING_NS_CC;
class PhysicsScene : public Scene
{
public:
	PhysicsScene();
	virtual ~PhysicsScene();
public:
	CREATE_FUNC(PhysicsScene);
	virtual bool init() override;
	void initPhysicsWorld();
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
private:
	b2World *_physicsWorld;
	cocos2d::Mat4 _modelViewMV;

private:
	void onDraw();
};
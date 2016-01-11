#include "PhysicsScene.h"
#include "base\ccMacros.h"
#include "Box2D\Dynamics\b2Body.h"
PhysicsScene::PhysicsScene()
{

}

PhysicsScene::~PhysicsScene()
{
	CC_SAFE_DELETE(_physicsWorld);
}

bool PhysicsScene::init()
{
	initPhysicsWorld();
	return true;
}

void PhysicsScene::initPhysicsWorld()
{
	b2Vec2 gravity = b2Vec2(0, -10);
	_physicsWorld = new(std::nothrow) b2World(gravity);
	_physicsWorld->SetAllowSleeping(true);
	_physicsWorld->SetContinuousPhysics(true);

	//ground body
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);   //// bottom-left corner 

	// Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.

	b2Body *groundBody = _physicsWorld->CreateBody(&groundBodyDef);

}

void PhysicsScene::draw(cocos2d::Renderer * renderer, const cocos2d::Mat4 & transform, uint32_t flags)
{
	//
	// IMPORTANT:
	// This is only for debug purposes
	// It is recommend to disable it
	//
	Scene::draw(renderer,transform,flags);
	//enable vertex attribs
	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	auto direct = Director::getInstance();
	CCASSERT(nullptr != direct, "Director is null when seting matrix stack");
	direct->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	_modelViewMV = direct->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	CustomCommand command;
	command.init(_globalZOrder);
	command.func = CC_CALLBACK_0(PhysicsScene::onDraw,this);
	renderer->addCommand(&command);

	direct->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void PhysicsScene::onDraw()
{
	Director *direct = Director::getInstance();
	CCASSERT(nullptr != direct, "Director is null when seting matrix stack");

	auto oldMV = direct->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	direct->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewMV);
	_physicsWorld->DrawDebugData();
	direct->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, oldMV);
}


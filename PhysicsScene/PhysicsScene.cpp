#include "PhysicsScene.h"
#include "base\ccMacros.h"
#include "Box2D\Dynamics\b2Body.h"
#include "GameLayer\VisibleRect.h"
#include "Box2D\Collision\Shapes\b2EdgeShape.h"


#define PTM_RATIO    32
PhysicsScene::PhysicsScene()
{

}

PhysicsScene::~PhysicsScene()
{
	CC_SAFE_DELETE(_physicsWorld);
}

bool PhysicsScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	initPhysicsWorld();
	return true;
}

void PhysicsScene::initPhysicsWorld()
{
	b2Vec2 gravity = b2Vec2(0, -10);
	_physicsWorld = new(std::nothrow) b2World(gravity);
	_physicsWorld->SetAllowSleeping(true);
	_physicsWorld->SetContinuousPhysics(true);

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = _physicsWorld->CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2EdgeShape groundBox;

    // bottom
    groundBox.Set(b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);

    // top
    groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);

    // left
    groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);

    // right
    groundBox.Set(b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);

	_debugDraw = new GLESDebugDraw(PTM_RATIO);   //�����½�һ�� debug��Ⱦģ��                
	_physicsWorld->SetDebugDraw(_debugDraw);    //����                 
	uint32 flags = 0;   
	flags += b2Draw::e_shapeBit;   
	flags += b2Draw::e_centerOfMassBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_centerOfMassBit;
	flags += b2Draw::e_jointBit;
	//b2Draw::e_centerOfMassBit;   //��ȡ��Ҫ��ʾdebugdraw�Ŀ�                 
	//b2Draw::e_aabbBit;  //AABB��                 
	//b2Draw::e_centerOfMassBit; ��������                 
	//b2Draw::e_jointBit;  //�ؽ�                
	//b2Draw::e_shapeBit;   ��״                 
	_debugDraw->SetFlags(flags);   //��Ҫ��ʾ��Щ����                
}

void PhysicsScene::draw(cocos2d::Renderer * renderer, const cocos2d::Mat4 & transform, uint32_t flags)
{
	/*glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/
	_physicsWorld->DrawDebugData();   //���һ��Ҫд  
	/*glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);*/
}



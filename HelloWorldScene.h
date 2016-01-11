#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D\Dynamics\Joints\b2MouseJoint.h"
USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
private:
	Sprite *_MarblesSpr;
	b2MouseJoint * _mouseJoint;
};

#endif // __HELLOWORLD_SCENE_H__

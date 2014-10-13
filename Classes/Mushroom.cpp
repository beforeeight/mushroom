/*
 * Mushroom.cpp
 *
 *  Created on: Sep 28, 2014
 *      Author: zhangchen
 */

#include "Mushroom.h"
#include "FinishScene.h"

USING_NS_CC;

Mushroom::Mushroom() :
		vec(vec_stop), jumping(true), dying(false) {
	// TODO Auto-generated constructor stub
}

Mushroom::~Mushroom() {
	// TODO Auto-generated destructor stub
}

bool Mushroom::init() {
	if (CCSprite::initWithTexture(
			CCTextureCache::sharedTextureCache()->textureForKey(
					"mushroom.png"))) {
		this->setAnchorPoint(ccp(0.5, 0.5));
		return true;
	} else {
		return false;
	}
}

PHY_TYPE Mushroom::getPhyType() {
	return MUSHROOM;
}

void Mushroom::createPhyBody() {
	b2BodyDef b2BodyDef;
	b2BodyDef.type = b2_dynamicBody;
	b2BodyDef.bullet = true;
	b2BodyDef.fixedRotation = true;
	b2BodyDef.position = b2Vec2(c2p(this->getPosition().x),
	c2p(this->getPosition().y));
	b2body = PhyWorld::shareWorld()->CreateBody(&b2BodyDef);

	b2PolygonShape b2Shape;
	b2Shape.SetAsBox(c2p(this->getContentSize().width / 2),
	c2p(this->getContentSize().height / 2));

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &b2Shape;
	fixtureDef.density = 3.0f;
	// Add the shape to the body.
	b2body->CreateFixture(&fixtureDef);
//	b2MassData mass;
//	mass.mass = 1.0f;
//	mass.center = b2Vec2_zero;
//	b2body->SetMassData(&mass);
	//b2body->SetLinearDamping(0.0f);
	b2body->SetLinearVelocity(b2Vec2_zero);
	b2body->SetUserData(this);
}

void Mushroom::setVec(MushroomVec vec) {
	this->vec = vec;
}

bool Mushroom::isVec(MushroomVec vec) {
	return this->vec == vec;
}

void Mushroom::update(float delta) {
	float y = this->getPositionY();
	if (y < -LOCAL_RESOLUTION.height / 2) { //游戏结束
		CCDirector::sharedDirector()->replaceScene(
				CCTransitionFadeDown::create(0.5f, FinishLayer::scene()));
	}

	PhySprite::update(delta);
	if (dying) {
		//stop();
	} else if (jumping) { //进入腾空跳跃姿态

	} else { //走路姿态
		float x = this->getPositionX();
		float w = this->getContentSize().width / 2;
		switch (this->vec) {
		case vec_forward:
			if (x + w >= LOCAL_RESOLUTION.width / 2 - 1) {
				stop();
			} else {
				forward();
			}
			break;
		case vec_back:
			if (x - w <= -LOCAL_RESOLUTION.width / 2 + 1) {
				stop();
			} else {
				back();
			}
			break;
		case vec_stop:
			stop();
			break;
		}
	}
}

void Mushroom::forward() {
	b2Vec2 v = this->b2body->GetLinearVelocity();
	v.x = 5.0f;
	this->b2body->SetLinearVelocity(v);
}

void Mushroom::back() {
	b2Vec2 v = this->b2body->GetLinearVelocity();
	v.x = -5.0f;
	this->b2body->SetLinearVelocity(v);
}

void Mushroom::stop() {
	b2Vec2 v = this->b2body->GetLinearVelocity();
	v.x = 0.0f;
	this->b2body->SetLinearVelocity(v);
}

void Mushroom::jump() {
	if (!jumping) {
		this->jumping = true;
		b2Vec2 v = this->b2body->GetLinearVelocity();
		v.y = 5.0f;
		this->b2body->SetLinearVelocity(v);
	}
}

void Mushroom::beginContact(PhySprite *other, b2Contact* contact) {
	if (other->getPhyType() == BRICK) {
		b2Manifold *manifold = contact->GetManifold();
		b2ManifoldPoint mp1 = manifold->points[0];
		b2ManifoldPoint mp2 = manifold->points[1];
		if (mp1.localPoint.x == mp2.localPoint.x) { //侧边碰撞
			this->dying = true;
			b2Vec2 v = this->b2body->GetLinearVelocity();
			v.x = -v.x;
			this->b2body->SetLinearVelocity(v);
		} else if (mp1.localPoint.y == mp2.localPoint.y) { //底边碰撞
			if (this->getPositionY() > other->getPositionY()) { //蘑菇在上边
				this->dying = false;
				this->jumping = false;
			} else { //蘑菇在下边
					 //nothing to do
			}
		}
	}
}

void Mushroom::endContact(PhySprite *other, b2Contact* contact) {

}

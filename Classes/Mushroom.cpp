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
		vec(vec_nature), touchEdge(0), jumping(false), forwardSpeed(
				MUSHROOM_FORWARD_SPEED), backSpeed(MUSHROOM_BACK_SPEED), accPerSec(
				ACC_PER_SEC), contactFriction(0) {
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
	//b2BodyDef.bullet = true;
	b2BodyDef.fixedRotation = true;
	b2BodyDef.position = b2Vec2(c2b(this->getPosition().x),
	c2b(this->getPosition().y));
	b2PhyBody = PhyWorld::shareWorld()->CreateBody(&b2BodyDef);

	b2PolygonShape b2Shape;
	b2Shape.SetAsBox(c2b(this->getContentSize().width / 2),
	c2b(this->getContentSize().height / 2));

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &b2Shape;
	fixtureDef.density = 5.0f;
	fixtureDef.friction = 0.8f;
	// Add the shape to the body.
	b2PhyBody->CreateFixture(&fixtureDef);
	//b2PhyBody->SetLinearDamping(0.0f);
	b2PhyBody->SetLinearVelocity(b2Vec2_zero);
	b2PhyBody->SetUserData(this);
}

bool Mushroom::isVec(MushroomVec vec) {
	return this->vec == vec;
}

void Mushroom::gameover() {
	this->getParent()->removeAllChildren();
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionFadeDown::create(0.5f, FinishLayer::scene()));
}

void Mushroom::setSpeed(b2Vec2 vec) {
	float speed = vec.x;
	if ((speed < 0 && this->touchEdge < 0)
			|| (speed > 0 && this->touchEdge > 0)) { //碰到边界后x轴速度设置为0
		vec.x = 0;
	}
	this->b2PhyBody->SetLinearVelocity(vec);
}

void Mushroom::setSpeedX(float speed) {
	b2Vec2 v = this->b2PhyBody->GetLinearVelocity();
	v.x = speed;
	setSpeed(v);
}

void Mushroom::update(float delta) {
	float y = this->getPositionY();
	if (y < -LOCAL_RESOLUTION.height / 2) {
		gameover();
		return;
	}
	PhySprite::update(delta);
}

void Mushroom::setVec(MushroomVec vec) {
	if (!jumping) {
		if (this->vec != vec) {
			stop();
		}
//		switch (vec) {
//		case vec_forward:
//			this->b2PhyBody->ApplyLinearImpulse(
//					b2Vec2(this->b2PhyBody->GetMass() * 5, 0),
//					this->b2PhyBody->GetLocalCenter());
//			setSpeedX(MUSHROOM_FORWARD_SPEED);
//			break;
//		case vec_back:
//			this->b2PhyBody->ApplyLinearImpulse(
//					b2Vec2(this->b2PhyBody->GetMass() * -5, 0),
//					this->b2PhyBody->GetLocalCenter());
//			setSpeedX(MUSHROOM_BACK_SPEED);
//			break;
//		case vec_nature:
//			stop();
//			break;
//		}
	}
	this->vec = vec;
}

void Mushroom::keepForward(float friction) {
//	float x = this->b2PhyBody->GetMass() * WORLD_GRAVITY * friction;
//	this->b2PhyBody->ApplyForceToCenter(b2Vec2(x, 0));
	this->setSpeedX(MUSHROOM_FORWARD_SPEED);
}

void Mushroom::keepBack(float friction) {
//	float x = this->b2PhyBody->GetMass() * WORLD_GRAVITY * friction;
//	this->b2PhyBody->ApplyForceToCenter(b2Vec2(-x, 0));
	this->setSpeedX(MUSHROOM_BACK_SPEED);
}

void Mushroom::stop() {
	if (!jumping) {
		this->setSpeedX(0);
	}
}

void Mushroom::jump() {
	if (!jumping) {
		jumping = true;
		float mass = this->b2PhyBody->GetMass();
		b2Vec2 curVec = this->b2PhyBody->GetLinearVelocity();
		curVec.y = 0;
		//this->b2PhyBody->SetLinearVelocity(curVec);
		this->b2PhyBody->ApplyLinearImpulse(
				b2Vec2(0, mass * MUSHROOM_JUMP_VELOCITY),
				this->b2PhyBody->GetLocalCenter());
	}
}

void Mushroom::beginContact(PhySprite *other, b2Contact* contact) {
	if (other->getPhyType() == BRICK) {
		this->contactFriction = contact->GetFriction();
		b2Manifold *manifold = contact->GetManifold();
		b2ManifoldPoint mp1 = manifold->points[0];
		b2ManifoldPoint mp2 = manifold->points[1];
		if (mp1.localPoint.x == mp2.localPoint.x) { //侧边碰撞
			b2Vec2 v = this->b2PhyBody->GetLinearVelocity();
			v.x = -v.x;
			this->b2PhyBody->SetLinearVelocity(v);
		} else if (mp1.localPoint.y == mp2.localPoint.y) { //底边碰撞
			if (this->getPositionY() > other->getPositionY()) { //蘑菇在上边
				this->jumping = false;
			} else { //蘑菇在下边
				//nothing to do
			}
		}
	} else if (other->getPhyType() == WATER_EDGE) {
	} else if (other->getPhyType() == BROADSIDE) {
		touchEdge = this->getPositionX();
	}
}

void Mushroom::PreSolve(PhySprite *other, b2Contact* contact,
		const b2Manifold* oldManifold) {
	if (!jumping) { //脚底下有板子
		float friction = contact->GetFriction();
		switch (this->vec) {
		case vec_forward:
			keepForward(friction);
			break;
		case vec_back:
			keepBack(friction);
			break;
		case vec_nature:
			break;
		}
	}
}

void Mushroom::endContact(PhySprite *other, b2Contact* contact) {
	if (other->getPhyType() == BROADSIDE) {
		touchEdge = 0;
	}
}

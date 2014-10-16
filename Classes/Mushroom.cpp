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
		vec(vec_nature), touchEdge(0), underBricks(0) {
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
	fixtureDef.density = 3.0f;
	// Add the shape to the body.
	b2PhyBody->CreateFixture(&fixtureDef);
	//b2PhyBody->SetLinearDamping(0.0f);
	b2PhyBody->SetLinearVelocity(b2Vec2_zero);
	b2PhyBody->SetUserData(this);
}

void Mushroom::setVec(MushroomVec vec) {
	this->vec = vec;
}

bool Mushroom::isVec(MushroomVec vec) {
	return this->vec == vec;
}

void Mushroom::gameover() {
	this->getParent()->removeAllChildren();
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionFadeDown::create(0.5f, FinishLayer::scene()));
}

void Mushroom::setSpeedX(float speed) {
	b2Vec2 v = this->b2PhyBody->GetLinearVelocity();
	if ((speed < 0 && this->touchEdge < 0)
			|| (speed > 0 && this->touchEdge > 0)) {
		v.x = 0;
	} else {
		v.x = speed;
	}
	this->b2PhyBody->SetLinearVelocity(v);
}

void Mushroom::update(float delta) {
	float y = this->getPositionY();
	if (y < -LOCAL_RESOLUTION.height / 2) {
		gameover();
		return;
	}
	if (underBricks) { //脚底下有板子
		switch (this->vec) {
		case vec_forward:
			forward();
			break;
		case vec_back:
			back();
			break;
		case vec_nature:
			naturalSpeed();
			break;
		}
	}
	PhySprite::update(delta);
}

void Mushroom::forward() {
	float x = 6.0f;
	if (underBricks) {
		x += underBricks->getB2Body()->GetLinearVelocity().x;
	}
	this->setSpeedX(x);
}

void Mushroom::back() {
	float x = -5.0f;
	if (underBricks) {
		x += underBricks->getB2Body()->GetLinearVelocity().x;
	}
	this->setSpeedX(x);
}

void Mushroom::naturalSpeed() {
	if (underBricks) {
		this->setSpeedX(underBricks->getB2Body()->GetLinearVelocity().x);
	}
}

void Mushroom::stop() {
	this->setSpeedX(0);
}

void Mushroom::jump() {
	if (underBricks) {
		underBricks = 0;
		//this->jumping = true;
		float mass = this->b2PhyBody->GetMass();
		this->b2PhyBody->ApplyLinearImpulse(b2Vec2(0, mass * 10.0f),
				this->b2PhyBody->GetLocalCenter());
	}
}

void Mushroom::beginContact(PhySprite *other, b2Contact* contact) {

	if (other->getPhyType() == BRICK) {
		b2Manifold *manifold = contact->GetManifold();
		b2ManifoldPoint mp1 = manifold->points[0];
		b2ManifoldPoint mp2 = manifold->points[1];
		if (mp1.localPoint.x == mp2.localPoint.x) { //侧边碰撞
			//this->dying = true;
			b2Vec2 v = this->b2PhyBody->GetLinearVelocity();
			v.x = -v.x;
			this->b2PhyBody->SetLinearVelocity(v);
		} else if (mp1.localPoint.y == mp2.localPoint.y) { //底边碰撞
			if (this->getPositionY() > other->getPositionY()) { //蘑菇在上边
					//this->dying = false;
					//this->jumping = false;
				underBricks = (Bricks*) other;
			} else { //蘑菇在下边
				//nothing to do
			}
		}
	} else if (other->getPhyType() == WATER_EDGE) {
	} else if (other->getPhyType() == BROADSIDE) {
		touchEdge = this->getPositionX();
	}
}

void Mushroom::endContact(PhySprite *other, b2Contact* contact) {
	if (other->getPhyType() == BRICK) {
		//underBricks = 0;
	} else if (other->getPhyType() == BROADSIDE) {
		touchEdge = 0;
	}

}

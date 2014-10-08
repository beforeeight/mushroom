/*
 * Mushroom.cpp
 *
 *  Created on: Sep 28, 2014
 *      Author: zhangchen
 */

#include "Mushroom.h"

USING_NS_CC;

Mushroom::Mushroom() :
		vec(vec) {
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

void Mushroom::createPhyBody() {
	b2BodyDef b2BodyDef;
	b2BodyDef.type = b2_dynamicBody;
	b2BodyDef.position = b2Vec2(c2p(this->getPosition().x),
			c2p(this->getPosition().y));
	b2body = PhyWorld::shareWorld()->CreateBody(&b2BodyDef);

	b2PolygonShape b2Shape;
	b2Shape.SetAsBox(c2p(this->getContentSize().width / 2),
			c2p(this->getContentSize().height / 2));

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &b2Shape;
	// Add the shape to the body.
	b2body->CreateFixture(&fixtureDef);
//	b2MassData mass;
//	mass.mass = 1.0f;
//	mass.center = b2Vec2_zero;
//	b2body->SetMassData(&mass);
	b2body->SetLinearDamping(0.0f);
	b2body->SetLinearVelocity(b2Vec2_zero);
	b2body->SetUserData(this);
}

void Mushroom::setVec(MushroomVec vec) {
	this->vec = vec;
}

void Mushroom::update(float delta) {
	PhySprite::update(delta);
	switch (this->vec) {
	case vec_forward:
		forward();
		break;
	case vec_back:
		back();
		break;
	case vec_stop:
		stop();
		break;
	}
}

void Mushroom::forward() {
	b2Vec2 v = this->b2body->GetLinearVelocity();
	v.x = 2.0f;
	this->b2body->SetLinearVelocity(this->b2body->GetLinearVelocity());
}

void Mushroom::back() {
	b2Vec2 v = this->b2body->GetLinearVelocity();
	v.x = -2.0f;
	this->b2body->SetLinearVelocity(v);
}

void Mushroom::stop() {
	b2Vec2 v = this->b2body->GetLinearVelocity();
	v.x = 0.0f;
	this->b2body->SetLinearVelocity(v);
}

void Mushroom::jump() {
	b2Vec2 v = this->b2body->GetLinearVelocity();
	v.y = 10.0f;
	this->b2body->SetLinearVelocity(v);
}

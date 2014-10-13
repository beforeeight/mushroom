/*
 * PhySprite.cpp
 *
 *  Created on: Sep 29, 2014
 *      Author: zhangchen
 */

#include "PhySprite.h"

PhySprite::PhySprite() :
		b2body(NULL) {
}

PhySprite::~PhySprite() {
	PhyWorld::shareWorld()->DestroyBody(this->b2body);
}

void PhySprite::initPhySprite(PhySprite &sprite) {
	sprite.createPhyBody();
	sprite.scheduleUpdate();
	sprite.initPhyBody();
}

b2Body* PhySprite::getB2Body() {
	return this->b2body;
}

void PhySprite::update(float delta) {
	if (b2body) {
		this->setPosition(ccp(p2c(b2body->GetPosition().x),
				p2c(b2body->GetPosition().y)));
		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(b2body->GetAngle()));
	}
}

void PhySprite::initPhyBody() {
	//Nothing to do, implement in subclass
}

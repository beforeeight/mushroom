/*
 * PhySprite.cpp
 *
 *  Created on: Sep 29, 2014
 *      Author: zhangchen
 */

#include "PhySprite.h"

PhySprite::PhySprite() :
		b2PhyBody(NULL) {
}

PhySprite::~PhySprite() {
	PhyWorld::shareWorld()->DestroyBody(this->b2PhyBody);
}

void PhySprite::initPhySprite(PhySprite &sprite, bool update) {
	sprite.createPhyBody();
	if (update) {
		sprite.scheduleUpdate();
	}
	sprite.initPhyBody();
}

void PhySprite::initPhySprite(PhySprite &sprite) {
	initPhySprite(sprite, true);
}

b2Body* PhySprite::getB2Body() {
	return this->b2PhyBody;
}

void PhySprite::update(float delta) {
	if (b2PhyBody) {
		this->setPosition(ccp(p2c(b2PhyBody->GetPosition().x),
				p2c(b2PhyBody->GetPosition().y)));
		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(b2PhyBody->GetAngle()));
	}
}

void PhySprite::initPhyBody() {
	//Nothing to do, implement in subclass
}

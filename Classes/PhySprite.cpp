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
	if (b2PhyBody) {
		CCLog("release delete body %d", this->getPhyType());
		PhyWorld::shareWorld()->DestroyBody(this->b2PhyBody);
	}
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

bool PhySprite::initWithBody(b2Body *body) {
	if (CCSprite::init()) {
		b2PhyBody = body;
		b2PhyBody->SetUserData(this);
		this->setAnchorPoint(ccp(0.5, 0.5));
		this->setPosition(ccp(b2c(b2PhyBody->GetPosition().x),
		b2c(b2PhyBody->GetPosition().y)));
		return true;
	} else {
		return false;
	}
}

PhySprite* PhySprite::create(b2Body *body) {
	PhySprite *pRet = new PhySprite();
	if (pRet && pRet->initWithBody(body)) {
		pRet->autorelease();
		return pRet;
	} else {
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

b2Body* PhySprite::getB2Body() {
	return this->b2PhyBody;
}

void PhySprite::update(float delta) {
	if (b2PhyBody) {
		this->setPosition(ccp(b2c(b2PhyBody->GetPosition().x),
		b2c(b2PhyBody->GetPosition().y)));
		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(b2PhyBody->GetAngle()));
	}
}

void PhySprite::initPhyBody() {
	//Nothing to do, implement in subclass
}

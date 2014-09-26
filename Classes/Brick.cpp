/*
 * Brick.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: zhangchen
 */

#include "Brick.h"

USING_NS_CC;

Brick::Brick() :
		b2body(NULL) {
	// TODO Auto-generated constructor stub
}

Brick::~Brick() {
	PhyWorld::shareWorld()->DestroyBody(this->b2body);
}

bool Brick::init() {
	if (CCSprite::initWithTexture(
			CCTextureCache::sharedTextureCache()->textureForKey(
			TEXTURE_BRICK))) {
		this->setAnchorPoint(ccp(0.5, 0.5));
		this->setPosition(ccpp(0.5,0)-ccp(this->getContentSize().width/2,0));
		createPhyBody();
		return true;
	} else {
		return false;
	}
}

void Brick::createPhyBody() {
	b2BodyDef b2BodyDef;
	b2BodyDef.type = b2_kinematicBody;
	b2BodyDef.position = b2Vec2(c2p(this->getPosition().x),
			c2p(this->getPosition().y));
	b2body = PhyWorld::shareWorld()->CreateBody(&b2BodyDef);

	b2PolygonShape b2CircleShape;
	b2CircleShape.SetAsBox(c2p(this->getContentSize().width / 2),
			c2p(this->getContentSize().height / 2));

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &b2CircleShape;
	// Add the shape to the body.
	b2body->CreateFixture(&fixtureDef);
	b2body->SetUserData(this);
	CCLog("phy pos: %f:%f",b2body->GetPosition().x);
}

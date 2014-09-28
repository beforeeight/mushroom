/*
 * Brick.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: zhangchen
 */

#include "Brick.h"

USING_NS_CC;

BatchBrick::BatchBrick() {

}

BatchBrick::~BatchBrick() {

}

void BatchBrick::initBricks() {
	Brick* brick = Brick::create();
	brick->setPosition(ccpp(-0.5,0)+ccp(brick->getContentSize().width / 2, 0));
	this->addChild(brick);
	brick->createPhyBody();
	brick->scheduleUpdate();
	while (brick->getPositionX() < ccpp(0.5,0).x) {
		brick = createBrick(brick->getPosition()+ccp(brick->getContentSize().width,0));
	}
	brick->callbackEmitter = true;
}

bool BatchBrick::init() {
	if (CCSpriteBatchNode::initWithTexture(
			CCTextureCache::sharedTextureCache()->textureForKey(
			TEXTURE_BRICK), 1000)) {
		return true;
	} else {
		return false;
	}
}

Brick * BatchBrick::emitBrick(Brick *lastBrick) {
	CCPoint pos =
			lastBrick->getPosition() + ccp(lastBrick->getContentSize().width, 0);
	Brick* newBrick = createBrick(pos);
	newBrick->callbackEmitter = true;
	return newBrick;
}

Brick* BatchBrick::createBrick(const CCPoint& pos) {
	Brick* newBrick = Brick::create();
	newBrick->setPosition(pos);
	this->addChild(newBrick);
	newBrick->createPhyBody();
	newBrick->scheduleUpdate();
	return newBrick;
}

Brick::Brick() :
		b2body(NULL), status(brick_ready), callbackEmitter(false) {
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
	b2body->SetLinearVelocity(b2Vec2(-0.5f, 0));
}

void Brick::update(float delta) {
	//检测是否出边界
	updateStatus();
	if (this->status == brick_over) {
		this->removeFromParent();
		return;
	}
	if (b2body) {
		this->setPosition(
				ccp(p2c(b2body->GetPosition().x),
						p2c(b2body->GetPosition().y )));
		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(b2body->GetAngle()));
	}
}

void Brick::updateStatus() {
	CCPoint p = this->getPosition() + ccp(this->getContentSize().width / 2, 0);
	if ((this->status == brick_running || this->status == brick_ready)
			&& p.x < ccpp(-0.5, -1).x) {
		this->status = brick_over;
		return;
	}
	if (this->status == brick_ready && p.x < ccpp(0.5, 0).x) {
		if(callbackEmitter) {
			BatchBrick *batch = (BatchBrick *) this->getParent();
			batch->emitBrick(this);
		}
		this->status = brick_running;
		return;
	}
}

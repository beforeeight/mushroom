/*
 * Brick.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: zhangchen
 */

#include "Brick.h"

USING_NS_CC;

#define BRICK_WIDTH CCTextureCache::sharedTextureCache()->textureForKey("item_cubic.png")->getContentSize().width
#define BRICK_HEIGHT CCTextureCache::sharedTextureCache()->textureForKey("item_cubic.png")->getContentSize().height

BrickEmitter::BrickEmitter(CCLayer &layer) :
		targetLayer(layer) {
	//this->targetLayer = layer;
}

BrickEmitter::~BrickEmitter() {

}

void BrickEmitter::initBricks() {
	int num = 30;
	Bricks* newBricks = Bricks::create(num);
	lastBricks = newBricks;
	newBricks->emitter = this;
	newBricks->setPosition(ccpp(-0.5, 0) + ccp(0,POS_JUNCTION-BRICK_HEIGHT/2));
	PhySprite::initPhySprite(*newBricks);
	this->targetLayer.addChild(newBricks);
}

Bricks* BrickEmitter::emitBrick(int num, Bricks* lastBrick) {
	Bricks* newBricks = Bricks::create(num);
	lastBricks = newBricks;
	newBricks->emitter = this;
	if (lastBrick) {
		newBricks->previous = lastBrick;
		lastBrick->next = newBricks;
	}
	CCPoint pos = computeSpan(lastBrick, newBricks);
	newBricks->setPosition(pos);
	PhySprite::initPhySprite(*newBricks);
	this->targetLayer.addChild(newBricks);
	return newBricks;
}

CCPoint BrickEmitter::computeSpan(Bricks* lastBrick, Bricks* newBricks) {
	int num = rand() % 3 + 2;
	CCPoint pos =
			lastBrick->getPosition() + ccp(lastBrick->getContentSize().width/2, 0) + ccp(newBricks->getContentSize().width/2, 0)+ccp(BRICK_WIDTH*num,0);
	return pos;
}

Bricks * BrickEmitter::emitBrick(Bricks *lastBrick) {
	int num = rand() % 5 + 5;
	Bricks* newBricks = emitBrick(num, lastBrick);
	return newBricks;
}

void BrickEmitter::pause() {
	if (lastBricks) {
		lastBricks->pause();
	}
}

void BrickEmitter::resume() {
	if (lastBricks) {
		lastBricks->resume();
	}
}
/*------------ Single Brick ----------*/

Brick::Brick() {
}

Brick::~Brick() {

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

/*------------ Group Brick with b2Body ----------*/
Bricks::Bricks() :
		previous(0), next(0), emitter(0), status(brick_ready) {
}

Bricks::~Bricks() {
	if (previous) {
		previous->next = 0;
	}
	if (next) {
		next->previous = 0;
	}
}

bool Bricks::init(int num) {
	if (PhySprite::init()) {
		for (int i = 0; i < num; i++) {
			Brick *b = Brick::create();
			b->setAnchorPoint(ccp(0, 0));
			b->setPosition(ccp((i + 1 / 2) * b->getContentSize().width, 0));
			this->addChild(b);
		}
		this->setAnchorPoint(ccp(0.5, 0.5));
		this->setContentSize(CCSizeMake(BRICK_WIDTH*num, BRICK_HEIGHT));
		return true;
	} else {
		return false;
	}
}

PHY_TYPE Bricks::getPhyType() {
	return BRICK;
}

Bricks* Bricks::create(int num) {
	Bricks *pRet = new Bricks();
	if (pRet && pRet->init(num)) {
		pRet->autorelease();
		return pRet;
	} else {
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

void Bricks::createPhyBody() {
	b2BodyDef b2BodyDef;
	b2BodyDef.type = b2_kinematicBody;
	b2BodyDef.bullet = true;
	b2BodyDef.position = b2Vec2(c2p(this->getPosition().x),
	c2p(this->getPosition().y));
	b2body = PhyWorld::shareWorld()->CreateBody(&b2BodyDef);

	b2PolygonShape b2box;
	b2box.SetAsBox(c2p(this->getContentSize().width / 2),
	c2p(this->getContentSize().height / 2));

// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &b2box;
// Add the shape to the body.
	b2body->CreateFixture(&fixtureDef);
	//b2body->SetLinearDamping(0.0f);
	b2body->SetUserData(this);
}

void Bricks::initPhyBody() {
	if (b2body) {
		b2body->SetLinearVelocity(b2Vec2(-2, 0));
	}
}
void Bricks::update(float delta) {
//检测是否出边界
	updateStatus();
	if (this->status == brick_over) {
		this->removeFromParent();
		return;
	}
	PhySprite::update(delta);
}

void Bricks::updateStatus() {
	CCPoint p = this->getPosition() + ccp(this->getContentSize().width / 2, 0);

	if (this->status == brick_ready && p.x < ccpp(0.5, 0).x) {
		this->emitter->emitBrick(this);
		this->status = brick_running;
		return;
	}

	if ((this->status == brick_running || this->status == brick_ready)
			&& p.x < ccpp(-0.5, -1).x) {
		this->status = brick_over;
		return;
	}
}

void Bricks::pause() {
	linearVelocity = this->b2body->GetLinearVelocity();
	this->b2body->SetLinearVelocity(b2Vec2_zero);
	if (previous) {
		previous->pause();
	}
}

void Bricks::resume() {
	this->b2body->SetLinearVelocity(linearVelocity);
	if (previous) {
		previous->resume();
	}
}

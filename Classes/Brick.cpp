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
	int num = 10;
	Bricks* newBricks = Bricks::create(num);
	newBricks->score = 0;
	lastBricks = newBricks;
	newBricks->emitter = this;
	newBricks->setPosition(
			ccpp(0, 0) + ccp(0, POS_JUNCTION - BRICK_HEIGHT / 2));
	PhySprite::initPhySprite(*newBricks);
	this->targetLayer.addChild(newBricks);
}

Bricks* BrickEmitter::emitBrick(int num, Bricks* lastBrick) {
	Bricks* newBricks;

	if (rand() % 2 == 0 && lastBrick && lastBrick->isNormal()) {
		newBricks = HorizontalBricks::create(num);
	} else {
		newBricks = Bricks::create(num);
	}

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
	int num = rand() % 2 + 4;
	CCPoint pos = lastBrick->getPosition()
			+ ccp(lastBrick->getContentSize().width / 2, 0)
			+ ccp(newBricks->getContentSize().width / 2, 0)
			+ ccp(BRICK_WIDTH * num, 0);
	return pos;
}

Bricks * BrickEmitter::emitBrick(Bricks *lastBrick) {
	int num = rand() % 3 + 5;
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
		score(1), previous(0), next(0), emitter(0), status(brick_ready) {
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
		this->setContentSize(CCSizeMake(BRICK_WIDTH * num, BRICK_HEIGHT));
		return true;
	} else {
		return false;
	}
}

PHY_TYPE Bricks::getPhyType() {
	return BRICK;
}

void Bricks::beginContact(PhySprite *other, b2Contact* contact) {
	if (other->getPhyType() == MUSHROOM && this->score > 0) {
		b2Manifold *manifold = contact->GetManifold();
		b2ManifoldPoint mp1 = manifold->points[0];
		b2ManifoldPoint mp2 = manifold->points[1];
		if (mp1.localPoint.y == mp2.localPoint.y) { //底边碰撞
			if (this->getPositionY() < other->getPositionY()) { //蘑菇在上边
				LOCAL_CONTEXT->increaseScore(score);
				score = 0;
			}
		}
	}
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
	//b2BodyDef.type = b2_dynamicBody;
	//b2BodyDef.gravityScale = 0.0f;
	b2BodyDef.fixedRotation = true;
	b2BodyDef.position = b2Vec2(c2b(this->getPosition().x),
	c2b(this->getPosition().y));
	b2PhyBody = PhyWorld::shareWorld()->CreateBody(&b2BodyDef);
	b2PolygonShape b2box;
	b2box.SetAsBox(c2b(this->getContentSize().width / 2),
	c2b(this->getContentSize().height / 2));
// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &b2box;
	//fixtureDef.friction = 1.0f;
// Add the shape to the body.
	b2PhyBody->CreateFixture(&fixtureDef);
	b2PhyBody->SetUserData(this);
}

void Bricks::initPhyBody() {
	if (b2PhyBody) {
		b2PhyBody->SetLinearVelocity(b2Vec2(-2, 0));
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
	CCPoint head = this->getPosition()
			- ccp(this->getContentSize().width / 2, 0);
	CCPoint tail = this->getPosition()
			+ ccp(this->getContentSize().width / 2, 0);
	if (this->status == brick_ready && head.x < ccpp(0.5, 0).x) {
		this->emitter->emitBrick(this);
		this->status = brick_running;
		onRunning();
		return;
	}

	if ((this->status == brick_running || this->status == brick_ready)
			&& tail.x < ccpp(-0.5, -1).x) {
		this->status = brick_over;
		onOverStatus();
		return;
	}
}

void Bricks::pause() {
	linearVelocity = this->b2PhyBody->GetLinearVelocity();
	this->b2PhyBody->SetLinearVelocity(b2Vec2_zero);
	if (previous) {
		previous->pause();
	}
}

void Bricks::resume() {
	this->b2PhyBody->SetLinearVelocity(linearVelocity);
	if (previous) {
		previous->resume();
	}
}

HorizontalBricks::HorizontalBricks() :
		speedScale(2.0f) {
	score = 2;
}

HorizontalBricks::~HorizontalBricks() {
}

HorizontalBricks* HorizontalBricks::create(int num) {
	HorizontalBricks *pRet = new HorizontalBricks();
	if (pRet && pRet->init(num)) {
		pRet->autorelease();
		return pRet;
	} else {
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

void HorizontalBricks::update(float delta) {
	Bricks::update(delta);
	if (this->previous) {
		float x1 = previous->getPositionX()
				+ previous->getContentSize().width / 2;
		float x2 = this->getPositionX() - this->getContentSize().width / 2;
		if (x1 >= x2) {
			b2Vec2 speed = b2PhyBody->GetLinearVelocity();
			speed.x = abs(speed.x);
			b2PhyBody->SetLinearVelocity(speed);
		}
	}
	if (this->next) {
		float x1 = this->getPositionX() + this->getContentSize().width / 2;
		float x2 = next->getPositionX() - next->getContentSize().width / 2;
		if (x1 >= x2) {
			b2Vec2 speed = b2PhyBody->GetLinearVelocity();
			speed.x = -abs(speed.x);
			b2PhyBody->SetLinearVelocity(speed);
		}
	}

}

void HorizontalBricks::onRunning() {
	if (b2PhyBody) {
		b2Vec2 v = b2PhyBody->GetLinearVelocity();
		v.x = v.x * speedScale;
		b2PhyBody->SetLinearVelocity(v);
	}
}

/*
 * Brick.h
 *
 *  Created on: Sep 25, 2014
 *      Author: zhangchen
 */

#ifndef BRICK_H_
#define BRICK_H_

#include "cocos2d.h"
#include "PhyWorld.h"

#define TEXTURE_BRICK "item_cubic.png"

enum BrickStatus {
	brick_ready, brick_running, brick_over

// TODO_ERIN
//b2_bulletBody,
};

class Brick: public cocos2d::CCSprite {
public:
	Brick();

	virtual ~Brick();

	virtual bool init();

	CREATE_FUNC(Brick)

	virtual void update(float delta);

	friend class BatchBrick;

private:
	BrickStatus status;

	b2Body *b2body;

	bool callbackEmitter;

	void createPhyBody();

	void updateStatus();
};

class BatchBrick: public cocos2d::CCSpriteBatchNode {

public:
	BatchBrick();

	virtual ~BatchBrick();

	virtual bool init();

	CREATE_FUNC(BatchBrick)

	void initBricks();

	Brick * emitBrick(Brick *lastBrick);

private:
	Brick* createBrick(const CCPoint& pos);

};

#endif /* BRICK_H_ */

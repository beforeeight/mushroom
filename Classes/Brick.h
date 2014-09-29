/*
 * Brick.h
 *
 *  Created on: Sep 25, 2014
 *      Author: zhangchen
 */

#ifndef BRICK_H_
#define BRICK_H_

#include "PhySprite.h"
#include "PhyWorld.h"

#define TEXTURE_BRICK "item_cubic.png"

enum BrickStatus {
	brick_ready, brick_running, brick_over

// TODO_ERIN
//b2_bulletBody,
};

class Brick: public CCSprite {
public:
	Brick();

	virtual ~Brick();

	virtual bool init();

	CREATE_FUNC(Brick)

};

class BrickEmitter;

/*--------- Class Bricks --------*/
class Bricks: public PhySprite {

public:
	Bricks();

	virtual ~Bricks();

	virtual bool init(int num);

	static Bricks* create(int num);

	virtual void update(float delta);

	friend class BrickEmitter;

protected:
	virtual void createPhyBody();

	virtual void initPhyBody();

private:
	Bricks *previous, *next;

	BrickEmitter *emitter;

	BrickStatus status;

	void updateStatus();
};

/*--------- Class BatchBrick --------*/
class BrickEmitter {

public:
	BrickEmitter(CCLayer &layer);

	virtual ~BrickEmitter();

	void initBricks();

	Bricks * emitBrick(Bricks *lastBrick);

private:
	CCLayer &targetLayer;

	Bricks* emitBrick(int num, Bricks* lastBrick);

	CCPoint computeSpan(Bricks* lastBrick, Bricks* newBricks);
};

#endif /* BRICK_H_ */

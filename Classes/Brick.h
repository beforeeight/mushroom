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

#define BRICKS_SPEED 2
#define HOR_BRICKS_SPEED_SCALE 1.8f
#define VER_BRICKS_SPEED 5.0f
#define VER_ACCELERATED 5.0f

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

	CREATE_FUNC (Brick)

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

	virtual PHY_TYPE getPhyType();

	virtual void beginContact(PhySprite *other, b2Contact* contact);

	virtual bool isNormal() {
		return true;
	}

protected:
	Bricks *previous, *next;

	unsigned int score;

	float movingSpeed;

	virtual void createPhyBody();

	virtual void initPhyBody();

	virtual void onRunning() {
	}

	virtual void onOverStatus() {
	}

private:

	BrickEmitter *emitter;

	BrickStatus status;

	b2Vec2 linearVelocity;

	void updateStatus();

	void pause();

	void resume();
};

class HorizontalBricks: public Bricks {
public:
	HorizontalBricks();

	virtual ~HorizontalBricks();

	static HorizontalBricks* create(int num);

	virtual void update(float delta);

	virtual bool isNormal() {
		return false;
	}
protected:

	virtual void onRunning();

private:
	float speedScale;
};

class VerticalBricks: public Bricks {
public:
	VerticalBricks();

	virtual ~VerticalBricks();

	static VerticalBricks* create(int num);

	virtual void update(float delta);

	virtual bool isNormal() {
		return false;
	}
protected:

	virtual void onRunning();

private:
	float v0, accelerated, originalY, deltaUpdate;
};
/*--------- Class BatchBrick --------*/
class BrickEmitter {

public:
	BrickEmitter(CCLayer &layer);

	virtual ~BrickEmitter();

	void initBricks();

	Bricks * emitBrick(Bricks *lastBrick);

	void pause();

	void resume();

private:
	Bricks *tailBricks;

	CCLayer &targetLayer;

	Bricks* createBricks(Bricks*& lastBrick, int num);
	Bricks* emitBrick(int num, Bricks* lastBrick);

	CCPoint computeSpan(Bricks* lastBrick, Bricks* newBricks);
};

#endif /* BRICK_H_ */

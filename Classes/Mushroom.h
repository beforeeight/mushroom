/*
 * Mushroom.h
 *
 *  Created on: Sep 28, 2014
 *      Author: zhangchen
 */

#ifndef MUSHROOM_H_
#define MUSHROOM_H_

#define MUSHROOM_FORWARD_SPEED 7
#define MUSHROOM_BACK_SPEED -5
#define MUSHROOM_JUMP_VELOCITY 10.0f
#define ACC_PER_SEC 0.1f

#include "PhySprite.h"
#include "Brick.h"

enum MushroomVec {
	vec_forward, vec_back, vec_nature
};
class Mushroom: public PhySprite {

	MushroomVec vec;

public:

	Mushroom();

	virtual ~Mushroom();

	virtual bool init();

	CREATE_FUNC (Mushroom)

	virtual void update(float delta);

	void setVec(MushroomVec vec);

	bool isVec(MushroomVec vec);

	void jump();

	virtual PHY_TYPE getPhyType();

	virtual void beginContact(PhySprite *other, b2Contact* contact);

	virtual void PreSolve(PhySprite *other, b2Contact* contact,
			const b2Manifold* oldManifold);

	virtual void endContact(PhySprite *other, b2Contact* contact);

protected:

	int touchEdge;

	void forward();

	void back();

	void naturalSpeed();

	void stop();

	virtual void createPhyBody();

private:
	float forwardSpeed, backSpeed, accPerSec;

	float vecLastTime;

	Bricks *underBricks;

	void gameover();

	void setSpeed(b2Vec2 vec);

	void setSpeedX(float speed);
};

#endif /* MUSHROOM_H_ */

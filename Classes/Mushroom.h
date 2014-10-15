/*
 * Mushroom.h
 *
 *  Created on: Sep 28, 2014
 *      Author: zhangchen
 */

#ifndef MUSHROOM_H_
#define MUSHROOM_H_

#include "PhySprite.h"

enum MushroomVec {
	vec_forward, vec_back, vec_stop
};
class Mushroom: public PhySprite {

	MushroomVec vec;
	void gameover();

public:

	Mushroom();

	virtual ~Mushroom();

	virtual bool init();

	CREATE_FUNC(Mushroom)

	virtual void update(float delta);

	void setVec(MushroomVec vec);

	bool isVec(MushroomVec vec);

	void jump();

	virtual PHY_TYPE getPhyType();

	virtual void beginContact(PhySprite *other, b2Contact* contact);

	virtual void endContact(PhySprite *other, b2Contact* contact);

protected:
	bool jumping, dying, inwater;

	void forward();

	void back();

	void stop();

	virtual void createPhyBody();

};

#endif /* MUSHROOM_H_ */

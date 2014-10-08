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
public:
	Mushroom();

	virtual ~Mushroom();

	virtual bool init();

	CREATE_FUNC(Mushroom)

	virtual void update(float delta);

	void setVec(MushroomVec vec);

	void jump();

protected:
	MushroomVec vec;

	void forward();

	void back();

	void stop();

	virtual void createPhyBody();
};

#endif /* MUSHROOM_H_ */

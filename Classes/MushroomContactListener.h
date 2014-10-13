/*
 * MushroomContactListener.h
 *
 *  Created on: Oct 11, 2014
 *      Author: zhangchen
 */

#ifndef MUSHROOMCONTACTLISTENER_H_
#define MUSHROOMCONTACTLISTENER_H_

#include <Box2D/Box2D.h>
#include "Mushroom.h"
#include "Brick.h"

class MushroomContactListener: public b2ContactListener {
public:
	MushroomContactListener();
	virtual ~MushroomContactListener();

	virtual void BeginContact(b2Contact* contact);

	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

	virtual void EndContact(b2Contact* contact);

private:

	Mushroom *findMushroom(b2Contact*& contact);

	Bricks* findBricks(b2Contact*& contact);
};

#endif /* MUSHROOMCONTACTLISTENER_H_ */

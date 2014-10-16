/*
 * WaterEdge.h
 *
 *  Created on: Oct 14, 2014
 *      Author: zhangchen
 */

#ifndef WATEREDGE_H_
#define WATEREDGE_H_

#include "PhySprite.h"

class WaterEdge: public PhySprite {


public:
	WaterEdge();

	virtual ~WaterEdge();

	virtual bool init();

	CREATE_FUNC(WaterEdge)

	virtual PHY_TYPE getPhyType();

	virtual void beginContact(PhySprite *other, b2Contact* contact);

protected:

	virtual void createPhyBody();
};

#endif /* WATEREDGE_H_ */

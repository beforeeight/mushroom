/*
 * GameScene.h
 *
 *  Created on: Sep 16, 2014
 *      Author: zhangchen
 */

#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "Resources.h"
#include "Brick.h"

class GameLayer: public CCLayer {
public:
	GameLayer();

	virtual ~GameLayer();

	virtual bool init();

	CREATE_FUNC (GameLayer)

	static CCScene * scene();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

private:
	bool running;

	BrickEmitter *brickEmitter;

	b2Body *leftBody, *rightBody;

	void onPauseItem(CCObject *object);

	void onResumeItem(CCObject *object);

	void onBackItem(CCObject *object);

	CCLayer * createPauseButtonLayer();

	void createPauseLayer();

	void disable();

	void enable();

	void gameover();

	void resume();

	void backMenu();

	virtual void update(float delta);
};

#endif /* GAMESCENE_H_ */

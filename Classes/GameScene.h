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

	virtual void draw(void);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

private:
	bool running;

	BrickEmitter *brickEmitter;

	b2Body *leftEdge, *rightEdge;

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

	void increateScore(CCObject *p_sender, unsigned int score);
};

#endif /* GAMESCENE_H_ */

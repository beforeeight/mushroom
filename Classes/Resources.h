/*
 * Resources.h
 *
 *  Created on: Aug 4, 2014
 *      Author: zhangchen
 */

#ifndef RESOURCES_H_
#define RESOURCES_H_

#include "cocos2d.h"

USING_NS_CC;

typedef struct tagResource {
	cocos2d::CCSize size;
	char directory[100];
} Resource;

extern const Resource iphone5Resource;
extern const Resource iphone4Resource;

class LocalResources {

public:
	LocalResources();

	~LocalResources();

	static LocalResources *sharedResources();

	CCSize designResolutionSize();

	const CCString* valueByKey(const char * key);

	void playBgMusic();

	void playEffect(const char * mp3);

private:
	Resource resource;

	CCDictionary *conf_lang;

	CCDictionary *conf_resolution;

	CCDictionary *conf;

	void prepareResPath();

	void loadConf();

	void loadResources();

	const CCString* langByKey(const char * key);

	const CCString* resoByKey(const char * key);

	const CCString* confByKey(const char * key);
};

typedef void (CCObject::*ScoreFunc)(CCObject*, unsigned int);

class Context {
public:
	Context();

	virtual ~Context();

	static Context *sharedContext();

	virtual void save();

	virtual void load();

	ccColor3B getFontColor() const;

	bool isSound() const;

	void onSound();

	void offSound();

	unsigned int increaseScore();

	unsigned int increaseScore(unsigned int delta);

	void clearScore();

	unsigned int getScore() const;

	unsigned int getHighScore() const;

	bool isNewRecord() const;

	void setScoreTarget(CCObject *target, ScoreFunc);
private:

	unsigned int score;

	bool newrecord;

	bool sound;

	unsigned int highScore;

	CCObject *scoreTarget;

	ScoreFunc scoreFunc;
};

#define LOCAL_RESOURCES LocalResources::sharedResources()

#define LOCAL_RESOLUTION LocalResources::sharedResources()->designResolutionSize()

#define LOCAL_RESOLUTION_WIDTH LocalResources::sharedResources()->designResolutionSize().width

#define LOCAL_CONTEXT Context::sharedContext()

#define ccpx(__X__) ((float)(__X__))*LOCAL_RESOLUTION.width
#define ccpy(__Y__) ((float)(__Y__))*LOCAL_RESOLUTION.height
#define ccpp(__X__,__Y__) cocos2d::CCPointMake(ccpx(__X__), ccpy(__Y__))

#endif

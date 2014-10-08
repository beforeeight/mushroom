#ifndef __CCSHAPECACHE_H__
#define __CCSHAPECACHE_H__
#include "cocos2d.h"
USING_NS_CC;
class BodyDef;
class b2Body;
namespace cocos2d {
    class GB2ShapeCache : public CCObject{
    public:
        static GB2ShapeCache* sharedGB2ShapeCache(void);
    public:
        bool init();
        void addShapesWithFile(const std::string &plist);
        void addFixturesToBody(b2Body *body, const std::string &shape);
        cocos2d::CCPoint anchorPointForShape(const std::string &shape);
        void reset();
        float getPtmRatio() { return ptmRatio; }
        ~GB2ShapeCache() {}
    private:
        std::map<std::string, BodyDef *> shapeObjects;
        GB2ShapeCache(void) {}
        float ptmRatio;
    };
}
#endif /* defined(__CCSHAPECACHE_H__) */
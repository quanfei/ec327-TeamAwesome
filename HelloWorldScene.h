#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "HudLayer.h"
#include "GameMenu.h"
#include "Enemys.h"
#include "GameOverScene.h"
#define TAG_LEFT 0
#define TAG_RIGHT 1
#define TAG_UP 2
#define TAG_DOWN 3
#define ATTACK 4
using namespace cocos2d;
class Enemys;

class HelloWorld : public cocos2d::Layer
{
private:
    // this creates a private instance variable to keep track of the tile map itself
    // and another instance variable to keep track of the background layer in the map.
    
    TMXTiledMap *_tileMap;
    TMXLayer *_background;
    Sprite *_player;
    Sprite *_enemy;
    TMXLayer *_meta;
    TMXLayer *_foreground_1;
    TMXLayer *_foreground_2;
    HudLayer *_hud;
    GameMenu *_menu;
    cocos2d::SpriteBatchNode *_batchNode;
    int _numCollected;
    Enemys *searching_enemy;
    Sprite* archer;
    Sprite* boss;
    Sprite* princess;
    Vector<Sprite*> _enemies;
    Vector<Sprite*> _projectiles;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    Sprite* _pauseBG;
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
    void setViewPointCenter(Point position);
    void setPlayerPosition(Point position);
    void enemyMoveFinished(Object* pSender);
    void onTouchEnded(Touch* touch, Event* event);
    void animateEnemy(Sprite* enemy);
    void addEnemyAtPos(Point pos);
    
    void setOpenSteps ( cocos2d::CCArray* var );
    void setClosedSteps ( cocos2d::CCArray* var );
    void onPause(Object* pSender);
    void onResume(Object* pSender);
    
    //collision detection
    Sprite* testCollisions();
    void heroAttacking();
    void lose();
    void win();
    bool detection();
    //shot projectile
    void shooting(float dt);
    void projectileMoveFinished(Object *pSender);
    void bulletCollision(float dt);
    
    //get player
    Sprite* getPlayer();
    //boss attacking
    void bossAttacking(float dt);
    
    //void moveToward(CCPoint fromPos, CCPoint toPos);
    Point tellMeWhereIsPlayer(Point pos);
    Point get_tilecoordposition(const Point position) const;
    int computeHScoreFromCoordToCoord(const Point &fromCoord, const Point &toCoord);
    
    bool isPropAtTileCoordForLayer(const char *prop, const cocos2d::Point &tileCoord, cocos2d::TMXLayer *layer) const;
    bool isCollidable(const Point tileCoord) const;
    bool isValidTileCoord(const Point &tileCoord) const;
    virtual void callMainMenu(cocos2d::Object* pSender);
    
    void getPrincess(float dt);
    Point tileCoordForPosition(const CCPoint position) const;
    Point positionForTileCoord(const Point tileCoord) const;
    PointArray* walkableAdjacentTilesCoordForTileCoord(const Point &tileCoord) const;
    CCArray* m_shortestPaths;

    //cocos2d::Vector<ShortestPathStep*> _spOpenSteps;
    //cocos2d::Vector<ShortestPathStep*> _spClosedSteps;
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};



#endif // __HELLOWORLD_SCENE_H__



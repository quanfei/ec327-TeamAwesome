//
//  MainMenu.h
//  HelloCpp
//
//  Created by 徐孟涛 on 3/31/14.
//
//

#ifndef HelloCpp_MainMenu_h
#define HelloCpp_MainMenu_h
#include "cocos2d.h"


class MainMenu : public cocos2d::Layer
{
public:
    virtual void mainMenu();
    virtual void callMainMenu(cocos2d::Object* pSender);
    
};

#endif

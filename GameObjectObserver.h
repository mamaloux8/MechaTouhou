#ifndef GAMEOBJECT_OBSERVER_H
#define GAMEOBJECT_OBSERVER_H

#include "VisibleGameObject.h"

class GameObjectObserver
{
public:
	virtual ~GameObjectObserver() = default;
	//Autre que le player
	//Quand un ennemie se fait tuer ou qu'on rajouter un ennemie
	//Du coup faire un enum dans GameObjectManager
	//virtual void onNotifiy(const VisibleGameObject& entity, ) = 0;
};

#endif
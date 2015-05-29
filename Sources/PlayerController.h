/**
 * PlayerController.h
 *
 */

#ifndef __PLAYERCONTROLLER_H_DEFINED__
#define __PLAYERCONTROLLER_H_DEFINED__

#include "Input.h"
#include "PlayerActor.h"

class PlayerController
{
public:
	PlayerController(PlayerActor *player);
	void update();

private:
	PlayerActor *player;
};

#endif //__PLAYERCONTROLLER_H_DEFINED__
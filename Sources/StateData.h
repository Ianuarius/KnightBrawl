/**
 * StateData.h
 *
 */

#ifndef __STATEDATA_H_DEFINED__
#define __STATEDATA_H_DEFINED__

#include "Knight.h"

class StateData
{
public:
	StateData();
	std::vector<std::vector<Knight *> > *knights;
	std::vector<Knight *> selection;
	int players;

private:
};

#endif // __STATEDATA_H_DEFINED__
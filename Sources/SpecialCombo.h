/**
 * SpecialCombo.h
 *
 */

#ifndef __SPECIALCOMBO_H_DEFINED__
#define __SPECIALCOMBO_H_DEFINED__

#include <vector>

class SpecialCombo
{
public:
	SpecialCombo();

	bool executing;
	int state;
	std::vector<int> keys;
	int tmp_input;

private:
};

#endif //__SPECIALCOMBO_H_DEFINED__
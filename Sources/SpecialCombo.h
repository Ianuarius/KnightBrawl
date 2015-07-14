/**
 * SpecialCombo.h
 *
 * All actions
 */

#ifndef __SPECIALCOMBO_H_DEFINED__
#define __SPECIALCOMBO_H_DEFINED__

#include <vector>
#include "Entity.h"
#include "Rectangle.h"

class SpecialCombo : public Entity
{
public:
	enum format_types {
		TYPE_STRIKE,
		TYPE_RANGED,
		TYPE_MISC
	};
	
	SpecialCombo();
	void defineAnimation(Animation *new_animation);
	
	Rectangle hitbox;
	Animation *animation;
	
	std::string name;
	int format;
	int damage;
	bool executing;
	int state;
	std::vector<int> keys;
	int tmp_input;

private:
};

#endif //__SPECIALCOMBO_H_DEFINED__
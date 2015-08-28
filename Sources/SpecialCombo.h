/**
 * SpecialCombo.h
 *
 * All actions
 */

#ifndef __SPECIALCOMBO_H_DEFINED__
#define __SPECIALCOMBO_H_DEFINED__

#include <vector>
#include "AreaEffect.h"
#include "BuffEffect.h"
#include "Effect.h"
#include "Entity.h"
#include "Key.h"
#include "ProjectileSpawner.h"
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
	void setHitboxes();
	void execute();
	
	std::vector<Rectangle> hitboxes;
	Animation *animation;
	
	bool disabled;
	std::string name;
	std::string combo_name;
	int format;
	int damage;
	bool executing, start_execution, in_ground;
	int state;
	int repeats;
	int frames;
	std::vector<Key> keys;
	std::vector<Effect> effects;
	std::vector<AreaEffect> area_effects;
	std::vector<BuffEffect> buff_effects;
	std::vector<ProjectileSpawner> projectile_spawners;
	int tmp_input;

private:
};

#endif //__SPECIALCOMBO_H_DEFINED__
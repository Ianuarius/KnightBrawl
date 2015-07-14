/**
 * Knight.cpp
 *
 */

#include "Knight.h"

Knight::Knight(Window *window, int knight_number):
	window(window),
	hitbox(0, 0, 0, 0),
	decal(nullptr)
{
	roster_result = roster_document.load_file("Scripts/roster.xml");
	attack_hitboxes.reserve(8);

	alive = true;
	hit = false;
	
	moves.reserve(ANIMATION_MAX);
	
	std::string knight_file = 
		roster_document.child("roster").
		find_child_by_attribute("id", std::to_string(knight_number).c_str()).
		attribute("script").value();

	knight_result = knight_document.load_file(knight_file.c_str());
	
	if (!knight_result) {
		printf("Failed to load knight %s.\n", knight_file.c_str());
		return;
	}

	animations.resize(ANIMATION_MAX);

	// NOTE(juha): This is filled with each combo, pushed to *special_combos 
	//             and cleared before another fill
	SpecialCombo combo;
	
	/* NOTE(juha): example code
	atoi(levelDocument.child("map").attribute("height").value());
	levelDocument.child("map").find_child_by_attribute("name", "Background2").child("data");
	tmp = new Animation(window, "Graphics/Knights/wizard_knight_run.png", 50, 50, 0, 4, 8);*/
	
	pugi::xml_node tmp_node;
	tmp_node = knight_document.child("knight");

	parseEffects(&tmp_node);

	Rectangle tmp_hitbox(0, 0, 0, 0);
	std::vector<Rectangle> tmp_move_hitboxes;

	tmp_node = knight_document.child("knight").child("action");
	parseActions(&tmp_node);

	tmp_node = knight_document.child("knight").child("action").child("idle");
	animations[IDLE] = parseAction(&tmp_node);
	
	tmp_node = knight_document.child("knight").child("action").child("run");
	animations[RUN] = parseAction(&tmp_node);
	
	tmp_node = knight_document.child("knight").child("action").child("jump");
	animations[JUMP] = parseAction(&tmp_node);

	jump = atof(tmp_node.attribute("height").value());
	
	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "basic");
	animations[ATTACK] = parseAction(&tmp_node);
	
	tmp_node = knight_document.child("knight").child("action").child("block");
	animations[BLOCK] = parseAction(&tmp_node);
	
	tmp_node = knight_document.child("knight").child("action").child("crouch");
	animations[CROUCH] = parseAction(&tmp_node);
	
	tmp_node = knight_document.child("knight").child("action").child("death");
	animations[DEATH] = parseAction(&tmp_node);
	
	tmp_node = knight_document.child("knight").child("action").child("dodge");
	animations[DODGE] = parseAction(&tmp_node);
	
	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "midair_downthrust");
	animations[DOWN_THRUST] = parseAction(&tmp_node);
	
	tmp_node = knight_document.child("knight").child("action").child("hanging");
	animations[HANGING] = parseAction(&tmp_node);
	
	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "midair_basic");
	animations[MID_AIR_BASIC_ATTACK] = parseAction(&tmp_node);
	
	tmp_node = knight_document.child("knight").child("action").child("pushback");
	animations[PUSHBACK] = parseAction(&tmp_node);
	
	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "special1");
	animations[SPECIAL_I] = parseAction(&tmp_node);

	for(pugi::xml_node_iterator iterator = tmp_node.child("control").begin();
			iterator != tmp_node.child("control").end();
			++iterator)
		{
			std::string tmp_key = std::string(iterator->child_value());
			combo.keys.push_back(parseKey(tmp_key));
		}
	
	special_combos.push_back(combo);
	combo.keys.clear();

	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "special2");
	animations[SPECIAL_II] = parseAction(&tmp_node);
	
	for(pugi::xml_node_iterator iterator = tmp_node.child("control").begin();
			iterator != tmp_node.child("control").end();
			++iterator)
		{
			std::string tmp_key = std::string(iterator->child_value());
			combo.keys.push_back(parseKey(tmp_key));
		}
	
	special_combos.push_back(combo);
	combo.keys.clear();

	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "special3");
	animations[SPECIAL_III] = parseAction(&tmp_node);
	
	for(pugi::xml_node_iterator iterator = tmp_node.child("control").begin();
			iterator != tmp_node.child("control").end();
			++iterator)
		{
			std::string tmp_key = std::string(iterator->child_value());
			combo.keys.push_back(parseKey(tmp_key));
		}
	
	special_combos.push_back(combo);
	combo.keys.clear();

	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "special4");
	animations[SPECIAL_IV] = parseAction(&tmp_node);

	for(pugi::xml_node_iterator iterator = tmp_node.child("control").begin();
			iterator != tmp_node.child("control").end();
			++iterator)
		{
			std::string tmp_key = std::string(iterator->child_value());
			combo.keys.push_back(parseKey(tmp_key));
		}
	
	special_combos.push_back(combo);
	combo.keys.clear();

	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "throw");
	animations[THROW] = parseAction(&tmp_node);
	
	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "uppercut");
	animations[UPPERCUT] = parseAction(&tmp_node);
	
}

void Knight::parseEffects(pugi::xml_node *tmp_node)
{
	for(pugi::xml_node_iterator iterator = tmp_node->begin();
			iterator != tmp_node->end();
			++iterator)
	{
		std::string tmp_string = iterator->name();
			
		if (tmp_string.compare("stats") == 0) {
			// NOTE(juha): Parsing the <stats>
			truename   = iterator->child("truename").child_value();
			knightname = iterator->child("knightname").child_value();
			decal = new Sprite(window,
							 iterator->child("decal").attribute("filename").value(),
						atoi(iterator->child("decal").attribute("width").value()),
						atoi(iterator->child("decal").attribute("height").value()));
			hitpoints = atoi(iterator->child("hitpoints").child_value());
			hitbox.x  = atoi(iterator->child("hitbox").attribute("x").value());
			hitbox.y  = atoi(iterator->child("hitbox").attribute("y").value());
			hitbox.w  = atoi(iterator->child("hitbox").attribute("width").value());
			hitbox.h  = atoi(iterator->child("hitbox").attribute("height").value());
			speed     = atof(iterator->child("speed").child_value());
		}

		if (tmp_string.compare("projectile") == 0) {
			
			Projectile tmp_projectile;
			Animation *tmp_animation = nullptr;
			tmp_projectile.name = iterator->attribute("name").value();
			
			std::string tmp_type_string = iterator->attribute("type").value();
			if (tmp_type_string.compare("linear") == 0) {
				tmp_projectile.type = tmp_projectile.TYPE_LINEAR;
			} else if (tmp_type_string.compare("arc") == 0) {
				tmp_projectile.type = tmp_projectile.TYPE_ARC;
			} else {
				tmp_projectile.type = tmp_projectile.TYPE_MISC;
			}
			tmp_projectile.range = atoi(iterator->attribute("range").value());
			tmp_projectile.power = atoi(iterator->attribute("power").value());
			tmp_projectile.speed = atoi(iterator->attribute("speed").value());

			tmp_animation = new Animation(window, 
					 iterator->child("animation").attribute("filename").value(),
				atoi(iterator->child("animation").attribute("width").value()),
				atoi(iterator->child("animation").attribute("height").value()),
				atoi(iterator->child("animation").attribute("startframe").value()),
				atoi(iterator->child("animation").attribute("framecount").value()),
				atoi(iterator->child("animation").attribute("framerate").value()));
			tmp_projectile.defineAnimation(tmp_animation);

			tmp_type_string = iterator->child("collision").attribute("collision").value();
			if (tmp_type_string.compare("all") == 0) {
				tmp_projectile.collision_type = tmp_projectile.COLLISION_ALL;
			} else if (tmp_type_string.compare("player") == 0) {
				tmp_projectile.collision_type = tmp_projectile.COLLISION_PLAYER;
			} else if (tmp_type_string.compare("wall") == 0) {
				tmp_projectile.collision_type = tmp_projectile.COLLISION_WALL;
			} else {
				tmp_projectile.collision_type = tmp_projectile.COLLISION_WALL;
			}
			
			tmp_type_string = iterator->child("collision").attribute("collisiondestruct").value();
			if (tmp_type_string.compare("true") == 0) {
				tmp_projectile.collision_destruct = true;
			} else {
				tmp_projectile.collision_destruct = false;
			}

			tmp_projectile.hitbox.x = atoi(iterator->child("hitbox").attribute("x").value());
			tmp_projectile.hitbox.y = atoi(iterator->child("hitbox").attribute("y").value());
			tmp_projectile.hitbox.w = atoi(iterator->child("hitbox").attribute("width").value());
			tmp_projectile.hitbox.h = atoi(iterator->child("hitbox").attribute("height").value());

			projectiles.push_back(tmp_projectile);
		}
			
		if (tmp_string.compare("trapeffect") == 0) {
			TrapEffect tmp_trap_effect;
			Animation *tmp_animation = nullptr;
			tmp_trap_effect.name = iterator->attribute("name").value();
			
			tmp_animation = new Animation(window, 
					 iterator->child("animation").attribute("filename").value(),
				atoi(iterator->child("animation").attribute("width").value()),
				atoi(iterator->child("animation").attribute("height").value()),
				atoi(iterator->child("animation").attribute("startframe").value()),
				atoi(iterator->child("animation").attribute("framecount").value()),
				atoi(iterator->child("animation").attribute("framerate").value()));
			tmp_trap_effect.defineAnimation(tmp_animation);

			std::string tmp_type_string = iterator->child("collision").attribute("collision").value();
			if (tmp_type_string.compare("all") == 0) {
				tmp_trap_effect.collision_type = tmp_trap_effect.COLLISION_ALL;
			} else if (tmp_type_string.compare("player") == 0) {
				tmp_trap_effect.collision_type = tmp_trap_effect.COLLISION_PLAYER;
			} else if (tmp_type_string.compare("wall") == 0) {
				tmp_trap_effect.collision_type = tmp_trap_effect.COLLISION_WALL;
			} else {
				tmp_trap_effect.collision_type = tmp_trap_effect.COLLISION_WALL;
			}
			
			tmp_type_string = iterator->child("collision").attribute("collisiondestruct").value();
			if (tmp_type_string.compare("true") == 0) {
				tmp_trap_effect.collision_destruct = true;
			} else {
				tmp_trap_effect.collision_destruct = false;
			}
			
			tmp_trap_effect.hitbox.x = atoi(iterator->child("hitbox").attribute("x").value());
			tmp_trap_effect.hitbox.y = atoi(iterator->child("hitbox").attribute("y").value());
			tmp_trap_effect.hitbox.w = atoi(iterator->child("hitbox").attribute("width").value());
			tmp_trap_effect.hitbox.h = atoi(iterator->child("hitbox").attribute("height").value());
			
			tmp_trap_effect.damage = atoi(iterator->child("damage").attribute("value").value());

			tmp_type_string = iterator->child("effect").attribute("type").value();
			if (tmp_type_string.compare("pushback") == 0) {
				tmp_trap_effect.effect_type = tmp_trap_effect.TYPE_PUSHBACK;
			} else {
				tmp_trap_effect.effect_type = tmp_trap_effect.TYPE_MISC;
			}
			
			tmp_trap_effect.effect_power = atoi(iterator->child("effect").attribute("power").value());

			trap_effects.push_back(tmp_trap_effect);
		}
			
		if (tmp_string.compare("areaeffect") == 0) {
			AreaEffect tmp_area_effect;
			tmp_area_effect.name = iterator->attribute("name").value();
			
			tmp_area_effect.hitbox.x = atoi(iterator->child("hitbox").attribute("x").value());
			tmp_area_effect.hitbox.y = atoi(iterator->child("hitbox").attribute("y").value());
			tmp_area_effect.hitbox.w = atoi(iterator->child("hitbox").attribute("width").value());
			tmp_area_effect.hitbox.h = atoi(iterator->child("hitbox").attribute("height").value());
			
			tmp_area_effect.damage = atoi(iterator->child("damage").attribute("value").value());
			
			area_effects.push_back(tmp_area_effect);
		}
			
		if (tmp_string.compare("buffeffect") == 0) {
			BuffEffect tmp_buff_effect;
			Animation *tmp_animation = nullptr;
			tmp_buff_effect.name = iterator->attribute("name").value();
			
			/* TODO(juha): Xml not ready, yet. Needs update.
			tmp_animation = new Animation(window, 
					 iterator->child("animation").attribute("filename").value(),
				atoi(iterator->child("animation").attribute("width").value()),
				atoi(iterator->child("animation").attribute("height").value()),
				atoi(iterator->child("animation").attribute("startframe").value()),
				atoi(iterator->child("animation").attribute("framecount").value()),
				atoi(iterator->child("animation").attribute("framerate").value()));
			tmp_buff_effect.defineAnimation(tmp_animation);
			*/
			std::string tmp_type_string = iterator->child("collision").attribute("collision").value();
			if (tmp_type_string.compare("all") == 0) {
				tmp_buff_effect.collision_type = tmp_buff_effect.COLLISION_ALL;
			} else if (tmp_type_string.compare("player") == 0) {
				tmp_buff_effect.collision_type = tmp_buff_effect.COLLISION_PLAYER;
			} else if (tmp_type_string.compare("wall") == 0) {
				tmp_buff_effect.collision_type = tmp_buff_effect.COLLISION_WALL;
			} else {
				tmp_buff_effect.collision_type = tmp_buff_effect.COLLISION_WALL;
			}
			
			tmp_type_string = iterator->child("collision").attribute("collisiondestruct").value();
			if (tmp_type_string.compare("true") == 0) {
				tmp_buff_effect.collision_destruct = true;
			} else {
				tmp_buff_effect.collision_destruct = false;
			}
			
			tmp_buff_effect.hitbox.x = atoi(iterator->child("hitbox").attribute("x").value());
			tmp_buff_effect.hitbox.y = atoi(iterator->child("hitbox").attribute("y").value());
			tmp_buff_effect.hitbox.w = atoi(iterator->child("hitbox").attribute("width").value());
			tmp_buff_effect.hitbox.h = atoi(iterator->child("hitbox").attribute("height").value());
			
			tmp_buff_effect.damage = atoi(iterator->child("damage").attribute("value").value());

			/* TODO(juha): Multiple effects require a vector.
			tmp_type_string = iterator->child("effect").attribute("type").value();
			if (tmp_type_string.compare("pushback") == 0) {
				tmp_buff_effect.effect_type = tmp_buff_effect.TYPE_PUSHBACK;
			} else {
				tmp_buff_effect.effect_type = tmp_buff_effect.TYPE_MISC;
			}
			
			tmp_buff_effect.effect_power = atoi(iterator->child("effect").attribute("power").value());
			*/
			buff_effects.push_back(tmp_buff_effect);
		}
			
		if (tmp_string.compare("action") == 0) {
			// Do nothing - parsed later.
		}
	}
}

void Knight::parseActions(pugi::xml_node *tmp_node)
{
	Animation *tmp_animation = nullptr;

	// NOTE(juha): Goes through all the actions
	for(pugi::xml_node_iterator action_it = tmp_node->begin();
			action_it != tmp_node->end();
			++action_it)
	{
		SpecialCombo tmp_combo;
		tmp_combo.name = action_it->attribute("name").value();
		
		std::string tmp_type_string = action_it->attribute("format").value();
		if (tmp_type_string.compare("strike") == 0) {
			tmp_combo.format = tmp_combo.TYPE_STRIKE;
		} else if (tmp_type_string.compare("ranged") == 0) {
			tmp_combo.format = tmp_combo.TYPE_RANGED;
		} else {
			tmp_combo.format = tmp_combo.TYPE_MISC;
		}
			
		// NOTE(juha): Goes through all the properties for an action
		for(pugi::xml_node_iterator property_it = action_it->begin();
			property_it != action_it->end();
			++property_it)
		{
			std::string tmp_string = property_it->name();
			
			if (tmp_string.compare("damage") == 0) {
				tmp_combo.damage = atoi(action_it->child("damage").attribute("value").value());
			}
			
			if (tmp_string.compare("animation") == 0) {
				tmp_animation = new Animation(window, 
						 action_it->child("animation").attribute("filename").value(),
					atoi(action_it->child("animation").attribute("width").value()),
					atoi(action_it->child("animation").attribute("height").value()),
					atoi(action_it->child("animation").attribute("startframe").value()),
					atoi(action_it->child("animation").attribute("framecount").value()),
					atoi(action_it->child("animation").attribute("framerate").value()));
			}
			tmp_combo.defineAnimation(tmp_animation);

			if (tmp_string.compare("hitbox") == 0) {
				tmp_combo.hitbox.x = atoi(action_it->child("hitbox").attribute("x").value());
				tmp_combo.hitbox.y = atoi(action_it->child("hitbox").attribute("y").value());
				tmp_combo.hitbox.w = atoi(action_it->child("hitbox").attribute("width").value());
				tmp_combo.hitbox.h = atoi(action_it->child("hitbox").attribute("height").value());
			}
			
			if (tmp_string.compare("control") == 0) {
				
			}
			
			if (tmp_string.compare("name") == 0) {

			}
			
			if (tmp_string.compare("effect") == 0) {

			}
			
			if (tmp_string.compare("buffeffect") == 0) {

			}
			
			if (tmp_string.compare("projectile") == 0) {

			}
			
			if (tmp_string.compare("projectilespawner") == 0) {

			}
		}
	}
}

Animation *Knight::parseAction(pugi::xml_node *tmp_node)
{
	Animation *tmp_animation = nullptr;


	for(pugi::xml_node_iterator iterator = tmp_node->begin();
			iterator != tmp_node->end();
			++iterator)
		{
			SpecialCombo tmp_combo;
			std::string tmp_string = iterator->name();

			if (tmp_string.compare("animation") == 0) {
				tmp_animation = new Animation(window, 
						 iterator->attribute("filename").value(),
					atoi(iterator->attribute("width").value()),
					atoi(iterator->attribute("height").value()),
					atoi(iterator->attribute("startframe").value()),
					atoi(iterator->attribute("framecount").value()),
					atoi(iterator->attribute("framerate").value()));
			}
			tmp_combo.defineAnimation(tmp_animation);

			if (tmp_string.compare("hitbox") == 0) {
				tmp_combo.hitbox.x = atoi(iterator->child("hitbox").attribute("x").value());
				tmp_combo.hitbox.y = atoi(iterator->child("hitbox").attribute("y").value());
				tmp_combo.hitbox.w = atoi(iterator->child("hitbox").attribute("width").value());
				tmp_combo.hitbox.h = atoi(iterator->child("hitbox").attribute("height").value());
			}
			
			if (tmp_string.compare("damage") == 0) {
				tmp_combo.damage = atoi(iterator->child("damage").attribute("value").value());
			}
			
			if (tmp_string.compare("control") == 0) {
				
			}
			
			if (tmp_string.compare("name") == 0) {

			}
			
			if (tmp_string.compare("effect") == 0) {

			}
			
			if (tmp_string.compare("buffeffect") == 0) {

			}
			
			if (tmp_string.compare("projectile") == 0) {

			}
			
			if (tmp_string.compare("projectilespawner") == 0) {

			}
		}

	return tmp_animation;
}


float Knight::getSpeed()
{
	return speed;
}

float Knight::getJump()
{
	return jump;
}

int Knight::getHitpoints()
{
	return hitpoints;
}

Sprite *Knight::getDecal()
{
	return decal;
}

std::string Knight::getTruename() {
	return truename;
}

std::vector<SpecialCombo> *Knight::getSpecialCombos()
{
	return &special_combos;
}

Animation *Knight::getAnimations(int animation)
{
	return animations[animation];
}

void Knight::damage(int amount) {
	hitpoints -= amount;
	
	if (hitpoints < 0) {
		hitpoints = 0;
	}

}

int Knight::parseKey(std::string key)
{
	if (key == "up") {
		return UP;
	} else if (key == "down") {
		return DOWN;
	} else if (key == "forward") {
		return FORWARD;
	} else if (key == "backward") {
		return BACKWARD;
	} else if (key == "jump") {
		return JUMP;
	} else {
		return ACTION;
	}
}
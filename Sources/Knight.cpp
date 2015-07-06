/**
 * Knight.cpp
 *
 */

#include "Knight.h"

Knight::Knight(Window *window, int knight_number):
	window(window),
	hitbox(0, 0, 0, 0)
{
	roster_result = roster_document.load_file("Scripts/roster.xml");
	attack_hitboxes.reserve(8);

	alive = true;
	hit = false;

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
	
	// NOTE(juha): Parsing the <stats>
	speed = atof(knight_document.child("knight").child("stats").child("speed").child_value());
	hitpoints = atoi(knight_document.child("knight").child("stats").child("hitpoints").child_value());
	truename = knight_document.child("knight").child("stats").child("truename").child_value();
	knightname = knight_document.child("knight").child("stats").child("knightname").child_value();
	hitbox.x = atoi(knight_document.child("knight").child("stats").child("hitbox").attribute("x").value());
	hitbox.y = atoi(knight_document.child("knight").child("stats").child("hitbox").attribute("y").value());
	hitbox.w = atoi(knight_document.child("knight").child("stats").child("hitbox").attribute("w").value());
	hitbox.h = atoi(knight_document.child("knight").child("stats").child("hitbox").attribute("h").value());

	pugi::xml_node tmp_node;
	Rectangle tmp_hitbox(0, 0, 0, 0);
	std::vector<Rectangle> tmp_move_hitboxes;

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

Animation *Knight::parseAction(pugi::xml_node *tmp_node)
{
	Animation *tmp_animation = nullptr;

	for(pugi::xml_node_iterator iterator = tmp_node->begin();
			iterator != tmp_node->end();
			++iterator)
		{
			std::string tmp_string = iterator->name();

			if (tmp_string.compare("animation") == 0) {
				tmp_animation = new Animation(window, 
						 tmp_node->child("animation").attribute("filename").value(),
					atoi(tmp_node->child("animation").attribute("width").value()),
					atoi(tmp_node->child("animation").attribute("height").value()),
					atoi(tmp_node->child("animation").attribute("startframe").value()),
					atoi(tmp_node->child("animation").attribute("framecount").value()),
					atoi(tmp_node->child("animation").attribute("framerate").value()));
			}

			if (tmp_string.compare("hitbox") == 0) {

			}
			
			if (tmp_string.compare("damage") == 0) {
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
/**
 * Knight.cpp
 *
 */

#include "Knight.h"

Knight::Knight(Window *window, int knight_number):
	window(window)
{
	roster_result = roster_document.load_file("Scripts/roster.xml");

	std::string knight_file = 
		roster_document.child("roster").
		find_child_by_attribute("id", std::to_string(knight_number).c_str()).
		attribute("script").value();

	knight_result = knight_document.load_file(knight_file.c_str());
	
	if (!knight_result) {
		printf("Failed to load knight %s.\n", knight_file.c_str());
		return;
	}

	Animation *tmp = nullptr;
	animations.resize(ANIMATION_MAX);
	
	special_one_combo.push_back(FORWARD);
	special_one_combo.push_back(FORWARD);
	special_one_combo.push_back(ACTION);
	
	/* NOTE(juha): example code
	atoi(levelDocument.child("map").attribute("height").value());
	levelDocument.child("map").find_child_by_attribute("name", "Background2").child("data");
	tmp = new Animation(window, "Graphics/Knights/wizard_knight_run.png", 50, 50, 0, 4, 8);*/
	
	// NOTE(juha): Parsing the <stats>
	speed = atof(knight_document.child("knight").child("stats").child("speed").child_value());
	hitpoints = atoi(knight_document.child("knight").child("stats").child("hitpoints").child_value());
	truename = knight_document.child("knight").child("stats").child("truename").child_value();
	knightname = knight_document.child("knight").child("stats").child("knightname").child_value();
	
	pugi::xml_node tmp_node;

	tmp_node = knight_document.child("knight").child("action").child("idle");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[IDLE] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").child("run");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[RUN] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").child("jump");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[JUMP] = tmp;

	jump = atof(tmp_node.attribute("height").value());
	
	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "basic");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[ATTACK] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").child("block");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[BLOCK] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").child("crouch");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[CROUCH] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").child("death");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[DEATH] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").child("dodge");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[DODGE] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "midair_downthrust");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[DOWN_THRUST] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").child("hanging");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[HANGING] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "midair_basic");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[MID_AIR_BASIC_ATTACK] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").child("pushback");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[PUSHBACK] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "special1");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[SPECIAL_I] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "special2");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[SPECIAL_II] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "special3");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[SPECIAL_III] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "special4");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[SPECIAL_IV] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "throw");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[THROW] = tmp;
	
	tmp_node = knight_document.child("knight").child("action").find_child_by_attribute("name", "uppercut");
	tmp = new Animation(window, 
		     tmp_node.child("animation").attribute("filename").value(),
		atoi(tmp_node.child("animation").attribute("width").value()),
		atoi(tmp_node.child("animation").attribute("height").value()),
		atoi(tmp_node.child("animation").attribute("startframe").value()),
		atoi(tmp_node.child("animation").attribute("framecount").value()),
		atoi(tmp_node.child("animation").attribute("framerate").value()));
	animations[UPPERCUT] = tmp;
	
}

float Knight::getSpeed()
{
	return speed;
}

float Knight::getJump()
{
	return jump;
}

std::vector<int> *Knight::getSpecialOneCombo()
{
	return &special_one_combo;
}

Animation *Knight::getAnimations(int animation)
{
	return animations[animation];
}

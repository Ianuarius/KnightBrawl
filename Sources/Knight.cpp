/**
 * Knight.cpp
 *
 */

#include "Knight.h"

Knight::Knight(Window *window, int knight_name)
{

	/*
	TODO(juha): 
	- Näppäinkomentojen lukeminen XML-filusta
	*/
	roster_result = roster_document.load_file("Scripts/roster.xml");

	std::string knight_file = 
		roster_document.child("roster").
		find_child_by_attribute("id", std::to_string(knight_name).c_str()).
		attribute("script").value();

	knight_result = knight_document.load_file(knight_file.c_str());
	
	if (!knight_result) {
		printf("Failed to load knight %s.\n", knight_file.c_str());
		return;
	}
	Animation *tmp = nullptr;

	animations.resize(ANIMATION_MAX);
	
	/*atoi(levelDocument.child("map").attribute("height").value());
	levelDocument.child("map").find_child_by_attribute("name", "Background2").child("data");
	tmp = new Animation(window, "Graphics/Knights/wizard_knight_run.png", 50, 50, 0, 4, 8);*/
	
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
	
}

Animation *Knight::getAnimations(int animation)
{
	return animations[animation];
}
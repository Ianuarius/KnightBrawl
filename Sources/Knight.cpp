/**
 * Knight.cpp
 *
 */

#include "Knight.h"

Knight::Knight(Window *window, int knight_number, int lives_total):
	window(window),
	hitbox(0, 0, 0, 0),
	decal(nullptr),
	lives(lives_total),
	deaths(0),
	specialpower(0)
{
	roster_result = roster_document.load_file("Scripts/roster.xml");
	attack_hitboxes.reserve(8);

	alive = true;
	hit = false;
	jumping = false;
	landing = false;
	is_landed = false;
	is_jumping = false;
	
	moves.resize(ANIMATION_MAX);

	
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
	
	parseSounds(&tmp_node);

	parseEffects(&tmp_node);

	Rectangle tmp_hitbox(0, 0, 0, 0);
	std::vector<Rectangle> tmp_move_hitboxes;

	tmp_node = knight_document.child("knight").child("action");
	parseActions(&tmp_node);
}

void Knight::parseSounds(pugi::xml_node *tmp_node)
{
	for(pugi::xml_node_iterator iterator = tmp_node->child("soundeffects").begin();
			iterator != tmp_node->child("soundeffects").end();
			++iterator)
	{
		Sound tmp_sound;

		tmp_sound.filename = iterator->attribute("filename").value();
		tmp_sound.type = iterator->attribute("type").value();

		sounds.push_back(tmp_sound);
	}
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
			speed     = (float)atof(iterator->child("speed").child_value());
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
			
		}
			
		if (tmp_string.compare("buffeffect") == 0) {
			
			/* TODO(juha): Multiple effects require a vector.
			tmp_type_string = iterator->child("effect").attribute("type").value();
			if (tmp_type_string.compare("pushback") == 0) {
				tmp_buff_effect.effect_type = tmp_buff_effect.TYPE_PUSHBACK;
			} else {
				tmp_buff_effect.effect_type = tmp_buff_effect.TYPE_MISC;
			}
			
			tmp_buff_effect.effect_power = atoi(iterator->child("effect").attribute("power").value());
			*/
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

		if (tmp_combo.name.compare("jump") == 0) {
			jump = (float)atof(action_it->attribute("height").value());
		}

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
				tmp_animation->running = false;
			}
			tmp_combo.defineAnimation(tmp_animation);

			if (tmp_string.compare("hitbox") == 0) {
				tmp_combo.hitbox.x = atoi(action_it->child("hitbox").attribute("x").value());
				tmp_combo.hitbox.y = atoi(action_it->child("hitbox").attribute("y").value());
				tmp_combo.hitbox.w = atoi(action_it->child("hitbox").attribute("width").value());
				tmp_combo.hitbox.h = atoi(action_it->child("hitbox").attribute("height").value());
			}
			
			if (tmp_string.compare("control") == 0) {
				Key tmp_key_object;

				for(pugi::xml_node_iterator key_iterator = action_it->child("control").begin();
						key_iterator != action_it->child("control").end();
						++key_iterator)
				{
					std::string tmp_key_value = std::string(key_iterator->child_value());
					tmp_key_object.keycode = parseKey(tmp_key_value);

					tmp_key_value = std::string(key_iterator->attribute("pressed").value());
					
					if (tmp_key_value.compare("true") == 0) {
						tmp_key_object.pressed = true;
					}
					
					tmp_key_value = std::string(key_iterator->attribute("inair").value());
					
					if (tmp_key_value.compare("true") == 0) {
						tmp_key_object.in_air = true;
					}

					tmp_combo.keys.push_back(tmp_key_object);
				}
			}
			
			if (tmp_string.compare("name") == 0) {
				tmp_combo.combo_name = action_it->child("name").child_value();
			}
			
			if (tmp_string.compare("effect") == 0) {
				Effect tmp_effect;
				
				std::string tmp_fxtype_string = action_it->child("effect").attribute("type").value();
				if (tmp_fxtype_string.compare("slow") == 0) {
					tmp_effect.type = tmp_effect.SLOW;
				} else if (tmp_fxtype_string.compare("stun") == 0) {
					tmp_effect.type = tmp_effect.STUN;
					tmp_effect.duration = (float)atof(action_it->child("effect").attribute("duration").value());
				} else if (tmp_fxtype_string.compare("burn") == 0) {
					tmp_effect.type = tmp_effect.BURN;
				} else if (tmp_fxtype_string.compare("pushback") == 0) {
					tmp_effect.type = tmp_effect.PUSHBACK;
					tmp_effect.power = atoi(action_it->child("effect").attribute("power").value());
				} else if (tmp_fxtype_string.compare("buff") == 0) {
					tmp_effect.type = tmp_effect.BUFF;
					
					std::string tmp_stat_string = action_it->child("effect").attribute("stat").value();
					if (tmp_stat_string.compare("defense") == 0) {
						tmp_effect.power = tmp_effect.DEFENSE;
					} else if (tmp_stat_string.compare("attack") == 0) {
						tmp_effect.power = tmp_effect.ATTACK;
					} else if (tmp_stat_string.compare("speed") == 0) {
						tmp_effect.power = tmp_effect.SPEED;
					}
					
					tmp_effect.power = atoi(action_it->child("effect").attribute("power").value());

					std::string tmp_format_string = action_it->child("effect").attribute("format").value();
					if (tmp_format_string.compare("duration") == 0) {
						tmp_effect.power = tmp_effect.DURATION;
					} else if (tmp_format_string.compare("count") == 0) {
						tmp_effect.power = tmp_effect.COUNT;
					} else if (tmp_format_string.compare("pressed") == 0) {
						tmp_effect.power = tmp_effect.PRESSED;
					}
					
					tmp_effect.time = atoi(action_it->child("effect").attribute("time").value());

				} else if (tmp_fxtype_string.compare("damagereturn") == 0) {
					tmp_effect.type = tmp_effect.DAMAGE_RETURN;
					tmp_effect.power = atoi(action_it->child("effect").attribute("power").value());
					
					std::string tmp_format_string = action_it->child("effect").attribute("format").value();
					if (tmp_format_string.compare("duration") == 0) {
						tmp_effect.power = tmp_effect.DURATION;
					} else if (tmp_format_string.compare("count") == 0) {
						tmp_effect.power = tmp_effect.COUNT;
					} else if (tmp_format_string.compare("pressed") == 0) {
						tmp_effect.power = tmp_effect.PRESSED;
					}
					
					tmp_effect.time = atoi(action_it->child("effect").attribute("time").value());

				} else if (tmp_fxtype_string.compare("movement") == 0) {
					tmp_effect.type = tmp_effect.MOVEMENT;
					tmp_effect.speed = atoi(action_it->child("effect").attribute("speed").value());
					tmp_effect.range = atoi(action_it->child("effect").attribute("range").value());
					tmp_effect.angle = atoi(action_it->child("effect").attribute("angle").value());
				}
			
				tmp_combo.effects.push_back(tmp_effect);
			}
			// TODO(juha): buffeffect, projectile, projectilespawner, trapspawner,
			//			   control parameters, hitbox frames, areaeffect.
			
			if (tmp_string.compare("areaeffect") == 0) {
				AreaEffect tmp_area_effect;
				tmp_area_effect.name = action_it->attribute("name").value();
			
				tmp_area_effect.hitbox.x = atoi(action_it->child("hitbox").attribute("x").value());
				tmp_area_effect.hitbox.y = atoi(action_it->child("hitbox").attribute("y").value());
				tmp_area_effect.hitbox.w = atoi(action_it->child("hitbox").attribute("width").value());
				tmp_area_effect.hitbox.h = atoi(action_it->child("hitbox").attribute("height").value());
			
				tmp_area_effect.damage = atoi(action_it->child("damage").attribute("value").value());
			
				tmp_combo.area_effects.push_back(tmp_area_effect);
			}
			
			if (tmp_string.compare("buffeffect") == 0) {
				BuffEffect tmp_buff_effect;
				Animation *tmp_animation = nullptr;
				tmp_buff_effect.name = action_it->attribute("name").value();
			
				std::string tmp_type_string = action_it->child("collision").attribute("collision").value();
				if (tmp_type_string.compare("all") == 0) {
					tmp_buff_effect.collision_type = tmp_buff_effect.COLLISION_ALL;
				} else if (tmp_type_string.compare("player") == 0) {
					tmp_buff_effect.collision_type = tmp_buff_effect.COLLISION_PLAYER;
				} else if (tmp_type_string.compare("wall") == 0) {
					tmp_buff_effect.collision_type = tmp_buff_effect.COLLISION_WALL;
				} else {
					tmp_buff_effect.collision_type = tmp_buff_effect.COLLISION_WALL;
				}
			
				tmp_type_string = action_it->child("collision").attribute("collisiondestruct").value();
				if (tmp_type_string.compare("true") == 0) {
					tmp_buff_effect.collision_destruct = true;
				} else {
					tmp_buff_effect.collision_destruct = false;
				}
			
				tmp_buff_effect.hitbox.x = atoi(action_it->child("hitbox").attribute("x").value());
				tmp_buff_effect.hitbox.y = atoi(action_it->child("hitbox").attribute("y").value());
				tmp_buff_effect.hitbox.w = atoi(action_it->child("hitbox").attribute("width").value());
				tmp_buff_effect.hitbox.h = atoi(action_it->child("hitbox").attribute("height").value());
			
				tmp_buff_effect.damage = atoi(action_it->child("damage").attribute("value").value());
			
				tmp_combo.buff_effects.push_back(tmp_buff_effect);
			}
			
			if (tmp_string.compare("projectilespawner") == 0) {
				ProjectileSpawner tmp_projectile_spawner;

				tmp_projectile_spawner.type = action_it->child("projectilespawner").attribute("name").value();
				tmp_projectile_spawner.amount = atoi(action_it->child("projectilespawner").attribute("amount").value());
				tmp_projectile_spawner.angle = atoi(action_it->child("projectilespawner").attribute("angle").value());
				tmp_projectile_spawner.angle_interval = atoi(action_it->child("projectilespawner").attribute("angleinterval").value());
				tmp_projectile_spawner.delay = atoi(action_it->child("projectilespawner").attribute("delay").value());
				tmp_projectile_spawner.delay_interval = atoi(action_it->child("projectilespawner").attribute("delayinterval").value());
				
				projectile_spawners.push_back(tmp_projectile_spawner);
			}
			
			if (tmp_string.compare("trapspawner") == 0) {
				TrapSpawner tmp_trap_spawner;

				tmp_trap_spawner.type = action_it->child("trapspawner").attribute("name").value();
				tmp_trap_spawner.amount = atoi(action_it->child("trapspawner").attribute("amount").value());
				tmp_trap_spawner.angle = atoi(action_it->child("trapspawner").attribute("angle").value());
				tmp_trap_spawner.angle_interval = atoi(action_it->child("trapspawner").attribute("angleinterval").value());
				tmp_trap_spawner.delay = atoi(action_it->child("trapspawner").attribute("delay").value());
				tmp_trap_spawner.delay_interval = atoi(action_it->child("trapspawner").attribute("delayinterval").value());

				trap_spawners.push_back(tmp_trap_spawner);
			}
		}
		if (tmp_combo.name.compare("basic") == 0) {
			moves[ATTACK] = tmp_combo;
		} else if (tmp_combo.name.compare("midair_basic") == 0) {
			tmp_combo.disabled = true;
			moves[MID_AIR_BASIC_ATTACK] = tmp_combo;
		} else if (tmp_combo.name.compare("midair_downthrust") == 0) {
			tmp_combo.disabled = true;
			moves[DOWN_THRUST] = tmp_combo;
		} else if (tmp_combo.name.compare("special1") == 0) {
			tmp_combo.disabled = true;
			moves[SPECIAL_I] = tmp_combo;
		} else if (tmp_combo.name.compare("special2") == 0) {
			tmp_combo.disabled = true;
			moves[SPECIAL_II] = tmp_combo;
		} else if (tmp_combo.name.compare("special3") == 0) {
			tmp_combo.disabled = true;
			moves[SPECIAL_III] = tmp_combo;
		} else if (tmp_combo.name.compare("special4") == 0) {
			tmp_combo.disabled = true;
			moves[SPECIAL_IV] = tmp_combo;
		} else if (tmp_combo.name.compare("throw") == 0) {
			tmp_combo.disabled = true;
			moves[THROW] = tmp_combo;
		} else if (tmp_combo.name.compare("uppercut") == 0) {
			tmp_combo.disabled = true;
			moves[UPPERCUT] = tmp_combo;
		} else if (tmp_combo.name.compare("block") == 0) {
			tmp_combo.disabled = true;
			moves[BLOCK] = tmp_combo;
		} else if (tmp_combo.name.compare("crouch") == 0) {
			moves[CROUCH] = tmp_combo;
		} else if (tmp_combo.name.compare("death") == 0) {
			moves[DEATH] = tmp_combo;
		} else if (tmp_combo.name.compare("dodge") == 0) {
			tmp_combo.disabled = true;
			moves[DODGE] = tmp_combo;
		} else if (tmp_combo.name.compare("hanging") == 0) {
			tmp_combo.disabled = true;
			moves[HANGING] = tmp_combo;
		} else if (tmp_combo.name.compare("idle") == 0) {
			moves[IDLE] = tmp_combo;
		} else if (tmp_combo.name.compare("jump") == 0) {
			moves[JUMP] = tmp_combo;
		} else if (tmp_combo.name.compare("pushback") == 0) {
			tmp_combo.disabled = true;
			moves[PUSHBACK] = tmp_combo;
		} else if (tmp_combo.name.compare("run") == 0) {
			moves[RUN] = tmp_combo;
		}
	}
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

void Knight::respawn()
{
	if (lives > 0) {
		alive = true;
		hitpoints = 100;
		moves[DEATH].animation->times_played = 0;
	}
}

Sprite *Knight::getDecal()
{
	return decal;
}

std::string Knight::getTruename() {
	return truename;
}

std::vector<SpecialCombo> *Knight::getMoves()
{
	return &moves;
}

std::vector<Sound> *Knight::getSounds()
{
	return &sounds;
}
Animation *Knight::getAnimations(int animation)
{
	return moves[animation].animation;
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

int Knight::getLives()
{
	return lives;
}

int Knight::getDeaths()
{
	return deaths;
}

void Knight::kill()
{
	if (alive) {
		hitpoints = 0;
		lives--;
		alive = false;
		deaths++;
	}
}

int Knight::getSpecialPower()
{
	return specialpower;
}

void Knight::powerup()
{
	specialpower += 10;
	
	if (specialpower > 100) {
		specialpower = 100;
	}
}
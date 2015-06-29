/**
 * Input.h
 * 
 * Input manager.
 * 
 * Esimerkki:
 * // Jossain kohtaa koodia:
 * Input::update();
 *
 * Input::keyState(SDL_SCANCODE_A); // Palauttaa true jos 'A' on alhaalla
 * 
 * TODO(jouni):
 *	- Muunnetaan luokka staattisesta oikeaan oliopohjaiseen
 */

#ifndef __INPUT_H_DEFINED__
#define __INPUT_H_DEFINED__

// Keyboard size
#define KEYBOARD_SIZE 282

// Includet
#include "SDL.h"

class Input {
	public:
		Input();
		// P‰ivitt‰‰ n‰pp‰imen painallukset
		void update();

		// Jos n‰pp‰in on alhaalla palauttaa true, muuten false
		bool keyState(int key);
		bool keyPressed(int key);
		
	    bool isKeyPressed(int k);
		
		/// Tells if the keyboard `key` was released just now.
		bool isKeyDown(int key);

		/// Tells if the keyboard `key` was pressed just now.
		bool isKeyUp(int key);

		// Avustavametodi tarkastamaan onko SHIFT alhalla
		bool shift();

		// Avustavametodi tarkastamaan onko CTRL alhaalla
		bool ctrl();

		// Avustavametodi tarkastamaan onko ALT alhaalla
		bool alt();

		// 'Lukitsee' input managerin. N‰pp‰imi‰ ei p‰ivitet‰, jolloin esim.
		// keyState palauttaa aina false.
		void lock();

		// Poistaa lukituksen
		void unlock();

	private:

};

#endif
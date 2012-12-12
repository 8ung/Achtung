//Powerup är en abstrakt klass som alla powerupklasser ärver ifrån.
#include "Powerup.h"
#include "SDL_stdinc.h"

using namespace std;

Powerup::Powerup(const Position_class new_position,
		const Uint32 color, const char* image_path)
{
	position = new_position;
	//Detta är den färg som finns på den cirkel som ligger bakom powerup-bilden.
	colour_id = color;
	radius = 30;
	//Detta är bilden som används som grafisk representation för alla powerups.
	image = SDL_LoadBMP(image_path);
}

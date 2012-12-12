//Powerup �r en abstrakt klass som alla powerupklasser �rver ifr�n.
#include "Powerup.h"
#include "SDL_stdinc.h"

using namespace std;

Powerup::Powerup(const Position_class new_position,
		const Uint32 color, const char* image_path)
{
	position = new_position;
	//Detta �r den f�rg som finns p� den cirkel som ligger bakom powerup-bilden.
	colour_id = color;
	radius = 30;
	//Detta �r bilden som anv�nds som grafisk representation f�r alla powerups.
	image = SDL_LoadBMP(image_path);
}

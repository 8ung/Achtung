#include "Powerup.h"
#include "SDL_stdinc.h"

using namespace std;

Powerup::Powerup(Position_class new_position, Uint32 color, const char* image_path)
{
	position = new_position;
	colour_id = color;
	radius = 30;
	image = SDL_LoadBMP(image_path);
}

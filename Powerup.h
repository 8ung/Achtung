#ifndef POWERUP_H
#define POWERUP_H

#include "SDL_stdinc.h"
#include <vector>
#include "Worm.h"

class Powerup
{

public:
	Position_class position;
	Powerup(Position_class, Uint32);
	virtual ~Powerup() = default;

	Uint32 colour_id;
	int radius;

	virtual void execute(int, std::vector<Worm*>&) = 0;
};

#endif

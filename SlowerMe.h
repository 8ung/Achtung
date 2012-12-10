#ifndef SLOWERME_H
#define SLOWERME_H

#include "Position_class.h"
#include "SDL_stdinc.h"
#include "Powerup.h"
#include "Worm.h"

class SlowerMe : public Powerup
{

public:
	bool affect_map = false;
	SlowerMe(Position_class, Uint32);

	void execute(int, std::vector<Worm*>&);
};

#endif


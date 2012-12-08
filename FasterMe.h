#ifndef FASTERME_H
#define FASTERME_H

#include "Position_class.h"
#include "SDL_stdinc.h"
#include "Powerup.h"
#include "Worm.h"

class FasterMe : public Powerup
{

public:
	bool affect_map = false;
	FasterMe(Position_class, Uint32);

	void execute(int, std::vector<Worm*>&);
};

#endif

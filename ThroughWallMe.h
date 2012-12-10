#ifndef THROUGHWALL_H
#define THROUGHWALL_H

#include "Position_class.h"
#include "SDL_stdinc.h"
#include "Powerup.h"
#include "Worm.h"

class ThroughWallMe : public Powerup
{
public:
	bool affect_map;
	ThroughWallMe(Position_class, Uint32);
	void execute(int, std::vector<Worm*>&);
};

#endif

#ifndef THROUGHWALL_H
#define THROUGHWALL_H

#include "C:\Users\Dennis\C++\Achtung\src\src\Position_class.h"
#include "SDL_stdinc.h"
#include "Powerup.h"

class ThroughWallMe : public Powerup
{
public:
	bool affect_map;
	ThroughWallMe(Position_class, Uint32);
	void execute(int, std::vector<Worm>&);
};

#endif

#ifndef GHOSTME_H
#define GHOSTME_H

#include "C:\Users\Dennis\C++\Achtung\src\src\Position_class.h"
#include "SDL_stdinc.h"
#include "Powerup.h"

class GhostMe : public Powerup
{
public:
	bool affect_map;
	GhostMe(Position_class, Uint32);
	void execute(int, std::vector<Worm>&);
};

#endif

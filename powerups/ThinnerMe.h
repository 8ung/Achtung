#ifndef THINNERME_H
#define THINNERME_H

#include "C:\Users\Dennis\C++\Achtung\src\src\Position_class.h"
#include "SDL_stdinc.h"
#include "Powerup.h"

class ThinnerMe : public Powerup
{
public:
	bool affect_map;
	ThinnerMe(Position_class, Uint32);
	void execute(int, std::vector<Worm>&);
};

#endif

#ifndef SLOWERYOU_H
#define SLOWERYOU_H

#include "C:\Users\Dennis\C++\Achtung\src\src\Position_class.h"
#include "SDL_stdinc.h"
#include "Powerup.h"

class SlowerYou : public Powerup
{

public:
	bool affect_map = false;
	SlowerYou(Position_class, Uint32);

	void execute(int, std::vector<Worm>&);
};

#endif

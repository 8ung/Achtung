#ifndef SHARPTURNME_H
#define SHARPTURNME_H

#include "C:\Users\Dennis\C++\Achtung\src\src\Position_class.h"
#include "SDL_stdinc.h"
#include "Powerup.h"

class SharpTurnMe : public Powerup
{

public:
	bool affect_map = false;
	SharpTurnMe(Position_class, Uint32);

	void execute(int, std::vector<Worm>&);
};

#endif

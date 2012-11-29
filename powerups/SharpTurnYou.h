#ifndef SHARPTURNYOU_H
#define SHARPTURNYOU_H

#include "C:\Users\Dennis\C++\Achtung\src\src\Position_class.h"
#include "SDL_stdinc.h"
#include "Powerup.h"

class SharpTurnYou : public Powerup
{

public:
	bool affect_map = false;
	SharpTurnYou(Position_class, Uint32);

	void execute(int, std::vector<Worm>&);
};


#endif

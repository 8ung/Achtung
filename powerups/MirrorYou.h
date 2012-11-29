#ifndef MIRRORYOU_H
#define MIRRORYOU_H

#include "C:\Users\Dennis\C++\Achtung\src\src\Position_class.h"
#include "SDL_stdinc.h"
#include "Powerup.h"

class MirrorYou : public Powerup
{

public:
	bool affect_map = false;
	MirrorYou(Position_class, Uint32);

	void execute(int, std::vector<Worm>&);
};

#endif

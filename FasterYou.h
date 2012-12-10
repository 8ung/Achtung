#ifndef FASTERYOU_H
#define FASTERYOU_H

#include "Position_class.h"
#include "SDL_stdinc.h"
#include "Powerup.h"
#include "Worm.h"

class FasterYou : public Powerup
{

public:
	bool affect_map = false;
	FasterYou(Position_class, Uint32);

	void execute(int, std::vector<Worm*>&);
};

#endif

#ifndef SLOWERYOU_H
#define SLOWERYOU_H

#include "Position_class.h"
#include "SDL_stdinc.h"
#include "Powerup.h"
#include "Worm.h"

class SlowerYou : public Powerup
{

public:
	bool affect_map = false;
	SlowerYou(const Position_class, const Uint32);

	void execute(const int, std::vector<Worm*>&);
};

#endif

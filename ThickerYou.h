#ifndef THICKERYOU_H
#define THICKERYOU_H

#include "Position_class.h"
#include "SDL_stdinc.h"
#include "Powerup.h"
#include "Worm.h"

class ThickerYou : public Powerup
{

public:
	bool affect_map = false;
	ThickerYou(Position_class, Uint32);

	void execute(int, std::vector<Worm*>&);
};

#endif

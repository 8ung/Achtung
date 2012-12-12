#ifndef MIRRORYOU_H
#define MIRRORYOU_H

#include "Position_class.h"
#include "SDL_stdinc.h"
#include "Powerup.h"
#include "Worm.h"

class MirrorYou : public Powerup
{

public:
	bool affect_map = false;
	MirrorYou(const Position_class, const Uint32);

	void execute(const int, std::vector<Worm*>&);
};

#endif

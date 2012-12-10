#ifndef FASTERME_H
#define FASTERME_H

#include "Position_class.h"
#include "SDL_stdinc.h"
#include "Powerup.h"
#include "Worm.h"



class FasterMe : public Powerup
{

public:
	std::string image_path;
	bool affect_map = false;
	FasterMe(Position_class, Uint32);
	//SDL_Surface* image;

	void execute(int, std::vector<Worm*>&);

	~FasterMe() = default;
};

#endif

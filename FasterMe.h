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
	FasterMe(const Position_class, const Uint32);
	//SDL_Surface* image;

	void execute(const int, std::vector<Worm*>&);

	~FasterMe() = default;
};

#endif

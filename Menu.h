#ifndef MENU_H
#define MENU_H

#include "SDL_stdinc.h"
#include "Position_class.h"

class Menu
{

private:
	Position_class first_position;
	bool team_play;
	int spacing;
	//SDL_Surface* background_image = 0;
	enum
	{
		red = 16711680,
		yellow = 16776960,
		orange = 16753920,
		green = 65280,
		cyan = 65535,
		blue = 255
	};

public:

	int position;
	Position_class marker_position;

	bool start_menu;

	Menu(int);

	int execute_start_menu();

	Uint32 execute_select_worm_menu();

	void move_up();

	void move_down();
};


#endif /* MENU_H */

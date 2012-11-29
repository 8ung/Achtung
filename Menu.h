#ifndef MENU_H
#define MENU_H

#include "SDL_stdinc.h"
#include "Position_class.h"

class Menu
{

private:
	Position_class marker_position;
	Position_class first_position;
	bool team_play;
	bool start_menu;
	int spacing;
	int position;
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
	Menu(bool, bool);

	int execute_start_menu();

	Uint32 execute_select_worm_menu();

	void move_up();

	void move_down();
};


#endif /* MENU_H */

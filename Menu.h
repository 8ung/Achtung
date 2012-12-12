#ifndef MENU_H
#define MENU_H

#include "SDL_stdinc.h"
#include "Position_class.h"

class Menu
{

private:
	bool team_play;
	int spacing;
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

	Position_class first_position;
	Position_class marker_position;
	int position;
	bool start_menu;

	Menu(const int);

	void move_up();

	void move_down();

	const int execute_start_menu();

	const Uint32 execute_select_worm_menu();

	void reset_menu();
};


#endif /* MENU_H */

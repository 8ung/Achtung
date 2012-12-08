#ifndef GAME_H
#define GAME_H

#include <vector>
#include "SDL.h"
#include "Playground.h"
#include "SDL_ttf.h"
#include "Menu.h"
#include "Position_class.h"

class Game {

private:
	bool team_play = false;
	Uint32 input_colour;
	int input_left_controler;
	int input_right_controler;
	int keys[ SDLK_LAST ];
	int frameSkip;
	int running;
	bool in_menu;
	int window_height;
	bool game_paused = false;
	bool in_start_menu = true;
	std::vector<Position_class*> check_box;
	SDL_Rect menu_pos;
	SDL_Surface* display;
	Playground* playground;
	Menu* menu;
	SDL_Event event;
	SDL_Surface* start_menu = nullptr;
	SDL_Surface* menu_all = nullptr;
	SDL_Surface* menu_team = nullptr;
	SDL_Surface *message = NULL;
	TTF_Font *font = NULL;
	SDL_Color textColor = { 255, 255, 255 };


public:
	Game();

private:
	void initialize();

	void listen_to_keys();

	void draw_rectangle(SDL_Surface*, SDL_Rect*, int, int, int);

	void draw_blank(int, int, int, int);

	void draw_boundaries();

	void draw_playground();

	void draw_winner();

	void draw_menu();

	void draw_scoreboard();

	void game_finished();

	void key_pressed( SDL_Event* event );

	void key_unpressed( SDL_Event* event );

	void quit();

	void fpsChanged(int);

public:
	void run();

	int get_window_height();
};

#endif /* GAME_H */

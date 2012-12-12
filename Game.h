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
	Playground* playground;
	Menu* menu;
	SDL_Surface* rules = nullptr;
	SDL_Surface* display = nullptr;
	SDL_Surface* start_menu = nullptr;
	SDL_Surface* menu_all = nullptr;
	SDL_Surface* menu_team = nullptr;
	SDL_Surface* text = nullptr;
	TTF_Font* font = NULL;
	SDL_Color textColor = { 255, 255, 255 };
	int window_height;
	int frameSkip;
	bool team_play = false;
	Uint32 input_colour;
	int input_left_controler;
	int input_right_controler;
	SDL_Event event;
	int keys[SDLK_LAST];
	bool in_menu;
	bool in_start_menu = true;
	std::vector<Position_class*> check_box;
	SDL_Rect menu_pos;
	SDL_Rect game_rules;
	int in_game;
	SDL_Rect colour_box;
	bool game_paused = false;

	void initialize();

	void listen_to_keys();

	void key_pressed(const SDL_Event*);

	void key_unpressed(const SDL_Event*);

	void fps_changed(const int);

	void menu_loop(int&, int&, int&, int&, int&);

	void game_loop(int&, int&, int&, int&, int&);

	void draw_menu();

	void draw_blank(const int, const int, const int, const int);

	void draw_boundaries();

	void fill_circle(const SDL_Surface*, const double, const double,
			const double, const Uint32);

	void draw_playground();

	void draw_scoreboard();

	void game_finished();

	void quit();

public:
	Game();

	void run();

};

#endif /* GAME_H */

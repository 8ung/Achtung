#ifndef GAME_H
#define GAME_H

#include <vector>
#include "SDL.h"
#include "Playground.h"

class Game {

private:
	bool team_play;
	Uint32 input_colour;
	int input_left_controler;
	int input_right_controler;
	int keys[ SDLK_LAST ];
	int frameSkip;
	int running;
	int window_height;
	SDL_Surface* display;
	Playground* playground;
	SDL_Event event;
	SDL_Surface* hello = nullptr;


public:
	Game();

private:
	void initialize();

	void listen_to_keys();

	void draw_rectangle(SDL_Surface*, SDL_Rect*, int, int, int);

	void draw_blank();

	void draw_boundaries();

	void draw_playground();

	void draw_menu();

	void draw_scoreboard();

	bool game_finished();

	void key_pressed( SDL_Event* event );

	void key_unpressed( SDL_Event* event );

	void quit();

	void fpsChanged(int);

public:
	void run();

	int get_window_height();
};

#endif /* GAME_H */

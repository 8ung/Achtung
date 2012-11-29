#include "Game.h"
#include "Playground.h"
#include "SDL.h"
#include <iostream>
#include <time.h>
#include <math.h>

using namespace std;

Game::Game()
{
	frameSkip = 0;
	running = 0;
	display = NULL;
	for ( int i=0; i<SDLK_LAST; ++i ) {
		this->keys[ i ] = 0 ;
	}
	//scoreboard = new Scoreboard();
	initialize();
	//skapa objekt av Menu och Scoreboard här också
}

void Game::initialize()
{

	int flags = SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_ANYFORMAT;//SDL_INIT_EVERYTHING;//  ;

	/* initialize SDL */
	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVERYTHING) ) {
		return ;
	}

	/* set video surface */
	this->display = SDL_SetVideoMode(0, 0, 0, flags | SDL_FULLSCREEN);
	if ( display == NULL )
	{
		return ;
	}
	window_height = this->display->h;

	/* Set caption */
	SDL_WM_SetCaption( "Achtung, die Kurve!", NULL );

	this->running = 1 ;

	playground = new Playground(window_height);
	Uint32 worm_colour = SDL_MapRGB(display->format, 255, 0, 0);
	Uint32 worm_colour2 = SDL_MapRGB(display->format, 0, 255, 0);
	Uint32 worm_colour3 = SDL_MapRGB(display->format, 0, 0, 255);
	//Uint32 worm_colour = 0x00000001;
	playground->initialize(worm_colour, 276,275);
	playground->initialize(worm_colour2, 49,50);
	playground->initialize(worm_colour3, 49,50);
	//playground->initialize(worm_colour, 276,275);

	hello = SDL_LoadBMP( "C:/Users/Oscar/Workspace/Projektet/src/achtung_header.bmp" );

	run();
}

void Game::listen_to_keys()
{
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:    quit();            break;
		case SDL_KEYDOWN: key_pressed( &event ); break;
		case SDL_KEYUP:   key_unpressed( &event );   break;
		}
	}
}

void Game::draw_rectangle( SDL_Surface* surface, SDL_Rect* rc, int r, int g, int b ) {
	SDL_FillRect( surface, rc, SDL_MapRGB(surface->format, r, g, b) );
}

void fill_circle(SDL_Surface *surface, double cx, double cy, double radius, Uint32 pixel)
{
	static const int BPP = 4;
	double r = (double)radius;
	for (double dy = 1; dy <= r; dy += 1.0)
	{
		double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
		int x = cx - dx;

		// Grab a pointer to the left-most pixel for each half of the circle
		Uint8 *target_pixel_a = (Uint8 *)surface->pixels + ((int)(cy + r - dy)) * surface->pitch + x * BPP;
		Uint8 *target_pixel_b = (Uint8 *)surface->pixels + ((int)(cy - r + dy)) * surface->pitch + x * BPP;

		for (; x <= cx + dx; x++)
		{
			*(Uint32 *)target_pixel_a = pixel;
			*(Uint32 *)target_pixel_b = pixel;
			target_pixel_a += BPP;
			target_pixel_b += BPP;
		}
	}
}

void Game::draw_blank()
{
	SDL_Rect blank;
	blank.x = playground->upper_left_corner->x_koord + 10;
	blank.y = playground->upper_left_corner->y_koord + 10;
	blank.w = window_height - 20;
	blank.h = window_height - 20;
	draw_rectangle(display, &blank, 0,0,0);
}

void Game::draw_boundaries()
{
	SDL_Rect boundary;
	boundary.x = playground->upper_left_corner->x_koord;
	boundary.y = playground->upper_left_corner->y_koord;
	boundary.w = window_height;
	boundary.h = window_height;
	draw_rectangle(display, &boundary, 255,255,255);
	draw_blank();
}


void Game::draw_playground()
{


	int survivor_vector_size = playground->survivor_vector.size();
	for(int worm_index = 0; worm_index < survivor_vector_size; worm_index++)
	{
		if(playground->survivor_vector[worm_index]->get_distance_to_hole() > 0)
		{
			/* Create visible worm */
			double worm_xpos = playground->survivor_vector[worm_index]->get_position()->x_koord;
			double worm_ypos = playground->survivor_vector[worm_index]->get_position()->y_koord;
			Uint32 worm_colour = playground->survivor_vector[worm_index]->get_colour();
			int worm_thickness = playground->survivor_vector[worm_index]->thickness;
			fill_circle( display, worm_xpos, worm_ypos, worm_thickness, worm_colour);
		}
	}
	SDL_Flip( display );
}

void Game::draw_menu() {
	throw "Not yet implemented";
}

void Game::draw_scoreboard() {
	//Apply image to screen
	SDL_Rect rect;
	rect.x = window_height + 30;
	rect.y = 0;
	rect.w = 30;
	rect.h = 30;

	//SDL_BlitSurface( hello, NULL, display, &rect );
	//Free the loaded image
	//SDL_FreeSurface( hello );
	int vector_size = playground->worm_vector.size();
	for(int worm_index = 0; worm_index < vector_size ; worm_index++)
	{
		rect.y = 200 + worm_index * 50;
		SDL_FillRect(display, &rect, playground->worm_vector[worm_index]->get_colour());
		//SDL_BlitSurface( hello, NULL, display, &rect );

			//Update Screen
		SDL_Flip( display );

	}
}

bool Game::game_finished() {
	throw "Not yet implemented";
}


void Game::key_pressed( SDL_Event* event )
{
	keys[ event->key.keysym.sym ] = 1;
}

void Game::key_unpressed( SDL_Event* event )
{
	keys[ event->key.keysym.sym ] = 0;
}

void Game::quit()
{
	SDL_FreeSurface( hello );
	running = 0;
}

void Game::fpsChanged( int fps ) {
	char szFps[ 128 ] ;
	//int jump = SDL_GetTicks();
	sprintf( szFps, "%s: %d FPS", "SDL Base C++ - Use Arrow Keys to Move", fps);
	SDL_WM_SetCaption( szFps, NULL );
}

void Game::run() {
	int past = SDL_GetTicks();
	int now = past, pastFps = past ;
	int fps = 0, framesSkipped = 0 ;
	srand(time(NULL));
	draw_boundaries();

	//draw_scoreboard();
	while ( running )
	{
		int timeElapsed = 0 ;
		listen_to_keys();
		++fps;
		int survivor_vector_size = playground->survivor_vector.size();
		if(keys[SDLK_ESCAPE])
		{
			quit();
		}
		else if(keys[SDLK_SPACE])
		{
			draw_blank();
			playground->start_new_round();
		}
		for(int i = 0; i < survivor_vector_size; i++)
		{
			if(keys[playground->survivor_vector[i]->get_left_control()] == 1)
			{
				playground->update(i,true,false);
			}
			else if(keys[playground->survivor_vector[i]->get_right_control()] == 1)
			{
				playground->update(i,false,true);
			}
			else
			{
				playground->update(i,false,false);
			}
		}
		playground->collision(display);

		/* Menu */

		/* update/draw */
		timeElapsed = (now=SDL_GetTicks()) - past ;
		if ( timeElapsed >= 1000/60  )
		{
			past = now ;
			//update();
			if ( framesSkipped++ >= frameSkip )
			{
				draw_playground();
				draw_scoreboard();
				++fps ;
				framesSkipped = 0 ;
			}
		}

		/* fps */
		if ( now - pastFps >= 1000 )
		{
			pastFps = now ;
			this->fpsChanged( fps );
			fps = 0 ;
		}
		/* sleep? */
		SDL_Delay( 1 );
	}
}

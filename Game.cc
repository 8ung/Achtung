#include "Game.h"
#include "Playground.h"
#include "SDL.h"
#include <iostream>
#include <time.h>
#include <math.h>
#include <sstream>
#include "SDL_keyboard.h"
#include "SDL_image.h"


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

	//Initialize SDL_ttf
	if( TTF_Init() == -1 )
	{
		return;
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
	menu = new Menu(window_height);

	start_menu = SDL_LoadBMP( "Meny.bmp" );
	menu_all = SDL_LoadBMP( "meny_alla_mot_alla.bmp" );
	menu_team = SDL_LoadBMP( "meny_lag.bmp" );
	font = TTF_OpenFont( "lazy.ttf", 28 );
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

void Game::draw_blank(int x, int y, int width, int height)
{
	SDL_Rect blank;
	blank.x = x;
	blank.y = y;
	blank.w = width;
	blank.h = height;
	draw_rectangle(display, &blank, 0,0,0);
}

void Game::draw_winner()
{
	SDL_Rect winner;
	winner.x = window_height + 200;
	winner.y = 200;
	winner.w = 40;
	winner.h = 40;
	draw_rectangle(display, &winner, 255,255,255);
}

void Game::draw_boundaries()
{
	SDL_Rect boundary;
	boundary.x = playground->upper_left_corner->x_koord;
	boundary.y = playground->upper_left_corner->y_koord;
	boundary.w = window_height;
	boundary.h = window_height;
	draw_rectangle(display, &boundary, 255,255,255);
	draw_blank(playground->upper_left_corner->x_koord + 10,
			playground->upper_left_corner->y_koord + 10,
			window_height - 20,
			window_height - 20);
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
	draw_blank(window_height, 0, 400, 400);
	menu_pos.x = window_height + 1;
	menu_pos.y = 0;
	if(in_start_menu)
	{
		SDL_BlitSurface( start_menu, NULL, display, &menu_pos );
		SDL_Flip( display );
	}
	else if(team_play)
	{
		SDL_BlitSurface( menu_team, NULL, display, &menu_pos );
		SDL_Flip( display );
	}
	else
	{
		SDL_BlitSurface( menu_all, NULL, display, &menu_pos );
		SDL_Flip( display );
	}

	int check_box_size = check_box.size();
	for(int check_box_index = 0; check_box_index < check_box_size; check_box_index++)
	{
		SDL_Rect draw_check_box;
		draw_check_box.x = check_box[check_box_index]->x_koord;
		draw_check_box.y = check_box[check_box_index]->y_koord;
		draw_check_box.w = 20;
		draw_check_box.h = 20;
		SDL_FillRect(display, &draw_check_box, SDL_MapRGB(display->format, 0, 255, 0));
		SDL_BlitSurface( NULL, NULL, display, &draw_check_box );
		SDL_Flip(display);
	}

	SDL_Rect pointer;
	pointer.x = menu->marker_position.x_koord;
	pointer.y = menu->marker_position.y_koord;
	pointer.w = 20;
	pointer.h = 20;

	SDL_FillRect(display, &pointer, SDL_MapRGB(display->format, 255, 255, 255));
	SDL_BlitSurface( NULL, NULL, display, &pointer );
	SDL_Flip(display);
}

void Game::draw_scoreboard() {
	//Apply image to screen
	SDL_Rect rect;
	rect.x = window_height + 30;
	rect.y = 0;
	rect.w = 30;
	rect.h = 30;

	draw_blank(window_height + 80, 100, 50, 400);

	SDL_Rect position;
	position.x = window_height + 80;
	position.y = menu->first_position.y_koord;

	int scoreboard_spacing = 50;

	int vector_size = playground->worm_vector.size();
	//SDL_FillRect(display, &test, playground->worm_vector[0]->get_colour());
	for(int worm_index = 0; worm_index < vector_size ; worm_index++)
	{
		rect.y = menu->first_position.y_koord + worm_index * scoreboard_spacing;
		SDL_FillRect(display, &rect, playground->worm_vector[worm_index]->get_colour());
		//SDL_BlitSurface( hello, NULL, display, &rect );

		std::stringstream strm;
		position.y = menu->first_position.y_koord + worm_index * scoreboard_spacing;
		strm << playground->worm_vector[worm_index]->get_score();
		message = TTF_RenderText_Solid( font, strm.str().c_str(), textColor );
		SDL_BlitSurface(message, NULL, display, &position);
	}

	//Update Screen
	SDL_Flip( display );
}

void Game::game_finished() {
	//draw_winner();
	//SDL_Delay(5000);
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
	//SDL_FreeSurface( hello );
	in_menu = 0;
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
	draw_menu();

	while(in_menu)
	{


		int timeElapsed = 0 ;
		++fps;


		/* update/draw */
		timeElapsed = (now=SDL_GetTicks()) - past ;
		if ( timeElapsed >= 1000/60  )
		{
			past = now ;
			if ( framesSkipped++ >= frameSkip )
			{
				//draw_menu();
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

		listen_to_keys();
		if(keys[SDLK_RETURN] && in_start_menu)
		{
			SDL_Delay(300);
			int position = menu->execute_start_menu();
			switch(position)
			{
			case 1:
				team_play = false;
				break;
			case 2:
				team_play = true;
				break;
			case 3:
				quit();
				break;
			}
			in_start_menu = false;
			draw_menu();
		}
		else if(keys[SDLK_RETURN] && !in_start_menu)
		{
			SDL_Delay(300);
			int worm_vector_size = playground->worm_vector.size();
			for(int worm_index = 0; worm_index < worm_vector_size; worm_index++)
			{
				if(playground->worm_vector[worm_index]->get_colour() ==
						menu->execute_select_worm_menu())
				{
					playground->survivor_vector.erase(playground->survivor_vector.begin() + worm_index);
					playground->worm_vector.erase(playground->worm_vector.begin() + worm_index);
				}
			}
			int check_box_size = check_box.size();
			int check_box_index = 0;
			bool did_erase = false;
			while(check_box_index < check_box_size-1)
			{
				if(check_box[check_box_index]->y_koord == menu->marker_position.y_koord)
				{
					check_box.erase(check_box.begin() + check_box_index);
					check_box.erase(check_box.begin() + check_box_index);
					did_erase = true;
				}
				check_box_index++;
				check_box_size = check_box.size();
			}
			draw_menu();
			if(!did_erase)
			{
				int left_controler = 0;
				int right_controler = 0;
				while(left_controler == 0)
				{
					listen_to_keys();
					for ( int i=0; i<SDLK_LAST; ++i )
					{
						if(keys[i] != 0 && i != SDLK_SPACE
								&& i != SDLK_ESCAPE)
						{
							left_controler = i ;
						}
					}
				}
				check_box.push_back(new Position_class(window_height + 96,
						menu->marker_position.y_koord));
				draw_menu();

				while(right_controler == 0)
				{
					listen_to_keys();
					for ( int i=0; i<SDLK_LAST; ++i )
					{
						if(keys[i] != 0 && i != left_controler
								&& i != SDLK_SPACE && i != SDLK_ESCAPE)
						{
							right_controler = i ;
						}
					}
				}
				check_box.push_back(new Position_class(window_height + 188,
						menu->marker_position.y_koord));
				draw_menu();

				playground->initialize(menu->execute_select_worm_menu(),
						left_controler,right_controler);
			}
		}
		else if(keys[SDLK_UP])
		{
			menu->move_up();
			draw_menu();
			SDL_Delay(300);
		}
		else if(keys[SDLK_DOWN])
		{
			menu->move_down();
			draw_menu();
			SDL_Delay(300);
		}
		else if(keys[SDLK_SPACE] && playground->worm_vector.size() >= 2)
		{
			bool hot = false;
			bool cold = false;
			if(team_play)
			{
				int worm_vector_size = playground->worm_vector.size();
				for(int worm_vector_index = 0; worm_vector_index < worm_vector_size; worm_vector_index++)
				{
					if(playground->worm_vector[worm_vector_index]->team == "cold")
					{
						cold = true;
					}
					else
					{
						hot = true;
					}
				}
				if(cold && hot)
				{
					in_menu = false;
					running = true;
					SDL_Delay(300);
					draw_blank(window_height + 1, 100, 400, window_height);
				}
			}
			else
			{
				in_menu = false;
				running = true;
				SDL_Delay(300);
				draw_blank(window_height + 1, 100, 400, window_height);
			}

		}


		/* sleep? */
		SDL_Delay( 1 );
	}

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
			SDL_Delay(300);
			if(!playground->round_finished && !game_paused)
			{
				game_paused = true;
			}
			else if (playground->round_finished)
			{
				draw_boundaries();
				playground->start_new_round();
				game_paused = false;
			}
			else
			{
				game_paused = false;
			}
		}
		if(!game_paused)
		{
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
			playground->collision(display, team_play);

			/*draw_blank(50,50,50,20);
			SDL_Rect posx;
			posx.x = 50;
			posx.y = 50;
			std::stringstream strm;
			strm << playground->test_variable2;
			message = TTF_RenderText_Solid( font, strm.str().c_str(), textColor );
			SDL_BlitSurface(message, NULL, display, &posx);
			SDL_Flip(display);

			draw_blank(50,100,50,20);
			SDL_Rect posy;
			posy.x = 50;
			posy.y = 100;
			std::stringstream strm2;
			strm2 << playground->test_variable;
			message = TTF_RenderText_Solid( font, strm2.str().c_str(), textColor );
			SDL_BlitSurface(message, NULL, display, &posy);
			SDL_Flip(display);*/

			if(playground->round_finished)
			{
				if(playground->game_finished(team_play))
				{
					//game_finished();
					in_menu = true;
					in_start_menu = true;
					team_play = false;
					playground->reset();
					menu->reset_menu();
					int check_box_size = check_box.size();
					while(check_box_size != 0)
					{
						check_box.erase(check_box.begin());
						check_box_size = check_box.size();
					}

					SDL_Rect winner_colour;
					winner_colour.x = window_height/2 - 40;
					winner_colour.y = window_height/2 - 40;
					winner_colour.w = 80;
					winner_colour.h = 80;

					int first = playground->worm_vector.size();
					SDL_FillRect(display, &winner_colour,
							playground->survivor_vector[first]->get_colour());
					SDL_BlitSurface(NULL, NULL, display, &winner_colour);
					SDL_Flip(display);

					SDL_Rect winner_text;
					winner_text.x = window_height/2 - 40;
					winner_text.y = window_height/2 - 60;
					std::stringstream strm;
					strm << "Vinnaren är :";
					message = TTF_RenderText_Solid( font, strm.str().c_str(), textColor );
					SDL_BlitSurface(message, NULL, display, &winner_text);
					SDL_Flip(display);

					SDL_Delay(2000);
					running = false;
					run();
				}
				game_paused = true;
			}
		}

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
				if(playground->powerup_to_draw != NULL)
				{
					fill_circle(display, playground->powerup_to_draw->position.x_koord,
							playground->powerup_to_draw->position.y_koord,
							playground->powerup_to_draw->radius,
							playground->powerup_to_draw->colour_id);
					SDL_Rect new_pos;
					new_pos.x = playground->powerup_to_draw->position.x_koord -12.5;
					new_pos.y = playground->powerup_to_draw->position.y_koord -12.5;
					SDL_BlitSurface( playground->powerup_to_draw->image, NULL, display, &new_pos);
					SDL_Flip( display );
					playground->powerup_to_draw = NULL;
				}
				if(playground->powerup_to_erase != NULL)
				{
					fill_circle(display, playground->powerup_to_erase->position.x_koord,
							playground->powerup_to_erase->position.y_koord,
							playground->powerup_to_erase->radius,
							SDL_MapRGB(display->format, 0, 0, 0));
					//delete playground->powerup_to_erase;
					playground->powerup_to_erase = NULL;
				}
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

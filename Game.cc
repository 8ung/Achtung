#include "Game.h"
#include "Playground.h"
#include "SDL.h"
#include <iostream>
#include <time.h>
#include <math.h>
#include <sstream>
#include "SDL_keyboard.h"

using namespace std;

Game::Game()
{
	frameSkip = 0;
	display = NULL;
	// Nollställer listan med nedtryckta tangenter
	for(int i = 0; i < SDLK_LAST; ++i)
	{
		this->keys[i] = 0;
	}
	initialize();
}

void Game::initialize()
{
	int flags = SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_ANYFORMAT;
	// Initiera SDL
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVERYTHING))
	{
		return;
	}
	// Initiera SDL_ttf
	if(TTF_Init() == -1)
	{
		return;
	}
	// Sätter video i fullskärm
	this->display = SDL_SetVideoMode(0, 0, 0, flags | SDL_FULLSCREEN);
	if(display == NULL)
	{
		return;
	}
	window_height = this->display->h;
	// Sätter fönstertext
	SDL_WM_SetCaption( "Achtung, die Kurve!", NULL );

	playground = new Playground(window_height);
	menu = new Menu(window_height);

	// Ladda upp bilderna samt typsnittet som kommer att användas
	start_menu = SDL_LoadBMP("Meny.bmp");
	menu_all = SDL_LoadBMP("meny_alla_mot_alla.bmp");
	menu_team = SDL_LoadBMP("meny_lag.bmp");
	rules = SDL_LoadBMP("Instruction_pic2.bmp");
	font = TTF_OpenFont("lazy.ttf", 28);
}

/*
 * Tar in tangenttryckningar
 */
void Game::listen_to_keys()
{
	if(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			quit();
			break;
		case SDL_KEYDOWN:
			key_pressed(&event);
			break;
		case SDL_KEYUP:
			key_unpressed(&event);
			break;
		}
	}
}

/*
 * Sätter 1:a på tangenter som är nedtryckta
 */
void Game::key_pressed(const SDL_Event* event)
{
	keys[event->key.keysym.sym] = 1;
}

/*
 * Sätter 0:a på tangenter som inte är nedtryckta
 */
void Game::key_unpressed(const SDL_Event* event)
{
	keys[event->key.keysym.sym] = 0;
}


void Game::fps_changed(const int fps)
{
	char szFps[128];
	sprintf( szFps, "%s: %d FPS", "Achtung, die Kurve ! ", fps);
	SDL_WM_SetCaption(szFps, NULL);
}

/*
 * Ritar ut menyn och spelreglerna. Vid knapptryckningar uppdateras skärmen.
 */
void Game::draw_menu()
{
	// Ritar ut en svart fyrkant 400 x 400 där menyn är (rensar)
	draw_blank(window_height, 0, 400, 400);
	menu_pos.x = window_height + 1;
	menu_pos.y = 0;

	if(in_start_menu)
	{
		SDL_BlitSurface(start_menu, NULL, display, &menu_pos);
		SDL_Flip(display);
	}
	else if(team_play)
	{
		SDL_BlitSurface(menu_team, NULL, display, &menu_pos);
		SDL_Flip(display);
	}
	else
	{
		SDL_BlitSurface(menu_all, NULL, display, &menu_pos);
		SDL_Flip(display);
	}

	// Visar bilden för spelregler etc. på position (50,50)
	game_rules.x = 50;
	game_rules.y = 50;
	SDL_BlitSurface(rules, NULL, display, &game_rules);
	SDL_Flip(display);

	int check_box_size = check_box.size();
	for(int check_box_index = 0; check_box_index < check_box_size; check_box_index++)
	{
		// Ritar ut en grön fyrkant för valda tangenter
		SDL_Rect draw_check_box;
		draw_check_box.x = check_box[check_box_index]->x_koord;
		draw_check_box.y = check_box[check_box_index]->y_koord;
		draw_check_box.w = 20;
		draw_check_box.h = 20;
		SDL_FillRect(display, &draw_check_box, SDL_MapRGB(display->format, 0, 255, 0));
		SDL_BlitSurface( NULL, NULL, display, &draw_check_box );
		SDL_Flip(display);
	}
	// Ritar ut markören i menyn på rätt position vilken fås av menu
	SDL_Rect pointer;
	pointer.x = menu->marker_position.x_koord;
	pointer.y = menu->marker_position.y_koord;
	pointer.w = 20;
	pointer.h = 20;

	SDL_FillRect(display, &pointer, SDL_MapRGB(display->format, 255, 255, 255));
	SDL_BlitSurface( NULL, NULL, display, &pointer );
	SDL_Flip(display);
}

/*
 * Ritar ut en svart fyrkant på koordinaterna x och y med given bredd och höjd. Används för att sudda
 */
void Game::draw_blank(const int x, const int y, const int width, const int height)
{
	SDL_Rect blank;
	blank.x = x;
	blank.y = y;
	blank.w = width;
	blank.h = height;
	SDL_FillRect(display, &blank, SDL_MapRGB(display->format, 0, 0, 0));
}

/*
 * Ritar ut en vit ram som representerar spelplanens väggar
 */
void Game::draw_boundaries()
{
	SDL_Rect boundary;
	boundary.x = 0;
	boundary.y = 0;
	boundary.w = window_height;
	boundary.h = window_height;
	SDL_FillRect(display, &boundary, SDL_MapRGB(display->format, 255, 255, 255));
	draw_blank(10, 10, window_height - 20, window_height - 20);
}

/*
 * fill circle ritar en cirkel. Används för powerup och för maskens huvud.
 */
void Game::fill_circle(const SDL_Surface* surface, const double cx, const double cy,
		double radius, Uint32 pixel)
{
	static const int BPP = 4;
	double r = (double)radius;
	for (double dy = 1; dy <= r; dy += 1.0)
	{
		double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
		int x = cx - dx;

		Uint8* target_pixel_a = (Uint8*)surface->pixels +
				((int)(cy + r - dy)) * surface->pitch + x * BPP;
		Uint8* target_pixel_b = (Uint8*)surface->pixels +
				((int)(cy - r + dy)) * surface->pitch + x * BPP;

		for (; x <= cx + dx; x++)
		{
			*(Uint32*)target_pixel_a = pixel;
			*(Uint32*)target_pixel_b = pixel;
			target_pixel_a += BPP;
			target_pixel_b += BPP;
		}
	}
}

/*
 * draw playground ritar ut maskarna på deras nya positioner
 */
void Game::draw_playground()
{
	int survivor_vector_size = playground->survivor_vector.size();
	for(int worm_index = 0; worm_index < survivor_vector_size; worm_index++)
	{
		if(playground->survivor_vector[worm_index]->get_distance_to_hole() > 0)
		{
			// Visar maskarna grafiskt
			double worm_xpos = playground->survivor_vector[worm_index]->position->x_koord;
			double worm_ypos = playground->survivor_vector[worm_index]->position->y_koord;
			Uint32 worm_colour = playground->survivor_vector[worm_index]->get_colour();
			int worm_thickness = playground->survivor_vector[worm_index]->thickness;
			fill_circle(display, worm_xpos, worm_ypos, worm_thickness, worm_colour);
		}
	}
	SDL_Flip(display);
}

/*
 * draw scoreboard uppdaterar och ritar ut poängtavlan.
 */
void Game::draw_scoreboard()
{
	// Rensar resultattavlan
	draw_blank(window_height + 80, 100, 50, 400);

	// Ritar ut färgrutor i resultattavlan
	colour_box.x = window_height + 30;
	colour_box.y = 0;
	colour_box.w = 30;
	colour_box.h = 30;

	SDL_Rect position;
	position.x = window_height + 80;
	position.y = menu->first_position.y_koord;
	int scoreboard_spacing = 50;

	int vector_size = playground->worm_vector.size();
	for(int worm_index = 0; worm_index < vector_size ; worm_index++)
	{
		colour_box.y = menu->first_position.y_koord + worm_index * scoreboard_spacing;
		SDL_FillRect(display, &colour_box, playground->worm_vector[worm_index]->get_colour());

		// Utskrift av poäng för respektive mask
		std::stringstream strm;
		position.y = menu->first_position.y_koord + worm_index * scoreboard_spacing;
		strm << playground->worm_vector[worm_index]->get_score();
		text = TTF_RenderText_Solid( font, strm.str().c_str(), textColor );
		SDL_BlitSurface(text, NULL, display, &position);
	}
	SDL_Flip( display );
}

/*
 * Återställer alla variabler efter avslutat spel.
 */
void Game::game_finished()
{
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

	// Ritar ut färgen på vinnaren av spelet
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

	// Skriver ut "Vinnaren är "
	SDL_Rect winner_text;
	winner_text.x = window_height/2 - 40;
	winner_text.y = window_height/2 - 60;
	std::stringstream strm;
	strm << "Vinnaren är :";
	text = TTF_RenderText_Solid(font, strm.str().c_str(), textColor);
	SDL_BlitSurface(text, NULL, display, &winner_text);
	SDL_Flip(display);

	SDL_Delay(5000);
	in_game = false;
	run();
}

/*
 * När användaren startar spelet så kommer man till en meny. Menyn startas med menu_loop.
 */
void Game::menu_loop(int& past, int& now, int& pastFps, int& fps, int& framesSkipped)
{
	int timeElapsed = 0;
	++fps;


	// Uppdatera fönstret
	timeElapsed = (now = SDL_GetTicks()) - past;
	if (timeElapsed >= 1000 / 60)
	{
		past = now;
		if (framesSkipped++ >= frameSkip)
		{
			++fps;
			framesSkipped = 0;
		}
	}
	if (now - pastFps >= 1000)
	{
		pastFps = now;
		this->fps_changed(fps);
		fps = 0;
	}
	// Tar in knapptryckningar
	listen_to_keys();
	if(keys[SDLK_RETURN] && in_start_menu) //Om enter trycks in i första menyfönstret
	{
		SDL_Delay(300); // Delay för att inte utföra knapptryck oönskat många gånger
		int position = menu->execute_start_menu(); // Variablen bestämmer vilket läge markören står på i menyn vid enterslag.
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
	// I denna meny väljs färg, kontroller och antal spelare
	else if(keys[SDLK_RETURN] && !in_start_menu) //Om enter trycks in i andra menyfönstret
	{
		SDL_Delay(300);	// Delay för att inte utföra knapptryck oönskat många gånger
		int worm_vector_size = playground->worm_vector.size();
		for(int worm_index = 0; worm_index < worm_vector_size; worm_index++)
		{
			if(playground->worm_vector[worm_index]->get_colour() ==
					menu->execute_select_worm_menu()) // Om en tidigare vald mask markeras igen med enter tas den bort.
			{
				playground->survivor_vector.erase(playground->survivor_vector.begin() + worm_index);
				playground->worm_vector.erase(playground->worm_vector.begin() + worm_index);
			}
		}
		int check_box_size = check_box.size(); // antal tidigare valda kontroller
		int check_box_index = 0;
		bool did_erase = false;
		while(check_box_index < check_box_size - 1) // Om kvadraterna redan är ifyllda så töms de.
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
		//Om det är så att vi valt en tidigare ej vald mask så sätts här vänster- och högerkontroll till masken.
		if(!did_erase)
		{
			int left_controler = 0;
			int right_controler = 0;
			while(left_controler == 0)
			{
				listen_to_keys();
				for (int i = 0; i < SDLK_LAST; ++i)
				{
					if((keys[i] != 0) && (i != SDLK_SPACE) && (i != SDLK_ESCAPE))
					{
						left_controler = i;
					}
				}
			}
			// 96 nedan för passa rutan i meny-bilden
			check_box.push_back(new Position_class(window_height + 96,
					menu->marker_position.y_koord));
			draw_menu();

			while(right_controler == 0)
			{
				listen_to_keys();
				for (int i = 0; i < SDLK_LAST; ++i)
				{
					if((keys[i] != 0) && (i != left_controler) &&
							(i != SDLK_SPACE) && (i != SDLK_ESCAPE))
					{
						right_controler = i;
					}
				}
			}
			// 188 nedan för att passa rutan i meny-bilden
			check_box.push_back(new Position_class(window_height + 188,
					menu->marker_position.y_koord));
			draw_menu();

			playground->initialize(menu->execute_select_worm_menu(),
					left_controler,right_controler);
		}
	}
	else if(keys[SDLK_UP]) // Om pil upp tryckts
	{
		menu->move_up(); // Flytta markörens läge
		draw_menu();
		SDL_Delay(300); // Delay för att inte utföra knapptryck oönskat många gånger
	}
	else if(keys[SDLK_DOWN]) // Om pil ned tryckts
	{
		menu->move_down(); // Flytta markörens läge
		draw_menu();
		SDL_Delay(300); // Delay för att inte utföra knapptryck oönskat många gånger
	}
	else if(keys[SDLK_ESCAPE] && !in_start_menu) // om escape tryckts. Gå tillbaka till första menyn. Ta bort ev. skapade maskar.
	{
		SDL_Delay(300); // Delay för att inte utföra knapptryck oönskat många gånger
		playground->reset();
		int check_box_size = check_box.size();
		int check_box_index = 0;
		while(check_box_index < check_box_size)
		{
			check_box.erase(check_box.begin());
			check_box_index++;
		}
		in_start_menu = true;
		menu->reset_menu();
		draw_menu();
	}
	else if(keys[SDLK_SPACE] && (playground->worm_vector.size() >= 2)) // Om mellanslag tryckts. Starta spelet om minst två maskar valts
	{
		SDL_Delay(300); // Delay för att inte utföra knapptryck oönskat många gånger
		bool hot = false;
		bool cold = false;
		if(team_play) // Om lagspel måste det finnas någon mask i varje lag
		{
			int worm_vector_size = playground->worm_vector.size();
			for(int worm_vector_index = 0;
					worm_vector_index < worm_vector_size;
					worm_vector_index++)
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
				in_game = true;
				draw_blank(window_height + 1, 100, 400, window_height);
			}
		}
		else
		{
			in_menu = false;
			in_game = true;
			draw_blank(window_height + 1, 100, 400, window_height);
		}
		draw_blank(10, 10, window_height - 20, window_height - 20);
	}
}

/*
 *Själva spelloopen. Anropas efter menyn.
 */
void Game::game_loop(int& past, int& now, int& pastFps, int& fps, int& framesSkipped)
{
	int timeElapsed = 0;
	listen_to_keys();
	++fps;
	int survivor_vector_size = playground->survivor_vector.size();
	if(keys[SDLK_ESCAPE]) // Programmet avslutas om escape trycks
	{
		quit();
	}
	else if(keys[SDLK_SPACE]) // Spelet pausas och återupptas om mellanslag trycks
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
				playground->update(i, true, false); // Uppdatera mask i. Den svänger vänster.
			}
			else if(keys[playground->survivor_vector[i]->get_right_control()] == 1)
			{
				playground->update(i, false, true);// Uppdatera mask i. Den svänger höger.
			}
			else
			{
				playground->update(i, false, false); // Uppdatera mask i. Den svänger inte.
			}
		}
		playground->collision(display, team_play); //Kontrollerar om det skett en kollision och hanterar de fallen.

		if(playground->round_finished) //om omgången är slut
		{
			if(playground->game_finished(team_play)) // om spelet är slut
			{
				game_finished();
			}
			game_paused = true; //Spelet pausar till mellanslag trycks
		}
	}

	// Uppdatera fönster samt utföra ritfunktionerna.
	timeElapsed = (now = SDL_GetTicks()) - past;
	if (timeElapsed >= 1000 / 60)
	{
		past = now;
		if (framesSkipped++ >= frameSkip)
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
				SDL_Flip(display);
				playground->powerup_to_draw = NULL;
			}
			if(playground->powerup_to_erase != NULL)
			{
				fill_circle(display, playground->powerup_to_erase->position.x_koord,
						playground->powerup_to_erase->position.y_koord,
						playground->powerup_to_erase->radius,
						SDL_MapRGB(display->format, 0, 0, 0));
				playground->powerup_to_erase = NULL;
			}
			++fps;
			framesSkipped = 0;
		}
	}
	if (now - pastFps >= 1000)
	{
		pastFps = now;
		this->fps_changed(fps);
		fps = 0;
	}
}

/*
 * Avslutar programmloopen, stänger alla fönster
 */
void Game::quit()
{
	in_menu = 0;
	in_game = 0;
	SDL_FreeSurface(rules);
	SDL_FreeSurface(start_menu);
	SDL_FreeSurface(menu_all);
	SDL_FreeSurface(menu_team);
	SDL_FreeSurface(text);
	SDL_FreeSurface(display);
}
/*
 * Programmets startfunktion. Anropas från main för att starta programmet.
 */
void Game::run()
{
	int past = SDL_GetTicks();
	int now = past;
	int pastFps = past;
	int fps = 0;
	int framesSkipped = 0;
	srand(time(NULL));
	draw_boundaries();
	draw_menu();

	while(in_menu)
	{
		menu_loop(past, now, pastFps, fps, framesSkipped);
		SDL_Delay(1);
	}

	while (in_game)
	{
		game_loop(past, now, pastFps, fps, framesSkipped);
		SDL_Delay(1);
	}
}

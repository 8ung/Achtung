#include "Menu.h"
#include "SDL_stdinc.h"
#include <iostream>
#include "Position_class.h"
#include <string>


Menu::Menu(const int window_height)
{
	team_play = false;
	start_menu = true;
	position = 1;
	first_position = Position_class(window_height + 28, 112);
	marker_position = first_position;
	spacing = 56;
}

/*move_up anropas från Game då användaren trycker på uppknappen. Funktionen flyttar upp "marker" med "spacing" pixlar
 *  och flyttar upp den abstrakta positionen med ett steg.
 */
void Menu::move_up()
{
	if(start_menu)
	{
		if(1 < position && position < 4)
		{
			position = position - 1;
			marker_position.y_koord = marker_position.y_koord - spacing;
		}
		else if(position == 1)
		{
			position = 3;
			marker_position.y_koord = first_position.y_koord + 2*spacing;
		}
	}
	else
	{
		if(1 < position && position < 7)
		{
			position = position - 1;
			marker_position.y_koord = marker_position.y_koord - spacing;
		}
		else if(position == 1)
		{
			position = 6;
			marker_position.y_koord = first_position.y_koord + 5*spacing;
		}
	}
}

/*move_down anropas från Game då användaren trycker på nerknappen. Funktionen flyttar ner "marker" med "spacing" pixlar
 *  och flyttar ner den abstrakta positionen med ett steg.
*/
void Menu::move_down()
{
	if(start_menu)
	{
		if(0 < position && position < 3)
		{
			position = position + 1;
			marker_position.y_koord = marker_position.y_koord + spacing;
		}
		else if(position == 3)
		{
			position = 1;
			marker_position.y_koord = first_position.y_koord;
		}
	}
	else
	{
		if(0 < position && position < 6)
		{
			position = position + 1;
			marker_position.y_koord = marker_position.y_koord + spacing;
		}
		else if(position == 6)
		{
			position = 1;
			marker_position.y_koord = first_position.y_koord;
		}
	}
}

//execute_start_menu returnerar den abstrakta position som "marker" befinner som på. Denna anropas i filen Game.
const int Menu::execute_start_menu()
{
	start_menu = false;
	return position;
}

/*reset_menu anropas från game då ett spel är slut och man återkommer till startmenyn. Den återställer menu-parametrar
till de ursprungliga värdena.*/
void Menu::reset_menu()
{
	start_menu = true;
	team_play = false;
	position = 1;
	marker_position = first_position;
}

//execute_select_worm_menu returnerar den färg som "marker" markerar i det grafiska.//
const Uint32 Menu::execute_select_worm_menu()
{
	switch(position)
	{
	case 1:
		return red;
		break;
	case 2:
		return yellow;
		break;
	case 3:
		return orange;
		break;
	case 4:
		return green;
		break;
	case 5:
		return cyan;
		break;
	case 6:
		return blue;
		break;
	}
	return 0;
}

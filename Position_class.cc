#include "Position_class.h"
#include <stdlib.h>
#include <time.h>


using namespace std;

Position_class::Position_class(const int x, const int y)
{
	x_koord = x;
	y_koord = y;
}

Position_class::~Position_class()
{}

//random_position anropas då en mask eller powerup ska slumpas ut på spelplanen.
void Position_class::random_position(const int window_height)
{
	x_koord = rand() % (window_height - 2 * boundary_diff) + boundary_diff;
	y_koord = rand() % (window_height - 2 * boundary_diff) + boundary_diff;
}


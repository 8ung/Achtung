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
{

}

void Position_class::random_position(int window_height)
{
	//srand(time(NULL));
	x_koord = rand() % (window_height - 2 * boundary_diff) + boundary_diff;
	//srand(time(NULL));
	y_koord = rand() % (window_height - 2 * boundary_diff) + boundary_diff;
}


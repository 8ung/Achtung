#include "Powerup.h"
//#include "C:\Users\Dennis\C++\Achtung\src\src\Playground.h"
#include "SDL_stdinc.h"
#include "FasterMe.h"
/*#include "FasterYou.h"
#include "GhostMe.h"
#include "MirrorYou.h"
#include "SharpTurnMe.h"
#include "SharpTurnYou.h"
#include "SlowerMe.h"
#include "SlowerYou.h"
#include "ThickerYou.h"
#include "ThinnerMe.h"
#include "ThroughWallMe.h"*/

using namespace std;

Powerup::Powerup(Position_class new_position, Uint32 color)
{
	position = new_position;
	colour_id = color;
	radius = 30;
}

#ifndef POSITION_CLASS_H
#define POSITION_CLASS_H

class Position_class
{
private:
	enum
	{
		// So that the random-function do not generate values near the edge
		boundary_diff = 50
	};

public:
	double x_koord;
	double y_koord;

	Position_class(int x = 0, int y = 0);
	//~Position_class();

	void random_position(int window_height);
};

#endif /* POSITION_CLASS_H */

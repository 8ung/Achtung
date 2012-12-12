#ifndef POSITION_CLASS_H
#define POSITION_CLASS_H

class Position_class
{
private:
	enum
	{
		// Så att slumpfunktionen inte ger värden för nära kanten
		boundary_diff = 50
	};

public:
	double x_koord;
	double y_koord;

	Position_class(const int x = 0, const int y = 0);
	~Position_class();

	void random_position(const int window_height);
};

#endif /* POSITION_CLASS_H */

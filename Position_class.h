#ifndef POSITION_CLASS_H
#define POSITION_CLASS_H

class Position_class
{
private:
	enum
	{
		// S� att slumpfunktionen inte ger v�rden f�r n�ra kanten
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

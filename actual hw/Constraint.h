class Constraint
{
public:
	Constraint(int cord_x_, int cord_y_, char section_);

	int getX();
	int getY();
	char getSection();

	void setX();
	void setY();
	void setSection();

private:
	int cord_x;
	int cord_y;
	char section;

};
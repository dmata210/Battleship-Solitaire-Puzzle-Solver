class Ship
{
public:
	Ship(int size_, std::string ship_type_, std::vector<char> ship_struct_, bool is_vert_, bool unknown);

	int getSize() const;
	std::string getType() const;
	std::vector<char> get_ship_struct() const;
	bool get_is_vert() const;
	void change_vert();

	void setX(int temp);
	void setY(int temp);
	void setUnknown();

	int getX() const;
	int getY() const;

	int getMin() const;
	int getMax() const;

	bool isUnknown() const;
	bool operator<(Ship& temp)
	{	
		return this->size > temp.size ;
	}
	Ship& operator=(const Ship& temp);
	// i prob need to add a coord thing to the ship class

private:
	int min_size, max_size, size;
	std::string ship_type;
	std::vector<char> ship_struct;
	bool is_vert;
	bool is_unknown;
	int x;
	int y;
};
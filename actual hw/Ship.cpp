#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <algorithm> 

#include "Ship.h"
//ship class because its easier to hold and work with all this infomation in one class object
Ship::Ship(int size_, std::string ship_type_, std::vector<char> ship_struct_, bool is_vert_, bool unknown)
{
	size = size_;
	ship_type = ship_type_;
	ship_struct = ship_struct_;
	is_vert = is_vert_;
	x = -1;
	y = -1; // -1 means it aint been set yet
	is_unknown = unknown;
	if (is_unknown)
	{
		min_size = 1;
		max_size = 7;
	}
	else{
		min_size = max_size = size;
	}
}

int Ship::getSize() const {return size;};
std::string Ship::getType() const {return ship_type;};
std::vector<char> Ship::get_ship_struct() const {return ship_struct;};
bool Ship::get_is_vert() const {return is_vert;};
void Ship::change_vert()
{
	is_vert = !is_vert;
	if (!is_vert)// this is horizontal
	{
		if (ship_struct.size() > 1)
		{
			ship_struct[0] = '<';
			ship_struct[ship_struct.size()-1] = '>';
		}
	}
	if (is_vert)//this is vertical
	{
		if (ship_struct.size() > 1)
		{
			ship_struct[0] = '^';
			ship_struct[ship_struct.size()-1] = 'v';
		}
	}
};
void Ship::setX(int temp){x = temp;};
void Ship::setY(int temp){y = temp;};
void Ship::setUnknown(){
	is_unknown = true;
	min_size = 1;
	max_size = 7;
}
int Ship::getX() const {return x;};
int Ship::getY() const {return y;};
int Ship::getMin() const {return min_size;};
int Ship::getMax() const {return max_size;};
bool Ship::isUnknown() const {return is_unknown;};

Ship& Ship::operator=(const Ship& temp)
{
	size = temp.getSize();
	ship_type = temp.getType();
	ship_struct = temp.get_ship_struct();
	is_vert = temp.get_is_vert();
	is_unknown = temp.isUnknown();
	x = temp.getX();
	y = temp.getY();
	min_size = temp.getMin();
	max_size = temp.getMax();
	return *this;
};
// i prob need to add a coord thing to the ship class. I did, goodjob you

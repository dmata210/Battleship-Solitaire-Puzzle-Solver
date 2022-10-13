#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <algorithm> 

#include "Constraint.h"
#include "Ship.h"
#include "Grid.h"

void read_file(const std::string &fname, std::vector<char> &int_rows, std::vector<char> &int_cols, std::vector<std::string> &int_all_ships, std::vector<Constraint> &vec_of_con)
{
	//parses the file into data sets i can later use for what i need to
	//ship class, constraint class, grid class
	std::ifstream istr(fname);

	if(!istr) 
	{
		std::cerr << "ERROR: cannot open the text file " << fname << std::endl;
		exit(0);
	}
	std::string line;
	while(std::getline(istr,line))
	{

		if (line == "") continue;

		std::stringstream sstr;

		sstr << line;

		std::string temp2;
		sstr >> temp2;
		int row;
		int col;
		if (temp2 == "board")
		{
			sstr >> row >> col;
			continue;
		}
		if (temp2 == "rows")
		{
			for (int i = 0; i < row; ++i)
			{
				char num;
				sstr >> num;
				int_rows.push_back(num);
			}
			continue;
		}
		if (temp2 == "cols")
		{
			for (int i = 0; i < col; ++i)
			{
				char num;
				sstr >> num;
				int_cols.push_back(num);
			}
			continue;
		}
		if (temp2 == "constraint")
		{
			int x;
			int y;
			char section;
			sstr >> x >> y >> section;
			Constraint c(x,y,section);
			vec_of_con.push_back(c);
			continue;
		}
		else
		{
			int_all_ships.push_back(temp2);
		}

	}
}

std::vector<char> create_struct(int size)
{
	//creates structure for the ship
	std::vector<char> ship;
	if (size == 1)
	{
		ship.push_back('o');
		return ship;
	}
	ship.push_back('<');
	for (int i = 0; i < size-2; ++i)
	{
		ship.push_back('X');
	}
	ship.push_back('>');
	return ship;
}

Ship create_ship(std::string ship_name)
{
	//creates the ship by calling helper functions
	//this is also another helper function
	int size = 0;
	std::string ship_type = ship_name;
	std::vector<char> ship_struct;
	bool is_vert = false;
	bool is_unknown = false;

	if (ship_name == "submarine")
	{
		size = 1;
	}
	if(ship_name == "destroyer")
	{
		size = 2;
	}
	if (ship_name == "cruiser")
	{
		size = 3;
	}
	if (ship_name == "battleship")
	{
		size = 4;
	}
	if (ship_name == "carrier")
	{
		size = 5;
	}
	if (ship_name == "cargo")
	{
		size = 6;
	}
	if (ship_name == "tanker")
	{
		size = 7;
	}
	if (ship_name == "unknown")
	{
		size = 0;
		is_unknown = true;
	}

	ship_struct = create_struct(size);
	Ship s(size, ship_type, ship_struct, is_vert, is_unknown);
	return s;
}

void make_all_ships(const std::vector<std::string> &int_all_ships, std::vector<Ship> &actual_ships)
{
	//main make ship function
	for (unsigned int i = 0; i < int_all_ships.size(); ++i)
	{
		Ship s = create_ship(int_all_ships[i]);
		actual_ships.push_back(s);
	}
}
std::string get_ship(int size)
{
	if (size == 1)
	{
		return "submarine";
	}
	if (size == 2)
	{
		return "destroyer";
	}
	if (size == 3)
	{
		return "cruiser";
	}
	if (size == 4)
	{
		return "battleship";
	}
	if (size == 5)
	{
		return "carrier";
	}
	if (size == 6)
	{
		return "cargo";
	}
	if (size == 7)
	{
		return "tanker";
	}
}

void The_Recursion(int index, std::vector<Ship> &vec_of_ships,
 Grid &grid, std::vector<Grid> &solutions,bool just_one)
{
	if (solutions.size() > 0 && just_one)
	{
		return;
		//here its done and only looking for one soultion
	}

	if (index >= vec_of_ships.size())
	{
		if (grid.final_check())
		{
			solutions.push_back(grid);
			//here we find a vaild soultion and push it back
		}
	}
	else
	{
		for (int i = vec_of_ships[index].getMax(); i >= vec_of_ships[index].getMin(); --i)
		{
			//here is for the unknowns, basically i go through every every possible combo of 
			//unknown ships, starting with the largest possible combo
			// ex. 3 tankers at first and it fails. Then it becomes 2 tankers and 1 cargo, that fails,
			//then 1 tanker, 2 cargo, till it fits
			if(vec_of_ships[index].isUnknown()){
				std::string ship_name = get_ship(i);
				Ship curr_ship = create_ship(ship_name);
				curr_ship.setUnknown();
				vec_of_ships[index] = curr_ship;
			}
			int row_index = 0;
			int col_index = 0;
			//here if you already go through a grid with a ship of the same size
			//then you already know you dont need to go through it again
			if (index > 0 && vec_of_ships[index - 1].getSize() == vec_of_ships[index].getSize())
			{
				row_index = vec_of_ships[index - 1].getX();
				col_index = vec_of_ships[index - 1].getY();
			}
			for (int r = row_index; r <grid.get_row() ; ++r)
			{
				for (int c = col_index; c < grid.get_col(); ++c)
				{
					//checks to see if its a vaild place
					if (grid.is_valid(vec_of_ships[index],r,c))
					{
						
						grid.add_ship(vec_of_ships[index],r,c);
						
						The_Recursion(index + 1, vec_of_ships, grid, solutions, just_one);
						//i recursivley call the recursive function, moving by one
						grid.delete_ship(vec_of_ships[index]);
						
					}
					//changes the orientation and then does the same as above
					if (vec_of_ships[index].getSize() > 1)
					{
						vec_of_ships[index].change_vert();
						if (grid.is_valid(vec_of_ships[index],r,c))
						{
							grid.add_ship(vec_of_ships[index],r,c);
							The_Recursion(index + 1, vec_of_ships, grid, solutions, just_one);
							grid.delete_ship(vec_of_ships[index]);
						}
					}
					//ensures it can start from the beginning
					if (col_index != 0)
					{
						col_index = 0;
					}
				}
			}
		}	
	}
}

int main(int argc, char *argv[])
{
	//file parsing stuff
	std::vector<char> int_rows;
	std::vector<char> int_cols; 
	std::vector<std::string> int_all_ships;
	std::vector<Ship> actual_ships;
	std::vector<Ship> print_ships;
	std::vector<Constraint> vec_of_con;
	//grid stuff
	std::vector<std::vector<char>> int_grid;
	std::vector<int> max_in_row;
	std::vector<int> max_in_col;
	std::vector<Grid> solutions;

	if (argc == 3 || argc == 2)
	{	
		bool only_one = true;
		if (argc == 3)
		{
			only_one = false;
		}
		read_file(argv[1], int_rows, int_cols, int_all_ships, vec_of_con);

		Grid temp(int_rows, int_cols, vec_of_con);

		make_all_ships(int_all_ships, actual_ships);

		sort(actual_ships.begin(), actual_ships.end());
		//start off with checking the longest ships, so order it that way

		The_Recursion(0, actual_ships, temp, solutions, only_one);

		if (solutions.size() > 0)
		{
			for (int i = 0; i < solutions.size(); ++i)
			{
				std::cout << "Solution:" << std::endl;
				solutions[i].print_grid();

			}

			std::cout <<"Found " << solutions.size() << " solution(s)." << std::endl;
		}
		else
		{
			std::cout << "No solutions." << std::endl;
		}

	}
	else
	{
		std::cerr << "ERROR: INVALID INPUT " << std::endl;
		std::cerr << "Program Terminated";
		//not the right input, checked by the amount of arguments
		exit(1);		
	}

	return 0;
}
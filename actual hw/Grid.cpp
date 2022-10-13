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
#include "Constraint.h"
#include "Grid.h"


Grid::Grid(const std::vector<char> &int_rows_, const std::vector<char> &int_cols_, const std::vector<Constraint> &vec_of_con_)
{
	int_rows = int_rows_;
	int_cols = int_cols_;
	row_size = int_rows.size();
	col_size = int_cols.size();

	vec_of_con = vec_of_con_;
	make_grid();

	for (int i = 0; i < row_size; ++i)
	{
		curt_in_row.push_back(0);
	}
	for (int i = 0; i < col_size; ++i)
	{
		curt_in_col.push_back(0);
	}
}
int Grid::get_col()
{
	return col_size;
}
int Grid::get_row()
{
	return row_size;
}

//youre going to call is vaild for each ship in the grid, which will bring in the board, and that ship
bool Grid::is_valid(Ship &ship, int row, int col)
{
	ship.setX(row);
	ship.setY(col);
	if (row < row_size && col < col_size && row >= 0 && col >= 0 && no_ships_close(ship))
	{
		if (ship.get_is_vert() && row + ship.getSize() -1 >= row_size)
		{
			ship.setX(-1);
			ship.setY(-1);
			return false;
		}
		if (!ship.get_is_vert() && col + ship.getSize() -1 >= col_size)
		{
			ship.setX(-1);
			ship.setY(-1);
			return false;
		}
		if (is_taken(ship, row, col))
		{
			ship.setX(-1);
			ship.setY(-1);
			return false;
		}

		//so the whole board is made, but we dont know if the ship is right
		//you check both the row and col and the constraints
		if (row_check(ship.getX(), ship.get_is_vert(), ship.getSize()) && col_check(ship.getY(), ship.get_is_vert(), ship.getSize()))
		 {
		 	//now youre doing the check to see if there is any ships in adjecent or diagonal cells
		 	return true;
		 } 
	}
	ship.setX(-1);
	ship.setY(-1);
	return false;
}
bool Grid::is_taken(Ship &ship, int x, int y)
{
	//checks where the ship will be at
	if (ship.get_is_vert())//vert
	{
		for (int i = x ; i < x + ship.getSize(); ++i)
		{
			if (grid[i][y]!= ' ')
			{
				return true;
			}
		}
		
	}
	if (!ship.get_is_vert())//horizontal 
	{
		for (int i = y; i < y + ship.getSize(); ++i)
		{
			if (grid[x][i] != ' ')
			{
				return true;
			}
		}
	}
	return false;
}
bool Grid::no_ships_close(Ship &ship)
{
	//ship will have x and y, this is how I will tell where its at. Now I will check all around it
	//checks boarder around ship to ensure theres no ships close by
	int row_x = ship.getX();
	int col_y = ship.getY();

	if (ship.get_is_vert()) //vertical 
	{
		for (int i = col_y - 1; i < col_y + 2; ++i)
		{
			if (i < 0 || i >= col_size)
			{
				//within the bounds, like the actual ship is right here
				continue;
			}
			if (i == col_y)
			{
				if (row_x - 1 >= 0 && row_x - 1 < row_size)
				{
				 	if (grid[row_x - 1][i] != ' ')
				 	{
				 		return false;
				 	}
				}
				if (row_x + ship.getSize() >= 0 && row_x + ship.getSize() < row_size )
				{
					if (grid[row_x + ship.getSize()][i] != ' ')
					{
						return false;
					}
				} 
			}
			else
			{
				for (int k = row_x - 1 ; k <= row_x + ship.getSize(); ++k)
				{
					if (k < 0 || k >= row_size)
					{
						continue;
					}
					if (grid[k][i] != ' ' )
					{
						return false;
					}
				}

			}			
		}
	}
	if (!ship.get_is_vert())// horizontals
	{
		for (int i = row_x - 1; i < row_x + 2; ++i)
		{
			if (i < 0 || i >= row_size)
			{
				continue;
			}
			if (i == row_x)
			{
				if (col_y - 1 >= 0 && col_y - 1 < col_size)
				{
					if (grid[i][col_y-1] != ' ')
					{
						return false;
					}
				}
				if (col_y + ship.getSize() >= 0 && col_y + ship.getSize() < col_size)
				{

					if (grid[i][col_y + ship.getSize()] != ' ')
					{
						return false;
					}
				}
			}
			else
			{
				for (int k = col_y - 1 ; k <= col_y + ship.getSize(); ++k)
				{
					if (k < 0 || k >= col_size)
					{
						continue;
					}
					if (grid[i][k] != ' ')
					{
						return false;
					}
				}

			}			
			
		}
	}
	return true;

}
bool Grid::row_check(int index, bool is_vert, int size)
{
	//index is where on the gird, and they should be in the same place for both vectors
	// if its vertical then check all rows touching the ship, if its horizontal then it
	//just check that row
	if (is_vert) //vertical
		{
			for (int i = index; i < size + index ; ++i)
			{
				int current = curt_in_row[i] + 1;
				if (int_rows[i] == '?')
				{
					continue;
				}
				int compare = int(int_rows[i]) - 48;

				//char representaion of a number, - 48 is the actual number
				if (compare < current )
				{
					// here you know it fits within the max num of things in that row, so youre good
					return false;
				}
			}
		}
	if (!is_vert) //horizontal
	{
		int current = curt_in_row[index] + size;
		if (int_rows[index] == '?')
		{
			return true;
		}
		int compare = int(int_rows[index]) - 48;
		if (compare < current )
		{
			return false;
		}
	}
	return true;	
}
bool Grid::col_check(int index, bool is_vert, int size)
{
	if (is_vert)//vertical
	{
		int current = curt_in_col[index] + size;
		if (int_cols[index] == '?')
		{
			return true;
		}
		int compare = int(int_cols[index]) - 48;
		//char representaion of a number, - 48 is the actual number
		if (compare < current)
		{
			// here you know it fits within the max num of things in that col, so youre good
			return false;
		}
	}
	if (!is_vert)//horizontal
	{
		for (int i = index; i < size + index; ++i)
		{
			int current = curt_in_col[i] + 1;
			if (int_cols[i] == '?')
			{
				continue;
			}
			int compare = int(int_cols[i]) - 48;
			//char representaion of a number, - 48 is the actual number
			if (compare < current)
			{
				return false;
			}
		}
	}
	return true;
}
bool Grid::add_ship(Ship &ship, int row_x, int col_y)
{
	//adds ship to board at the x and y, it assumes that its right due to the is vaild function
	if (ship.get_is_vert())//if its vertical, print put it on the board one way
	{	
		int temp = 0;
		curt_in_col[col_y] += ship.getSize();//keeps track of current blocks taken in that colum
		for (int i = row_x; i < row_x + ship.getSize(); ++i)
		{
			grid[i][col_y] = ship.get_ship_struct()[temp];
			curt_in_row[i] += 1;
			temp ++;
		}
		ship.setX(row_x);
		ship.setY(col_y);
		ships.push_back(ship);
		return true;
	}
	if (!ship.get_is_vert())// if its horizontal
	{
		int temp = 0;
		curt_in_row[row_x] += ship.getSize();//keeps track of current blocks taken in that row
		for (int i = col_y; i < col_y + ship.getSize(); ++i)
		{
			grid[row_x][i] = ship.get_ship_struct()[temp];
			curt_in_col[i] += 1;
			temp ++;
		}
		ship.setX(row_x);
		ship.setY(col_y);
		ships.push_back(ship);		
		return true;
	}
	return false;
}
bool Grid::delete_ship(Ship &ship)//just like add ship but we already know the x and y as its part of the ship
{
	int row_x = ship.getX();
	int col_y = ship.getY();

	for (unsigned int i = 0; i < ships.size(); ++i)
	{
		bool name = (ships[i].getType() == ship.getType());
		bool r = (ships[i].getX() == ship.getX());
		bool c = (ships[i].getY() == ship.getY());
		if (name && r && c)
		{
			ships.erase(ships.begin()+i);
			break;
		}
	}
	//above deletes the index in the vector

	if (ship.get_is_vert())//vertical
	{	
		int temp = 0;
		curt_in_col[col_y] -= ship.getSize();//keeps track of current blocks taken in that colum, takes it down
		for (int i = row_x; i < row_x + ship.getSize(); ++i)
		{
			grid[i][col_y] = ' ';
			curt_in_row[i] -= 1; //minus one in the one that tracks rows
			temp ++;
		}
		ship.setX(-1);
		ship.setY(-1);
		return true;
	}
	if (!ship.get_is_vert())// if its horizontal
	{
		int temp = 0;
		curt_in_row[row_x] -= ship.getSize();//keeps track of current blocks taken in that row
		for (int i = col_y; i < col_y + ship.getSize(); ++i)
		{
			grid[row_x][i] = ' ';
			curt_in_col[i] -= 1;
			temp ++;
		}
		ship.setX(-1);
		ship.setY(-1);		
		return true;
	}
	return false;

}
void Grid::make_grid()
{
	//here im making the first grid, empty and with nothing in it

	for (unsigned int i = 0; i < int_rows.size(); ++i)
	{
		std::vector<char> temp;
		for ( unsigned int k = 0; k < int_cols.size(); ++k)
		{
			temp.push_back(' ');
		}
		grid.push_back(temp);
	}
}

bool order_ships(const Ship &a, const Ship &b)
{
	//this orders the ship
	if (a.getX() == b.getX())
	{
		return a.getY() < b.getY();
	}
	return a.getX() < b.getX();
}

void Grid::print_ships()
{
	std::sort(ships.begin(), ships.end(), order_ships);
	for (unsigned int i = 0; i < ships.size(); ++i)
	{
		std::cout << std::left << std::setw(10) << std::setfill(' ') << ships[i].getType() << " " << ships[i].getX() << " " << ships[i].getY();
        if (ships[i].getSize() > 1)
        {
            if (ships[i].get_is_vert()) //vertical
            {
            	std::cout << " vertical";
            }
            if (!ships[i].get_is_vert())//horizontal
            {
            	std::cout << " horizontal";
            }
        }
        std::cout << std::endl;
	}
}
void Grid::print_grid()
{
	// calls print ships, outputs the grid
	print_ships();

	std::cout << "+";
	std::cout << std::string(col_size, '-');
	std::cout << "+"  << std::endl;
	for (int i = 0; i < row_size; ++i)
	{
		std::cout << "|";
		for (int k = 0; k < col_size; ++k)
		{
			std::cout << grid[i][k];
		}
		std::cout << "|" << curt_in_row[i] << std::endl;
	}
	std::cout << "+";
	std::cout << std::string(col_size, '-');
	std::cout << "+" << std::endl;
	std::cout << " ";
	for (unsigned int i = 0; i < int_cols.size(); ++i)
	{
		std::cout << curt_in_col[i];
	}
	std::cout << std::endl;
}

bool Grid::final_check()
{
	//here im checking the board once it is "done"
	//going thru the vector of constraints
	//making sure theyre fullfied, then its true
	//then final row and col check as in like its full, if these arent equal then not true;
	for (unsigned int i = 0; i < vec_of_con.size(); ++i)
	{
		int x = vec_of_con[i].getX();
		int y = vec_of_con[i].getY();
		char con = vec_of_con[i].getSection();
		if (con == '_' )
		{
			con = ' ';
		}
		if (grid[x][y] != con)
		{
			return false;
		}
	}
	for (unsigned int i = 0; i < int_rows.size(); ++i)
	{
		if (int_rows[i] != '?' && (int(int_rows[i])-48) != curt_in_row[i])
		{
			return false;
		}
	}
	for (unsigned int i = 0; i < int_cols.size(); ++i)
	{
		if (int_cols[i] != '?' && (int(int_cols[i])-48) != curt_in_col[i])
		{
			return false;
		}
	}
	return true;
}

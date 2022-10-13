#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <algorithm> 

class Grid
{
public:
	Grid(const std::vector<char> &int_rows_, const std::vector<char> &int_cols_, const std::vector<Constraint> &vec_of_con_);
	void make_grid();
	bool add_ship(Ship &ship, int row_x, int col_y);
	bool delete_ship(Ship &ship);
	void print_grid();
	void print_ships();
	bool is_taken(Ship &ship, int x, int y);
	bool row_check(int index, bool is_vert, int size);
	bool col_check(int index, bool is_vert, int size);
	bool no_ships_close(Ship &ship);
	bool is_valid(Ship &ship, int row, int col);
	bool final_check();
	int get_col();
	int get_row();
private:
	std::vector< std::vector<char> > grid;
	std::vector<char> int_rows;
	std::vector<char> int_cols;
	std::vector<int> curt_in_row;
	std::vector<int> curt_in_col;
	std::vector<Ship> ships;
	int row_size;
	int col_size;
	std::vector<Constraint> vec_of_con;
};
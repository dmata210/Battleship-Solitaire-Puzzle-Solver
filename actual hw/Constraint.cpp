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
//like a mini ship class, holds the constraint infomation easily to later be used in the final check

Constraint::Constraint(int cord_x_, int cord_y_, char section_)
{
	cord_x = cord_x_;
	cord_y = cord_y_;
	section = section_;	
}
int Constraint::getX(){return cord_x;};
int Constraint::getY(){return cord_y;};
char Constraint::getSection(){return section;};

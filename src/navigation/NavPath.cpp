
#include "NavPath.h"
#include <fstream>
#include <math.h>

void NavPath::clearAllocated()
{
    _path.clear();
}

void NavPath::addCase(const NavCase& c)
{
    _path.push_back(c);
}

int NavPath::getPathLength() const
{
    return _path.size();
}

const NavCase& NavPath::getPathStep(int n) const
{
    return _path[n];
}


float NavPath::getOrientation(int n) const
{
	if (n == 0)
		return 0;
	else
	{
		int dx = _path[n - 1].getX() - _path[n].getX();
		int dy = _path[n - 1].getY() - _path[n].getY();

		if (dx < 0)
		{
			if (dy < 0)
				return (float)M_PI * 0.75f;
			else if (dy > 0)
				return (float)M_PI * 0.25f;
			else
				return (float)M_PI * 0.5f;
		}
		else if (dx > 0)
		{
			if (dy < 0)
				return -(float)M_PI*0.75f;
			else if (dy > 0)
				return -(float)M_PI * 0.25f;
			else
				return -(float)M_PI * 0.5f;
		}
		else
		{
			if (dy < 0)
				return (float)M_PI;
			else
				return 0;
		}
	}
}

void NavPath::outputPath(ofstream& ofs) const
{
	ofs << "Path [" << _path.size() << "]:" << endl;
	for (unsigned int i = 0; i < _path.size(); ++i)
	{
		ofs << "(" << _path[i].getX() << " , " << _path[i].getY() << ")" << endl;
	}

	ofs << endl;
}

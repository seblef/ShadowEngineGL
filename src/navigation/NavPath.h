#pragma once

#include "NavCase.h"
#include "../core/TAllocated.h"
#include <iostream>

using namespace Core;

class NavPath : public TAllocated<NavPath>
{
protected:

	typedef vector<NavCase>		CaseVector;

	CaseVector					_path;

public:

	NavPath()					{}

	void						clearAllocated();

	void						addCase(const NavCase& c);

	int							getPathLength() const;
	const NavCase&				getPathStep(int n) const;

	float						getOrientation(int n) const;

	void						outputPath(ofstream& ofs) const;
};

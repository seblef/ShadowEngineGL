
#pragma once

#include "TTab.h"
#include "Matrix4.h"

namespace Core
{

class MatrixHierarchy : public TTab<Matrix4>
{
protected:

	int*				_parents;
	Matrix4*			_offsets;
	bool*				_offsetsEnable;

public:

	MatrixHierarchy(int count, bool offsets);
	MatrixHierarchy(const MatrixHierarchy& mh);
	~MatrixHierarchy();

	int					getParent(int n) const;
	void				setParent(int n, int p);

	int*				getParents() const;

	void				enableOffset(int idx, bool enable=true);
	void				setOffset(int idx, const Matrix4& offset);

	void				update(const Matrix4& world, const TTab<Matrix4>& pose);
};

};


#pragma once

#include "Vector2.h"


namespace Core
{

class BBox2
{
protected:

	Vector2			_min, _max;

public:

	static BBox2	BoundBox;
	static BBox2	NullBox;

	BBox2();
	BBox2(const Vector2& min, const Vector2& max);
	BBox2(const BBox2& b);
	BBox2(float xmin, float ymin, float xmax, float ymax);
	~BBox2()													{}

	const Vector2&	getMin() const;
	const Vector2&	getMax() const;
	void			getMinMax(Vector2& min, Vector2& max);

	void			setMin(const Vector2& min);
	void			setMax(const Vector2& max);
	void			setMinMax(const Vector2& min, const Vector2& max);

	BBox2&			operator=(const BBox2& b);

	BBox2			operator+(const Vector2& v)	const;
	BBox2			operator-(const Vector2& v)	const;

	BBox2&			operator+=(const Vector2& v);
	BBox2&			operator-=(const Vector2& v);

	BBox2&			operator << (const Vector2& v);
	BBox2&			operator << (const BBox2& b);

	bool			isIn(const Vector2& v) const;
	bool			isIn(const BBox2& b) const;
	bool			isCompletlyIn(const BBox2& b) const;

	void			repair();

	Vector2			getSize() const;
	void			getSize(Vector2& size) const;

	Vector2			getCenter() const;
	void			getCenter(Vector2& c) const;
};

inline ostream& operator << (ostream& os, const BBox2& b)
{
	os << b.getMin() << " <--> " << b.getMax();
    return os;
}

};

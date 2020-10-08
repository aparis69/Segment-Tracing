#include "fundamentals.h"

/*!
\brief
*/
Ray::Ray(const Vector& pp, const Vector& dd)
{
	o = pp;
	d = dd;
}

/*!
\brief
*/
Vector Ray::operator()(double t) const
{
	return o + t * d;
}


/*!
\brief
*/
Box::Box()
{
	a = b = Vector(0);
}

/*!
\brief
*/
Box::Box(const Vector& aa, const Vector& bb)
{
	a = aa;
	b = bb;
}

/*!
\brief
*/
Box::Box(const Box& b1, const Box& b2)
{
	a = Vector::Min(b1.a, b2.a);
	b = Vector::Max(b1.b, b2.b);
}

/*!
\brief
*/
bool Box::Inside(const Vector& p) const
{
	return (p > a&& p < b);
}

/*!
\brief
*/
int Box::Intersect(const Ray& ray, double& tmin, double& tmax, double epsilon) const
{
	tmin = -1e16;
	tmax = 1e16;

	Vector p = ray.o;
	Vector d = ray.d;

	double t;
	// Ox
	if (d[0] < -epsilon)
	{
		t = (a[0] - p[0]) / d[0];
		if (t < tmin)
			return 0;
		if (t <= tmax)
			tmax = t;
		t = (b[0] - p[0]) / d[0];
		if (t >= tmin)
		{
			if (t > tmax)
				return 0;
			tmin = t;
		}
	}
	else if (d[0] > epsilon)
	{
		t = (b[0] - p[0]) / d[0];
		if (t < tmin)
			return 0;
		if (t <= tmax)
			tmax = t;
		t = (a[0] - p[0]) / d[0];
		if (t >= tmin)
		{
			if (t > tmax)
				return 0;
			tmin = t;
		}
	}
	else if (p[0]<a[0] || p[0]>b[0])
		return 0;

	// Oy
	if (d[1] < -epsilon)
	{
		t = (a[1] - p[1]) / d[1];
		if (t < tmin)
			return 0;
		if (t <= tmax)
			tmax = t;
		t = (b[1] - p[1]) / d[1];
		if (t >= tmin)
		{
			if (t > tmax)
				return 0;
			tmin = t;
		}
	}
	else if (d[1] > epsilon)
	{
		t = (b[1] - p[1]) / d[1];
		if (t < tmin)
			return 0;
		if (t <= tmax)
			tmax = t;
		t = (a[1] - p[1]) / d[1];
		if (t >= tmin)
		{
			if (t > tmax)
				return 0;
			tmin = t;
		}
	}
	else if (p[1]<a[1] || p[1]>b[1])
		return 0;

	// Oz
	if (d[2] < -epsilon)
	{
		t = (a[2] - p[2]) / d[2];
		if (t < tmin)
			return 0;
		if (t <= tmax)
			tmax = t;
		t = (b[2] - p[2]) / d[2];
		if (t >= tmin)
		{
			if (t > tmax)
				return 0;
			tmin = t;
		}
	}
	else if (d[2] > epsilon)
	{
		t = (b[2] - p[2]) / d[2];
		if (t < tmin)
			return 0;
		if (t <= tmax)
			tmax = t;
		t = (a[2] - p[2]) / d[2];
		if (t >= tmin)
		{
			if (t > tmax)
				return 0;
			tmin = t;
		}
	}
	else if (p[2]<a[2] || p[2]>b[2])
		return 0;

	return 1;
}

/*!
\brief
*/
bool Box::Intersect(const Box& box) const
{
	if (((a[0] >= box.b[0]) || (a[1] >= box.b[1]) || (a[2] >= box.b[2]) || (b[0] <= box.a[0]) || (b[1] <= box.a[1]) || (b[2] <= box.a[2])))
		return false;
	else
		return true;
}

/*!
\brief
*/
Vector Box::operator[](int i) const
{
	if (i == 0)
		return a;
	return b;
}

/*!
\brief
*/
Vector Box::Diagonal() const
{
	return b - a;
}

/*!
\brief
*/
Vector Box::Center() const
{
	return 0.5 * (a + b);
}


/*!
\brief
*/
Segment::Segment()
{
	a = b = Vector(0);
}

/*!
\brief
*/
Segment::Segment(const Vector& aa, const Vector& bb)
{
	a = aa;
	b = bb;
}

/*!
\brief
*/
Vector Segment::operator[](int i) const
{
	if (i == 0)
		return a;
	return b;
}

/*!
\brief
*/
bool Segment::Intersect(const Box& box) const
{
	Vector ba = box.Diagonal();

	Vector d = 0.5 * (b - a);
	Vector c = 0.5 * (a + b);

	double fd[3];
	Vector cc = c - box.Center();

	fd[0] = fabs(d[0]);
	if (fabs(cc[0]) > ba[0] + fd[0])
		return false;

	fd[1] = fabs(d[1]);
	if (fabs(cc[1]) > ba[1] + fd[1])
		return false;

	fd[2] = fabs(d[2]);
	if (fabs(cc[2]) > ba[2] + fd[2])
		return false;

	if (fabs(d[1] * cc[2] - d[2] * cc[1]) > ba[1] * fd[2] + ba[2] * fd[1])
		return false;

	if (fabs(d[2] * cc[0] - d[0] * cc[2]) > ba[0] * fd[2] + ba[2] * fd[0])
		return false;

	if (fabs(d[0] * cc[1] - d[1] * cc[0]) > ba[0] * fd[1] + ba[1] * fd[0])
		return false;

	return true;
}

/*!
\brief
*/
Box Segment::GetBox() const
{
	return Box(Vector::Min(a, b), Vector::Max(a, b));
}

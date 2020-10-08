#include "evector.h"

/*!
\class Vector evector.h
\brief Vectors in three dimensions.
*/

const Vector Vector::Null = Vector(0.0, 0.0, 0.0);

/*!
\brief Normalize a vector, computing the inverse of its norm and scaling
the components.

This function does not check if the vector is null,
which might resulting in errors.
*/
void Normalize(Vector& u)
{
	u *= 1.0 / Norm(u);
}

/*!
\brief Swap two vectors.
\param a, b Vectors.
*/
void Swap(Vector& a, Vector& b)
{
	Vector t = a;
	a = b;
	b = t;
}

/*!
\brief Returns a vector orthogonal to the argument vector.

The returned orthogonal vector is not computed randomly.
First, we find the two coordinates of the argument vector with
maximum absolute value. The orthogonal vector is defined by
swapping those two coordinates and changing one sign, whereas
the third coordinate is set to 0.

The returned orthogonal vector lies in the plane orthogonal
to the first vector.
*/
Vector Vector::Orthogonal() const
{
	Vector a = Abs(*this);
	int i = 0;
	int j = 1;
	if (a[0] > a[1])
	{
		if (a[2] > a[1])
		{
			j = 2;
		}
	}
	else
	{
		i = 1;
		j = 2;
		if (a[0] > a[2])
		{
			j = 0;
		}
	}
	a = Vector(0.0);
	a[i] = c[j];
	a[j] = -c[i];
	return a;
}

/*!
\brief Given a vector, creates two vectors xand y that form an orthogonal basis.

This algorithm pickes the minor axis in order to reduce numerical instability
\param x, y Returned vectors such that (x,y,n) form an orthonormal basis (provided n is normalized).
*/
void Vector::Orthonormal(Vector& x, Vector& y) const
{
	x = Normalized(Orthogonal());
	y = Normalized(*this / x);
}

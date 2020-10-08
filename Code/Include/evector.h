#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifndef __Vector__
#define __Vector__

// Mathematics fundamentals
#include "mathematics.h"

// Class
class Vector
{
protected:
	double c[3]; //!< Components.
public:
	//! Empty 
	inline Vector() { c[0] = c[1] = c[2] = 0.0; }

	explicit Vector(const double&);
	explicit Vector(const double&, const double&, const double&);

	// Access members
	double& operator[] (int);
	double operator[] (int) const;

	// Unary operators
	Vector operator+ () const;
	Vector operator- () const;

	// Assignment operators
	Vector& operator+= (const Vector&);
	Vector& operator-= (const Vector&);
	Vector& operator*= (const Vector&);
	Vector& operator/= (const Vector&);
	Vector& operator*= (const double&);
	Vector& operator/= (const double&);

	// Binary operators
	friend int operator> (const Vector&, const Vector&);
	friend int operator< (const Vector&, const Vector&);

	friend int operator>= (const Vector&, const Vector&);
	friend int operator<= (const Vector&, const Vector&);

	// Binary operators
	friend Vector operator+ (const Vector&, const Vector&);
	friend Vector operator- (const Vector&, const Vector&);

	friend double operator* (const Vector&, const Vector&);

	friend Vector operator* (const Vector&, double);
	friend Vector operator* (const double&, const Vector&);
	friend Vector operator/ (const Vector&, double);

	friend Vector operator/ (const Vector&, const Vector&);

	// Boolean functions
	friend int operator==(const Vector&, const Vector&);
	friend int operator!=(const Vector&, const Vector&);

	// Norm
	friend double Norm(const Vector&);
	friend double SquaredNorm(const Vector&);

	int MaxIndex() const;

	friend void Normalize(Vector&);
	friend Vector Normalized(const Vector&);

	// Compare functions
	static Vector Min(const Vector&, const Vector&);
	static Vector Max(const Vector&, const Vector&);

	// Abs
	friend Vector Abs(const Vector&);

	// Orthogonal and orthonormal vectors
	Vector Orthogonal() const;
	void Orthonormal(Vector&, Vector&) const;

	friend void Swap(Vector&, Vector&);
	friend Vector Clamp(const Vector&, const Vector&, const Vector&);
	friend Vector Lerp(const Vector&, const Vector&, const double&);

	// Scale
	Vector Scale(const Vector&) const;
	Vector Inverse() const;

	static Vector Solve(const Vector&, const Vector&, const double&, const double&);

public:
	static const Vector Null; //!< Null vector.
};

/*!
\brief Create a vector with the same coordinates.
\param a Real.
*/
inline Vector::Vector(const double& a)
{
	c[0] = c[1] = c[2] = a;
}

/*!
\brief Create a vector with argument coordinates.
\param a,b,c Coordinates.
*/
inline Vector::Vector(const double& a, const double& b, const double& c)
{
	Vector::c[0] = a;
	Vector::c[1] = b;
	Vector::c[2] = c;
}

//! Gets the i-th coordinate of vector.
inline double& Vector::operator[] (int i)
{
	return c[i];
}

//! Returns the i-th coordinate of vector.
inline double Vector::operator[] (int i) const
{
	return c[i];
}

// Unary operators

//! Overloaded.
inline Vector Vector::operator+ () const
{
	return *this;
}

//! Overloaded.
inline Vector Vector::operator- () const
{
	return Vector(-c[0], -c[1], -c[2]);
}

// Assignment unary operators

//! Destructive addition.
inline Vector& Vector::operator+= (const Vector& u)
{
	c[0] += u.c[0]; c[1] += u.c[1]; c[2] += u.c[2];
	return *this;
}

//! Destructive subtraction.
inline Vector& Vector::operator-= (const Vector& u)
{
	c[0] -= u.c[0]; c[1] -= u.c[1]; c[2] -= u.c[2];
	return *this;
}

//! Destructive scalar multiply.
inline Vector& Vector::operator*= (const double& a)
{
	c[0] *= a; c[1] *= a; c[2] *= a;
	return *this;
}

/*!
\brief Scale a vector.
\param a Scaling vector.
*/
inline Vector Vector::Scale(const Vector& a) const
{
	return Vector(c[0] * a[0], c[1] * a[1], c[2] * a[2]);
}

/*!
\brief Inverse of a vector.

This function inverses the components of the vector. This is the same as:
\code
Vector v=Vector(1.0/u[0],1.0/u[1],1.0/u[2]);
\endcode
*/
inline Vector Vector::Inverse() const
{
	return Vector(1.0 / c[0], 1.0 / c[1], 1.0 / c[2]);
}

//! Destructive division by a scalar.
inline Vector& Vector::operator/= (const double& a)
{
	c[0] /= a; c[1] /= a; c[2] /= a;
	return *this;
}

/*!
\brief Destructively scale a vector by another vector.

This is the same as Scale:
\code
Vector u(2.0,-1.0,1.0);
u.Scale(Vector(3.0,1.0,2.0)); // u*=Vector(3.0,1.0,2.0);
\endcode
*/
inline Vector& Vector::operator*= (const Vector& u)
{
	c[0] *= u.c[0]; c[1] *= u.c[1]; c[2] *= u.c[2];
	return *this;
}

//! Destructively divide the components of a vector by another vector.
inline Vector& Vector::operator/= (const Vector& u)
{
	c[0] /= u.c[0]; c[1] /= u.c[1]; c[2] /= u.c[2];
	return *this;
}

//! Compare two vectors.
inline int operator> (const Vector& u, const Vector& v)
{
	return ((u.c[0] > v.c[0]) && (u.c[1] > v.c[1]) && (u.c[2] > v.c[2]));
}

//! Compare two vectors.
inline int operator< (const Vector& u, const Vector& v)
{
	return ((u.c[0] < v.c[0]) && (u.c[1] < v.c[1]) && (u.c[2] < v.c[2]));
}

//! Overloaded
inline int operator>= (const Vector& u, const Vector& v)
{
	return ((u.c[0] >= v.c[0]) && (u.c[1] >= v.c[1]) && (u.c[2] >= v.c[2]));
}

//! Overloaded
inline int operator<= (const Vector& u, const Vector& v)
{
	return ((u.c[0] <= v.c[0]) && (u.c[1] <= v.c[1]) && (u.c[2] <= v.c[2]));
}

//! Adds up two vectors.
inline Vector operator+ (const Vector& u, const Vector& v)
{
	return Vector(u.c[0] + v.c[0], u.c[1] + v.c[1], u.c[2] + v.c[2]);
}

//! Difference between two vectors.
inline Vector operator- (const Vector& u, const Vector& v)
{
	return Vector(u.c[0] - v.c[0], u.c[1] - v.c[1], u.c[2] - v.c[2]);
}

//! Scalar product.
inline double operator* (const Vector& u, const Vector& v)
{
	return (u.c[0] * v.c[0] + u.c[1] * v.c[1] + u.c[2] * v.c[2]);
}

//! Right multiply by a scalar.
inline Vector operator* (const Vector& u, double a)
{
	return Vector(u.c[0] * a, u.c[1] * a, u.c[2] * a);
}

//! Left multiply by a scalar.
inline Vector operator* (const double& a, const Vector& v)
{
	return v * a;
}

//! Cross product.
inline Vector operator/ (const Vector& u, const Vector& v)
{
	return Vector(u.c[1] * v.c[2] - u.c[2] * v.c[1], u.c[2] * v.c[0] - u.c[0] * v.c[2], u.c[0] * v.c[1] - u.c[1] * v.c[0]);
}

//! Left multiply by a scalar
inline Vector operator/ (const Vector& u, double a)
{
	return Vector(u.c[0] / a, u.c[1] / a, u.c[2] / a);
}

// Boolean functions

//! Strong equality test.
inline int operator== (const Vector& u, const Vector& v)
{
	return ((u.c[0] == v.c[0]) && (u.c[1] == v.c[1]) && (u.c[2] == v.c[2]));
}

//! Strong difference test.
inline int operator!= (const Vector& u, const Vector& v)
{
	return (!(u == v));
}

/*!
\brief Compute the Euclidean norm of a vector.

This function involves a square root computation, it is in general more efficient to rely on
the squared norm of a vector instead.
\param u %Vector.
\sa SquaredNorm
*/
inline double Norm(const Vector& u)
{
	return sqrt(u.c[0] * u.c[0] + u.c[1] * u.c[1] + u.c[2] * u.c[2]);
}

/*!
\brief Compute the squared Euclidean norm of a vector.
\param u %Vector.
\sa Norm
*/
inline double SquaredNorm(const Vector& u)
{
	return (u.c[0] * u.c[0] + u.c[1] * u.c[1] + u.c[2] * u.c[2]);
}

/*!
\brief Return a normalized vector.

Compute the inverse of its norm and scale the components.

This function does not check if the vector is null.
\param u %Vector.
*/
inline Vector Normalized(const Vector& u)
{
	return u * (1.0 / Norm(u));
}

/*!
\brief Compute the index of the maximum component of a vector.


\code
Vector a(-1.0,-3.0,2.0);
int i=MaxIndex(Abs(a)); // Should be 1
\endcode

This function can be used to find the most stretched axis of a bounding box,
for instance to cut the box in the middle of this stretched axis:

\code
Box box;
int axis = box.Diagonal().MaxIndex();
\endcode

\sa Max
*/
inline int Vector::MaxIndex() const
{
	if (c[0] >= c[1])
	{
		if (c[0] >= c[2])
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}
	else
	{
		if (c[1] >= c[2])
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
}

/*!
\brief Computes the absolute value of a vector.
\param u %Vector.
*/
inline Vector Abs(const Vector& u)
{
	return Vector(u[0] > 0.0 ? u[0] : -u[0], u[1] > 0.0 ? u[1] : -u[1], u[2] > 0.0 ? u[2] : -u[2]);
}

/*!
\brief Return a vector with coordinates set to the minimum coordinates
of the two argument vectors.
*/
inline Vector Vector::Min(const Vector& a, const Vector& b)
{
	return Vector(a[0] < b[0] ? a[0] : b[0], a[1] < b[1] ? a[1] : b[1], a[2] < b[2] ? a[2] : b[2]);
}

/*!
\brief Return a vector with coordinates set to the maximum coordinates
of the two argument vectors.
*/
inline Vector Vector::Max(const Vector& a, const Vector& b)
{
	return Vector(a[0] > b[0] ? a[0] : b[0], a[1] > b[1] ? a[1] : b[1], a[2] > b[2] ? a[2] : b[2]);
}

/*!
\brief Clamp a vector between two bounds.
\param x Input vector
\param a, b %Vector bounds.
*/
inline Vector Clamp(const Vector& x, const Vector& a, const Vector& b)
{
	return Vector(Math::Clamp(x[0], a[0], b[0]), Math::Clamp(x[1], a[1], b[1]), Math::Clamp(x[2], a[2], b[2]));
}

/*!
\brief Linear interpolation between two vectors.
\param a,b Interpolated points.
\param t Interpolant.
*/
inline Vector Lerp(const Vector& a, const Vector& b, const double& t)
{
	return a + t * (b - a);
}

/*!
\brief Compute the point on a segment such that the linear function satisfies f(a)=va and f(b)=vb.

This function can be used as a first approximation for computing the intersection between a segment and an implicit surface.
\image html intersection.png

\sa Linear::Solve(const double& a, const double& b, const Vector&, const Vector&);
*/
inline Vector Vector::Solve(const Vector& a, const Vector& b, const double& va, const double& vb)
{
	return (vb * a - va * b) / (vb - va);
}

#endif


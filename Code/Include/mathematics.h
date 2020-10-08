// Fundamentals

#ifndef __Mathematics__
#define __Mathematics__

#include <math.h>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

/*!
\brief Minimum of two integers.
*/
inline int min(int a, int b)
{
	return (a < b ? a : b);
}

/*!
\brief Minimum of two unsigned integers.
*/
inline unsigned int min(unsigned int a, unsigned int b)
{
	return (a < b ? a : b);
}

/*!
\brief Maximum of two integers.
*/
inline int max(int a, int b)
{
	return (a > b ? a : b);
}

/*!
\brief Minimum of two doubles.
*/
inline float min(const float& a, const float& b)
{
	return (a < b ? a : b);
}

/*!
\brief Maximum of two doubles.
*/
inline float max(const float& a, const float& b)
{
	return (a > b ? a : b);
}

/*!
\brief Minimum of two doubles.
*/
inline double min(const double& a, const double& b)
{
	return (a < b ? a : b);
}

/*!
\brief Maximum of two doubles.
*/
inline double max(const double& a, const double& b)
{
	return (a > b ? a : b);
}

/*!
\brief Minimum of three doubles.
*/
inline double min(const double& a, const double& b, const double& c)
{
	return min(min(a, b), c);
}

/*!
\brief Maximum of three doubles.
*/
inline double max(const double& a, const double& b, const double& c)
{
	return max(max(a, b), c);
}

/*!
\brief Minimum of four doubles.
*/
inline double min(const double& a, const double& b, const double& c, const double& d)
{
	return min(min(a, b), min(c, d));
}

/*!
\brief Maximum of four doubles.
*/
inline double max(const double& a, const double& b, const double& c, const double& d)
{
	return max(max(a, b), max(c, d));
}

/*!
\brief Minimum of four integers.
*/
inline int min(int a, int b, int c, int d)
{
	return min(min(a, b), min(c, d));
}

/*!
\brief Minimum of four unsigned integers.
*/
inline unsigned int min(unsigned int a, unsigned int b, unsigned int c, unsigned int d)
{
	return min(min(a, b), min(c, d));
}

/*!
\brief Maximum of four integers.
*/
inline int max(int a, int b, int c, int d)
{
	return max(max(a, b), max(c, d));
}

//! Clamps an integer value between two bounds.
inline int Clamp(int x, int a, int b)
{
	return (x < a ? a : (x > b ? b : x));
}

//! Clamps a double value between two bounds.
inline double Clamp(double x, double a, double b)
{
	return (x < a ? a : (x > b ? b : x));
}

//! Clamps a double value between two bounds.
inline float Clamp(float x, float a, float b)
{
	return (x < a ? a : (x > b ? b : x));
}

// Math class for constants
class Math
{
public:
	static const double Pi; //!< Pi.
	static const double HalfPi; //!< Half of pi.
	static const double e; //!< Exponential.
	static const double TwoPiOverThree; //!< 2/3 Pi.
	static const double FourPiOverThree; //!< 4/3 Pi.
	static const double Infinity; //! Infinity
	static const double Sqrt5; //!< Constant &radic;5
	static const double Sqrt3; //!< Constant &radic;3
	static const double Sqrt2; //!< Constant &radic;2
	static const double Golden; //!< Constant (&radic;5+1)/2
public:
	static double Clamp(const double&, const double& = 0.0, const double& = 1.0);
	static int Clamp(int, int = 0, int = 255);

	// Squares
	static double Abs(const double&);

	// Minimum and maximum
	static double Min(const double&, const double&);
	static double Max(const double&, const double&);
	static double Min(const double&, const double&, const double&);
	static double Max(const double&, const double&, const double&);
	static double Min(const double&, const double&, const double&, const double&);
	static double Max(const double&, const double&, const double&, const double&);

	// Angles
	static double DegreeToRadian(const double&);
	static double RadianToDegree(const double&);
};

/*!
\brief Convert degrees to randians.
\param a Angle in degrees.
*/
inline double Math::DegreeToRadian(const double& a)
{
	return a * Math::Pi / 180.0;
}

/*!
\brief Convert radian to degrees.
\param a Angle in radian.
*/
inline double Math::RadianToDegree(const double& a)
{
	return a * 180.0 / Math::Pi;
}

/*!
\brief Clamp a double value between two bounds.
\param x Input value.
\param a, b Lower and upper bounds.
*/
inline double Math::Clamp(const double& x, const double& a, const double& b)
{
	return (x < a ? a : (x > b ? b : x));
}

/*!
\brief Clamp an integer value between two bounds.
\param x Input value.
\param a, b Lower and upper bounds.
*/
inline int Math::Clamp(int x, int a, int b)
{
	return (x < a ? a : (x > b ? b : x));
}

/*!
\brief Minimum of two reals.
\param a, b Real values.
*/
inline double Math::Min(const double& a, const double& b)
{
	return (a < b ? a : b);
}

/*!
\brief Maximum of two reals.
\param a, b Real values.
*/
inline double Math::Max(const double& a, const double& b)
{
	return (a > b ? a : b);
}

/*!
\brief Maximum of three reals.
\param a, b, c Real values.
*/
inline double Math::Max(const double& a, const double& b, const double& c)
{
	return Math::Max(Math::Max(a, b), c);
}

/*!
\brief Minimum of three reals.
\param a, b, c Real values.
*/
inline double Math::Min(const double& a, const double& b, const double& c)
{
	return Math::Min(Math::Min(a, b), c);
}

/*!
\brief Maximum of four reals.
\param a, b, c, d Real values.
*/
inline double Math::Max(const double& a, const double& b, const double& c, const double& d)
{
	return Math::Max(Math::Max(a, b), Math::Max(c, d));
}

/*!
\brief Minimum of four reals.
\param a, b, c, d Real values.
*/
inline double Math::Min(const double& a, const double& b, const double& c, const double& d)
{
	return Math::Min(Math::Min(a, b), Math::Min(c, d));
}

/*!
\brief Absolute value.

\param x Real.
*/
inline double Math::Abs(const double& x)
{
	return fabs(x);
}

#endif

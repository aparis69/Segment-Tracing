#include <float.h>

#include "mathematics.h"

/*!
\defgroup Math Core math classes.

\brief Core math classes include several classes such as Vector, Quadric, Cubic and higher order
polynomials and many others that are useful in many graphic applications.

\changed 12.12.23
*/

/*!
\class Math mathematics.h
\brief Core class implementing some useful functions and constants.

<P><I>How can I use the constant Pi?</I>
<BR>Simply use the static constant Math::Pi as follows:
\code
double v=4.0*Math::Pi*r*r*r/3.0; // Volume of a sphere.
\endcode
Note that in this case, you could also have used:
\code
double v=Sphere::Volume(r);
\endcode

<P><I>How many min/max functions have been implemented?</I>
<BR>Up to four arguments are supported; for a larger number of
arguments, a specific routine operating on an array should be written.

<P><I>Is there a function to compute the square of a real sumber?</I>
<BR>Use the following:
\code
double s=Math::Sqr((1.0-sqrt(5.0))/2.0); // Square of golden ratio
\endcode
For a Vector, use SquaredNorm(const Vector&);

<P><I>Are there predefined square roots constants?</I>
<BR>The sqrt function used not te be constexpr, so square roots of reals are not computed at compilation time.
Some constants are provided, such as the following one:
\code
double s=Math::Sqrt3; // Square root of 3
\endcode

\ingroup Math
\changed 13.01.24

<P><I>How are implemented the step and smooth-step functions that are often used in procedural modeling?</I>
Different smoothing kernels, such as Cubic::Smooth(), are implented in odd-degree polynomials Cubic, Quintic and Septic.
The corresponding step functions, such as Cubic::SmoothStep(), are also implemented.
\sa Linear::Step, Cubic::Smooth, Quintic::Smooth, Cubic::SmoothStep, Quintic::SmoothStep, Septic::SmoothStep.

*/

const double Math::Pi = 3.14159265358979323846;

const double Math::HalfPi = Math::Pi / 2.0;

const double Math::e = 2.7182818284590452354;

const double Math::TwoPiOverThree = 2.0943951023931954923084;

const double Math::FourPiOverThree = 4.1887902047863909846168;

const double Math::Infinity = DBL_MAX;

const double Math::Sqrt5 = sqrt(5.0);

const double Math::Sqrt3 = sqrt(3.0);

const double Math::Sqrt2 = sqrt(2.0);

const double Math::Golden = (sqrt(5.0) + 1.0) / 2.0;

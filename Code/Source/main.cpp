#include <chrono>		// high resolution timer
#include <iostream>		// std::cout
#include "blobtree.h"	// Implicit construction tree

// Render parameters as global file variable
const int imgWidth = 500;
const int imgHeight = 500;
const Vector sunDir = Vector(0.0f, -1.0f, 0.0f);
const Vector camera = Vector(0.0f, -80.0f, 0.0f);
BlobTree* tree = new BlobTree("../Scenes/particles.txt");

enum RayTraceMethod
{
	SphereTracing = 0,
	EnhancedSphereTracing = 1,
	SegmentTracing = 2,
	COUNT = 3
};

/*!
\brief Compute a ray from a pixel coordinates.
\param px pixel coordinate
\param py pixel coordinate
\return the ray going through this pixel.
*/
Ray ComputeRayFromPixel(int px, int py)
{
	// Camera parameters
	const double cah = 1.995;
	const double cav = 1.5;
	const double fl = 35.0;

	// Get coordinates
	Vector view = Normalized(-camera);
	Vector horizontal = Normalized(view / Vector(0, 0, 1.0f));
	Vector vertical = Normalized(horizontal / view);
	const double length = 1.0f;

	double avh = 2.0 * atan(cah * 25.4 * 0.5 / fl);
	double avv = 2.0 * atan(tan(avh / 2.0) * double(imgHeight) / double(imgWidth));
	double rad = avv; // Fov

	double vLength = tan(rad / 2.0f) * length;
	double hLength = vLength * (double(imgWidth) / double(imgHeight));
	vertical *= vLength;
	horizontal *= hLength;

	// Translate mouse coordinates so that the origin lies in the center of the view port
	double x = px - imgWidth / 2.0;
	double y = imgHeight / 2.0 - py;

	// Scale mouse coordinates so that half the view port width and height becomes 1.0
	x /= imgWidth / 2.0;
	y /= imgHeight / 2.0;

	// Direction is a linear combination to compute intersection of picking ray with view port plane
	return Ray(camera, Normalized(view * length + horizontal * x + vertical * y));
}

/*!
\brief Sphere tracing for a ray
\param ray the ray
\param t returned intersection depth
\param s returned step count
\param k global lipschitz constant
\return true of intersection occured, false otherwise.
*/
bool SphereTrace(const Ray& ray, double& t, int& s, double k)
{
	// First check intersection with bounding box
	double a, b;
	if (!tree->GetBox().Intersect(ray, a, b))
		return false;

	// Classic sphere tracing using global lipschitz constant
	t = a;
	s = 0;
	while (t < b)
	{
		s++;
		double I = tree->Intensity(ray(t));
		if (I > 0.0)
			return true;
		double ts = Math::Max(fabs(I) / k, Epsilon());
		t += ts;
	}
	return false;
}

/*!
\brief Enhanced sphere tracing for a ray
\param ray the ray
\param t returned intersection depth
\param s returned step count
\param k global lipschitz constant
\return true of intersection occured, false otherwise.
*/
bool EnhancedSphereTrace(const Ray& ray, double& t, int& s, double k)
{
	// First check intersection with bounding box
	double a, b;
	if (!tree->GetBox().Intersect(ray, a, b))
		return false;

	// Enhanced sphere tracing using overstepping factor and global lipschitz constant
	t = a;
	s = 0;
	double e = 1.25; // Overstep factor in [1.0, 2.0]

	// Marching distance used in the previous step 
	double te = 0.0;
	while (t < b)
	{
		s++;
		double i = tree->Intensity(ray(t));

		// Got inside
		if (i > 0.0)
			return true;

		// Safe stepping distance
		double tk = fabs(i) / k;

		// We moved too far and the Lipschitz check fails: we need to move backward
		if (tk < (e - 1.0) * te)
		{
			t -= (e - 1.0) * te;
			te = 0.0;
		}
		// Over-estimated stepping distance is fine, so move on to the next position with over-estimated stepping distance
		else
		{
			te = tk;
			t += Math::Max(tk * e, Epsilon());
		}
	}
	return false;
}

/*!
\brief Segment tracing for a ray
\param ray the ray
\param t returned intersection depth
\param s returned step count
\return true of intersection occured, false otherwise.
*/
bool SegmentTrace(const Ray& ray, double& t, int& s)
{
	// First check intersection with bounding box
	double a, b;
	if (!tree->GetBox().Intersect(ray, a, b))
		return false;

	// Segment tracing using local lipschitz computation
	t = a;
	s = 0;
	double e = 1.0;	// Overstep factor in [1.0, 2.0]
	double c = 1.5;	// Acceleration factor defining the stepping distance increase factor

	// Start with a huge step
	double ts = (b - a);

	// Marching distance used in the previous step 
	double te = 0.0;
	double ce = (e - 1.0);

	while (t < b)
	{
		s++;
		double i = tree->Intensity(ray(t));

		// Got inside
		if (i > 0.0)
			return true;

		Vector pt = ray(t);
		Vector pts = ray(t + ts);
		double k = tree->K(Segment(pt, pts));

		// Safe stepping distance
		double tk = fabs(i) / k;
		tk = Math::Min(tk, ts);
		ts = tk;

		// We moved too far and the Lipschitz check fails: move backward
		if (tk < ce * te)
		{
			t -= ce * te;
			te = 0.0;
		}
		// Over-estimated stepping distance is fine, so move on to the next position with over-estimated stepping distance
		else
		{
			te = Math::Max(tk * e, Epsilon());
			t += te;
		}
		// Try to increase step bound
		ts = tk * c;
	}
	return false;
}

/*!
\brief Compute a pixel color.
\param i pixel coordinate
\param j pixel coordinate
\param k global lipschitz constant used for sphere tracing and enhanced sphere tracing
\param method raytracing method
\param color returned color for the pixel
\param cost returned cost (as a RGBA color) for the pixel
*/
void PixelColor(int i, int j, double k, RayTraceMethod method, Vector& color, Vector& cost)
{
	color = cost = Vector(0);

	// Compute ray
	Ray ray = ComputeRayFromPixel(i, j);

	// Compute intersection
	double t	= 0.0;
	int s		= 0;
	bool hit	= false;
	switch (method)
	{
	case SphereTracing:
		hit = SphereTrace(ray, t, s, k);
		break;
	case EnhancedSphereTracing:
		hit = EnhancedSphereTrace(ray, t, s, k);
		break;
	case SegmentTracing:
		hit = SegmentTrace(ray, t, s);
		break;
	default:
		hit = false;
		break;
	};

	// Compute pixel color
	if (hit)
	{
		// Hit position and normal
		Vector hitPosition = ray(t);
		Vector hitNormal = -Normalized(tree->Gradient(hitPosition));

		// Diffuse lighting
		double NDotL = Math::Max(hitNormal * sunDir, 0.1);
		color = Vector(255 * NDotL, 0, 0);
	}

	// Compute cost
	double c = 0.0;
	c = Math::Min(double(s) / 512, 1.0);
	cost = Vector(0, c * 255.0, 0);
}

/*!
\brief Export an array of pixels in a ppm file.
\param path file path
\param pixels array of RGBA pixels.
\return true of function terminated properly, false otherwise.
*/
bool WriteToFile(const char* path, Vector** pixels)
{
	FILE* fp = NULL;
	fp = fopen(path, "wb");
	if (fp == NULL)
	{
		std::cout << "Couldn't write to file - exiting" << std::endl;
		return false;
	}
	fprintf(fp, "P6\n%d %d\n255\n", imgWidth, imgHeight);
	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{
			static unsigned char color[3];
			Vector c = pixels[j][i];
			color[0] = ((int)c[0]) % 256;
			color[1] = ((int)c[1]) % 256;
			color[2] = ((int)c[2]) % 256;
			(void)fwrite(color, 1, 3, fp);
		}
	}
	fclose(fp);
	return true;
}

/*!
\brief
*/
int main()
{
	// Init pixels
	Vector** pixels = new Vector * [imgWidth];
	Vector** pixelsCost = new Vector * [imgWidth];
	for (int i = 0; i < imgWidth; i++)
	{
		pixels[i] = new Vector[imgHeight];
		pixelsCost[i] = new Vector[imgHeight];
	}

	// Global Lipschitz constant foe sphere tracing and enhanced sphere tracing
	const double k = tree->K();

	//int l = 0;  // Put  this line if Raytrace all methods: sphere tracing, enhanced sphere tracing and segment tracing
	int l = RayTraceMethod::SegmentTracing;	// By default, program will only use segment tracing.
	for (/* empty */; l < RayTraceMethod::COUNT; l++)
	{
		RayTraceMethod method = (RayTraceMethod)l;

		// Compute pixels
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#pragma omp parallel for schedule(dynamic, 16)
		for (int i = 0; i < imgWidth; i++)
		{
			for (int j = 0; j < imgHeight; j++)
			{
				Vector col = Vector(0);
				Vector cost = Vector(0);
				PixelColor(i, j, k, method, col, cost);
				pixels[i][j] = col;
				pixelsCost[i][j] = cost;
			}
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		// Print stats
		std::cout << ((l == 0) ? "SphereTracing" : (l == 1) ? "Enhanced Sphere Tracing" : "Segment Tracing") << std::endl;
		long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
		int seconds = int(double(milliseconds) / 1000.0);
		std::cout << "Time: " << seconds << "s" << milliseconds % 1000 << "ms" << std::endl;

		// Output to ppm files
		char path[40];
		char pathCost[40];
		sprintf(path, "../Renders/render%d.ppm", l);
		sprintf(pathCost, "../Renders/render%d_cost.ppm", l);
		if (!WriteToFile(path, pixels))
			std::cout << "WriteToFile Error - failed to write file 1 to disk" << std::endl;
		if (!WriteToFile(pathCost, pixelsCost))
			std::cout << "WriteToFile Error - failed to write file 2 to disk" << std::endl;
	}

	// Free memory
	for (int i = 0; i < imgHeight; i++)
	{
		delete[] pixels[i];
		delete[] pixelsCost[i];
	}
	delete[] pixels;
	delete[] pixelsCost;

	return 0;
}

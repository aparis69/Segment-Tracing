#pragma once

#include "fundamentals.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <omp.h>

class BlobTreeNode
{
protected:
	Box box;	//!< Bounding box
	double k;	//!< Lipschitz constant

public:
	BlobTreeNode(const Box& b);

	virtual double Intensity(const Vector& p) const = 0;
	virtual Vector Gradient(const Vector& p) const;

	virtual inline double K() const
	{
		return k;
	}

	virtual inline double K(const Segment&) const
	{
		return k;
	}

	inline Box GetBox() const
	{
		return box;
	}

	inline double CubicFalloff(double x, double r) const
	{
		return (x > r) ? 0.0 : (1.0 - x / r) * (1.0 - x / r) * (1.0 - x / r);
	}

	inline double CubicFalloffK(double e, double R) const
	{
		return 1.72 * abs(e) / R;
	}

	inline double CubicFalloffK(double a, double b, double R, double s) const
	{
		if (a > R * R)
			return 0.0;
		if (b < (R * R) / 5.0)
		{
			double t = (1.0 - b / (R * R));
			return abs(s) * 6.0 * (sqrt(b) / (R * R)) * (t * t);
		}
		else if (a > (R * R) / 5.0)
		{
			double t = (1.0 - a / (R * R));
			return abs(s) * 6.0 * (sqrt(a) / (R * R)) * (t * t);
		}
		else
			return CubicFalloffK(s, R);
	}
};

class BlobTreeBlend : public BlobTreeNode
{
protected:
	BlobTreeNode* e[2]; //!< Child nodes.

public:
	BlobTreeBlend(BlobTreeNode* e1, BlobTreeNode* e2);

	double Intensity(const Vector& p) const;
	Vector Gradient(const Vector& p) const;
	double K() const;
	double K(const Segment& s) const;
};

class BlobTreePoint : public BlobTreeNode
{
private:
	Vector c;	//!< Center
	double r;	//!< Radius
	double e;	//!< Energy

public:
	BlobTreePoint(const Vector& pp, double rr, double ee);

	double Intensity(const Vector& p) const;
	double K(const Segment& s) const;

	static BlobTreeNode* BVHRecursive(std::vector<BlobTreeNode*>& pts, int begin, int end);
	static BlobTreeNode* OptimizeHierarchy(std::vector<BlobTreeNode*>& pts, int begin, int end);
	static BlobTreeNode* OptimizeHierarchy(const std::vector<Vector>& c, double r);
};

class BlobTree
{
private:
	BlobTreeNode* root;

public:
	BlobTree();
	BlobTree(BlobTreeNode* rr);
	BlobTree(const char* path);

	double Intensity(const Vector& p);
	Vector Gradient(const Vector& p) const;
	double K() const;
	double K(const Segment& s) const;

	Box GetBox() const;
};

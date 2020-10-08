#pragma once
#include "evector.h"

// Annoying & useless MSVC warning
#pragma warning(disable: 6387 26812)

inline double Epsilon()
{
	return 1e-3;
}

class Ray
{
public:
	Vector o;
	Vector d;

	Ray(const Vector& pp, const Vector& dd);
	Vector operator()(double t) const;
};

class Box
{
private:
	Vector a;
	Vector b;

public:
	Box();
	Box(const Vector& aa, const Vector& bb);
	Box(const Box& b1, const Box& b2);

	bool Inside(const Vector& p) const;
	int Intersect(const Ray& ray, double& tmin, double& tmax, double epsilon = 1e-3) const;
	bool Intersect(const Box& box) const;
	Vector operator[](int i) const;
	Vector Diagonal() const;
	Vector Center() const;
};

class Segment
{
private:
	Vector a;
	Vector b;

public:
	Segment();
	Segment(const Vector& aa, const Vector& bb);

	Vector operator[](int i) const;
	bool Intersect(const Box& box) const;
	Box GetBox() const;
};

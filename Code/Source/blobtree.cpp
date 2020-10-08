#include "blobtree.h"
#include <iostream>


/*!
\brief Constructor for a bounded node.
\param b bounding box
*/
BlobTreeNode::BlobTreeNode(const Box& b)
{
	box = b;
	k = 1.0f;
}

/*
\brief Computes the gradient of the node at a given point in space.
\param p point
*/
Vector BlobTreeNode::Gradient(const Vector& p) const
{
	double x = Intensity(Vector(p[0] + Epsilon(), p[1], p[2])) - Intensity(Vector(p[0] - Epsilon(), p[1], p[2]));
	double y = Intensity(Vector(p[0], p[1] + Epsilon(), p[2])) - Intensity(Vector(p[0], p[1] - Epsilon(), p[2]));
	double z = Intensity(Vector(p[0], p[1], p[2] + Epsilon())) - Intensity(Vector(p[0], p[1], p[2] - Epsilon()));
	return Vector(x, y, z) / (2.0f * Epsilon());
}


/*!
\brief Constructor for a binary blending node.
\param e1 first child
\param e2 second child
*/
BlobTreeBlend::BlobTreeBlend(BlobTreeNode* e1, BlobTreeNode* e2) : BlobTreeNode(Box(e1->GetBox(), e2->GetBox()))
{
	e[0] = e1;
	e[1] = e2;
}

/*!
\brief Computes the intensity of the tree at a given point.
\param p point
*/
double BlobTreeBlend::Intensity(const Vector& p) const
{
	if (!box.Inside(p))
		return 0.0;
	return e[0]->Intensity(p) + e[1]->Intensity(p);
}

/*!
\brief Computes the gradient of the tree at a given point.
\param p point
*/
Vector BlobTreeBlend::Gradient(const Vector& p) const
{
	if (!box.Inside(p))
		return Vector(0.0);
	return e[0]->Gradient(p) + e[1]->Gradient(p);
}

/*!
\brief Computes the global lipschitz constant as a recursive query.
*/
double BlobTreeBlend::K() const
{
	return e[0]->K() + e[1]->K();
}

/*!
\brief Computes the local lipschitz constant over a segment.
\param s segment
*/
double BlobTreeBlend::K(const Segment& s) const
{
	if (!box.Intersect(s.GetBox()))
		return 0.0;
	return e[0]->K(s) + e[1]->K(s);
}


/*!
\brief Constructor for a point primitive.
\param pp center
\param rr radius
\param ee intensity
*/
BlobTreePoint::BlobTreePoint(const Vector& pp, double rr, double ee) : BlobTreeNode(Box(pp - Vector(rr, rr, rr), pp + Vector(rr, rr, rr)))
{
	c = pp;
	r = rr;
	e = ee;
	k = CubicFalloffK(e, r);
}

/*!
\brief Computes the intensity of the tree at a given point.
\param p point
*/
double BlobTreePoint::Intensity(const Vector& p) const
{
	if (!box.Inside(p))
		return 0.0;
	Vector delta = p - c;
	return CubicFalloff(delta * delta, r * r);
}

/*!
\brief Computes the local lipschitz constant over a segment.
\param s segment
*/
double BlobTreePoint::K(const Segment& s) const
{
	Vector a = s[0];
	Vector b = s[1];
	if (!s.Intersect(box))
		return 0.0f;
	Vector axis = Normalized(b - a);
	double l = (c - a) * axis;
	double kk = 0.0;
	if (l < 0.0)
	{
		kk = CubicFalloffK(SquaredNorm(c - a), SquaredNorm(c - b), r, e);
	}
	else if (Norm(b - a) < l)
	{
		kk = CubicFalloffK(SquaredNorm(c - b), SquaredNorm(c - a), r, e);
	}
	else
	{
		double dd = SquaredNorm(c - a) - (l * l);
		Vector pc = a + axis * l;
		kk = CubicFalloffK(dd, Math::Max(SquaredNorm(c - b), SquaredNorm(c - a)), r, e);
	}
	double grad = Math::Max(Math::Abs(axis * Normalized(c - a)), Math::Abs(axis * Normalized(c - b)));
	return kk * grad;
}

/*!
\brief Create a bounding box hierarchy.
\param pts Set of nodes.
\param begin, end Indexes of the nodes that should be organized into the hierarchy.
*/
BlobTreeNode* BlobTreePoint::BVHRecursive(std::vector<BlobTreeNode*>& pts, int begin, int end)
{
	/*
	\brief BVH space partition predicate. Could also use a lambda function to avoid the structure.
	*/
	struct BVHPartitionPredicate
	{
		int axis;
		double cut;

		BVHPartitionPredicate(int a, double c) : axis(a), cut(c)
		{
		}

		bool operator()(BlobTreeNode* p) const
		{
			return (p->GetBox().Center()[axis] < cut);
		}
	};

	// If leaf, returns primitive
	int nodeCount = end - begin;
	if (nodeCount <= 1)
		return pts[begin];

	// Bounding box of primitive in [begin, end] range
	Box bbox = pts[begin]->GetBox();
	for (int i = begin + 1; i < end; i++)
		bbox = Box(bbox, pts[i]->GetBox());

	// Find the most stretched axis of the bounding box
	// Cut the box in the middle of this stretched axis
	int stretchedAxis = bbox.Diagonal().MaxIndex();
	double axisMiddleCut = (bbox[0][stretchedAxis] + bbox[1][stretchedAxis]) / 2.0f;

	// Partition our primitives in relation to the axisMiddleCut
	auto pmid = std::partition(pts.begin() + begin, pts.begin() + end, BVHPartitionPredicate(stretchedAxis, axisMiddleCut));

	// Ensure the partition is not degenerate : all primitives on the same side
	int midIndex = int(std::distance(pts.begin(), pmid));
	if (midIndex == begin || midIndex == end)
		midIndex = (begin + end) / 2;

	// Recursive construction of sub trees
	BlobTreeNode* left = BVHRecursive(pts, begin, midIndex);
	BlobTreeNode* right = BVHRecursive(pts, midIndex, end);

	// Blend of the two child nodes
	return new BlobTreeBlend(left, right);
}

/*!
\brief Recursive BVH Tree construction from a vector<TNode*>.
\param begin start index
\param end end index
*/
BlobTreeNode* BlobTreePoint::OptimizeHierarchy(std::vector<BlobTreeNode*>& pts, int begin, int end)
{
	if (pts.empty())
		return nullptr;
	return BVHRecursive(pts, begin, end);
}

/*!
\brief Entry point of the BVH construction.
\param c vector of position in world space, for all sphere primitives
\param r radius for all primitives.
*/
BlobTreeNode* BlobTreePoint::OptimizeHierarchy(const std::vector<Vector>& c, double r)
{
	std::vector<BlobTreeNode*> all(c.size());
	for (int i = 0; i < c.size(); i++)
		all[i] = new BlobTreePoint(c[i], r, 1.0f);
	return OptimizeHierarchy(all, 0, int(all.size()));
}


/*!
\brief Default constructor.
*/
BlobTree::BlobTree()
{
	root = nullptr;
}

/*!
\brief Constructor from a node.
\param rr root node
*/
BlobTree::BlobTree(BlobTreeNode* rr)
{
	root = rr;
}

/*!
\brief Utility constructor for building the tree from a file containing sphere primitives.
\param path file path
*/
BlobTree::BlobTree(const char* path)
{
	std::vector<Vector> centers;
	std::ifstream inFile;
	inFile.open(path);
	if (!inFile)
	{
		std::cout << "Unable to open particle file - exiting." << std::endl;
		root = nullptr;
		return;
	}
	for (std::string line; std::getline(inFile, line); /* empty */)
	{
		std::istringstream in(line);
		double x, y, z;
		in >> x >> y >> z;
		centers.push_back(Vector(x, y, z)); // Upscale all the vector field
	}
	root = BlobTreePoint::OptimizeHierarchy(centers, 2.25);	// Hardcoded radius for the file
	std::cout << "Primitive count: " << centers.size() << std::endl << std::endl;
}

/*!
\brief Computes the intensity of the tree at a given point.
\param p point
*/
double BlobTree::Intensity(const Vector& p)
{
	return root->Intensity(p) - 0.5f;
}

/*!
\brief Computes the gradient of the tree at a given point.
\param p point
*/
Vector BlobTree::Gradient(const Vector& p) const
{
	return root->Gradient(p);
}

/*!
\brief Computes the global lipschitz constant as a recursive query.
*/
double BlobTree::K() const
{
	return root->K();
}

/*!
\brief Computes the local lipschitz constant over a segment.
\param s segment
*/
double BlobTree::K(const Segment& s) const
{
	return root->K(s);
}

/*!
\brief Computes and returns the bounding box of the construction tree, as a recursive query.
*/
Box BlobTree::GetBox() const
{
	return root->GetBox();
}

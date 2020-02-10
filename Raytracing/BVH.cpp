#include "BVH.h"
#include "Utilities.h"

#include <algorithm>

BVH::BVH(std::vector<Triangle>* _tris)
{
	tris = _tris;

	//bb.Load(*tris);
}

BVH::~BVH()
{
	tris = nullptr;
}

void BVH::Create(std::vector<Triangle>* _tris)
{
	tris = _tris;

	//bb.Load(*tris);
}

void BVH::Build()
{
	node = std::make_unique<Node>();
	node->tris = *tris;

	Build(node.get());

	//SplitAndSortAxis(node.get());
	//FindSplitCost(node.get());

	//if (node->splitCost.cost < leafCost)
	//	node->isLeaf = true;
	//else
	//{
	//	node->leftNode = std::make_unique<Node>();
	//	node->rightNode = std::make_unique<Node>();

	//	std::vector<Triangle> leftTris;
	//	std::vector<Triangle> rightTris;

	//	for (size_t i = 0; i < node->tris->size(); ++i)
	//	{
	//		if (node->tris->at(i).MinValue(node->splitCost.axis) < node->splitCost.splitPoint)
	//		{
	//			if (!node->leftNode->tris)
	//			{
	//				node->leftNode->tris = new std::vector<Triangle>();
	//			}
	//			(*node->leftNode->tris).push_back(tris->at(i));
	//		}
	//		else
	//		{
	//			if (!node->rightNode->tris)
	//			{
	//				node->rightNode->tris = new std::vector<Triangle>();
	//			}
	//			(*node->rightNode->tris).push_back(tris->at(i));
	//		}
	//	}

	//	Build(node->leftNode.get());
	//	Build(node->rightNode.get());
	//}
}

void BVH::Build(Node* _node) {

	_node->bb.Load(_node->tris);
	SplitAndSortAxis(_node);
	FindSplitCost(_node);

	if (_node->splitCost.cost < leafCost)
	{
		_node->isLeaf = true;
	}
	else
	{
		_node->leftNode = std::make_unique<Node>(_node->level + 1);
		_node->rightNode = std::make_unique<Node>(_node->level + 1);

		_node->leftNode->left = true;
		_node->rightNode->left = false;

		_node->leftNode->parentLeft = _node->left;
		_node->rightNode->parentLeft = _node->left;

		for (size_t i = 0; i < _node->tris.size(); ++i)
		{
			if (_node->tris.at(i).MinValue(_node->splitCost.axis) < _node->splitCost.splitPoint)
			{
				_node->leftNode->tris.push_back(_node->tris.at(i));
			}
			else
			{
				_node->rightNode->tris.push_back(_node->tris.at(i));
			}
		}

		if (!_node->leftNode->tris.empty())
			Build(_node->leftNode.get());
		else
			_node->leftNode = nullptr;

		if (!_node->rightNode->tris.empty())
			Build(_node->rightNode.get());
		else
			_node->rightNode = nullptr;
	}
}

void BVH::SplitAndSortAxis(Node* _node)
{
	std::vector<float> xAxis;
	std::vector<float> yAxis;
	std::vector<float> zAxis;

	for (size_t i = 0; i < _node->tris.size(); ++i)
	{
		xAxis.push_back(_node->tris.at(i).MinValue(0));
		yAxis.push_back(_node->tris.at(i).MinValue(1));
		zAxis.push_back(_node->tris.at(i).MinValue(2));
	}

	std::sort(xAxis.begin(), xAxis.end());
	std::sort(yAxis.begin(), yAxis.end());
	std::sort(zAxis.begin(), zAxis.end());

	_node->axisSplits.at(0) = xAxis;
	_node->axisSplits.at(1) = yAxis;
	_node->axisSplits.at(2) = zAxis;
}

void BVH::FindSplitCost(Node* _node)
{
	SplitCost minSplitCost;
	float cost;
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < _node->axisSplits.at(i).size(); ++j)
		{
			auto splitVal = _node->axisSplits.at(i)[j];
			auto pertMin = Utilities::fPercentageFromMin(_node->bb.min.axis[i], _node->bb.max.axis[i], splitVal);
			auto leftSide = (pertMin *
				TrisLessThan(_node, i, splitVal) * 2.0f);

			auto pertMax = Utilities::fPercentageFromMax(_node->bb.min.axis[i], _node->bb.max.axis[i], splitVal);
			auto rightSide = (pertMax *
				TrisMoreThan(_node, i, splitVal) * 2.0f);

			cost = 1.0f + leftSide + rightSide;

			if (cost < minSplitCost.cost)
			{
				minSplitCost.axis = i;
				minSplitCost.splitPoint = _node->axisSplits.at(i)[j];
				minSplitCost.cost = cost;
			}
		}
	}

	_node->splitCost.cost = minSplitCost.cost;
	_node->splitCost.axis = minSplitCost.axis;
	_node->splitCost.splitPoint = minSplitCost.splitPoint;
}

void BVH::SplitByTris()
{
	std::vector<float> xAxis;
	std::vector<float> yAxis;
	std::vector<float> zAxis;

	for (size_t i = 0; i < tris->size(); ++i)
	{
		xAxis.push_back(tris->at(i).MinValue(0));
		yAxis.push_back(tris->at(i).MinValue(1));
		zAxis.push_back(tris->at(i).MinValue(2));
	}

	std::sort(xAxis.begin(), xAxis.end());
	std::sort(yAxis.begin(), yAxis.end());
	std::sort(zAxis.begin(), zAxis.end());

	axisSplits.at(0) = xAxis;
	axisSplits.at(1) = yAxis;
	axisSplits.at(2) = zAxis;

	SplitCost minSplitCost;
	float cost;
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < axisSplits.at(i).size(); ++j)
		{
			auto pertMin = Utilities::fPercentageFromMin(bb.min.axis[i], bb.max.axis[i], axisSplits.at(i)[j]);
			auto leftSide = (pertMin *
				TrisLessThan(i, axisSplits.at(i)[j]) * 2.0f);

			auto pertMax = Utilities::fPercentageFromMax(bb.min.axis[i], bb.max.axis[i], axisSplits.at(i)[j]);
			auto rightSide = (pertMax *
				TrisMoreThan(i, axisSplits.at(i)[j]) * 2.0f);

			cost = 1.0f + leftSide + rightSide;

			if (cost < minSplitCost.cost)
			{
				minSplitCost.axis = i;
				minSplitCost.splitPoint = axisSplits.at(i)[j];
				minSplitCost.cost = cost;
			}
		}
	}

	std::vector<Triangle>leftTris;
	std::vector<Triangle>rightTris;

	for (size_t i = 0; i < tris->size(); ++i)
	{
		if (tris->at(i).MinValue(minSplitCost.axis) < minSplitCost.splitPoint)
		{
			leftTris.push_back(tris->at(i));
		}
		else
		{
			rightTris.push_back(tris->at(i));
		}
	}

	float costy = minSplitCost.cost;

}

int BVH::TrisMoreThan(Node* _node, int axis, float value)
{
	int trisNum = 0;
	for (size_t i = 0; i < _node->tris.size(); ++i)
	{
		if (_node->tris.at(i).MinValue(axis) >= value)
		{
			++trisNum;
		}
	}

	return trisNum;
}

int BVH::TrisLessThan(Node* _node, int axis, float value)
{
	int trisNum = 0;
	for (size_t i = 0; i < _node->tris.size(); ++i)
	{
		if (_node->tris.at(i).MinValue(axis) < value)
		{
			++trisNum;
		}
	}

	return trisNum;
}

int BVH::TrisMoreThan(int axis, float value)
{
	int trisNum = 0;
	for (size_t i = 0; i < tris->size(); ++i)
	{
		if (tris->at(i).MinValue(axis) >= value)
		{
			++trisNum;
		}
	}

	return trisNum;
}

int BVH::TrisLessThan(int axis, float value)
{
	int trisNum = 0;
	for (size_t i = 0; i < tris->size(); ++i)
	{
		if (tris->at(i).MinValue(axis) < value)
		{
			++trisNum;
		}
	}

	return trisNum;
}

void BVH::Search(const Ray& ray, std::vector<HitRecord>& hits)
{
	Search(ray, hits, node.get());
}

void BVH::Search(const Ray& ray, std::vector<HitRecord>& hits, Node* _node)
{

	if (RayBoxIntersect(ray, _node->bb))
	{
		if (_node->isLeaf)
		{
			for (size_t i = 0; i < _node->tris.size(); ++i)
			{
				float t, u, v, w;
				if (_node->tris[i].Hit(ray, t, u, v))
				{
					HitRecord hit;
					hit.t = t;
					w = (1 - u - v);
					hit.colour = (Vec3(_node->tris[i].vertices[0].colour) * u) +
						(Vec3(_node->tris[i].vertices[1].colour) * v) +
						(Vec3(_node->tris[i].vertices[2].colour) * w);
					hits.push_back(hit);
				}
			}
		}
		else
		{
			if(_node->leftNode)
				Search(ray, hits, _node->leftNode.get());

			if (_node->rightNode)
				Search(ray, hits, _node->rightNode.get());
		}
	}
}

bool BVH::RayBoxIntersect(const Ray& r, const Bounds& _bb)
{
	//float tmin, tmax, tymin, tymax, tzmin, tzmax;

	float tmin = (_bb.min.x() - r.origin.x()) / r.direction.x();
	float tmax = (_bb.max.x() - r.origin.x()) / r.direction.x();

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (_bb.min.y() - r.origin.y()) / r.direction.y();
	float tymax = (_bb.max.y() - r.origin.y()) / r.direction.y();

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (_bb.min.z() - r.origin.z()) / r.direction.z();
	float tzmax = (_bb.max.z() - r.origin.z()) / r.direction.z();

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return true;
}

//void BVH::Destroy()
//{
//	delete node.get();
//	node = nullptr;
//}

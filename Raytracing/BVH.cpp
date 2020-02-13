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
				_node->leftNode->tris.emplace_back(_node->tris.at(i));
			}
			else
			{
				_node->rightNode->tris.emplace_back(_node->tris.at(i));
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
		xAxis.emplace_back(_node->tris.at(i).MinValue(0));
		yAxis.emplace_back(_node->tris.at(i).MinValue(1));
		zAxis.emplace_back(_node->tris.at(i).MinValue(2));
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

			if (cost < _node->splitCost.cost)
			{
				_node->splitCost.axis = i;
				_node->splitCost.splitPoint = _node->axisSplits.at(i)[j];
				_node->splitCost.cost = cost;
			}
		}
	}

	//_node->splitCost.cost = minSplitCost.cost;
	//_node->splitCost.axis = minSplitCost.axis;
	//_node->splitCost.splitPoint = minSplitCost.splitPoint;
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

void BVH::Search(const Ray& ray, std::vector<HitRecord>& hits, Image& img)
{
	Search(ray, hits, node.get(), img);
}

void BVH::Search(const Ray& ray, std::vector<HitRecord>& hits, Node* _node, Image& img)
{
	if (_node->bb.RayBoxIntersect(ray))
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
					w = (1.0f - u - v);

					Vec2 texCo(_node->tris[i].vertices[0].texCoord * u + 
						_node->tris[i].vertices[1].texCoord * v + 
						_node->tris[i].vertices[2].texCoord * w);

					hit.colour = Vec3(img.GetColour(texCo.axis[0], texCo.axis[1]));
					hits.emplace_back(hit);
				}
			}
		}
		else
		{
			if(_node->leftNode)
				Search(ray, hits, _node->leftNode.get(), img);

			if (_node->rightNode)
				Search(ray, hits, _node->rightNode.get(), img);
		}
	}
}

//void BVH::Destroy()
//{
//	delete node.get();
//	node = nullptr;
//}

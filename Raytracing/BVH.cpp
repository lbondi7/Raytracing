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

static int TrisMoreThan(Node* _node, int axis, float value)
{
	int trisNum = 0;
	for (size_t i = 0; i < _node->tris.size(); ++i)
	{
		if (_node->tris.at(i).minPoints[axis] >= value)
		{
			++trisNum;
		}
	}

	return trisNum;
}

static int TrisLessThan(Node* _node, int axis, float value)
{
	int trisNum = 0;
	for (size_t i = 0; i < _node->tris.size(); ++i)
	{
		if (_node->tris.at(i).minPoints[axis] < value)
		{
			++trisNum;
		}
	}

	return trisNum;
}

static bool sort(const std::pair<int, float>& pair1, const std::pair<int, float>& pair2)
{
	return pair1.second < pair2.second;
}

//static bool sortY(const std::pair<int, float>& pair1, const std::pair<int, float>& pair2)
//{
//	return pair1.minPoints[1] < pair2.minPoints[1];
//}
//
//
//static bool sortZ(const std::pair<int, float>& pair1, const std::pair<int, float>& pair2)
//{
//	return pair1.minPoints[2] < pair2.minPoints[2]
//}

static bool sortTriX(const Triangle& tri1, const Triangle& tri2)
{
	return tri1.minPoints[0] < tri2.minPoints[0];
}

static bool sortTriY(const Triangle& tri1, const Triangle& tri2)
{
	return tri1.minPoints[1] < tri2.minPoints[1];
}


static bool sortTriZ(const Triangle& tri1, const Triangle& tri2)
{
	return tri1.minPoints[2] < tri2.minPoints[2];
}

static void BuildNode(Node* _node, float leafCost) {

	_node->bb.Load(_node->tris);


	/*************Find Splits***************/


	std::vector<int> place;
	int amount = int(std::ceil(_node->tris.size() / 8.0f));

	for (size_t i = 0; i < _node->tris.size(); i += 20)
	{
		_node->axisSplits.at(0).emplace_back(_node->tris.at(i).minPoints[0]);
		_node->axisSplits.at(1).emplace_back(_node->tris.at(i).minPoints[1]);
		_node->axisSplits.at(2).emplace_back(_node->tris.at(i).minPoints[2]);
		place.emplace_back(i);
	}

	std::sort(_node->axisSplits.at(0).begin(), _node->axisSplits.at(0).end());
	std::sort(_node->axisSplits.at(1).begin(), _node->axisSplits.at(1).end());
	std::sort(_node->axisSplits.at(2).begin(), _node->axisSplits.at(2).end());


	/**************************************/


	/*************Find Split Cost***************/

	auto trisSize = _node->tris.size();
	float cost;
	float traverseValue = 1.0f;
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < _node->axisSplits.at(i).size(); ++j)
		{

			//auto leftSide = (Utilities::fPercentageFromMin(_node->bb.min.axis[i], _node->bb.max.axis[i], _node->axisSplits.at(i)[j]) *
			//	TrisLessThan(_node, i, _node->axisSplits.at(i)[j]) * traverseValue);

			//auto rightSide = (Utilities::fPercentageFromMax(_node->bb.min.axis[i], _node->bb.max.axis[i], _node->axisSplits.at(i)[j]) *
			//	TrisMoreThan(_node, i, _node->axisSplits.at(i)[j]) * traverseValue);

			auto leftSide = (Utilities::fPercentageFromMin(_node->bb.min.axis[i], _node->bb.max.axis[i], _node->axisSplits.at(i)[j]) *
				place[j] * traverseValue);

			auto rightSide = (Utilities::fPercentageFromMax(_node->bb.min.axis[i], _node->bb.max.axis[i], _node->axisSplits.at(i)[j]) *
				(trisSize - place[j]) * traverseValue);

			cost = 0.1f + leftSide + rightSide;

			if (cost < _node->splitCost.cost)
			{
				_node->splitCost.axis = i;
				_node->splitCost.splitPoint = _node->axisSplits.at(i)[j];
				_node->splitCost.cost = cost;
			}
		}
	}

	/**************************************/

	if (_node->splitCost.cost < leafCost)
	{
		_node->isLeaf = true;
	}
	else
	{
		_node->leftNode = std::make_unique<Node>();
		_node->rightNode = std::make_unique<Node>();

		if (_node)
		{

			switch (_node->splitCost.axis)
			{
			case 0:
				std::sort(_node->tris.begin(), _node->tris.end(), sortTriX);
				break;
			case 1:
				std::sort(_node->tris.begin(), _node->tris.end(), sortTriY);
				break;
			case 2:
				std::sort(_node->tris.begin(), _node->tris.end(), sortTriZ);
				break;
			}

			//auto it = _node->tris.begin();// +_node->splitCost.splitPointNum - 1;
			//it += _node->tris.at(_node->splitCost.splitPointNum - 1);

			for (size_t i = 0; i < _node->tris.size(); ++i)
			{
				if (_node->tris.at(i).minPoints[_node->splitCost.axis] > _node->splitCost.splitPoint)
				{
					_node->leftNode->tris.insert(_node->leftNode->tris.begin(), _node->tris.begin(), _node->tris.begin() + (i - 1));
					_node->rightNode->tris.insert(_node->rightNode->tris.begin(), _node->tris.begin() + i, _node->tris.end());
					break;
				}
			}

			//for (size_t i = 0; i < _node->tris.size(); ++i)
			//{
			//	if (_node->tris.at(i).minPoints[_node->splitCost.axis] < _node->splitCost.splitPoint)
			//	{
			//		_node->leftNode->tris.emplace_back(_node->tris.at(i));
			//	}
			//	else
			//	{
			//		_node->rightNode->tris.emplace_back(_node->tris.at(i));
			//	}
			//}

			if (!_node->leftNode->tris.empty())
			{
				std::thread(BuildNode, _node->leftNode.get(), leafCost).detach();
			}
			else
				_node->leftNode = nullptr;

			if (!_node->rightNode->tris.empty())
			{
				std::thread(BuildNode, _node->rightNode.get(), leafCost).detach();
				//Build(_node->rightNode.get(), leafCost);
			}
			else
				_node->rightNode = nullptr;
		}
	}
}

void BVH::Build()
{
	node = std::make_unique<Node>();
	node->tris = *tris;

	//BuildNode(node.get(), leafCost);
	//std::lock_guard<std::mutex> lock(mtx);
	std::thread(BuildNode, node.get(), leafCost).join();
	
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
		_node->leftNode = std::make_unique<Node>();
		_node->rightNode = std::make_unique<Node>();

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
			BVH::Build(_node->rightNode.get());
		else
			_node->rightNode = nullptr;
	}
}

void BVH::SplitAndSortAxis(Node* _node)
{
	std::vector<float> xAxis;
	std::vector<float> yAxis;
	std::vector<float> zAxis;

	for (size_t i = 0; i < _node->tris.size(); i += 3)
	{
		xAxis.emplace_back(_node->tris.at(i).MinValue(0));
		yAxis.emplace_back(_node->tris.at(i).MinValue(1));
		zAxis.emplace_back(_node->tris.at(i).MinValue(2));
	}

	std::sort(xAxis.begin(), xAxis.end());
	std::sort(yAxis.begin(), yAxis.end());
	std::sort(zAxis.begin(), zAxis.end());

	//_node->axisSplits.at(0) = xAxis;
	//_node->axisSplits.at(1) = yAxis;
	//_node->axisSplits.at(2) = zAxis;
}

void BVH::FindSplitCost(Node* _node)
{
	float cost;
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < _node->axisSplits.at(i).size(); ++j)
		{
			//auto splitVal = _node->axisSplits.at(i)[j];
			//auto pertMin = Utilities::fPercentageFromMin(_node->bb.min.axis[i], _node->bb.max.axis[i], splitVal);
			//auto leftSide = (pertMin *
			//	TrisLessThan(_node, i, splitVal) * 2.0f);

			//auto pertMax = Utilities::fPercentageFromMax(_node->bb.min.axis[i], _node->bb.max.axis[i], splitVal);
			//auto rightSide = (pertMax *
			//	TrisMoreThan(_node, i, splitVal) * 2.0f);

			//cost = 1.0f + leftSide + rightSide;

			//if (cost < _node->splitCost.cost)
			//{
			//	_node->splitCost.axis = i;
			//	_node->splitCost.splitPoint = _node->axisSplits.at(i)[j];
			//	_node->splitCost.cost = cost;
			//}
		}
	}
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

static void SearchBVH(Ray ray, std::vector<HitRecord>* hits, Node* _node, Image img)
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
					(*hits).emplace_back(hit);
				}
			}
		}
		else
		{
			if (_node->leftNode)
				std::thread(SearchBVH, ray, hits, _node->leftNode.get(), img).detach();

			if (_node->rightNode)
				std::thread(SearchBVH, ray, hits, _node->rightNode.get(), img).detach();

		}
	}
}


void BVH::Search(const Ray& ray, std::vector<HitRecord>& hits, Image& img)
{
	//std::thread(SearchBVH, ray, hits, node.get(), img).detach();
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

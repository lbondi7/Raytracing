#include "BVH.h"
#include "Utilities.h"
#include "Constants.h"
#include "Locator.h"
#include "ThreadManager.h"
#include "DebugPrinter.h"

#include <algorithm>


const Vec3 LIGHT_POS = Vec3(-10.0f, 3.0f, -20.0f);

BVH::BVH(std::vector<Triangle>* _tris)
{
	//tris = _tris;
	tris->insert(tris->begin(), _tris->begin(), _tris->end());

	//bb.Load(*tris);
}

BVH::~BVH()
{
	tris = nullptr;
}

void BVH::Create(std::vector<Triangle>* _tris)
{
	tris = _tris;

	objectBVH = true;

	//bb.Load(*tris);
}

//void BVH::Create(std::vector<Object>* _objs)
//{
//	objects = _objs;
//
//	objectBVH = false;
//	//bb.Load(*tris);
//}

static int TrisMoreThan(Node* _node, int axis, float value)
{
	int trisNum = 0;
	for (size_t i = 0; i < _node->tris.size(); ++i)
	{
		if (_node->tris.at(i)->minPoints[axis] >= value)
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
		if (_node->tris.at(i)->minPoints[axis] < value)
		{
			++trisNum;
		}
	}

	return trisNum;
}

static bool sortTriX(const Triangle* tri1, const Triangle* tri2)
{
	return tri1->minPoints[0] < tri2->minPoints[0];
}

static bool sortTriY(const Triangle* tri1, const Triangle* tri2)
{
	return tri1->minPoints[1] < tri2->minPoints[1];
}


static bool sortTriZ(const Triangle* tri1, const Triangle* tri2)
{
	return tri1->minPoints[2] < tri2->minPoints[2];
}

static void GetCost(Node* _node, std::vector<int> place, int axis, SplitCost* _splitCost)
{
	float tmpCost = INFINITY;
	auto travCost = 1.0f;
	for (size_t j = 0; j < _node->axisSplits.at(axis).size(); ++j)
	{
		auto leftSide = (Utilities::fPercentageFromMin(_node->bb.min.axis[axis], _node->bb.max.axis[axis], _node->axisSplits[axis][j]) *
			place[j] * travCost);

		auto rightSide = (Utilities::fPercentageFromMax(_node->bb.min.axis[axis], _node->bb.max.axis[axis], _node->axisSplits[axis][j]) *
			(_node->tris.size() - place[j]) * travCost);

		tmpCost = 1.0f + leftSide + rightSide;
		if (tmpCost < _splitCost->cost)
		{
			_splitCost->cost = tmpCost;
			_splitCost->splitPoint = _node->axisSplits[axis][j];
			_splitCost->axis = axis;
		}
	}
}

#if THREADED_BUILD
static void StaticBuildNode(Node* _node) {

	_node->bb.Load(_node->tris);

	/*************Find Splits***************/

	std::vector<int> place;
	//int amount = int(std::ceil(_node->tris.size() / 8.0f));

	for (size_t i = 0; i < _node->tris.size(); i += 20)
	{
		_node->axisSplits.at(0).emplace_back(_node->tris.at(i)->minPoints[0]);
		_node->axisSplits.at(1).emplace_back(_node->tris.at(i)->minPoints[1]);
		_node->axisSplits.at(2).emplace_back(_node->tris.at(i)->minPoints[2]);
		place.emplace_back(i);
	}

	std::sort(_node->axisSplits.at(0).begin(), _node->axisSplits.at(0).end());
	std::sort(_node->axisSplits.at(1).begin(), _node->axisSplits.at(1).end());
	std::sort(_node->axisSplits.at(2).begin(), _node->axisSplits.at(2).end());

	/***************************************/

	/*************Find Split Cost***************/

	SplitCost splitCostX;
	SplitCost splitCostY;
	SplitCost splitCostZ;

	int t1, t2, t3;
	Locator::GetTM()->SetTask(false, t1) = std::bind(GetCost, _node, place, 0, &splitCostX);
	Locator::GetTM()->SetTask(false, t2) = std::bind(GetCost, _node, place, 1, &splitCostY);
	Locator::GetTM()->SetTask(false, t3) = std::bind(GetCost, _node, place, 2, &splitCostZ);

	std::vector<int> ths{t1, t2, t3};
	Locator::GetTM()->WaitForThreads(ths);


	_node->splitCost.Compare(splitCostX, splitCostY, splitCostZ);

	/*******************************************/

	if (_node->splitCost.cost < 600.0f)
	{
		_node->isLeaf = true;
	}
	else
	{
		_node->leftNode = std::make_unique<Node>();
		_node->rightNode = std::make_unique<Node>();

		_node->leftNode->level = _node->level + 1;
		_node->rightNode->level = _node->level + 1;


		/*************Sort Tris on Axis***************/

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

		/*********************************************/

		/*************Insert Tris into Left and Right Node***************/

		for (size_t i = 0; i < _node->tris.size(); ++i)
		{
			if (_node->tris.at(i)->minPoints[_node->splitCost.axis] > _node->splitCost.splitPoint)
			{
				_node->leftNode->tris.insert(_node->leftNode->tris.begin(), _node->tris.begin(), _node->tris.begin() + i);
				_node->rightNode->tris.insert(_node->rightNode->tris.begin(), _node->tris.begin() + (i + 1), _node->tris.end());
				break;
			}
		}

		/****************************************************************/

		/*************Build Left/Right Nodes***************/

		if (_node->level != 0)
		{
			if (!_node->leftNode->tris.empty())
				StaticBuildNode(_node->leftNode.get());
			else
				_node->leftNode = nullptr;

			if (!_node->rightNode->tris.empty())
				StaticBuildNode(_node->rightNode.get());
			else
				_node->rightNode = nullptr;
		}
		else
		{
			if (!_node->leftNode->tris.empty())
				Locator::GetTM()->SetTask() = std::bind(StaticBuildNode, _node->leftNode.get());
			else
				_node->leftNode = nullptr;

			if (!_node->rightNode->tris.empty())
				Locator::GetTM()->SetTask() = std::bind(StaticBuildNode, _node->rightNode.get());
			else
				_node->rightNode = nullptr;

			Locator::GetTM()->WaitForAllThreads();
		}
		/**************************************************/
	}
}
#else

void BVH::BuildNode(Node* _node) {

	_node->bb.Load(_node->tris);

	/*************Find Splits***************/

	std::vector<int> place;
	//int amount = int(std::ceil(_node->tris.size() / 8.0f));

	for (size_t i = 0; i < _node->tris.size(); i += 20)
	{
		_node->axisSplits.at(0).emplace_back(_node->tris.at(i)->minPoints[0]);
		_node->axisSplits.at(1).emplace_back(_node->tris.at(i)->minPoints[1]);
		_node->axisSplits.at(2).emplace_back(_node->tris.at(i)->minPoints[2]);
		place.emplace_back(i);
	}

	std::sort(_node->axisSplits.at(0).begin(), _node->axisSplits.at(0).end());
	std::sort(_node->axisSplits.at(1).begin(), _node->axisSplits.at(1).end());
	std::sort(_node->axisSplits.at(2).begin(), _node->axisSplits.at(2).end());

	/***************************************/

	/*************Find Split Cost***************/

	SplitCost splitCostX;
	SplitCost splitCostY;
	SplitCost splitCostZ;

	GetCost(_node, place, 0, &splitCostX);
	GetCost(_node, place, 1, &splitCostY);
	GetCost(_node, place, 2, &splitCostZ);

	_node->splitCost.Compare(splitCostX, splitCostY, splitCostZ);

	/*******************************************/

	if (_node->splitCost.cost < leafCost)
	{
		_node->isLeaf = true;
	}
	else
	{
		_node->leftNode = std::make_unique<Node>();
		_node->rightNode = std::make_unique<Node>();


		/*************Sort Tris on Axis***************/

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

		/*********************************************/

		/*************Insert Tris into Left and Right Node***************/

		for (size_t i = 0; i < _node->tris.size(); ++i)
		{
			if (_node->tris.at(i)->minPoints[_node->splitCost.axis] > _node->splitCost.splitPoint)
			{
				_node->leftNode->tris.insert(_node->leftNode->tris.begin(), _node->tris.begin(), _node->tris.begin() + i);
				_node->rightNode->tris.insert(_node->rightNode->tris.begin(), _node->tris.begin() + (i + 1), _node->tris.end());
				break;
			}
		}

		/****************************************************************/

		/*************Build Left/Right Nodes***************/
		if (!_node->leftNode->tris.empty())
			BuildNode(_node->leftNode.get());
		else
			_node->leftNode = nullptr;

		if (!_node->rightNode->tris.empty())
			BuildNode(_node->rightNode.get());
		else
			_node->rightNode = nullptr;
		/**************************************************/
	}
}
#endif

void BVH::Build()
{
	node = std::make_unique<Node>();

	for (auto& tri : *tris)
	{
		node->tris.emplace_back(&tri);
	}

	//node->tris.insert(node->tris.begin(), &tris->begin(), &tris->end());
	//node->tris = *tris;

	//Build(node.get());

#if THREADED_BUILD
	StaticBuildNode(node.get());
#else
	BuildNode(node.get());
#endif // THREADED_BUILD
}

void BVH::Build(Node* _node) {

	//_node->bb.Load(_node->tris);
	//SplitAndSortAxis(_node);
	//FindSplitCost(_node);

	//if (_node->splitCost.cost < leafCost)
	//{
	//	_node->isLeaf = true;
	//}
	//else
	//{
	//	_node->leftNode = std::make_unique<Node>();
	//	_node->rightNode = std::make_unique<Node>();

	//	for (size_t i = 0; i < _node->tris.size(); ++i)
	//	{
	//		if (_node->tris.at(i)->minPoints[_node->splitCost.axis] < _node->splitCost.splitPoint)
	//		{
	//			_node->leftNode->tris.emplace_back(&_node->tris.at(i));
	//		}
	//		else
	//		{
	//			_node->rightNode->tris.emplace_back(&_node->tris.at(i));
	//		}
	//	}

	//	if (!_node->leftNode->tris.empty())
	//		Build(_node->leftNode.get());
	//	else
	//		_node->leftNode = nullptr;

	//	if (!_node->rightNode->tris.empty())
	//		BVH::Build(_node->rightNode.get());
	//	else
	//		_node->rightNode = nullptr;
	//}
}

void BVH::SplitAndSortAxis(Node* _node)
{
	std::vector<float> xAxis;
	std::vector<float> yAxis;
	std::vector<float> zAxis;

	for (size_t i = 0; i < _node->tris.size(); i += 20)
	{
		xAxis.emplace_back(_node->tris.at(i)->minPoints[0]);
		yAxis.emplace_back(_node->tris.at(i)->minPoints[1]);
		zAxis.emplace_back(_node->tris.at(i)->minPoints[2]);
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
}

int BVH::TrisMoreThan(Node* _node, int axis, float value)
{
	int trisNum = 0;
	for (size_t i = 0; i < _node->tris.size(); ++i)
	{
		if (_node->tris.at(i)->minPoints[axis] >= value)
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
		if (_node->tris.at(i)->minPoints[axis] < value)
		{
			++trisNum;
		}
	}

	return trisNum;
}

void BVH::Search(Ray& ray, std::vector<HitRecord>& hits, std::vector<Light>& lights, Image& img)
{
	//std::thread(SearchBVH, ray, hits, node.get(), img).detach();
	Search(ray, hits, node.get(), lights, img);
}

void BVH::Search(const Ray& ray, std::vector<HitRecord>& hits)
{
	//std::thread(SearchBVH, ray, hits, node.get(), img).detach();
	Search(ray, hits, node.get());
}

void BVH::Search(Ray& ray, std::vector<HitRecord>& hits, Node* _node, std::vector<Light>& lights, Image& img)
{
	if (_node->bb.RayBoxIntersect(ray))
	{
		if (_node->isLeaf)
		{
			for (auto& tri : _node->tris)
			{
				float t, u, v;
				if (tri->MTHit(ray, t, u, v))
				{
					//for(auto& h : hits)
					//{
					//	if(hit )
					//}

					HitRecord hit;
					hit.t = t;
					hit.point = ray.origin + hit.t * ray.direction;
					hit.tri_id = tri->tri_id;
					hit.obj_id = tri->obj_id;

					if (tri->isLight)
					{
						hit.colour = Vec3(1.0f, 0.6f, 0.0f);
						hits.emplace_back(hit);
						continue;
					}
					float w = (1.0f - u - v);

					for (auto& l : lights)
					{
						const int ShadowRayCount = 1;
						auto shadRayOri = hit.point + (tri->normal * kEPSILON);
						auto half = (l.size / 2.0f);
						auto max = l.position + half;
						auto min = l.position - half;
						Vec3 colour(0.0f, 0.0f, 0.0f);
						for (size_t i = 0; i < ShadowRayCount;++i)
						{
							auto r = l.position - (shadRayOri);
							auto rNorm = Normalise(r);
							Ray shadowRay(shadRayOri, rNorm);
							int vis = 1 - SearchShad(shadowRay, hit, l, node.get());
							Vec2 texCo(tri->vertices[0].texCoord * w +
								tri->vertices[1].texCoord * u +
								tri->vertices[2].texCoord * v);

							auto col = (tri->normal.Dot(rNorm) / (r.Length() * r.Length()) * vis * l.colour * l.intensity * Vec3(img.GetColour(texCo.axis[0], texCo.axis[1])));

							for (auto& a : col.axis)
							{
								if (a < 0.0f)
									a = 0.0f;
							}

							colour += col;
						}
						colour /= ShadowRayCount;
						hit.colour = colour;
						//auto r = l.position - (shadRayOri);
						//auto rNorm = Normalise(r);
						//Ray shadowRay(shadRayOri,
						//	rNorm);
						//int vis = 1 - SearchShad(shadowRay, hit, l, node.get());
						//Vec2 texCo(tri->vertices[0].texCoord * w +
						//	tri->vertices[1].texCoord * u +
						//	tri->vertices[2].texCoord * v);

						//hit.colour = (tri->normal.Dot(rNorm) / (r.Length() * r.Length()) * vis * l.colour * l.intensity * Vec3(img.GetColour(texCo.axis[0], texCo.axis[1])));

						//for (size_t i = 0; i < 3; i++)
						//{
						//	if (hit.colour.axis[i] < 0.0f)
						//		hit.colour.axis[i] = 0.0f;
						//}

						//if(hit.colour.axis[0])
						//hit.colour.axis[0] = (hit.colour.axis[0], 0.0f, 1.0f);

						//if (vis > 0)
						//{
						//	DebugPrinter::Print(std::to_string(vis) + ", " + Utilities::Vec3ToString("Colour", hit.colour));
						//}

						//if (SearchShad(shadowRay, hit, l, node.get()))
						//{
						//	hit.colour = Vec3(1, 0, 0);
						//}
						//else
						//{
						//	Vec2 texCo(tri->vertices[0].texCoord * w +
						//		tri->vertices[1].texCoord * u +
						//		tri->vertices[2].texCoord * v);

						//	hit.colour = Vec3(img.GetColour(texCo.axis[0], texCo.axis[1]));
						//}
					}

					//Vec3 shadRayOri = hit.point + (tri->normal * kEPSILON);
					//Ray shadowRay(shadRayOri, 
					//	Normalise(LIGHT_POS - (shadRayOri)));
					////std::vector<HitRecord> _hits;

					//if (SearchShad(shadowRay, hit, node.get()))
					//{
					//	hit.colour = Vec3(1, 0, 0);
					//}
					//else
					//{
					//	Vec2 texCo(tri->vertices[0].texCoord * w +
					//		tri->vertices[1].texCoord * u +
					//		tri->vertices[2].texCoord * v);

					//	hit.colour = Vec3(img.GetColour(texCo.axis[0], texCo.axis[1]));
					//}

					hits.emplace_back(hit);
				}
			}
		}
		else
		{
			if(_node->leftNode)
				Search(ray, hits, _node->leftNode.get(), lights, img);

			if (_node->rightNode)
				Search(ray, hits, _node->rightNode.get(), lights, img);
		}
	}
}

void BVH::Search(const Ray& ray, std::vector<HitRecord>& hits, Node* _node)
{
	if (_node->bb.RayBoxIntersect(ray))
	{
		if (_node->isLeaf)
		{
			for (size_t i = 0; i < _node->tris.size(); ++i)
			{

				float t, u, v;
				if (_node->tris[i]->Hit(ray, t, u, v))
				{
					HitRecord hit;
					hit.t = t;
					hit.point = ray.origin + hit.t * ray.direction;
					hit.colour = Vec3(0, 0, 0);
					hits.emplace_back(hit);
				}
			}
		}
		else
		{
			if (_node->leftNode)
				Search(ray, hits, _node->leftNode.get());

			if (_node->rightNode)
				Search(ray, hits, _node->rightNode.get());
		}
	}
}

bool BVH::SearchShad(Ray& ray, HitRecord& hitTri, Light& light, Node* _node)
{
	bool hit = false;
	if (_node->bb.RayBoxIntersect(ray))
	{
		if (_node->isLeaf)
		{
			for (auto& tri : _node->tris)
			{
				if (tri->isLight)
					continue;

				float t, u, v;
				if (tri->ShadowMTHit(ray, t, u, v))
				{
					//DebugPrinter::Print(std::to_string(i));
					//std::string v1 = Utilities::Vec3ToString("Vertex 1", _node->tris[i].vertices[0].pos);
					//std::string v2 = Utilities::Vec3ToString("Vertex 2", _node->tris[i].vertices[1].pos);
					//std::string v3 = Utilities::Vec3ToString("Vertex 3", _node->tris[i].vertices[2].pos);
					//DebugPrinter::Print(v1);
					//DebugPrinter::Print(v2);
					//DebugPrinter::Print(v3);
	/*				HitRecord hiti;
					hiti.t = t;
					hiti.point = ray.origin + hiti.t * ray.direction;
					hiti.colour = Vec3(0, 0, 0);
					hits.emplace_back(hiti);*/

					//if (((hitTri.obj_id != _node->tris[i].obj_id) ||
					//	(hitTri.obj_id == _node->tris[i].obj_id && hitTri.tri_id != _node->tris[i].tri_id)) &&
					//	t < (ray.origin - LIGHT_POS).Length())

					float l = (ray.origin - light.position).Length();
					if(t < l && t > 0)
						hit = true;


					if(hit) return hit;
				}
			}
		}
		else
		{
			if (_node->leftNode)
			{
				hit = SearchShad(ray, hitTri, light, _node->leftNode.get());
				if (hit) return hit;
			}

			if (_node->rightNode)
			{
				hit = SearchShad(ray, hitTri, light, _node->rightNode.get());
				if (hit) return hit;
			}
		}
	}
	return hit;
}

bool BVH::Search(Ray& ray, Node* _node)
{
	bool hit = false;
	if (_node->bb.RayBoxIntersect(ray))
	{
		if (_node->isLeaf)
		{
			for (size_t i = 0; i < _node->tris.size(); ++i)
			{
				float t, u, v;
				if (_node->tris[i]->MTHit(ray, t, u, v))
				{
					//DebugPrinter::Print(std::to_string(i));
					//std::string v1 = Utilities::Vec3ToString("Vertex 1", _node->tris[i].vertices[0].pos);
					//std::string v2 = Utilities::Vec3ToString("Vertex 2", _node->tris[i].vertices[1].pos);
					//std::string v3 = Utilities::Vec3ToString("Vertex 3", _node->tris[i].vertices[2].pos);
					//DebugPrinter::Print(v1);
					//DebugPrinter::Print(v2);
					//DebugPrinter::Print(v3);
					hit = true;
				}
			}
		}
		else
		{
			if (_node->leftNode)
			{
				hit = Search(ray, _node->leftNode.get());
			}

			if (_node->rightNode)
			{
				hit = Search(ray, _node->rightNode.get());
			}
		}
	}
	return hit;
}

//void BVH::Destroy()
//{
//	delete node.get();
//	node = nullptr;
//}

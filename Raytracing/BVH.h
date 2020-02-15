#pragma once

#include "Object.h"
#include "Bounds.h"
#include "Ray.h"
#include "HitRecord.h"
#include "Image.h"

#include <memory>
#include <thread>
#include <future>

struct SplitCost
{
	int axis = 0;
	float splitPoint = 0.0f;
	float cost = INFINITY;
};

struct Node {

	Node() = default;
	Node(std::vector<Triangle>* _tris) : tris(*_tris)
	{
		bb.Load(*_tris);	
	}
	//Node(int _level) : level(_level) {};

	std::unique_ptr<Node> leftNode;
	std::unique_ptr<Node> rightNode;

	std::vector<Triangle> tris;
	Bounds bb;
	std::array<std::vector<float>, 3> axisSplits;
	//std::array<std::vector<std::pair<int, float>>, 3> axisSplits;
	SplitCost splitCost;
	bool isLeaf = false;
};

class BVH
{
public:
	BVH() = default;

	BVH(std::vector<Triangle>* _tris);
	~BVH();

	void Create(std::vector<Triangle>* _tris);

	void Build();

	void SplitAndSortAxis(Node* _node);

	void FindSplitCost(Node* _node);

	int TrisMoreThan(Node* _node, int axis, float value);

	int TrisLessThan(Node* _node, int axis, float value);

	void Search(const Ray& ray, std::vector<HitRecord>& hits, Image& img);

	void Search(const Ray& ray, std::vector<HitRecord>& hits, Node* _node, Image& img);

	void Destroy();

private:
	
	void Build(Node* _node);

	std::unique_ptr<Node> node;
	std::vector<Triangle>* tris = nullptr;

	std::vector<Vertex> vertices;

	std::array<std::vector<float>, 3> axisSplits;

	std::vector<std::future<void>> futures;

	float leafCost = 50.0f;

	mutable std::mutex mtx;
};


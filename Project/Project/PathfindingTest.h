#pragma once
#include "GameState.h"
#include <vector>
#include <utility>
#include <string_view>
#include <queue>
#include "TextObject.h"

struct Node
{
	double x;
	double y;
};

struct Extent
{
	double min;
	double max;
};

class Graph
{
public:
	void ParseFromFile(std::string_view path);
	
	std::vector<Node> nodes;
	std::vector<std::vector<std::pair<uint32_t, double>>> edges;
	Extent x;
	Extent y;
	Extent cost;
};

class Pathfinding
{
public:
	using P = std::pair<double, int>;
	std::priority_queue<P, std::vector<P>, std::greater<>> pq;
	std::vector<std::pair<double, double>> dist;
	std::vector<int> prev;
	bool useAStar;

	enum Status
	{
		RUNNING,
		FOUND,
		NOT_FOUND,
	};

	double EuclideanDistance(const Graph& graph, int from, int to);

	Status FindPath(const Graph& graph, int start, int target, std::vector<int>& prev, std::vector<int>& newIndices, bool newRequest);
};

class PathfindingTest : public GameState
{
public:
	void Load(const EngineContext& engineContext) override;
	void Init(const EngineContext& engineContext) override;
	void LateInit(const EngineContext& engineContext) override;
	void Update(float dt, const EngineContext& engineContext) override;
	void LateUpdate(float dt, const EngineContext& engineContext) override;
	void Draw(const EngineContext& engineContext) override;
	void Free(const EngineContext& engineContext) override;
	void Unload(const EngineContext& engineContext) override;

	void DrawNode(uint32_t node_id, float point_size, const std::vector<float>& color);
private:
	Graph map;
	//AEGfxVertexList* map_edges;
	//AEMtx33 map_transform;
	float centerX;
	float centerY;
	float normalizingSizeX;
	Pathfinding pathfinder;
	TextObject* statusText;
	TextObject* visitedNodeCountText;
	TextObject* pathNodeCountText;
	TextObject* algoText;

};
#include "PathfindingTest.h"
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>
#include <iostream>
#include <unordered_map>

std::vector<int> GetPath(int target, const std::vector<int>& prev) {
	std::vector<int> path;
	for (int cur = target; cur != -1; cur = prev[cur])
		path.push_back(cur);
	std::reverse(path.begin(), path.end());
	return path;
}

double Pathfinding::EuclideanDistance(const Graph& graph, int from, int to)
{
	double dx = (graph.nodes[from].x - graph.nodes[to].x);
	double dy = (graph.nodes[from].y - graph.nodes[to].y);
	return sqrt(dx * dx + dy * dy);
}

Pathfinding::Status Pathfinding::FindPath(const Graph& graph, int start, int target, std::vector<int>& prev, std::vector<int>& newIndices, bool newRequest)
{
	if (newRequest)
	{
		constexpr double INF = std::numeric_limits<double>::max();
		int n = graph.nodes.size();
		prev.clear();
		prev.assign(n, -1);
		dist.clear();
		dist.resize(n, { INF, INF });
		std::priority_queue<P, std::vector<P>, std::greater<>> clean;
		pq.swap(clean);
		dist[start] = { 0, 0 };
		pq.push({ 0, start });
	}

	while (!pq.empty()) {
		auto [curDist, u] = pq.top(); pq.pop();
		if (u == target) {
			return Pathfinding::FOUND;
		}

		if (curDist > dist[u].first + dist[u].second)
		{
			// This path to this node is slower than the one previously found.
			continue;
		}

		for (const auto& edge : graph.edges[u]) {
			int v = edge.first;

			double actualDistance = dist[u].first + edge.second;
			double heuristic = useAStar? EuclideanDistance(graph, target,v) : 0.0;
			double newDist = actualDistance + heuristic;

			if (newDist < dist[v].first + dist[v].second) {
				dist[v].first = actualDistance;
				dist[v].second = heuristic;
				prev[v] = u;
				pq.push({ newDist, v });
				newIndices.push_back(v);
			}
		}

		return Pathfinding::RUNNING;
	}

	return Pathfinding::NOT_FOUND;
}

void PathfindingTest::DrawNode(uint32_t node_id, float point_size, const std::vector<float>& color)
{
	/*Renderer& renderer = Renderer::GetInstance();
	AEMtx33 scale, trs;
	AEMtx33Scale(&scale, point_size, point_size);
	f32 x = (map.nodes[node_id].x - center_x) * normalizing_size_x;
	f32 y = (map.nodes[node_id].y - center_y) * normalizing_size_x;
	AEMtx33Trans(&trs, x, y);
	AEMtx33Concat(&trs, &trs, &scale);
	renderer.RenderCircle(trs, color);*/
}

void Graph::ParseFromFile(std::string_view path)
{
	nodes.clear();
	edges.clear();

	std::ifstream ifs{ path.data()};
	if (!ifs.is_open())
	{
		return;
	}

	std::string line;
	std::getline(ifs, line);

	std::istringstream iss(line);
	iss >> x.min >> x.max;

	std::getline(ifs, line);
	iss.str(line); iss.clear();
	iss >> y.min >> y.max;

	std::getline(ifs, line);
	iss.str(line); iss.clear();
	iss >> cost.min >> cost.max;

	std::getline(ifs, line);
	iss.str(line); iss.clear();
	uint32_t node_count = 0;
	iss >> line >> node_count;

	nodes.resize(node_count);
	edges.resize(node_count);
	

	for(uint32_t i = 0; i < node_count; ++i)
	{
		std::getline(ifs, line);
		iss.str(line); iss.clear();
		iss >> nodes[i].x >> nodes[i].y;
	}

	// read Edge
	std::getline(ifs, line);

	while (std::getline(ifs, line))
	{
		iss.str(line); iss.clear();
		uint32_t from = 0;
		uint32_t to = 0;
		double cost = 0;
		iss >> from >> to >> cost;
		edges[from].push_back(std::make_pair(to, cost));
		edges[to].push_back(std::make_pair(from, cost));
	}
}

void PathfindingTest::Load(const EngineContext& engineContext)
{
}

void PathfindingTest::Init(const EngineContext& engineContext)
{
	std::ifstream ifs{ "Assets/game.ini" };
	std::string filepath{ "Assets/busan_2000_map.txt" };

	if (ifs.is_open())
	{
		std::string filepath_read;
		std::getline(ifs, filepath_read);
		std::ifstream check{ filepath_read };
		if (check.is_open())
		{
			filepath = filepath_read;
		}
	}

	engineContext.windowManager->Resize(1600, 900);

	map.ParseFromFile(filepath);

	normalizingSizeX = 1.f / (map.x.max - map.x.min);
	normalizingSizeX = normalizingSizeX * engineContext.windowManager->GetHeight();

	centerX = (map.x.max + map.x.min) * 0.5f;
	centerY = (map.y.max + map.y.min) * 0.5f;

	glm::vec2 pos = {
			-engineContext.windowManager->GetWidth() * 0.5f + 50.f,
			engineContext.windowManager->GetHeight() * 0.5f - 50.f
	};

	visitedNodeCountText = new TextObject(engineContext.renderManager->GetFontByTag("default"), std::to_string(0), TextAlignH::Left, TextAlignV::Top);
	objectManager.AddObject(std::unique_ptr<TextObject>(visitedNodeCountText), "debugText");
	visitedNodeCountText->GetTransform2D().SetPosition(
		pos + glm::vec2(0, -50));
	visitedNodeCountText->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
	visitedNodeCountText->SetRenderLayer(engineContext, "UI.Text");
	visitedNodeCountText->SetText("Visited : " + std::to_string(0));

	pathNodeCountText = new TextObject(engineContext.renderManager->GetFontByTag("default"), std::to_string(0), TextAlignH::Left, TextAlignV::Top);
	objectManager.AddObject(std::unique_ptr<TextObject>(pathNodeCountText), "debugText");
	pathNodeCountText->GetTransform2D().SetPosition(
		pos + glm::vec2(0, -100));
	pathNodeCountText->SetRenderLayer(engineContext, "UI.Text");
	pathNodeCountText->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
	pathNodeCountText->SetText("Path : " + std::to_string(0));

	algoText = new TextObject(engineContext.renderManager->GetFontByTag("default"), std::to_string(0), TextAlignH::Left, TextAlignV::Top);
	objectManager.AddObject(std::unique_ptr<TextObject>(algoText), "debugText");
	algoText->GetTransform2D().SetPosition(
		pos + glm::vec2(0, -150));
	algoText->SetRenderLayer(engineContext, "UI.Text");
	algoText->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
	algoText->SetText("A*");

	statusText = new TextObject(engineContext.renderManager->GetFontByTag("default"), std::to_string(0), TextAlignH::Left, TextAlignV::Top);
	objectManager.AddObject(std::unique_ptr<TextObject>(statusText), "debugText");
	statusText->GetTransform2D().SetPosition(pos);
	statusText->SetIgnoreCamera(true);
	statusText->SetRenderLayer(engineContext, "UI.Text");
	statusText->SetText("N/A");


	//AEGfxMeshStart();
	//for (uint32_t from = 0; from < map.nodes.size(); ++from)
	//{
	//	float fromX = map.nodes[from].x;
	//	float fromY = map.nodes[from].y;
	//	for (uint32_t j = 0; j < map.edges[from].size(); ++j)
	//	{
	//		uint32_t to = map.edges[from][j].first;
	//		float toX = map.nodes[to].x;
	//		float toY = map.nodes[to].y;
	//		AEGfxVertexAdd(fromX - centerX, fromY - centerY, 0xFFFFFFFF, 0, 0);
	//		AEGfxVertexAdd(toX - centerX, toY - centerY, 0xFFFFFFFF, 0, 0);
	//	}
	//}
	//map_edges = AEGfxMeshEnd();
}

void PathfindingTest::LateInit(const EngineContext& engineContext)
{
}

void PathfindingTest::LateUpdate(float dt, const EngineContext& engineContext)
{
}

void PathfindingTest::Update(float dt, const EngineContext& engineContext)
{
	static uint32_t start_node_id = 0;
	static uint32_t goal_node_id = 0;

	if (engineContext.inputManager->IsKeyDown(KEY_COMMA))
	{
		start_node_id = (start_node_id + 1) % map.nodes.size();
	}
	if (engineContext.inputManager->IsKeyDown(KEY_PERIOD))
	{
		goal_node_id = (goal_node_id + 1) % map.nodes.size();
	}
	if (engineContext.inputManager->IsKeyReleased(KEY_M))
	{
		pathfinder.useAStar = !pathfinder.useAStar;
	}

	static std::vector<int> path;
	static std::vector<int> visited;
	static Pathfinding::Status status = Pathfinding::NOT_FOUND;

	if (engineContext.inputManager->IsKeyReleased(KEY_R))
	{
		path.clear();
		visited.clear();
		goal_node_id = 0;
		start_node_id = 0;
		status = Pathfinding::NOT_FOUND;
		Restart(engineContext);
		return;
	}

	std::vector<int> newIndices;

	if (engineContext.inputManager->IsKeyReleased(KEY_ENTER))
	{
		path.clear();
		visited.clear();
		status = pathfinder.FindPath(map, start_node_id, goal_node_id, pathfinder.prev, newIndices, true);
		statusText->SetText("Running");
	}

	if (status == Pathfinding::RUNNING)
	{
		status = pathfinder.FindPath(map, start_node_id, goal_node_id, pathfinder.prev, newIndices, false);

		if (status != Pathfinding::RUNNING)
		{
			path = GetPath(goal_node_id, pathfinder.prev);
			statusText->SetText(status == Pathfinding::FOUND ? "FOUND" : "NOT FOUND");
		}
	}

	visitedNodeCountText->SetText("Visited: " + std::to_string(visited.size()));
	pathNodeCountText->SetText("Path: " + std::to_string(path.size()));
	algoText->SetText(pathfinder.useAStar ? "A*" : "Dijkstra");
	

	visited.insert(visited.end(), newIndices.begin(), newIndices.end());
	GameState::Update(dt, engineContext);
}

void PathfindingTest::Draw(const EngineContext& engineContext)
{
	//renderer.RenderMesh(map_transform, map_edges, AE_GFX_MDM_LINES, Colors::Blue);

	//DrawNode(start_node_id, 10, Colors::Red);
	//DrawNode(goal_node_id, 10, Colors::Green);

	//if (!visited.empty())
	//{
	//	for (const auto& node : visited)
	//	{
	//		DrawNode(node, 2, Colors::Orange);
	//	}
	//}

	//if (!path.empty())
	//{
	//	for (const auto& node : path)
	//	{
	//		DrawNode(node, 5, Colors::Red);
	//	}
	//}

	objectManager.DrawAll(engineContext, cameraManager.GetActiveCamera());
}

void PathfindingTest::Free(const EngineContext& engineContext)
{
}

void PathfindingTest::Unload(const EngineContext& engineContext)
{
}

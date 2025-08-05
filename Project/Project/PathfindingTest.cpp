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
	engineContext.windowManager->Resize(1600, 900);
	engineContext.renderManager->RegisterMaterial("m_map", "s_colorOnly", {}); 
	engineContext.renderManager->RegisterTexture("t_circle", "Textures/circle.png"); 
	engineContext.renderManager->RegisterMaterial("m_node", "s_default", { std::pair<std::string, std::string>("u_Texture","t_circle") });
}

Object* PathfindingTest::AddNode(const EngineContext& engineContext, uint32_t nodeIdx, float nodeSize, const glm::vec4& color, std::string_view tag)
{
	Object* obj = objectManager.AddObject(std::make_unique<GameObject>(), tag.data());
	float x = (map.nodes[nodeIdx].x - centerX) * normalizingSizeX;
	float y = (map.nodes[nodeIdx].y - centerY) * normalizingSizeX;
	obj->GetTransform2D().SetPosition({x, y}); 
	obj->SetMesh(engineContext, "default");
	obj->SetMaterial(engineContext, "m_node");
	obj->GetTransform2D().SetScale({nodeSize, nodeSize});
	obj->SetRenderLayer(engineContext, "Game");
	obj->SetVisibility(true);
	obj->SetColor(color);
	return obj;
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
	map.ParseFromFile(filepath);

	centerX = (map.x.max + map.x.min) * 0.5f;
	centerY = (map.y.max + map.y.min) * 0.5f;
	std::vector<Vertex> vertices;
	vertices.reserve(map.nodes.size() * 2);
	for (uint32_t from = 0; from < map.nodes.size(); ++from)
	{
		float fromX = map.nodes[from].x;
		float fromY = map.nodes[from].y;
		for (uint32_t j = 0; j < map.edges[from].size(); ++j)
		{
			uint32_t to = map.edges[from][j].first;
			float toX = map.nodes[to].x;
			float toY = map.nodes[to].y;
			Vertex p0, p1;
			p0.position = { fromX - centerX, fromY - centerY, 0.0f };
			p1.position = { toX - centerX, toY - centerY, 0.0f};
			vertices.push_back(p0);
			vertices.push_back(p1);
		}
	}
	engineContext.renderManager->RegisterMesh("map", vertices, {}, PrimitiveType::Lines);

	normalizingSizeX = 1.f / (map.x.max - map.x.min);
	normalizingSizeX = normalizingSizeX * engineContext.windowManager->GetHeight();

	auto* mapObj = objectManager.AddObject(std::make_unique<GameObject>(), "map");
	mapObj->SetMesh(engineContext, "map");
	mapObj->SetMaterial(engineContext, "m_map");
	mapObj->GetTransform2D().SetScale({ normalizingSizeX, normalizingSizeX });
	mapObj->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
	mapObj->SetRenderLayer(engineContext, "Game.Background");
	mapObj->SetColor({0,0,1,1});

	glm::vec2 text_pos = {
			-engineContext.windowManager->GetWidth() * 0.5f + 50.f,
			engineContext.windowManager->GetHeight() * 0.5f - 50.f
	};

	visitedNodeCountText = new TextObject(engineContext.renderManager->GetFontByTag("default"), std::to_string(0), TextAlignH::Left, TextAlignV::Top);
	objectManager.AddObject(std::unique_ptr<TextObject>(visitedNodeCountText), "debugText");
	visitedNodeCountText->GetTransform2D().SetPosition(
		text_pos + glm::vec2(0, -50));
	visitedNodeCountText->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
	visitedNodeCountText->SetRenderLayer(engineContext, "UI.Text");
	visitedNodeCountText->SetText("Visited : " + std::to_string(0));

	pathNodeCountText = new TextObject(engineContext.renderManager->GetFontByTag("default"), std::to_string(0), TextAlignH::Left, TextAlignV::Top);
	objectManager.AddObject(std::unique_ptr<TextObject>(pathNodeCountText), "debugText");
	pathNodeCountText->GetTransform2D().SetPosition(
		text_pos + glm::vec2(0, -100));
	pathNodeCountText->SetRenderLayer(engineContext, "UI.Text");
	pathNodeCountText->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
	pathNodeCountText->SetText("Path : " + std::to_string(0));

	algoText = new TextObject(engineContext.renderManager->GetFontByTag("default"), std::to_string(0), TextAlignH::Left, TextAlignV::Top);
	objectManager.AddObject(std::unique_ptr<TextObject>(algoText), "debugText");
	algoText->GetTransform2D().SetPosition(
		text_pos + glm::vec2(0, -150));
	algoText->SetRenderLayer(engineContext, "UI.Text");
	algoText->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
	algoText->SetText("A*");

	statusText = new TextObject(engineContext.renderManager->GetFontByTag("default"), std::to_string(0), TextAlignH::Left, TextAlignV::Top);
	objectManager.AddObject(std::unique_ptr<TextObject>(statusText), "debugText");
	statusText->GetTransform2D().SetPosition(text_pos);
	statusText->SetIgnoreCamera(true);
	statusText->SetRenderLayer(engineContext, "UI.Text");
	statusText->SetText("N/A");
	
	startNodeIdx = 0;
	goalNodeIdx = 0;
	startNode = AddNode(engineContext, 0, 10, { 1,1,1,1 }, "Node.Start");
	goalNode = AddNode(engineContext, 0, 10, { 0,1,0,1 }, "Node.End");
}

void PathfindingTest::LateInit(const EngineContext& engineContext)
{
}

void PathfindingTest::LateUpdate(float dt, const EngineContext& engineContext)
{
}

void PathfindingTest::Update(float dt, const EngineContext& engineContext)
{
	if (engineContext.inputManager->IsKeyDown(KEY_COMMA))
	{
		startNodeIdx = (startNodeIdx + 1) % map.nodes.size();
	}
	if (engineContext.inputManager->IsKeyDown(KEY_PERIOD))
	{
		goalNodeIdx = (goalNodeIdx + 1) % map.nodes.size();
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
		goalNodeIdx = 0;
		startNodeIdx = 0;
		status = Pathfinding::NOT_FOUND;
		Restart(engineContext);
		return;
	}

	std::vector<int> newIndices;

	if (engineContext.inputManager->IsKeyReleased(KEY_ENTER))
	{
		std::vector<Object*> nodes;
		objectManager.FindByTag("Node", nodes);
		for (Object* node : nodes)
		{
			node->Kill();
		}
		path.clear();
		visited.clear();
		status = pathfinder.FindPath(map, startNodeIdx, goalNodeIdx, pathfinder.prev, newIndices, true);
		statusText->SetText("Running");
		if (status != Pathfinding::RUNNING)
		{
			SNAKE_ERR("didn't handle edge case correctly. yet.");
		}
	}		
	
	float x = (map.nodes[startNodeIdx].x - centerX) * normalizingSizeX;
	float y = (map.nodes[startNodeIdx].y - centerY) * normalizingSizeX;
	startNode->GetTransform2D().SetPosition({ x, y });

	x = (map.nodes[goalNodeIdx].x - centerX) * normalizingSizeX;
	y = (map.nodes[goalNodeIdx].y - centerY) * normalizingSizeX;
	goalNode->GetTransform2D().SetPosition({ x, y });

	if (status == Pathfinding::RUNNING)
	{
		status = pathfinder.FindPath(map, startNodeIdx, goalNodeIdx, pathfinder.prev, newIndices, false);
		visited.insert(visited.end(), newIndices.begin(), newIndices.end());

		if (status != Pathfinding::RUNNING)
		{
			path = GetPath(goalNodeIdx, pathfinder.prev);
			if (!path.empty())
			{
				glm::vec4 red = { 1, 0, 0, 1 };
				for (int nodeIdx : path)
					AddNode(engineContext, nodeIdx, 5, red, "Node");
			}
			statusText->SetText(status == Pathfinding::FOUND ? "FOUND" : "NOT FOUND");
		}
	}

	if (!newIndices.empty())
	{
		glm::vec4 orange = { 210.f / 255.f, 105.f / 255.f, 0, 1 };
		for (int nodeIdx : newIndices)
			AddNode(engineContext, nodeIdx, 2, orange, "Node");
	}

	visitedNodeCountText->SetText("Visited: " + std::to_string(visited.size()));
	pathNodeCountText->SetText("Path: " + std::to_string(path.size()));
	algoText->SetText(pathfinder.useAStar ? "A*" : "Dijkstra");
	
	GameState::Update(dt, engineContext);
}

void PathfindingTest::Draw(const EngineContext& engineContext)
{
	objectManager.DrawAll(engineContext, cameraManager.GetActiveCamera());
}

void PathfindingTest::Free(const EngineContext& engineContext)
{
}

void PathfindingTest::Unload(const EngineContext& engineContext)
{
}

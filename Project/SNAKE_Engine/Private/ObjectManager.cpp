#include "ObjectManager.h"
#include <cassert>


GameObject* ObjectManager::AddObject(std::unique_ptr<GameObject> obj)
{
	assert(obj != nullptr && "Cannot add null object");
	GameObject* rawPointer = obj.get();
	pendingObjects.push_back(std::move(obj));
	return rawPointer;
}



void ObjectManager::InitAll()
{
	for (const auto& obj: objects)
	{
		obj->Init();
	}
}

void ObjectManager::UpdateAll(float dt, const EngineContext& engineContext)
{
	for (const auto& obj : objects)
	{
		if (obj->IsAlive())
			obj->Update(dt,engineContext);
	}

	EraseDeadObjects();
	AddAllPendingObjects();
}

void ObjectManager::AddAllPendingObjects()
{
	for (auto& obj : pendingObjects)
	{
		objects.push_back(std::move(obj));
	}
	pendingObjects.clear();
}

void ObjectManager::EraseDeadObjects()
{
	objects.erase(
		std::remove_if(objects.begin(), objects.end(),
			[](const std::unique_ptr<GameObject>& obj) {
				return !obj->IsAlive();
			}),
		objects.end());
}

void ObjectManager::DrawAll(const EngineContext& engineContext)
{
	for (const auto& obj : objects)
	{
		obj->Draw(engineContext);
	}
}

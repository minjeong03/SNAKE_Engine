#include "ObjectManager.h"
#include "EngineContext.h"
#include <cassert>


void ObjectManager::AddObject(std::unique_ptr<GameObject> obj, std::string ID)
{
	assert(obj != nullptr && "Cannot add null object");

	obj->SetID(ID);

	if (!ID.empty())
	{
		assert(objectMap.find(ID) == objectMap.end() && "Duplicate GameObject ID");

		GameObject* rawPointer = obj.get();
		objectMap[ID] = rawPointer;
	}

	pendingObjects.push_back(std::move(obj));
}



void ObjectManager::InitAll()
{
	for (const auto& obj: objects)
	{
		obj->Init();
	}
	for (const auto& obj : objects)
	{
		obj->LateInit();
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
	std::vector<std::unique_ptr<GameObject>> tmp;
	std::swap(tmp, pendingObjects);
	for (auto& obj : tmp)
	{
		obj->Init();
	}
	for (auto& obj : tmp)
	{
		obj->LateInit();
		objects.push_back(std::move(obj));
	}
}

void ObjectManager::EraseDeadObjects()
{
	std::vector<GameObject*> deadObjects;
	for (const auto& obj : objects)
	{
		if (!obj->IsAlive()) 
		{
			deadObjects.push_back(obj.get());
		}
	}
	for (auto& obj : deadObjects) 
	{
		obj->Free();
	}
	for (auto& obj : deadObjects)
	{
		obj->LateFree();
		objectMap.erase(obj->GetID());
	}

	objects.erase(
		std::remove_if(objects.begin(), objects.end(),
			[](const std::unique_ptr<GameObject>& obj) 
			{
				return !obj->IsAlive();
			}),
		objects.end());
}

void ObjectManager::DrawAll(const EngineContext& engineContext)
{
	for (const auto& obj : objects)
	{
		if (obj->IsAlive() && obj->IsVisible())
		{
			GameObject* rawPointer = obj.get();
			engineContext.renderManager->Submit(
				[=]()
				{
					rawPointer->Draw(engineContext);
				}, rawPointer->GetRenderLayer());
		}
	}
}

void ObjectManager::FreeAll()
{
	for (const auto& obj : objects)
	{
		obj->Free();
	}
	for (const auto& obj : objects)
	{
		obj->LateFree();
	}
	objects.clear();
	objectMap.clear();
}

GameObject* ObjectManager::FindByID(const std::string& id) const
{
	auto it = objectMap.find(id);
	if (it != objectMap.end())
		return it->second;
	return nullptr;
}

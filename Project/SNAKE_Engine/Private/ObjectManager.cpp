#include "ObjectManager.h"
#include "EngineContext.h"
#include <cassert>

#include <iostream>
#include "Debug.h"


void ObjectManager::AddObject(std::unique_ptr<GameObject> obj, std::string ID)
{
    assert(obj != nullptr && "Cannot add null object");

    obj->SetID(ID);

    if (!ID.empty())
    {
	if (objectMap.find(ID) != objectMap.end())
	    SNAKE_WRN("Duplicate GameObject ID");

	GameObject* rawPointer = obj.get();
	objectMap[ID] = rawPointer;
    }

    pendingObjects.push_back(std::move(obj));
}



void ObjectManager::InitAll(const EngineContext& engineContext)
{
    for (const auto& obj : objects)
    {
	obj->Init(engineContext);
    }
    for (const auto& obj : objects)
    {
	obj->LateInit(engineContext);
    }
}


void ObjectManager::UpdateAll(float dt, const EngineContext& engineContext)
{
    for (const auto& obj : objects)
    {
	if (obj->IsAlive())
	    obj->Update(dt, engineContext);
    }

    EraseDeadObjects(engineContext);
    AddAllPendingObjects(engineContext);
}

void ObjectManager::AddAllPendingObjects(const EngineContext& engineContext)
{
    std::vector<std::unique_ptr<GameObject>> tmp;
    std::swap(tmp, pendingObjects);
    for (auto& obj : tmp)
    {
	obj->Init(engineContext);
    }
    for (auto& obj : tmp)
    {
	obj->LateInit(engineContext);
	objects.push_back(std::move(obj));
    }
}

void ObjectManager::EraseDeadObjects(const EngineContext& engineContext)
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
	obj->Free(engineContext);
    }
    for (auto& obj : deadObjects)
    {
	obj->LateFree(engineContext);
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
	    if (rawPointer)
	    {
		engineContext.renderManager->Submit(
		    [=]()
		    {

			Material* material = rawPointer->GetMaterial();
			Mesh* mesh = rawPointer->GetMesh();
			if (material && mesh)
			{
			    material->Bind();
			    rawPointer->Draw(engineContext); //send Uniforms
			    mesh->Draw();
			    material->UnBind();
			}
			else
			{
			    if (!material)
			    {
				SNAKE_ERR("Draw skipped: [" << rawPointer->GetID() << "]'s Material not set");
			    }
			    if (!mesh)
			    {
				SNAKE_ERR("Draw skipped: [" << rawPointer->GetID() << "]'s Mesh not set");
			    }
			}
		    }, rawPointer->GetRenderLayer());
	    }
	}
    }
}

void ObjectManager::FreeAll(const EngineContext& engineContext)
{
    for (const auto& obj : objects)
    {
	obj->Free(engineContext);
    }
    for (const auto& obj : objects)
    {
	obj->LateFree(engineContext);
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

#include "ObjectManager.h"
#include "EngineContext.h"
#include <cassert>

#include <iostream>
#include "Debug.h"

void ObjectManager::AddObject(std::unique_ptr<GameObject> obj, const std::string& tag)
{
    assert(obj != nullptr && "Cannot add null object");

    obj->SetTag(tag);

    if (!tag.empty())
    {
        if (objectMap.find(tag) != objectMap.end())
            SNAKE_WRN("Duplicate GameObject ID");

        GameObject* rawPointer = obj.get();
        objectMap[tag] = rawPointer;
    }
    rawPtrObjects.push_back(obj.get());
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
        objectMap.erase(obj->GetTag());
        rawPtrObjects.erase(std::remove(rawPtrObjects.begin(), rawPtrObjects.end(), obj), rawPtrObjects.end());
    }

    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
            [](const std::unique_ptr<GameObject>& obj)
            {
                return !obj->IsAlive();
            }),
        objects.end());
}

void ObjectManager::DrawAll(const EngineContext& engineContext, Camera2D* camera)
{
    engineContext.renderManager->Submit(engineContext, rawPtrObjects, camera);
}

void ObjectManager::DrawObjects(const EngineContext& engineContext, Camera2D* camera, const std::vector<GameObject*>& gameObjects)
{
    engineContext.renderManager->Submit(engineContext, gameObjects, camera);
}

void ObjectManager::DrawObjectsWithTag(const EngineContext& engineContext, Camera2D* camera, const std::string& tag)
{
    std::vector<GameObject*> filteredObjects;
    for (GameObject* obj : rawPtrObjects)
    {
        if (obj && obj->IsAlive() && obj->GetTag() == tag)
        {
            filteredObjects.push_back(obj);
        }
    }
    engineContext.renderManager->Submit(engineContext, filteredObjects, camera);
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

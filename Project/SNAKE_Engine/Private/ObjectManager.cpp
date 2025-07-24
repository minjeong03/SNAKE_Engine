#include "ObjectManager.h"
#include "EngineContext.h"
#include "Object.h"
#include "Debug.h"
#include <cassert>
#include <algorithm>

Object* ObjectManager::AddObject(std::unique_ptr<Object> obj, const std::string& tag)
{
    assert(obj != nullptr && "Cannot add null object");

    obj->SetTag(tag);

    if (!tag.empty())
    {
        if (objectMap.find(tag) != objectMap.end())
            SNAKE_WRN("Duplicate Object ID");

        Object* rawPointer = obj.get();
        objectMap[tag] = rawPointer;
    }

    Object* returnVal = obj.get();
    rawPtrObjects.push_back(obj.get());
    pendingObjects.push_back(std::move(obj));
    return returnVal;
}

void ObjectManager::InitAll(const EngineContext& engineContext)
{
    for (const auto& obj : objects)
        obj->Init(engineContext);

    for (const auto& obj : objects)
        obj->LateInit(engineContext);
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
    std::vector<std::unique_ptr<Object>> tmp;
    std::swap(tmp, pendingObjects);

    for (auto& obj : tmp)
        obj->Init(engineContext);

    for (auto& obj : tmp)
    {
        obj->LateInit(engineContext);
        objects.push_back(std::move(obj));
    }
}

void ObjectManager::EraseDeadObjects(const EngineContext& engineContext)
{
    std::vector<Object*> deadObjects;
    for (const auto& obj : objects)
    {
        if (!obj->IsAlive())
            deadObjects.push_back(obj.get());
    }

    for (auto& obj : deadObjects)
        obj->Free(engineContext);

    for (auto& obj : deadObjects)
    {
        obj->LateFree(engineContext);
        objectMap.erase(obj->GetTag());
        rawPtrObjects.erase(std::remove(rawPtrObjects.begin(), rawPtrObjects.end(), obj), rawPtrObjects.end());
    }

    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
            [](const std::unique_ptr<Object>& obj)
            {
                return !obj->IsAlive();
            }),
        objects.end());
}

void ObjectManager::DrawAll(const EngineContext& engineContext, Camera2D* camera)
{
    engineContext.renderManager->Submit(engineContext, rawPtrObjects, camera);
}

void ObjectManager::DrawObjects(const EngineContext& engineContext, Camera2D* camera, const std::vector<Object*>& objects)
{
    engineContext.renderManager->Submit(engineContext, objects, camera);
}

void ObjectManager::DrawObjectsWithTag(const EngineContext& engineContext, Camera2D* camera, const std::string& tag)
{
    std::vector<Object*> filteredObjects;
    FindByTag(tag, filteredObjects);
    engineContext.renderManager->Submit(engineContext, filteredObjects, camera);
}

void ObjectManager::FreeAll(const EngineContext& engineContext)
{
    for (const auto& obj : objects)
        obj->Free(engineContext);

    for (const auto& obj : objects)
        obj->LateFree(engineContext);

    objects.clear();
    objectMap.clear();
    rawPtrObjects.clear();
}

Object* ObjectManager::FindByTag(const std::string& tag) const
{
    auto it = objectMap.find(tag);
    if (it != objectMap.end())
        return it->second;
    return nullptr;
}

void ObjectManager::FindByTag(const std::string& tag, std::vector<Object*>& result)
{
    for (Object* obj : rawPtrObjects)
    {
        if (obj && obj->IsAlive() && obj->GetTag() == tag)
            result.push_back(obj);
    }
}

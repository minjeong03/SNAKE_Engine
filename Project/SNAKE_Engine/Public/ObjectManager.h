#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Camera2D.h"
#include "GameObject.h"

struct EngineContext;
class Camera2D;

class ObjectManager
{
public:
    void AddObject(std::unique_ptr<GameObject> obj, const std::string& tag = "");

    void InitAll(const EngineContext& engineContext);

    void UpdateAll(float dt, const EngineContext& engineContext);

    void AddAllPendingObjects(const EngineContext& engineContext);

    void EraseDeadObjects(const EngineContext& engineContext);

    void DrawAll(const EngineContext& engineContext, Camera2D* camera);

    void DrawObjects(const EngineContext& engineContext, Camera2D* camera, const std::vector<GameObject*>& gameObjects);

    void DrawObjectsWithTag(const EngineContext& engineContext, Camera2D* camera, const std::string& tag);

    void FreeAll(const EngineContext& engineContext);

    [[nodiscard]] GameObject* FindByID(const std::string& id) const;

private:
    std::vector<std::unique_ptr<GameObject>> objects;
    std::vector<std::unique_ptr<GameObject>> pendingObjects;
    std::vector<GameObject*> rawPtrObjects;

    std::unordered_map<std::string, GameObject*> objectMap;
};

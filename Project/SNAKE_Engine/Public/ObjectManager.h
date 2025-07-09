#pragma once
#include <memory>
#include <vector>
struct EngineContext;
/**
 * @brief
 * @details
 * @author Jinwoo Choi
 * @date 2025-07-09
 */
class GameObject
{
public:
    virtual void Init() = 0;
    virtual void Update(float dt, const EngineContext& engineContext) = 0;
    virtual void Draw(const EngineContext& engineContext) = 0;
    virtual ~GameObject() = default;
    bool IsAlive()
    {
        return isAlive;
    }
    void Kill()
    {
        isAlive = false;
    }
private:
    bool isAlive = true;
};
class ObjectManager
{
public:
    GameObject* AddObject(std::unique_ptr<GameObject> obj);
    void InitAll();
    void UpdateAll(float dt, const EngineContext& engineContext);
    void AddAllPendingObjects();
    void EraseDeadObjects();
    void DrawAll(const EngineContext& engineContext);
private:
    std::vector<std::unique_ptr<GameObject>> objects;
    std::vector<std::unique_ptr<GameObject>> pendingObjects;
};
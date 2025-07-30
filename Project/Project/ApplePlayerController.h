#pragma once
#include "Engine.h"

class ApplePlayerController : public GameObject
{
public:
    ApplePlayerController();
    void Init(const EngineContext& engineContext) override;
    void LateInit(const EngineContext& engineContext) override;
    void Update(float dt, const EngineContext& engineContext) override;
    void Draw(const EngineContext& engineContext) override;
    void Free(const EngineContext& engineContext) override;
    void LateFree(const EngineContext& engineContext) override;
    void OnCollision(Object* other) override;

    glm::vec2 ConvertScreenToCamera(Camera2D* cam, const glm::vec2& screen_pos);
private:
    enum InputState
    {
        Pressed,
        Down,
        Release,
        COUNT
    };

    void StartDragging(const EngineContext& engineContext);
    void EndDragging(const EngineContext& engineContext);
    void DoNothing(const EngineContext& engineContext);
    void OnDragging(const EngineContext& engineContext);
    void ShouldNotBeReached(const EngineContext& engineContext);
    void CheckSelectedApples(const EngineContext& engineContext);

    InputState prev_state = Release;
    InputState current_state = Release;
    glm::vec2 start_point;
    bool checkApples = false;
    std::vector<Object*> selected_objects;
    typedef void(ApplePlayerController::*InputActionFunc)(const EngineContext& engineContext);
    InputActionFunc funcs[InputState::COUNT][InputState::COUNT];
};


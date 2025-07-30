#include "ApplePlayerController.h"
#include "Apple.h"

ApplePlayerController::ApplePlayerController() :
    funcs{ nullptr }, start_point{0,0}
{
    /*
    +----------------+----------------+----------------+----------------+
    | prev \ current |   Pressed      |     Down       |   Released     |
    +----------------+----------------+----------------+----------------+
    | Pressed        |      N/A       | dragging       | small chance   |
    | Down           |      N/A       | dragging       | collision check|
    | Released       | start rect     | N/A            | do nothing     |
    +----------------+----------------+----------------+----------------+
    */
    funcs[Pressed][Pressed] = &ApplePlayerController::ShouldNotBeReached;
    funcs[Pressed][Down] = &ApplePlayerController::OnDragging;
    funcs[Pressed][Release] = &ApplePlayerController::DoNothing;
    funcs[Down][Pressed] = &ApplePlayerController::ShouldNotBeReached;
    funcs[Down][Down] = &ApplePlayerController::OnDragging;
    funcs[Down][Release] = &ApplePlayerController::EndDragging;
    funcs[Release][Pressed] = &ApplePlayerController::StartDragging;
    funcs[Release][Down] = &ApplePlayerController::ShouldNotBeReached;
    funcs[Release][Release] = &ApplePlayerController::DoNothing;
}

void ApplePlayerController::Init(const EngineContext& engineContext)
{
    SetMesh(engineContext, "default");
    SetMaterial(engineContext, "m_selection_box");
    SetRenderLayer(engineContext, "UI");
    SetVisibility(false);
    SetCollider(std::make_unique<AABBCollider>(this, glm::vec2(1, 1)));
    SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "player_selection", { "apple" });

    GetTransform2D().SetPosition({ -1000, -1000 });
    GetTransform2D().SetScale({ 0.0,0.0 });
}

void ApplePlayerController::LateInit(const EngineContext& engineContext)
{

}

void ApplePlayerController::Update(float dt, const EngineContext& engineContext)
{
    CheckSelectedApples(engineContext);

    prev_state = current_state;
    if (engineContext.inputManager->IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        current_state = Down;
    if (engineContext.inputManager->IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        current_state = Pressed;
    if (engineContext.inputManager->IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        current_state = Release;
    
    (this->*funcs[prev_state][current_state])(engineContext);
}

void ApplePlayerController::Draw(const EngineContext& engineContext)
{

}

void ApplePlayerController::Free(const EngineContext& engineContext)
{
}

void ApplePlayerController::LateFree(const EngineContext& engineContext)
{
}

void ApplePlayerController::OnCollision(Object* other)
{
    if (!checkApples) return;
    
    selected_objects.push_back(other);
}

void ApplePlayerController::StartDragging(const EngineContext& engineContext)
{
    SNAKE_ERR("[ApplePlayerController]  StartDragging");
    SetVisibility(true);
    start_point = {
        engineContext.inputManager->GetMouseX(),
        engineContext.inputManager->GetMouseY()
    };
    start_point = ConvertScreenToCamera(engineContext.stateManager->GetCurrentState()->GetActiveCamera(), start_point);
    GetTransform2D().SetScale({ 0.0,0.0 });
}

void ApplePlayerController::EndDragging(const EngineContext& engineContext)
{
    SNAKE_ERR("[ApplePlayerController]  EndDragging");
    SetVisibility(false);
    checkApples = true;
    selected_objects.clear();
}

void ApplePlayerController::DoNothing(const EngineContext& engineContext)
{
}

void ApplePlayerController::OnDragging(const EngineContext& engineContext)
{
    glm::vec2 end_point = {
        engineContext.inputManager->GetMouseX(),
        engineContext.inputManager->GetMouseY()
    };
    end_point = ConvertScreenToCamera(engineContext.stateManager->GetCurrentState()->GetActiveCamera(), end_point);
    GetTransform2D().SetPosition((end_point + start_point) * 0.5f);
    GetTransform2D().SetScale(glm::abs(end_point - start_point));
}

void ApplePlayerController::ShouldNotBeReached(const EngineContext& engineContext)
{
    SNAKE_ERR("[ApplePlayerController]  ShouldNotBeReached");
}

void ApplePlayerController::CheckSelectedApples(const EngineContext& engineContext)
{
    if (!checkApples) return;
    
    int sum = 0;
    for (Object* obj : selected_objects)
    {
        Apple* apple = (Apple*)obj;
        apple->SetSelected(false);
        sum += apple->GetValue();
    }

    if (sum == 10)
    {
        for (Object* obj : selected_objects)
        {
            obj->Kill();
        }
    }

    selected_objects.clear();
    checkApples = false;
    GetTransform2D().SetPosition({ -1000, -1000 });
}

glm::vec2 ApplePlayerController::ConvertScreenToCamera(Camera2D* cam, const glm::vec2& screen_pos)
{
    return cam->GetPosition() + glm::vec2{ screen_pos.x - cam->GetScreenWidth() / 2, cam->GetScreenHeight() / 2 - screen_pos.y } / cam->GetZoom();
}

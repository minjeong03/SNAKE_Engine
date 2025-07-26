#include "Player.h"
#include <random>

#include "Bullet.h"
#include "Debug.h"
#include "Engine.h"


void Player::Init(const EngineContext& engineContext)
{
    transform2D.SetPosition(glm::vec2(0, 0));
    transform2D.SetScale(glm::vec2(50.f));
    auto collider = std::make_unique<AABBCollider>(this, glm::vec2(0.5,0.5));
    collider->SetUseTransformScale(true); 
    SetCollider(std::move(collider));
    SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(),"player", { "bullet" });

    SetMesh(engineContext, "default");
    SetMaterial(engineContext, "m_animation");
    SetRenderLayer(engineContext, "Player");
    SpriteSheet* sheet =  engineContext.renderManager->GetSpriteSheetByTag("animTest");
    sheet->AddClip("sidewalk", { 0,1,2,3,4,5,6,7,8 }, 0.08f, true);
    sheet->AddClip("frontwalk", { 86,87,88,89,90,91 }, 0.08f, true);
    sheet->AddClip("backwalk", { 80,81,82,83,84,85 }, 0.08f, true);
    sheet->AddClip("idle", { 9 }, 0.08f, false);
    AttachAnimator(sheet, 0.08f);
    spriteAnimator->PlayClip("idle");
}

void Player::LateInit(const EngineContext& engineContext)
{

}

void Player::Update(float dt, const EngineContext& engineContext)
{

    if (engineContext.inputManager->IsKeyDown(KEY_W))
    {
        transform2D.AddPosition(glm::vec2(0, 150 * dt));
    }
    if (engineContext.inputManager->IsKeyDown(KEY_A))
    {
        transform2D.AddPosition(glm::vec2(-150 * dt, 0));
    }
    if (engineContext.inputManager->IsKeyDown(KEY_S))
    {
        transform2D.AddPosition(glm::vec2(0, -150 * dt));
    }
    if (engineContext.inputManager->IsKeyDown(KEY_D))
    {
        transform2D.AddPosition(glm::vec2(150 * dt, 0));
    }

    if (engineContext.inputManager->IsKeyPressed(KEY_W))
    {
        spriteAnimator->PlayClip("backwalk");
        checkIdle++;
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_A))
    {
        spriteAnimator->PlayClip("sidewalk");
        transform2D.SetScale({ -50,50 });
        checkIdle++;
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_S))
    {
        spriteAnimator->PlayClip("frontwalk");
        checkIdle++;
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_D))
    {
        spriteAnimator->PlayClip("sidewalk");
        transform2D.SetScale({ 50,50 });
        checkIdle++;
    }

    if (engineContext.inputManager->IsKeyReleased(KEY_W))
    {
        checkIdle--;
    }
    if (engineContext.inputManager->IsKeyReleased(KEY_A))
    {
        checkIdle--;
    }
    if (engineContext.inputManager->IsKeyReleased(KEY_S))
    {
        checkIdle--;
    }
    if (engineContext.inputManager->IsKeyReleased(KEY_D))
    {
        checkIdle--;
    }
    if (checkIdle==0)
    {
        spriteAnimator->PlayClip("idle");
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_SPACE))
    {
        SNAKE_LOG("player shot the bullet");

        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * glm::pi<float>());

        float angle = angleDist(gen);
        std::unique_ptr<Bullet> b = std::make_unique<Bullet>(transform2D.GetPosition(), glm::vec2(std::cos(angle), std::sin(angle)));
        engineContext.stateManager->GetCurrentState()->GetObjectManager().AddObject(std::move(b),"bullet");
    }
}

void Player::Draw(const EngineContext& engineContext)
{
    //GetMaterial()->SetUniform("u_Color", glm::vec4(1.0, 1.0, 1.0, 1.0));
}

void Player::Free(const EngineContext& engineContext)
{
    SNAKE_LOG("Player Free Called");
}

void Player::LateFree(const EngineContext& engineContext)
{
    SNAKE_LOG("Player LateFree Called");
}

void Player::OnCollision(Object* other)
{
    if (other->GetTag() == "enemyBullet")
    {
        other->Kill();
    }
    if (other->GetTag() == "enemy")
    {
        other->Kill();
    }
}

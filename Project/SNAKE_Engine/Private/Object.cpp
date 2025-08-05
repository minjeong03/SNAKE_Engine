#include "Object.h"
#include "EngineContext.h"
#include "RenderManager.h"
#include "ObjectManager.h"
const bool& Object::IsAlive() const
{
    return isAlive;
}

const bool& Object::IsVisible() const
{
    return isVisible;
}

void Object::SetVisibility(bool _isVisible)
{
    isVisible = _isVisible;
}

void Object::Kill()
{
    isAlive = false;
}

void Object::SetTag(const std::string& tag)
{
    objectTag = tag;
}

const std::string& Object::GetTag() const
{
    return objectTag;
}

const uint8_t& Object::GetRenderLayer() const
{
    return renderLayer;
}

void Object::SetRenderLayer(const EngineContext& engineContext, const std::string& tag)
{
    renderLayer = engineContext.renderManager->GetRenderLayerManager().GetLayerID(tag).value_or(0);
}

void Object::SetMaterial(const EngineContext& engineContext, const std::string& tag)
{
    material = engineContext.renderManager->GetMaterialByTag(tag);
}


Material* Object::GetMaterial() const
{
    return material;
}

void Object::SetMesh(const EngineContext& engineContext, const std::string& tag)
{
    mesh = engineContext.renderManager->GetMeshByTag(tag);
}

Mesh* Object::GetMesh() const
{
    return mesh;
}

bool Object::CanBeInstanced() const
{
    if (!mesh || !material) return false;
    if (!material->IsInstancingSupported()) return false;
    return true;
}

glm::mat4 Object::GetTransform2DMatrix()
{
    return transform2D.GetMatrix();
}

Transform2D& Object::GetTransform2D()
{
    return transform2D;
}

void Object::SetColor(const  glm::vec4& color_)
{
    color = color_;
}

const glm::vec4& Object::GetColor()
{
    return color;
}

void Object::SetCollision(ObjectManager& objectManager, const std::string& tag, const std::vector<std::string>& checkCollisionList)
{
    auto& reg = objectManager.GetCollisionGroupRegistry();
    collisionCategory = reg.GetGroupBit(tag);

    collisionMask = 0;
    for (const auto& c : checkCollisionList)
        collisionMask |= reg.GetGroupBit(c);
}

bool Object::ShouldIgnoreCamera() const
{
    return ignoreCamera;
}

void Object::SetIgnoreCamera(bool shouldIgnoreCamera, Camera2D* cameraForTransformCalc)
{
    ignoreCamera = shouldIgnoreCamera;
    if (ignoreCamera)
    {
        referenceCamera = cameraForTransformCalc;
    }
}

glm::vec2 Object::GetWorldPosition() const
{
    if (ShouldIgnoreCamera() && referenceCamera)
    {
        float zoom = referenceCamera->GetZoom();
        glm::vec2 camPos = referenceCamera->GetPosition();
        glm::vec2 screenSpace = transform2D.GetPosition();

        int windowW = referenceCamera->GetScreenWidth();
        int windowH = referenceCamera->GetScreenHeight();

        glm::vec2 offset = screenSpace * glm::vec2(1.0f / zoom);
        glm::vec2 corrected = camPos + offset;

        return corrected;
    }
    else
    {
        return transform2D.GetPosition();
    }
}


glm::vec2 Object::GetWorldScale() const
{
    if (ShouldIgnoreCamera() && referenceCamera)
        return transform2D.GetScale() / referenceCamera->GetZoom();
    else
        return transform2D.GetScale();
}

glm::vec2 Object::GetUVFlipVector() const
{
    return { flipUV_X ? -1.0f : 1.0f, flipUV_Y ? -1.0f : 1.0f };
}


float Object::GetBoundingRadius() const
{
    glm::vec2 halfSize = mesh ? mesh->GetLocalBoundsHalfSize() : glm::vec2(0.5f);
    glm::vec2 scaled = halfSize * transform2D.GetScale();
    return glm::length(scaled);
}

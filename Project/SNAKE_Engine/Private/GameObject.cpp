#include "GameObject.h"
#include "EngineContext.h"
#include "RenderManager.h"

const bool& GameObject::IsAlive() const
{
    return isAlive;
}

const bool& GameObject::IsVisible() const
{
    return isVisible;
}

void GameObject::SetVisibility(bool _isVisible)
{
    isVisible = _isVisible;
}

void GameObject::Kill()
{
    isAlive = false;
}

void GameObject::SetTag(const std::string& tag)
{
    objectTag = tag;
}

const std::string& GameObject::GetTag() const
{
    return objectTag;
}

const uint8_t& GameObject::GetRenderLayer() const
{
    return renderLayer;
}

void GameObject::SetRenderLayer(const EngineContext& engineContext, const std::string& tag)
{
    renderLayer = engineContext.renderManager->GetLayerManager().GetLayerID(tag).value_or(0);
}

void GameObject::SetMaterial(const EngineContext& engineContext, const std::string& tag)
{
    material = engineContext.renderManager->GetMaterialByTag(tag);
}


Material* GameObject::GetMaterial() const
{
    return material;
}

void GameObject::SetMesh(const EngineContext& engineContext, const std::string& tag)
{
    mesh = engineContext.renderManager->GetMeshByTag(tag);
}

Mesh* GameObject::GetMesh() const
{
    return mesh;
}

bool GameObject::CanBeInstanced() const
{
    if (!mesh || !material) return false;
    if (!material->IsInstancingSupported()) return false;
    return true;
}

glm::mat4& GameObject::GetTransform2DMatrix()
{
    return transform2D.GetMatrix();
}

Transform2D& GameObject::GetTransform2D()
{
    return transform2D;
}
float GameObject::GetBoundingRadius() const
{
    glm::vec2 halfSize = mesh ? mesh->GetLocalBoundsHalfSize() : glm::vec2(0.5f);
    glm::vec2 scaled = halfSize * transform2D.GetScale();
    return glm::length(scaled);
}

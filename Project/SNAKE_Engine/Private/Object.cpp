#include "Object.h"
#include "EngineContext.h"
#include "RenderManager.h"

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

const glm::vec4& Object::GetColor()
{
    return color;
}

float Object::GetBoundingRadius() const
{
    glm::vec2 halfSize = mesh ? mesh->GetLocalBoundsHalfSize() : glm::vec2(0.5f);
    glm::vec2 scaled = halfSize * transform2D.GetScale();
    return glm::length(scaled);
}

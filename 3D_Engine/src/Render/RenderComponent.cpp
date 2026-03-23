#include "pch.h"
#include "RenderComponent.h"
#include "Mesh.h"

RendererComponent::RendererComponent()
    : m_mesh(nullptr)
{
}

RendererComponent::RendererComponent(std::unique_ptr<Mesh> mesh)
    : m_mesh(std::move(mesh))
{
}

void RendererComponent::SetMesh(std::unique_ptr<Mesh> mesh)
{
    m_mesh = std::move(mesh);
}

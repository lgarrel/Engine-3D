#include "pch.h"
#include "CameraComponent.h"


CameraComponent::CameraComponent(float fovY, float aspectRatio, float nearZ, float farZ)
{
    SetPerspective(fovY, aspectRatio, nearZ, farZ);
}

void CameraComponent::SetPerspective(float fovY, float aspectRatio, float nearZ, float farZ)
{
    m_camera.SetLens(fovY, aspectRatio, nearZ, farZ);
}

void CameraComponent::SetAspectRatio(float aspectRatio)
{
    SetPerspective(m_camera.GetFOV(), aspectRatio, m_camera.GetNear(), m_camera.GetFar());
}

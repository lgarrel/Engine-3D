#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
    SetLens();

    m_view = Identity4x4();
    m_proj = Identity4x4();

    m_dirtyView = true;
}

Camera::~Camera()
{
}

void Camera::UpdateView(Transform* transform)
{
    if (m_dirtyView == true)
    {
        XMVECTOR _right = XMLoadFloat3(&transform->m_right);
        XMVECTOR _up = XMLoadFloat3(&transform->m_up);
        XMVECTOR _forw = XMLoadFloat3(&transform->m_forward);
        XMVECTOR _pos = XMLoadFloat3(&transform->m_worldPosition);

        _forw = XMVector3Normalize(_forw);//normalize
        _up = XMVector3Normalize(XMVector3Cross(_forw, _right));//normalize + cross
        _right = XMVector3Cross(_up, _forw);//just cross cause _up && _forw = norm

        // Fill in the view matrix entries.
        float x = -XMVectorGetX(XMVector3Dot(_pos, _right));
        float y = -XMVectorGetX(XMVector3Dot(_pos, _up));
        float z = -XMVectorGetX(XMVector3Dot(_pos, _forw));

        XMStoreFloat3(&transform->m_right, _right);
        XMStoreFloat3(&transform->m_up, _up);
        XMStoreFloat3(&transform->m_forward, _forw);

        XMFLOAT4 matRow1 = { transform->m_right.x, transform->m_up.x, transform->m_forward.x, 0.f };
        XMFLOAT4 matRow2 = { transform->m_right.y, transform->m_up.y, transform->m_forward.y, 0.f };
        XMFLOAT4 matRow3 = { transform->m_right.z, transform->m_up.z, transform->m_forward.z, 0.f };
        XMFLOAT4 matRow4 = { x, y, z, 1.f };

        m_view = _Utils::XMFLT4ToXMFLT4X4(matRow1, matRow2, matRow3, matRow4);


        m_dirtyView = false;
    }
}

void Camera::OnUpdate(Transform* transform)
{
    m_dirtyView = true;
    UpdateView(transform);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void Camera::SetLens(float _fov, float _aspect, float _near, float _far)
{
    m_FOV = _fov;
    m_aspect = _aspect;
    m_nearPlane = _near;
    m_farPlane = _far;

    XMMATRIX P = XMMatrixPerspectiveFovLH(m_FOV, m_aspect, m_nearPlane, m_farPlane);
    XMStoreFloat4x4(&m_proj, P);
}

void Camera::Walk(Transform* transform, float _dist)
{    
    XMVECTOR _distV = XMVectorReplicate(_dist);
    XMVECTOR _for = XMLoadFloat3(&transform->m_forward);
    XMVECTOR _pos = XMLoadFloat3(&transform->m_localPosition);
    XMStoreFloat3(&transform->m_localPosition, XMVectorMultiplyAdd(_distV, _for, _pos));

    m_dirtyView = true;
}

void Camera::LookAt(Transform* transform, XMFLOAT3 _target, XMFLOAT3 _up)
{
    XMVECTOR view;

    XMVECTOR _pos = XMLoadFloat3(&transform->m_localPosition);
    XMVECTOR _tar = XMLoadFloat3(&_target);
    XMVECTOR _upV = XMLoadFloat3(&_up);

    XMVECTOR __forward = XMVector3Normalize(XMVectorSubtract(_tar, _pos));
    XMVECTOR __right = XMVector3Normalize(XMVector3Cross(_upV, __forward));
    XMVECTOR __up = XMVector3Cross(__forward, __right);

    XMStoreFloat3(&transform->m_forward, __forward);
    XMStoreFloat3(&transform->m_right, __right);
    XMStoreFloat3(&transform->m_up, __up);

    m_dirtyView = true;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void Camera::Yaw(float _angle)
{
    //TODO

    m_dirtyView = true;
}

void Camera::Pitch(float _angle)
{
    //TODO

    m_dirtyView = true;
}

void Camera::Roll(float _angle)
{
    //TODO

    m_dirtyView = true;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

float Camera::GetNear()
{
    return m_nearPlane;
}

float Camera::GetFar()
{
    return m_farPlane;
}

float Camera::GetAspect()
{
    return m_aspect;
}

float Camera::GetFOV()
{
    return m_FOV;
}

XMFLOAT4X4 Camera::GetView()
{
    assert(!m_dirtyView);
    return m_view;
}

XMFLOAT4X4 Camera::GetProj()
{
    return m_proj;
}

#include "pch.h"
#include "S_Rail.h"

#include "../Engine/Scene.h"

S_Rail::S_Rail()
{
}

S_Rail::~S_Rail()
{
    mp_centerRail->~Entity();
    mp_leftRail->~Entity();
    mp_rightRail->~Entity();

    mp_centerRail = nullptr;
    mp_leftRail = nullptr;
    mp_rightRail = nullptr;

    mp_device = nullptr;

    delete mp_centerRail;
    delete mp_device;
    delete mp_leftRail;
    delete mp_rightRail;

}

void S_Rail::Init(Scene* _pScene, XMFLOAT3 _enterPnt, XMFLOAT3 _forw)
{
    m_eef.m_forward = _forw;

    m_eef.m_enterPoint = _enterPnt;

    XMFLOAT3 _exit = m_eef.m_enterPoint;
    _exit.z += 2.2f;
    m_eef.m_exitPoint = MultiplyXMFLT3_XMFLT3(_exit, _forw);

    /*XMFLOAT3 _centerPos = PartialXMFLT3Add(_centerPos, { _Z }, { 1.2f });
    XMFLOAT3 _leftPos = PartialXMFLT3Add(_leftPos, { _X, _Z }, { -.5f, 1.2f });
    XMFLOAT3 _rightPos = PartialXMFLT3Add(_rightPos, { _X, _Z }, { .5f, 1.2f });*/


    XMFLOAT3 _centerPos = { _enterPnt.x,
        _enterPnt.y,
        _enterPnt.z + 1.f
    };
    XMFLOAT3 _leftPos = { _enterPnt.x - .5f,
        _enterPnt.y,
        _enterPnt.z + 1.f
    };
    XMFLOAT3 _rightPos = { _enterPnt.x + .5f,
        _enterPnt.y,
        _enterPnt.z + 1.f
    };

    // Create Center
    mp_centerRail = _pScene->CreateEntity();

    TransformComponent* _transformC = mp_centerRail->AddComponent<TransformComponent>();
    _transformC->SetLocalForward(_forw);
    _transformC->SetLocalPosition(_centerPos);
    _transformC->SetLocalScale(XMFLOAT3(1.f, 1.f, 1.f));

    std::unique_ptr<Mesh> _cubeMeshC = ShapeFactory::CreateBox(mp_device, 1.2f, .2f, .2f);

    RendererComponent* _meshRendererC = mp_centerRail->AddComponent<RendererComponent>();
    _meshRendererC->SetMesh(std::move(_cubeMeshC));

    MaterialComponent* _matC = mp_centerRail->AddComponent<MaterialComponent>();
    _matC->SetColor(_ORANGE);

    mp_centerRail->AddComponent<RigidBodyComponent>();

    ColliderComponent* _colC = mp_centerRail->AddComponent<ColliderComponent>();
    _colC->extents = XMFLOAT3{ 0.6f, 0.1f, 0.1f };

    // Create Left
    mp_leftRail = _pScene->CreateEntity();

    TransformComponent* _transformL = mp_leftRail->AddComponent<TransformComponent>();
    _transformL->SetLocalForward(_forw);
    _transformL->SetLocalPosition(_leftPos);
    _transformL->SetLocalScale(XMFLOAT3(1.f, 1.f, 1.f));

    std::unique_ptr<Mesh> _cubeMeshL = ShapeFactory::CreateBox(mp_device, .2f, .2f, 2.2f);

    RendererComponent* _meshRendererL = mp_leftRail->AddComponent<RendererComponent>();
    _meshRendererL->SetMesh(std::move(_cubeMeshL));

    MaterialComponent* _matL = mp_leftRail->AddComponent<MaterialComponent>();
    _matL->SetColor(_GRAY);

    mp_leftRail->AddComponent<RigidBodyComponent>();

    ColliderComponent* _colL = mp_leftRail->AddComponent<ColliderComponent>();
    _colL->extents = XMFLOAT3{ 0.1f, 0.1f, 1.1f };

    mp_leftRail->SetParent(mp_centerRail);

    // Create Right
    mp_rightRail = _pScene->CreateEntity();

    TransformComponent* _transformR = mp_rightRail->AddComponent<TransformComponent>();
    _transformR->SetLocalForward(_forw);
    _transformR->SetLocalPosition(_rightPos);
    _transformR->SetLocalScale(XMFLOAT3(1.f, 1.f, 1.f));

    std::unique_ptr<Mesh> _cubeMeshR = ShapeFactory::CreateBox(mp_device, .2f, .2f, 2.2f);

    RendererComponent* _meshRendererR = mp_rightRail->AddComponent<RendererComponent>();
    _meshRendererR->SetMesh(std::move(_cubeMeshR));

    MaterialComponent* _matR = mp_rightRail->AddComponent<MaterialComponent>();
    _matR->SetColor(_GRAY);

    mp_rightRail->AddComponent<RigidBodyComponent>();

    ColliderComponent* _colR = mp_rightRail->AddComponent<ColliderComponent>();
    _colR->extents = XMFLOAT3{ 0.1f, 0.1f, 1.1f };

    mp_rightRail->SetParent(mp_centerRail);
}

void S_Rail::Update(float _dt)
{

}
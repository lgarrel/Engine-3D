#include "pch.h"
#include "Rail.h"

#include "../Engine/Scene.h"

Rail::Rail()
{
}

Rail::~Rail()
{
    if (mp_scene)
    {
        mp_scene->DestroyEntity(mp_leftRail);
        mp_scene->DestroyEntity(mp_centerRail);
        mp_scene->DestroyEntity(mp_rightRail);
    }

    mp_leftRail = nullptr;
    mp_centerRail = nullptr;
    mp_rightRail = nullptr;
}

void Rail::Init(Scene* _pScene, XMFLOAT3 _enterPos, float _rotY)
{
    mp_scene = _pScene;

    /*XMFLOAT3 _centerPos = PartialXMFLT3Add(_centerPos, { _Z }, { 1.2f });
    XMFLOAT3 _leftPos = PartialXMFLT3Add(_leftPos, { _X, _Z }, { -.5f, 1.2f });
    XMFLOAT3 _rightPos = PartialXMFLT3Add(_rightPos, { _X, _Z }, { .5f, 1.2f });*/


    XMFLOAT3 _centerPos = { _enterPos.x,
        _enterPos.y,
        _enterPos.z + 1.1f
    };
    XMFLOAT3 _leftPos = { _enterPos.x - .6f,
        _enterPos.y + 0.2f,
        _enterPos.z + 1.1f
    };
    XMFLOAT3 _rightPos = { _enterPos.x + .6f,
        _enterPos.y + 0.2f,
        _enterPos.z + 1.1f
    };

    // Create Center
    mp_centerRail = mp_scene->CreateEntity();

    TransformComponent* _transformC = mp_centerRail->AddComponent<TransformComponent>();
    _transformC->SetLocalPosition(_centerPos);
    _transformC->SetLocalScale(XMFLOAT3(1.f, 1.f, 1.f));
    _transformC->RotateLocalY(_rotY);

    std::unique_ptr<Mesh> _cubeMeshC = ShapeFactory::CreateBox(mp_device, 2.2f, .2f, .2f);

    RendererComponent* _meshRendererC = mp_centerRail->AddComponent<RendererComponent>();
    _meshRendererC->SetMesh(std::move(_cubeMeshC));

    MaterialComponent* _matC = mp_centerRail->AddComponent<MaterialComponent>();
    _matC->SetColor(_ORANGE);

    ColliderComponent* _colC = mp_centerRail->AddComponent<ColliderComponent>();
    _colC->extents = XMFLOAT3{ 0.6f, 0.1f, 0.1f };

    // Create Left
    mp_leftRail = mp_scene->CreateEntity();

    TransformComponent* _transformL = mp_leftRail->AddComponent<TransformComponent>();
    _transformL->SetLocalPosition(_leftPos);
    _transformL->SetLocalScale(XMFLOAT3(1.f, 1.f, 1.f));
    _transformL->RotateLocalY(_rotY);

    std::unique_ptr<Mesh> _cubeMeshL = ShapeFactory::CreateBox(mp_device, .2f, .2f, 2.2f);

    RendererComponent* _meshRendererL = mp_leftRail->AddComponent<RendererComponent>();
    _meshRendererL->SetMesh(std::move(_cubeMeshL));

    MaterialComponent* _matL = mp_leftRail->AddComponent<MaterialComponent>();
    _matL->SetColor(_GRAY);

    ColliderComponent* _colL = mp_leftRail->AddComponent<ColliderComponent>();
    _colL->extents = XMFLOAT3{ 0.1f, 0.1f, 1.1f };


    // Create Right
    mp_rightRail = mp_scene->CreateEntity();

    TransformComponent* _transformR = mp_rightRail->AddComponent<TransformComponent>();
    _transformR->SetLocalPosition(_rightPos);
    _transformR->SetLocalScale(XMFLOAT3(1.f, 1.f, 1.f));
    _transformR->RotateLocalY(_rotY);

    std::unique_ptr<Mesh> _cubeMeshR = ShapeFactory::CreateBox(mp_device, .2f, .2f, 2.2f);

    RendererComponent* _meshRendererR = mp_rightRail->AddComponent<RendererComponent>();
    _meshRendererR->SetMesh(std::move(_cubeMeshR));

    MaterialComponent* _matR = mp_rightRail->AddComponent<MaterialComponent>();
    _matR->SetColor(_GRAY);

     ColliderComponent* _colR = mp_rightRail->AddComponent<ColliderComponent>();
     _colR->extents = XMFLOAT3{ 0.1f, 0.1f, 1.1f };

}

void Rail::Move(XMFLOAT3 _delta)
{
    auto tcC = mp_centerRail->GetComponent<TransformComponent>();
    auto tcL = mp_leftRail->GetComponent<TransformComponent>();
    auto tcR = mp_rightRail->GetComponent<TransformComponent>();

    XMFLOAT3 posC = tcC->GetLocalPosition();
    posC.x += _delta.x;
    posC.y += _delta.y;
    posC.z += _delta.z;
    tcC->SetLocalPosition(posC);

    XMFLOAT3 posL = tcL->GetLocalPosition();
    posL.x += _delta.x;
    posL.y += _delta.y;
    posL.z += _delta.z;
    tcL->SetLocalPosition(posL);

    XMFLOAT3 posR = tcR->GetLocalPosition();
    posR.x += _delta.x;
    posR.y += _delta.y;
    posR.z += _delta.z;
    tcR->SetLocalPosition(posR);
}

void Rail::Update(float _dt)
{

}
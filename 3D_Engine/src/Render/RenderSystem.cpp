#include "pch.h"

RenderSystem::RenderSystem()
{
}

void RenderSystem::Initialize(CommandContext* context, Device* device, UploadBuffer<ObjectConstants>* objectCB, UploadBuffer<PassConstants>* passCB, ID3D12DescriptorHeap* cbvHeap, LightSystem* lightSystem)
{
    m_context = context;
    m_device = device;
    m_objectCB = objectCB;
    m_passCB = passCB;
    m_cbvHeap = cbvHeap;
    m_lightSystem = lightSystem;
}

void RenderSystem::RenderScene(Scene* scene, CameraSystem* cameraSystem)
{
    if (scene == nullptr || cameraSystem == nullptr)
        return;

    XMFLOAT4X4 view;
    XMFLOAT4X4 proj;

    if (!cameraSystem->GetMainCameraMatrices(scene, view, proj))
        return;

    XMMATRIX V = XMLoadFloat4x4(&view);
    XMMATRIX P = XMLoadFloat4x4(&proj);
    XMMATRIX viewProj = XMMatrixMultiply(V, P);
    XMFLOAT4X4 vp;
    XMStoreFloat4x4(&vp, viewProj);
    m_frustum.FromViewProj(vp);

    ID3D12GraphicsCommandList* cmdList = m_context->List();

    PassConstants passCb;
    XMStoreFloat4x4(&passCb.View, XMMatrixTranspose(V));
    XMStoreFloat4x4(&passCb.Proj, XMMatrixTranspose(P));

    std::vector<Entity*> cameraEntities = scene->GetEntitiesWithComponent<CameraComponent>();
    if (!cameraEntities.empty())
    {
        TransformComponent* tc = cameraEntities[0]->GetComponent<TransformComponent>();
        if (tc) passCb.EyePosW = tc->GetTransform().m_worldPosition;
    }

    if (m_lightSystem)
    {
        m_lightSystem->FillPassConstants(scene, passCb);
    }
    else
    {
        for (int i = 0; i < 16; ++i)
            passCb.Lights[i].m_strength = { 0.f, 0.f, 0.f };
    }

    m_passCB->CopyData(0, passCb);

    D3D12_GPU_DESCRIPTOR_HANDLE passCbvHandle = m_cbvHeap->GetGPUDescriptorHandleForHeapStart();
    passCbvHandle.ptr += (SIZE_T)(2 * GraphicsEngine::m_maxObjects) * m_device->GetCbvSrvUavSize();
    cmdList->SetGraphicsRootDescriptorTable(1, passCbvHandle);

    std::vector<Entity*> entities = scene->GetEntitiesWithComponent<RendererComponent>();

    int cbIndex = 0;
    for (Entity* entity : entities)
    {
        RenderEntity(entity, cbIndex);
        cbIndex++;
    }
}

void RenderSystem::RenderEntity(Entity* entity, int cbIndex)
{
    TransformComponent* transform = entity->GetComponent<TransformComponent>();
    RendererComponent* meshRenderer = entity->GetComponent<RendererComponent>();
    MaterialComponent* matComp = entity->GetComponent<MaterialComponent>();
    ColliderComponent* collider = entity->GetComponent<ColliderComponent>();

    if (transform == nullptr || meshRenderer == nullptr || !meshRenderer->HasMesh())
        return;

    Sphere boundingSphere;
    boundingSphere.center = transform->GetTransform().m_worldPosition;

    if (collider)
    {
        XMMATRIX worldMat = XMLoadFloat4x4(&transform->GetWorldMatrix());
        XMVECTOR scale, rot, trans;
        XMMatrixDecompose(&scale, &rot, &trans, worldMat);

        XMFLOAT3 worldScale;
        XMStoreFloat3(&worldScale, scale);

        XMVECTOR extentsVec = XMVectorSet(
            collider->extents.x * worldScale.x,
            collider->extents.y * worldScale.y,
            collider->extents.z * worldScale.z,
            0.0f
        );

        XMStoreFloat(&boundingSphere.radius, XMVector3Length(extentsVec));
    }
    else
    {
        boundingSphere.radius = 2.0f;
    }

    if (!m_frustum.Intersect(boundingSphere))
    {
        std::cout << "[CULLING] Objet ignore : " << entity->GetTag() << " (Rayon: " << boundingSphere.radius << ")" << std::endl;
        return;
    }

    Mesh* mesh = meshRenderer->GetMesh();
    ID3D12GraphicsCommandList* cmdList = m_context->List();

    ObjectConstants cb;
    XMStoreFloat4x4(&cb.m_world, XMMatrixTranspose(XMLoadFloat4x4(&transform->GetWorldMatrix())));

    int texIndex = 0;
    if (matComp)
    {
        cb.m_material = matComp->GetMaterial();
        if (matComp->GetTextureIndex() >= 0)
        {
            texIndex = matComp->GetTextureIndex();
        }
    }
    else
    {
        cb.m_material.m_diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        cb.m_material.m_schlick = XMFLOAT3(0.05f, 0.05f, 0.05f);
        cb.m_material.m_shininess = 0.5f;
    }
    m_objectCB->CopyData(cbIndex, cb);

    D3D12_GPU_DESCRIPTOR_HANDLE cbvHandle = m_cbvHeap->GetGPUDescriptorHandleForHeapStart();
    cbvHandle.ptr += (SIZE_T)cbIndex * m_device->GetCbvSrvUavSize();
    cmdList->SetGraphicsRootDescriptorTable(0, cbvHandle);

    D3D12_GPU_DESCRIPTOR_HANDLE texSrvHandle = m_cbvHeap->GetGPUDescriptorHandleForHeapStart();
    texSrvHandle.ptr += (SIZE_T)(2 * GraphicsEngine::m_maxObjects + GraphicsEngine::m_maxPass + 1 + texIndex) * m_device->GetCbvSrvUavSize();
    cmdList->SetGraphicsRootDescriptorTable(2, texSrvHandle);

    D3D12_VERTEX_BUFFER_VIEW vbv = mesh->GetVertexBufferView();
    D3D12_INDEX_BUFFER_VIEW  ibv = mesh->GetIndexBufferView();
    cmdList->IASetVertexBuffers(0, 1, &vbv);
    cmdList->IASetIndexBuffer(&ibv);
    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmdList->DrawIndexedInstanced(mesh->GetIndexCount(), 1, 0, 0, 0);
}

void RenderSystem::Update(Scene* currentScene, float deltaTime)
{
}
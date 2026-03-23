#pragma once

class RendererComponent : public Component
{
private:
	std::unique_ptr<Mesh> m_mesh;

public:
	RendererComponent();
	RendererComponent(std::unique_ptr<Mesh> mesh);

	const char* GetTypeName() const override { return "MeshRendererComponent"; }

	Mesh* GetMesh() { return m_mesh.get(); }
	void SetMesh(std::unique_ptr<Mesh> mesh);
	bool HasMesh() const { return m_mesh != nullptr; }
};


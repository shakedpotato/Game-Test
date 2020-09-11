#pragma once
#include "Renderer.h"
#include "Mesh.h"
#include "Shader3D.h"
#include "Camera.h"

class MeshRenderer
	:
	public Renderer
{
public:
	MeshRenderer();
	~MeshRenderer();
private:
	std::weak_ptr<Mesh> m_Mesh;
	std::shared_ptr<Shader3D> m_Shader;
public:
	void Render(std::weak_ptr<Camera> camera) override;
	void SetMeshData(std::weak_ptr<Mesh> mesh);
	void SetShader3D(std::shared_ptr<Shader3D> shader);
	std::weak_ptr<Shader3D> GetShader() { return m_Shader; }
private:
	void OnAwake();
};
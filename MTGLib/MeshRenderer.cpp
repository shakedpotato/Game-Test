#include "RenderingExecuter.h"
#include "MeshRenderer.h"
#include "Camera.h"
using namespace std;

MeshRenderer::MeshRenderer()
	:
	Renderer("MeshRenderer")
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Render(std::weak_ptr<Camera> camera)
{
	
	shared_ptr<Transform> transform = GetGameObject().lock()->GetComponent<Transform>().lock();
	// マトリックスの計算
	XMMATRIX world, view, proj;

	// camera kara morattekuru
	world = XMMatrixScaling(transform->GetLocalScale()->x, transform->GetLocalScale()->y, transform->GetLocalScale()->z);
	world *= transform->GetRotationMatrix();
	// world *= XMMatrixRotationRollPitchYaw( transform->GetLocalRotation()->x, transform->GetLocalRotation()->y, transform->GetLocalRotation()->z );
	world *= XMMatrixTranslation(transform->GetLocalPosition()->x, transform->GetLocalPosition()->y, transform->GetLocalPosition()->z);

	view = camera.lock()->GetViewMatrix();
	proj = camera.lock()->GetProjectionMatrix();

	XMMATRIX wvp = world * view * proj;

	XMFLOAT4X4 wvpf;
	XMStoreFloat4x4(&wvpf, wvp);

	XMFLOAT4X4 worldf;
	XMStoreFloat4x4(&worldf, world);


	// シェーダの設定
	m_Shader->SetWorldViewProjMatrix(&wvpf);
	m_Shader->SetWorldMatrix(&worldf);
	m_Shader->Set();

	// 描画命令
	m_Mesh.lock()->Draw();
}

void MeshRenderer::SetMeshData(std::weak_ptr<Mesh> mesh)
{
	m_Mesh = mesh;
}

void MeshRenderer::SetShader3D(std::shared_ptr<Shader3D> shader)
{
	m_Shader = shader;
}

void MeshRenderer::OnAwake()
{
	OrderedSingleton<RenderingExecuter>::Instance()->SetRenderer(GetSharedFromThisRenderer());
}
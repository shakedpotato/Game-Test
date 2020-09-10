#include "DX11Executor.h"
#include "Holders.h"
#include "RenderingExecuter.h"

using DX11 = OrderedSingleton<DX11Executor>;

RenderingExecuter::RenderingExecuter()
{
	OrderedSingleton<DX11Executor>::Instance()->Init();
	OrderedSingleton<MeshHolder>::Instance();
	OrderedSingleton<Shader3D>::Instance();
}

RenderingExecuter::~RenderingExecuter()
{

}

void RenderingExecuter::RenderAll()
{
	if (m_RendererCount < 1)
	{
		return;
	}

	DX11::Instance()->Begin();

	std::shared_ptr<Camera> camera = m_Camera.lock();

	camera->PrepareForRendering();

	for (auto r : m_Renderers)
	{
		r.lock()->Render(camera);
	}
	
	DX11::Instance()->End();

}

void RenderingExecuter::SetRenderer(std::weak_ptr<Renderer> renderer)
{
	// renderer.lock()
	m_Renderers.push_back(renderer);
	m_RendererCount++;
}

void RenderingExecuter::SetCamera(std::weak_ptr<Camera> camera)
{
	m_Camera = camera;
}
#pragma once
#include "Main.h"
#include "OrderedSingleton.h"
#include "Renderer.h"
#include "Camera.h"

class RenderingExecuter
{
private:
	RenderingExecuter();
	~RenderingExecuter();
	friend class OrderedSingleton<RenderingExecuter>;
private:
	unsigned int m_RendererCount = 0;
	std::list<std::weak_ptr<Renderer>> m_Renderers;
	std::weak_ptr<Camera> m_Camera;
public:
	void RenderAll();
public:
	void SetRenderer(std::weak_ptr<Renderer> renderer);
	void SetCamera(std::weak_ptr<Camera> camera);
};
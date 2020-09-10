#pragma once
#include <memory>
#include "ComponentBase.h"
#include "Camera.h"

class Renderer
	:
	public ComponentBase
{
public:
	Renderer(const TCHAR* name) : ComponentBase(name) {}
	virtual ~Renderer() {}

public:
	virtual void Render(std::weak_ptr<Camera> camera) {}
public:
	std::shared_ptr<Renderer> GetSharedFromThisRenderer() { return std::dynamic_pointer_cast<Renderer>(ComponentBase::shared_from_this()); } 
};
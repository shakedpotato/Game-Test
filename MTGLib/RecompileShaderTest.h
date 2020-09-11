#pragma once
#include "ComponentEssential.h"
#include "KeyInput.h"
#include "MeshRenderer.h"

class RecompileShaderTest
	:
	public ComponentBase
{
public:
	RecompileShaderTest()
		:
		ComponentBase("RecompileShaderTest")
	{

	}
private:
	std::weak_ptr<MeshRenderer> m_Renderer;
	std::weak_ptr<Shader3D> m_Shader;
public:
	void OnAwake() override
	{
		m_Renderer = GetGameObject().lock()->GetComponent<MeshRenderer>();
		m_Shader = m_Renderer.lock()->GetShader();
	}
	void OnUpdate(float dt) override
	{
		KeyInput* input = OrderedSingleton<KeyInput>::Instance();

		if (input->GetKeyPress('R'))
		{
			m_Shader.lock()->CompilePixelShader("data/SHADER/PixelShader3D.hlsl");
		}
	}
};
#include "ShaderHolder.h"

using namespace std;

Shader3DHolder::Shader3DHolder()
{

}

Shader3DHolder::~Shader3DHolder()
{
	for (auto shader : m_Shaders)
	{
		shader.second->Uninit();
	}

	m_Shaders.clear();
}

void Shader3DHolder::LoadShader(const string& vsh, const string& psh, const string& shaderSetName)
{
	std::shared_ptr<Shader3D> shader = make_shared<Shader3D>();

	shader->Init(vsh, psh);

	m_Shaders[shaderSetName] = shader;
}

void Shader3DHolder::LoadShader(const char* vsh, const char* psh, const char* shaderSetName)
{
	std::shared_ptr<Shader3D> shader = make_shared<Shader3D>();

	shader->Init(vsh, psh);

	m_Shaders[shaderSetName] = shader;
}

weak_ptr<Shader3D> Shader3DHolder::GetShader(const std::string& shaderSetName)
{
	return m_Shaders[shaderSetName];
}
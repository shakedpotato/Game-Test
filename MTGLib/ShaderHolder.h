#pragma once
#include "Main.h"
#include "OrderedSingleton.h"
#include "Shader3D.h"


class Shader3DHolder
{
private:
	Shader3DHolder();
	~Shader3DHolder();
	friend class OrderedSingleton<Shader3DHolder>;
private:
	std::unordered_map<std::string, std::shared_ptr<Shader3D>> m_Shaders;
public:
	// shader set name‚Æ‚©‚¢‚¤‚Ì‚â‚Î‚¢
	void LoadShader(const std::string& vsh, const std::string& psh, const std::string& shaderSetName);
	void LoadShader(const char* vsh, const char* psh, const char* shaderSetName);
	std::weak_ptr<Shader3D> GetShader(const std::string& shaderSetName);
};




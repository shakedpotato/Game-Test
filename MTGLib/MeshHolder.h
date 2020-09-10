#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "OrderedSingleton.h"
#include "Mesh.h"

class MeshHolder
{
private:
	MeshHolder();
	~MeshHolder();
	friend class OrderedSingleton<MeshHolder>;
private:
	std::unordered_map<std::string, std::shared_ptr<Mesh>> m_Meshes;
public:
	void Load(const TCHAR* filePass);
	void Load(std::string filePass);
public:
	std::weak_ptr<Mesh> GetData(std::string filePass);
};
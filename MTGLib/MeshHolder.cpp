#include "MeshHolder.h"

using namespace std;

MeshHolder::MeshHolder()
{

}

MeshHolder::~MeshHolder()
{

}

void MeshHolder::Load(const TCHAR* path)
{
	string p(path);
	Load(p);
}

void MeshHolder::Load(string path)
{
	m_Meshes[path] = make_shared<Mesh>();
	m_Meshes[path]->Load(path.c_str());
}

weak_ptr<Mesh> MeshHolder::GetData(std::string filePath)
{
	return m_Meshes[filePath];
}


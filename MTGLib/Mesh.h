#pragma once
#include "Main.h"
#include "DX11Executor.h"

// マテリアル構造体
struct ModelMaterialInfo
{
	char			Name[ 256 ];
	MaterialData	data;
	char			TextureName[ 256 ];
};

// 描画サブセット構造体
struct RenderingSubset
{
	unsigned short		StartIndex;
	unsigned short		IndexNum;
	ModelMaterialInfo	Material;
};

struct ModelInfo
{
	Vertex3D*			VertexArray;
	unsigned short		VertexNum;
	unsigned short*		IndexArray;
	unsigned short		IndexNum;
	RenderingSubset*	SubsetArray;
	unsigned short		SubsetNum;
};


// mesh for DX11 rendering
class Mesh
{
public:
	Mesh() {}
	~Mesh() {}
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;

	DX11_SUBSET* m_SubsetArray = NULL;
	unsigned short	m_SubsetNum = 0;

	void LoadObj( const char* FileName, ModelInfo* Model );
	void LoadMaterial( const char* FileName, ModelMaterialInfo** MaterialArray, unsigned short* MaterialNum );
public:
	void Draw();
	void Load( const TCHAR* fileName );
	void Unload();
	
public:
	// void Test();
	ID3D11Buffer* instanceBuffer = nullptr;
	int vertexCount = 0;
	int indexCount = 0;
};
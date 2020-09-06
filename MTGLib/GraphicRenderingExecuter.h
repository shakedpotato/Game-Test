#pragma once
#pragma once
#include "GameBasicSystem.h"
// DirectX
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class Texture;

// ���_�\����
struct Vertex3D
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
};




//// �F�\����
//struct Color
//{
//	Color() {}
//	Color(float _r, float _g, float _b, float _a)
//		:
//		r(0.0f), g(0.0f), b(0.0f), a(0.0f)
//	{
//		r = _r;
//		g = _g;
//		b = _b;
//		a = _a;
//	}
//
//	float r;
//	float g;
//	float b;
//	float a;
//};
//
//// �}�e���A���\����
//struct MaterialInfo
//{
//	Color		Ambient;
//	Color		Diffuse;
//	Color		Specular;
//	Color		Emission;
//	float		Shininess;
//	float		Dummy[ 3 ];//16bit���E�p
//};
//
//
//
//struct LIGHT
//{
//	XMFLOAT4	Direction;
//	Color		Diffuse;
//	Color		Ambient;
//};
//
//
//// �}�e���A���\����
//struct Dx11ModelMaterial
//{
//	MaterialInfo	Material;
//	class Texture*	Texture;
//};
//
//
//// �`��T�u�Z�b�g�\����
//struct DX11_SUBSET
//{
//	unsigned short	StartIndex;
//	unsigned short	IndexNum;
//	Dx11ModelMaterial	Material;
//};

class GraphicRenderingExecuter final
{
private:
	GraphicRenderingExecuter();
	friend class GameBasicSystemObject<GraphicRenderingExecuter>;
	~GraphicRenderingExecuter();
private:
	D3D_FEATURE_LEVEL			m_FeatureLevel; // DirectX �o�[�W����

	ID3D11Device*				m_D3DDevice;    
	ID3D11DeviceContext*		m_ImmediateContext;
	IDXGISwapChain*				m_SwapChain;
	ID3D11RenderTargetView*		m_FinalRenderTargetView;
	ID3D11DepthStencilView*		m_FinalDepthStencilView;

	ID3D11DepthStencilState*	m_DepthStateEnable;
	ID3D11DepthStencilState*	m_DepthStateDisable;


public:
	void Init();
	void Begin();
	void End();

	void SetDepthEnable(bool Enable);

	template<typename VertexInfoType>
	void SetVertexBuffers(ID3D11Buffer* VertexBuffer);
	void SetIndexBuffer(ID3D11Buffer* IndexBuffer);
	void SetTexture(Texture* Texture);
	void DrawIndexed(unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation);

	ID3D11Device* GetDevice(void) { return m_D3DDevice; }
	ID3D11DeviceContext* GetDeviceContext(void) { return m_ImmediateContext; }
};


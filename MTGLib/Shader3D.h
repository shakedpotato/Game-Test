#pragma once
#include "Main.h"
#include "DX11Executor.h"




class Shader3D
{
	struct CONSTANT3D
	{
		XMFLOAT4X4 WorldViewProjMatrix;
		XMFLOAT4X4 WorldMatrix;
	};
public:
	Shader3D() {}
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11Buffer* m_ConstantBuffer = nullptr; // 毎フレーム変更するマテリアル, 行列, ライト情報などなどを格納する。 
	CONSTANT3D			m_Constant;

public:
	void Init(const char* VertexShader, const char* PixelShader);
	void Init(const std::string& vertexShader, const std::string& pixelShader);
	void InitWithCompiledFile(const char* VertexShader, const char* PixelShader);
	void LoadVertexShader(const char* file);
	void CompileVertexShader(const char* file);
	void CompileVertexShader(const char* file, const char* entryPointName, const char* targetModel);
	void CompilePixelShader(const char* file);
	void CompilePixelShader(const char* path, const char* entryPointName, const char* targetModel);
	void LoadPixelShader(const char* file);
	void Uninit();
	void Set();
	
	void SetWorldViewProjMatrix(XMFLOAT4X4* Matrix) { m_Constant.WorldViewProjMatrix = Transpose(Matrix); }
	void SetWorldMatrix(XMFLOAT4X4* Matrix) { m_Constant.WorldMatrix= Transpose(Matrix); }

	XMFLOAT4X4 Transpose(XMFLOAT4X4* Matrix)
	{
		XMFLOAT4X4 outMatrix;

		outMatrix._11 = Matrix->_11;
		outMatrix._12 = Matrix->_21;
		outMatrix._13 = Matrix->_31;
		outMatrix._14 = Matrix->_41;

		outMatrix._21 = Matrix->_12;
		outMatrix._22 = Matrix->_22;
		outMatrix._23 = Matrix->_32;
		outMatrix._24 = Matrix->_42;

		outMatrix._31 = Matrix->_13;
		outMatrix._32 = Matrix->_23;
		outMatrix._33 = Matrix->_33;
		outMatrix._34 = Matrix->_43;

		outMatrix._41 = Matrix->_14;
		outMatrix._42 = Matrix->_24;
		outMatrix._43 = Matrix->_34;
		outMatrix._44 = Matrix->_44;

		return outMatrix;
	}

	template<typename ConstantT>
	void CreateConstantBuffer()
	{

		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		hBufferDesc.ByteWidth = sizeof(ConstantT);

		DX11Executor* dx11 = OrderedSingleton<DX11Executor>::Instance();
		dx11->GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_ConstantBuffer);
	}

};
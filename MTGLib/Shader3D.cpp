
#include "Shader3D.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <stdio.h>

void Shader3D::InitWithCompiledFile(const char* vertexShader, const char* pixelShader)
{
	DX11Executor* dx11 = OrderedSingleton<DX11Executor>::Instance();

	LoadVertexShader(vertexShader);
	LoadPixelShader(pixelShader);
	//// 頂点シェーダ生成
	//{
	//	FILE* file;
	//	long int fsize;

	//	file = fopen(VertexShader, "rb");
	//	assert(file);
	//	fsize = _filelength(_fileno(file));
	//	unsigned char* buffer = new unsigned char[fsize];
	//	fread(buffer, fsize, 1, file);
	//	fclose(file);

	//	HRESULT gr = dx11->GetDevice()->CreateVertexShader(buffer, fsize, NULL, &m_VertexShader);

	//	// 入力レイアウト生成
	//	{
	//		D3D11_INPUT_ELEMENT_DESC layout[] =
	//		{
	//			{
	//				"POSITION",
	//				0, // セマンテック番号。今回はPOSITION + 0。
	//				DXGI_FORMAT_R32G32B32_FLOAT,
	//				0,
	//				0, // データ中のオフセット POSITIONは先頭なので( 4 * 0 )バイト
	//				D3D11_INPUT_PER_VERTEX_DATA,
	//				0
	//			},
	//			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//		};
	//		UINT numElements = ARRAYSIZE(layout);

	//		HRESULT hr = dx11->GetDevice()->CreateInputLayout(layout,
	//			numElements,
	//			buffer,
	//			fsize,
	//			&m_VertexLayout);
	//	}

	//	delete[] buffer;
	//}

	//// ピクセルシェーダ生成
	//{
	//	FILE* file;
	//	long int fsize;

	//	file = fopen(PixelShader, "rb");
	//	assert(file);
	//	fsize = _filelength(_fileno(file));
	//	unsigned char* buffer = new unsigned char[fsize];
	//	fread(buffer, fsize, 1, file);
	//	fclose(file);

	//	dx11->GetDevice()->CreatePixelShader(buffer, fsize, NULL, &m_PixelShader);

	//	delete[] buffer;
	//}



	// 定数バッファ生成
	{
		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		hBufferDesc.ByteWidth = sizeof(CONSTANT3D);
		dx11->GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_ConstantBuffer);
	}
}

void Shader3D::Init(const char* vertexShader, const char* pixelShader)
{
	DX11Executor* dx11 = OrderedSingleton<DX11Executor>::Instance();

	CompileVertexShader(vertexShader);
	CompilePixelShader(pixelShader);
	// 定数バッファ生成
	{
		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		hBufferDesc.ByteWidth = sizeof(CONSTANT3D);
		dx11->GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_ConstantBuffer);
	}
}
void Shader3D::LoadVertexShader(const char* path)
{

	if (m_VertexLayout)		m_VertexLayout->Release();
	if (m_VertexShader)		m_VertexShader->Release();

	DX11Executor* dx11 = OrderedSingleton<DX11Executor>::Instance();

	FILE* file;
	long int fsize;

	file = fopen(path, "rb");
	assert(file);
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	HRESULT gr = dx11->GetDevice()->CreateVertexShader(buffer, fsize, NULL, &m_VertexShader);

	// 入力レイアウト生成
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{
				"POSITION",
				0, // セマンテック番号。今回はPOSITION + 0。
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				0, // データ中のオフセット POSITIONは先頭なので( 4 * 0 )バイト
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT numElements = ARRAYSIZE(layout);

		HRESULT hr = dx11->GetDevice()->CreateInputLayout(layout,
			numElements,
			buffer,
			fsize,
			&m_VertexLayout);
	}

	delete[] buffer;

}

void Shader3D::CompileVertexShader(const char* path)
{
	if (m_VertexLayout)		m_VertexLayout->Release();
	if (m_VertexShader)		m_VertexShader->Release();

	DX11Executor* dx11 = OrderedSingleton<DX11Executor>::Instance();

	size_t len = 0;
	WCHAR wpath[256];
	mbstowcs_s(&len, wpath, 256, path, _TRUNCATE);

	ID3DBlob* pBlob;
	ID3DBlob* pErrorMsg;
	HRESULT hr = D3DCompileFromFile(
		wpath,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_4_0_level_9_3",
		0,
		0,
		&pBlob,
		&pErrorMsg
	);
	if (FAILED(hr))
	{
		// シェーダのエラー内容を表示
		MessageBox(NULL, (char*)pErrorMsg->GetBufferPointer(), "Compile Error", MB_OK);
		delete pBlob;
	}
	else
	{
		HRESULT gr = dx11->GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_VertexShader);

	// 入力レイアウト生成
		{
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{
					"POSITION",
					0, // セマンテック番号。今回はPOSITION + 0。
					DXGI_FORMAT_R32G32B32_FLOAT,
					0,
					0, // データ中のオフセット POSITIONは先頭なので( 4 * 0 )バイト
					D3D11_INPUT_PER_VERTEX_DATA,
					0
				},
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			UINT numElements = ARRAYSIZE(layout);

			HRESULT hr = dx11->GetDevice()->CreateInputLayout(layout,
				numElements,
				pBlob->GetBufferPointer(),
				pBlob->GetBufferSize(),
				&m_VertexLayout);
		}

		delete[] pErrorMsg;
	}
}

void Shader3D::LoadPixelShader(const char* path)
{

	if (m_PixelShader)		m_PixelShader->Release();

	DX11Executor* dx11 = OrderedSingleton<DX11Executor>::Instance();

	FILE* file;
	long int fsize;

	file = fopen(path, "rb");
	assert(file);
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	dx11->GetDevice()->CreatePixelShader(buffer, fsize, NULL, &m_PixelShader);

	delete[] buffer;

}

void Shader3D::CompilePixelShader(const char* path)
{
	if (m_PixelShader)		m_PixelShader->Release();

	DX11Executor* dx11 = OrderedSingleton<DX11Executor>::Instance();

	size_t len = 0;
	WCHAR wpath[256];
	mbstowcs_s(&len, wpath, 256, path, _TRUNCATE);

	ID3DBlob* pBlob;
	ID3DBlob* pErrorMsg;
	HRESULT hr = D3DCompileFromFile(
		wpath,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_4_0_level_9_3",
		0,
		0,
		&pBlob,
		&pErrorMsg
	);
	if (FAILED(hr))
	{
		// シェーダのエラー内容を表示
		MessageBox(NULL, (char*)pErrorMsg->GetBufferPointer(), "Compile Error", MB_OK);
		delete pBlob;
	}
	else
	{
		dx11->GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_PixelShader);
	}

	

	delete[] pErrorMsg;
}

void Shader3D::Init(const std::string& vsh, const std::string& psh)
{
	Init(vsh.c_str(), psh.c_str());
}

void Shader3D::Uninit()
{
	if (m_ConstantBuffer)	m_ConstantBuffer->Release();
	if (m_VertexLayout)		m_VertexLayout->Release();
	if (m_VertexShader)		m_VertexShader->Release();
	if (m_PixelShader)		m_PixelShader->Release();
}

void Shader3D::Set()
{
	DX11Executor* dx11 = OrderedSingleton<DX11Executor>::Instance();

	// シェーダ設定
	dx11->GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	dx11->GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// 入力レイアウト設定
	dx11->GetDeviceContext()->IASetInputLayout(m_VertexLayout);


	// 定数バッファ更新
	dx11->GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_Constant, 0, 0);

	// 定数バッファ設定
	dx11->GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	dx11->GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);
}

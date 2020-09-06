#include "GameWindow.h"
#include "Texture.h"
#include "GraphicRenderingExecuter.h"

GraphicRenderingExecuter::GraphicRenderingExecuter()
{

}

GraphicRenderingExecuter::~GraphicRenderingExecuter()
{
		// �I�u�W�F�N�g���
	if (m_ImmediateContext)	m_ImmediateContext->ClearState();
	if (m_FinalRenderTargetView)	m_FinalRenderTargetView->Release();
	if (m_SwapChain)			m_SwapChain->Release();
	if (m_ImmediateContext)	m_ImmediateContext->Release();
	if (m_D3DDevice)			m_D3DDevice->Release();
}

void GraphicRenderingExecuter::Init()
{
	HRESULT hr = S_OK;

	unsigned int width = GameBasicSystemObject<GameWindow>::Instance()->Width();
	unsigned int height = GameBasicSystemObject<GameWindow>::Instance()->Height();
	HWND hWnd = GameBasicSystemObject<GameWindow>::Instance()->HWND();

	// �f�o�C�X�A�X���b�v�`�F�[���A�R���e�L�X�g����
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_SwapChain,
		&m_D3DDevice,
		&m_FeatureLevel,
		&m_ImmediateContext);


	{
		// �����_�[�^�[�Q�b�g�r���[�����A�ݒ� // �����������̗]�n����
		ID3D11Texture2D* pBackBuffer = NULL;
		m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_FinalRenderTargetView);
		pBackBuffer->Release();



		//�X�e���V���p�e�N�X�`���[�쐬
		ID3D11Texture2D* depthTexture = NULL;
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = sd.BufferDesc.Width;
		td.Height = sd.BufferDesc.Height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		td.SampleDesc = sd.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		m_D3DDevice->CreateTexture2D(&td, NULL, &depthTexture);

		//�X�e���V���^�[�Q�b�g�쐬
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = td.Format;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Flags = 0;
		m_D3DDevice->CreateDepthStencilView(depthTexture, &dsvd, &m_FinalDepthStencilView);


		m_ImmediateContext->OMSetRenderTargets(1, &m_FinalRenderTargetView, m_FinalDepthStencilView);



	}

	// �r���[�|�[�g�ݒ� // �����������̗]�n����
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_ImmediateContext->RSSetViewports(1, &vp);



	// ���X�^���C�U�X�e�[�g�ݒ�
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID; // D3D11_FILL_SOLID(filled triangle), D3D11_FILL_WIREFRAME(just lines connecting the vertices).
	rd.CullMode = D3D11_CULL_NONE;	// Culling mode.NONE, FRONT, BACK
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	ID3D11RasterizerState* rs;
	m_D3DDevice->CreateRasterizerState(&rd, &rs);

	m_ImmediateContext->RSSetState(rs);

	// �u�����h�X�e�[�g�ݒ�
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	m_D3DDevice->CreateBlendState(&blendDesc, &blendState);
	m_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);



	// �[�x�X�e���V���X�e�[�g�ݒ�
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = FALSE;

	m_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);//�[�x�L���X�e�[�g

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable);//�[�x�����X�e�[�g

	m_ImmediateContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);




	// �T���v���[�X�e�[�g�ݒ�
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = NULL;
	m_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);

	m_ImmediateContext->PSSetSamplers(0, 1, &samplerState);



}

void GraphicRenderingExecuter::Begin()
{
	MyOutputDebugString(_T("GraphicRenderExecuter::Begin()\n"));
	// �o�b�N�o�b�t�@�N���A
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_ImmediateContext->ClearRenderTargetView(m_FinalRenderTargetView, ClearColor);
	m_ImmediateContext->ClearDepthStencilView(m_FinalDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}



void GraphicRenderingExecuter::End()
{
	MyOutputDebugString(_T("GraphicRenderExecuter::End()\n"));
	m_SwapChain->Present(1, 0);
}




void GraphicRenderingExecuter::SetDepthEnable(bool Enable)
{
	if (Enable)
		m_ImmediateContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);
	else
		m_ImmediateContext->OMSetDepthStencilState(m_DepthStateDisable, NULL);
}

template<typename VertexInfoTypeT>
void GraphicRenderingExecuter::SetVertexBuffers(ID3D11Buffer* VertexBuffer)
{
	UINT stride = sizeof(VertexInfoTypeT);
	UINT offset = 0;
	ID3D11Buffer* vb[1] = { VertexBuffer };
	m_ImmediateContext->IASetVertexBuffers(0, 1, vb, &stride, &offset);
}



void GraphicRenderingExecuter::SetIndexBuffer(ID3D11Buffer* IndexBuffer)
{
	m_ImmediateContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}


void GraphicRenderingExecuter::SetTexture(Texture* Texture)
{
	ID3D11ShaderResourceView* srv[1] = { Texture->GetShaderResourceView() };
	m_ImmediateContext->PSSetShaderResources(0, 1, srv);
}


void GraphicRenderingExecuter::DrawIndexed(unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation)
{

	m_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_ImmediateContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);

}
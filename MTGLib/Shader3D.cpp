#include "Shader3D.h"

void Shader3D::Init(const char* VertexShader, const char* PixelShader)
{
	DX11Executor* dx11 = OrderedSingleton<DX11Executor>::Instance();

	// ���_�V�F�[�_����
	{
		FILE* file;
		long int fsize;

		file = fopen(VertexShader, "rb");
		assert(file);
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[ fsize ];
		fread(buffer, fsize, 1, file);
		fclose(file);

		HRESULT gr = dx11->GetDevice()->CreateVertexShader(buffer, fsize, NULL, &m_VertexShader);

		// ���̓��C�A�E�g����
		{
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{
					"POSITION",
					0, // �Z�}���e�b�N�ԍ��B�����POSITION + 0�B
					DXGI_FORMAT_R32G32B32_FLOAT,
					0,
					0, // �f�[�^���̃I�t�Z�b�g POSITION�͐擪�Ȃ̂�( 4 * 0 )�o�C�g
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

	// �s�N�Z���V�F�[�_����
	{
		FILE* file;
		long int fsize;

		file = fopen(PixelShader, "rb");
		assert(file);
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[ fsize ];
		fread(buffer, fsize, 1, file);
		fclose(file);

		dx11->GetDevice()->CreatePixelShader(buffer, fsize, NULL, &m_PixelShader);

		delete[] buffer;
	}



	// �萔�o�b�t�@����
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

void Shader3D::Init(const std::string& vsh, const std::string& psh)
{
	Init(vsh.c_str(), psh.c_str());
}

void Shader3D::Uninit()
{
	if( m_ConstantBuffer )	m_ConstantBuffer->Release();
	if( m_VertexLayout )		m_VertexLayout->Release();
	if( m_VertexShader )		m_VertexShader->Release();
	if( m_PixelShader )		m_PixelShader->Release();
}

void Shader3D::Set()
{
	DX11Executor* dx11 = OrderedSingleton<DX11Executor>::Instance();

	// �V�F�[�_�ݒ�
	dx11->GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	dx11->GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ���̓��C�A�E�g�ݒ�
	dx11->GetDeviceContext()->IASetInputLayout(m_VertexLayout);


	// �萔�o�b�t�@�X�V
	dx11->GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_Constant, 0, 0);

	// �萔�o�b�t�@�ݒ�
	dx11->GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	dx11->GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);
}
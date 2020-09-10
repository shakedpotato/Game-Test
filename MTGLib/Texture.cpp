#include "OrderedSingleton.h"
#include "DX11Executor.h"

#include "Texture.h"

using namespace std;

void Texture::Load(const char* FileName)
{

	unsigned char	header[18];
	unsigned char* image;
	unsigned int	width, height;
	unsigned char	depth;
	unsigned int	bpp;
	unsigned int	size;


	// ヘッダ読み込み
	FILE* file;
	file = fopen(FileName, "rb");
	assert(file);

	fread(header, sizeof(header), 1, file);


	// 画像サイズ取得
	width = header[13] * 256 + header[12];
	height = header[15] * 256 + header[14];
	depth = header[16];

	if (depth == 32)
		bpp = 4;
	else if (depth == 24)
		bpp = 3;
	else
		bpp = 0;

	if (bpp != 4)
	{
		assert(false);
	}

	size = width * height * bpp;

	// メモリ確保
	image = (unsigned char*)new unsigned char[size];

	// 画像読み込み
	fread(image, size, 1, file);

	// R<->B
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			unsigned char c;
			c = image[(y * width + x) * bpp + 0];
			image[(y * width + x) * bpp + 0] = image[(y * width + x) * bpp + 2];
			image[(y * width + x) * bpp + 2] = c;
		}
	}

	fclose(file);


	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = image;
	initData.SysMemPitch = width * 4;
	initData.SysMemSlicePitch = size;

	auto hr = OrderedSingleton<DX11Executor>::Instance()->GetDevice()->CreateTexture2D(&desc, &initData, &m_Texture);
	if (FAILED(hr))
	{
		assert(false);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	hr = OrderedSingleton<DX11Executor>::Instance()->GetDevice()->CreateShaderResourceView(m_Texture, &SRVDesc, &m_ShaderResourceView);
	if (FAILED(hr))
	{
		assert(false);
	}


	delete[] image;
}


void Texture::Unload()
{
	m_Texture->Release();
	m_ShaderResourceView->Release();
}

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{

}

void TextureManager::LoadTexture(const char* path)
{
	tstring s(path);

	if (m_Textures.count(s))
	{
		return;
	}

	m_Textures[s] = make_shared<Texture>();
	m_Textures[s]->Load(path);
}


shared_ptr<Texture> TextureManager::LoadAndGetTexture(const char* path)
{
	tstring s(path);

	if (m_Textures.count(s))
	{
		return m_Textures[s];
	}

	m_Textures[s] = make_shared<Texture>();
	m_Textures[s]->Load(path);

	return m_Textures[s];
}

shared_ptr<Texture> TextureManager::GetTexture(const char* path)
{
	tstring s(path);

	if (!m_Textures.count(s))
	{
		return LoadAndGetTexture(path);
	}

	return m_Textures[s];
}

bool TextureManager::isFileLoaded(const char* path)
{
	return m_Textures.count(tstring(path));
}
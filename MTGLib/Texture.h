#pragma once
#include <memory>
#include <unordered_map>
#include "CommonWinApp.h"
#include "GraphicRenderingExecuter.h"


class Texture
{

public:

	void Load(const char* FileName); // �e�N�X�`���̓Ǎ��@���̂Ƃ���TGA�̂݁@���t�H�[�}�b�g�ɑΉ����鎞�ɊO���Ɉϑ�����B
	void Unload();
private:
	ID3D11Texture2D* m_Texture;
	ID3D11ShaderResourceView* m_ShaderResourceView;
public:
	ID3D11Texture2D* GetTexture2D() { return m_Texture; }
	ID3D11ShaderResourceView* GetShaderResourceView() { return m_ShaderResourceView; }
};


class TextureManager final
{
private:
	friend class GameBasicSystemObject<TextureManager>;
	TextureManager();
public:
	~TextureManager();
private:
	std::unordered_map<tstring, std::shared_ptr<Texture>> m_Textures;
public:
	void						LoadTexture(const char* filePath);			// �e�N�X�`���̓Ǎ��̂�
	std::shared_ptr<Texture>	LoadAndGetTexture(const char* filePath);	// �e�N�X�`���̓Ǎ��Ǝ擾
	std::shared_ptr<Texture>	GetTexture(const char* filePath);			// �e�N�X�`���̎擾�̂�

	bool isFileLoaded(const char* filePath);
};
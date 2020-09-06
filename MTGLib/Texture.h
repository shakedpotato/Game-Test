#pragma once
#include <memory>
#include <unordered_map>
#include "CommonWinApp.h"
#include "GraphicRenderingExecuter.h"


class Texture
{

public:

	void Load(const char* FileName); // テクスチャの読込　今のところTGAのみ　他フォーマットに対応する時に外部に委託する。
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
	void						LoadTexture(const char* filePath);			// テクスチャの読込のみ
	std::shared_ptr<Texture>	LoadAndGetTexture(const char* filePath);	// テクスチャの読込と取得
	std::shared_ptr<Texture>	GetTexture(const char* filePath);			// テクスチャの取得のみ

	bool isFileLoaded(const char* filePath);
};
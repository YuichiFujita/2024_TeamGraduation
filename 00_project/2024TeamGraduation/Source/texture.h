//=============================================================================
// 
// テクスチャヘッダー [texture.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TEXTURE_H_
#define _TEXTURE_H_	// 二重インクルード防止

#include "constans.h"
#include "constans.h"

//==========================================================================
// クラス定義
//==========================================================================
//テクスチャクラス定義
class CTexture
{
private:

	//=============================
	// 構造体定義
	//=============================
	struct STexture
	{
		LPDIRECT3DTEXTURE9 pTexture;	// テクスチャのポインタ
		D3DXIMAGE_INFO imageInfo;		// テクスチャ素材情報
		D3DXVECTOR2 aspectratio;		// アスペクト比
		std::string filename;			// ファイル名
		int nFileNameLen;				// ファイル名の文字数
	};

public:

	CTexture();
	~CTexture();

	void Init();	// 初期化処理
	HRESULT LoadAll();	// 全てのテクスチャ読み込み
	void Unload();
	int Regist(const std::string& file);	// 読み込み
	LPDIRECT3DTEXTURE9 GetAdress(int nIdx);
	D3DXVECTOR2 GetImageSize(int nIdx);		// テクスチャ素材のサイズ取得

	int GetNumAll();		// 読み込んだ総数
	STexture GetTextureInfo(const std::string& file);	// テクスチャ情報取得
	STexture GetTextureInfo(int nIdxTex);	// テクスチャ情報取得

	static CTexture* Create();
	static CTexture* GetInstance() { return m_pTexture; }
private:

	//=============================
	// メンバ関数
	//=============================
	HRESULT LoadTex(const std::string& file);
	void SearchAllImages(const std::wstring& folderPath);

	//=============================
	// メンバ変数
	//=============================
	std::vector<STexture> m_TexInfo;			// テクスチャの情報
	std::vector<std::string> m_ImageNames;		// 読み込み用文字列
	std::vector<std::string> m_FolderFilePath;	// フォルダー格納のファイルパス
	static CTexture* m_pTexture;				// 自身のポインタ
};



#endif
//=============================================================================
// 
//  テクスチャ処理 [texture.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"

// 読み込むテクスチャのヘッダー
#include "map.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::wstring MAINFOLODER = L"data\\TEXTURE";
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CTexture* CTexture::m_pTexture = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CTexture::CTexture()
{
	m_TexInfo.clear();
	m_ImageNames.clear();	// 読み込み用文字列
	m_FolderFilePath.clear();	// フォルダー格納のファイルパス
}

//==========================================================================
// デストラクタ
//==========================================================================
CTexture::~CTexture()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTexture* CTexture::Create()
{
	if (m_pTexture == nullptr)
	{// まだ生成していなかったら

		// インスタンス生成
		m_pTexture = DEBUG_NEW CTexture;
		m_pTexture->Init();
	}

	return m_pTexture;
}

//==========================================================================
// 初期化処理
//==========================================================================
void CTexture::Init()
{
	STexture init = {};
	m_TexInfo.emplace_back();		// 最初にnullを追加
	m_ImageNames.emplace_back();	// 最初にnullを追加
}

//==========================================================================
// 全てのテクスチャ読み込み
//==========================================================================
HRESULT CTexture::LoadAll()
{
#ifndef _DEBUG

	// 全検索
	SearchAllImages(MAINFOLODER);

	// 読み込んだファイル名コピー
	std::vector<std::string> filenames = m_FolderFilePath;
	for (const auto& name : filenames)
	{
		LoadTex(name);
	}

#endif

	// マップ用の読み込み
	if (FAILED(MyMap::ReadTexture()))
	{// 失敗した場合
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 全ての画像検索
//==========================================================================
void CTexture::SearchAllImages(const std::wstring& folderPath) 
{
	std::stack<std::wstring> folderStack;
	folderStack.push(folderPath);

	while (!folderStack.empty()) 
	{
		std::wstring currentFolder = folderStack.top();
		folderStack.pop();

		WIN32_FIND_DATAW findFileData;
		HANDLE hFind = FindFirstFileW((currentFolder + L"\\*").c_str(), &findFileData);

		if (hFind == INVALID_HANDLE_VALUE) 
		{
			continue;
		}

		do {

			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
			{// ファイルパス格納
				std::string fileName = UtilFunc::Transformation::WideToMultiByte((currentFolder + L"\\" + findFileData.cFileName).c_str());
				if (fileName.find(".png") != std::string::npos || fileName.find(".jpg") != std::string::npos || fileName.find(".tga") != std::string::npos)
				{
					m_FolderFilePath.push_back(fileName);
				}
			}
			else if (lstrcmpW(findFileData.cFileName, L".") != 0 && lstrcmpW(findFileData.cFileName, L"..") != 0) 
			{// 階層確認
				std::wstring subFolderPath = currentFolder + L"\\" + findFileData.cFileName;
				folderStack.push(subFolderPath);
			}

		} while (FindNextFileW(hFind, &findFileData) != 0);	// 終端のフォルダまで確認

		FindClose(hFind);
	}
}

//==========================================================================
// 全てのテクスチャの破棄
//==========================================================================
void CTexture::Unload()
{

	for (const auto& texture : m_TexInfo)
	{
		if (texture.pTexture == nullptr)
		{
			continue;
		}
		texture.pTexture->Release();
	}

	m_TexInfo.clear();
	m_TexInfo.shrink_to_fit();
}

//============================================================
// テクスチャの登録処理 (生成)
//============================================================
int CTexture::Regist(const SInfo info)
{
	HRESULT hr;// 異常終了の確認用
	int nIdx = static_cast<int>(m_TexInfo.size());	// テクスチャ読込番号

	// 最後尾に空の要素を追加
	m_TexInfo.emplace_back();		// テクスチャの情報
	m_ImageNames.emplace_back();	// 読み込み用文字列

	// マップ情報のポインタを初期化
	m_TexInfo[nIdx].pTexture = nullptr;	// テクスチャへのポインタ

	// 空のテクスチャを生成
	hr = D3DXCreateTexture
	( // 引数
		GET_DEVICE,		// Direct3Dデバイス
		info.Width,		// テクスチャ横幅
		info.Height,	// テクスチャ縦幅
		info.MipLevels,	// ミップマップレベル
		info.Usage,		// 性質・確保オプション
		info.Format,	// ピクセルフォーマット
		info.Pool,		// 格納メモリ
		&m_TexInfo[nIdx].pTexture	// テクスチャへのポインタ
	);
	if (FAILED(hr))
	{ // テクスチャの生成に失敗した場合

		// 失敗を返す
		assert(false);
		return -1;
	}

	// テクスチャステータスを設定
	D3DXIMAGE_INFO *pStatus = &m_TexInfo[nIdx].imageInfo;	// ステータス情報
	pStatus->Width			 = info.Width;					// テクスチャ横幅
	pStatus->Height			 = info.Height;					// テクスチャ縦幅
	pStatus->Depth			 = 1;							// テクスチャ深度
	pStatus->MipLevels		 = info.MipLevels;				// ミップマップレベル
	pStatus->Format			 = info.Format;					// ピクセルフォーマット
	pStatus->ResourceType	 = D3DRTYPE_TEXTURE;			// リソース種類
	pStatus->ImageFileFormat = (D3DXIMAGE_FILEFORMAT)-1;	// ファイル形式 (作成のため無し)

	// アスペクト比を計算
	m_TexInfo[nIdx].aspectratio.x = (float)info.Width / (float)info.Height;
	m_TexInfo[nIdx].aspectratio.y = (float)info.Height / (float)info.Width;

	// ファイル名と長さを空にする
	m_TexInfo[nIdx].filename = "\0";
	m_TexInfo[nIdx].nFileNameLen = m_TexInfo[nIdx].filename.length();

	// 読み込み用文字列を空にする
	m_ImageNames.back() = "\0";

	// 生成したテクスチャの配列番号を返す
	return nIdx;
}

//==========================================================================
// テクスチャの登録処理 (読込)
//==========================================================================
int CTexture::Regist(const std::string& file)
{
	if (file == "")
	{
		return 0;
	}

	// 変換後のパス
	std::string transformFile(file);

	// \\変換
	transformFile = UtilFunc::Transformation::ReplaceBackslash(transformFile);
	transformFile = UtilFunc::Transformation::ReplaceForwardSlashes(transformFile);

	// 読み込み用文字列で検索
	auto itr = std::find(m_ImageNames.begin(), m_ImageNames.end(), transformFile);
	if (itr != m_ImageNames.end())
	{
		return  static_cast<int>(std::distance(m_ImageNames.begin(), itr));
	}

	// 総数保存
	int nNumAll = GetNumAll();

	// テクスチャ読み込み
	HRESULT hr = LoadTex(transformFile);
	if (FAILED(hr))
	{
		return 0;
	}

	return nNumAll;
}

//==========================================================================
// テクスチャの読み込み処理
//==========================================================================
HRESULT CTexture::LoadTex(const std::string& file)
{
	HRESULT hr;
	int nIdx = static_cast<int>(m_TexInfo.size());

	// 割り当て
	m_TexInfo.emplace_back();
	m_ImageNames.emplace_back();	// 読み込み用文字列

	// 画像情報の取得
	D3DXIMAGE_INFO size;	// テクスチャステータス
	if (FAILED(D3DXGetImageInfoFromFile(file.c_str(), &size)))
	{ // 画像情報の取得に失敗した場合

		std::string error = "画像情報の読み込み失敗！" + file;
		MyAssert::CustomAssert(false, error);
		return E_FAIL;
	}

	// テクスチャの読込
	hr = D3DXCreateTextureFromFileEx
	( // 引数
		GET_DEVICE,			// Direct3Dデバイス
		file.c_str(),		// テクスチャファイルパス
		size.Width,			// テクスチャ横幅
		size.Height,		// テクスチャ縦幅
		0,					// ミップマップレベル
		0,					// 性質・確保オプション
		D3DFMT_A8R8G8B8,	// ピクセルフォーマット
		D3DPOOL_MANAGED,	// 格納メモリ
		D3DX_DEFAULT,		// フィルタ
		D3DX_DEFAULT,		// ミップマップフィルタ
		0,					// カラーキー
		&m_TexInfo[nIdx].imageInfo,	// テクスチャステータスへのポインタ
		nullptr,					// テクスチャパレットへのポインタ
		&m_TexInfo[nIdx].pTexture	// テクスチャへのポインタ
	);
	if (FAILED(hr))
	{ // テクスチャの読込に失敗した場合

		// 要素削除
		m_TexInfo.erase(m_TexInfo.end() - 1);

		std::string error = "テクスチャ読み込み失敗！" + file;
		MyAssert::CustomAssert(false, error);
		return E_FAIL;
	}

	// ファイル名と長さ保存
	m_TexInfo[nIdx].filename = file;
	m_TexInfo[nIdx].nFileNameLen = m_TexInfo[nIdx].filename.length();

	m_ImageNames.back() = file;	// 読み込み用文字列

	return S_OK;
}

//==========================================================================
// テクスチャのアドレス取得
//==========================================================================
LPDIRECT3DTEXTURE9 CTexture::GetAdress(int nIdx)
{
	if (static_cast<int>(m_TexInfo.size()) <= nIdx)
	{
		return nullptr;
	}
	return m_TexInfo[nIdx].pTexture;
}

//==========================================================================
// テクスチャの総数取得
//==========================================================================
int CTexture::GetNumAll()
{
	return static_cast<int>(m_TexInfo.size());
}

//==========================================================================
// テクスチャ情報取得
//==========================================================================
CTexture::STexture CTexture::GetTextureInfo(const std::string& file)
{
	// 最大数取得
	int nNumAll = GetNumAll() + 1;

	if (file == "")
	{
		return m_TexInfo[nNumAll];
	}

	int nNowLen = file.length();	// 今回のファイル名長さ

	for (int nCntData = 0; nCntData < nNumAll; nCntData++)
	{
		if (m_TexInfo[nCntData].nFileNameLen != nNowLen)
		{// ファイル名の長さが違ったら
			continue;
		}

		// 既にテクスチャが読み込まれてないかの最終確認
		if (m_TexInfo[nCntData].filename == file)
		{// ファイル名が一致している

			return m_TexInfo[nCntData];
		}
	}

	int nIdx = nNumAll + 1;
	return m_TexInfo[nIdx];
}

//==========================================================================
// テクスチャ情報取得
//==========================================================================
CTexture::STexture CTexture::GetTextureInfo(int nIdxTex)
{
	return m_TexInfo[nIdxTex];
}

//==========================================================================
// テクスチャ素材のサイズ取得
//==========================================================================
D3DXVECTOR2 CTexture::GetImageSize(int nIdx)
{
	if (static_cast<int>(m_TexInfo.size()) <= nIdx)
	{
		return mylib_const::DEFAULT_VECTOR2;
	}
	return D3DXVECTOR2((float)m_TexInfo[nIdx].imageInfo.Width, (float)m_TexInfo[nIdx].imageInfo.Height);
}

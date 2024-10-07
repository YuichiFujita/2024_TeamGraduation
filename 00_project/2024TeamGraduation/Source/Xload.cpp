//=============================================================================
// 
//  Xファイル処理 [Xload.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "Xload.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::wstring MAINFOLODER = L"data\\MODEL";
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CXLoad* CXLoad::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CXLoad::CXLoad()
{
	m_XFileInfo.clear();		// Xファイルの情報
	m_ImageNames.clear();		// 読み込み用文字列
	m_FolderFilePath.clear();	// フォルダー格納用文字列
}

//==========================================================================
// デストラクタ
//==========================================================================
CXLoad::~CXLoad()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CXLoad* CXLoad::Create()
{
	if (m_pThisPtr == nullptr)
	{// まだ生成していなかったら

		// インスタンス生成
		m_pThisPtr = DEBUG_NEW CXLoad;
		m_pThisPtr->Init();
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CXLoad::Init()
{
	return S_OK;
}

//==========================================================================
// 全てのモデル読み込み
//==========================================================================
HRESULT CXLoad::LoadAll()
{
#ifndef _DEBUG

	// 全検索
	SearchAllXModel(MAINFOLODER);

	// 読み込んだファイル名コピー
	for (const auto& name : m_FolderFilePath)
	{
		if (FAILED(Load(name)))
		{
			return E_FAIL;
		}
	}

#endif
	return S_OK;
}

//==========================================================================
// 全てのモデル検索
//==========================================================================
void CXLoad::SearchAllXModel(const std::wstring& folderPath)
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
			{// パス
				std::string fileName = UtilFunc::Transformation::WideToMultiByte((currentFolder + L"\\" + findFileData.cFileName).c_str());
				if (fileName.find(".x") != std::string::npos)
				{
					m_FolderFilePath.push_back(fileName);
				}
			}
			else if (lstrcmpW(findFileData.cFileName, L".") != 0 && lstrcmpW(findFileData.cFileName, L"..") != 0)
			{// フォルダ
				std::wstring subFolderPath = currentFolder + L"\\" + findFileData.cFileName;
				folderStack.push(subFolderPath);
			}

		} while (FindNextFileW(hFind, &findFileData) != 0);

		FindClose(hFind);
	}
}

//==========================================================================
// 終了処理
//==========================================================================
void CXLoad::Uninit()
{
	for (auto& XInfo : m_XFileInfo)
	{
		if (XInfo.nIdxTexture != nullptr)
		{
			delete[] XInfo.nIdxTexture;
			XInfo.nIdxTexture = nullptr;
		}

		// メッシュの破棄
		if (XInfo.pMesh != nullptr)
		{
			XInfo.pMesh->Release();
			XInfo.pMesh = nullptr;
		}

		// マテリアルの破棄
		if (XInfo.pBuffMat != nullptr)
		{
			XInfo.pBuffMat->Release();
			XInfo.pBuffMat = nullptr;
		}

		// 頂点座標の破棄
		if (XInfo.pVtxPos != nullptr)
		{
			delete[] XInfo.pVtxPos;
			XInfo.pVtxPos = nullptr;
		}
	}

	m_XFileInfo.clear();
	m_ImageNames.clear();	// 読み込み用文字列

	if (m_pThisPtr != nullptr)
	{
		delete m_pThisPtr;
		m_pThisPtr = nullptr;
	}
}

//==========================================================================
// Xファイルの破棄
//==========================================================================
void CXLoad::Unload()
{

}

//==========================================================================
// Xファイルの読み込み
//==========================================================================
int CXLoad::XLoad(std::string file)
{
#if 0
	// 最大数取得
	int nIdx = 0;
	int nNumAll = GetNumAll();

	if (file == "")
	{
		return -1;
	}
	int nNowLen = file.length();


	for (int nCntData = 0; nCntData < nNumAll; nCntData++)
	{
		if (m_XFileInfo[nCntData].nFileNameLen != nNowLen)
		{// ファイル名の長さが違う
			continue;
		}

		// 既にテクスチャが読み込まれてないかの最終確認
		if (m_XFileInfo[nCntData].filename == file)
		{// ファイル名が一致している
			return nCntData;
		}
	}

	// 読み込み
	if (FAILED(Load(file)))
	{
		return E_FAIL;
	}

	// インデックス番号保存
	nIdx = nNumAll;
	return nIdx;

#else


	if (file == "")
	{
		return 0;
	}

	// \\変換
	file = UtilFunc::Transformation::ReplaceBackslash(file);
	file = UtilFunc::Transformation::ReplaceForwardSlashes(file);

	auto itr = std::find(m_ImageNames.begin(), m_ImageNames.end(), file);
	if (itr != m_ImageNames.end())
	{
		return itr - m_ImageNames.begin();
	}

	// 総数保存
	int nNumAll = GetNumAll();

	// 読み込み
	HRESULT hr = Load(file);
	if (FAILED(hr))
	{
		return 0;
	}

	return nNumAll;
#endif
}

//==========================================================================
// ロード処理
//==========================================================================
HRESULT CXLoad::Load(std::string file)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	int nIdx = GetNumAll();

	// 要素追加
	SXFile initinfo = {};
	m_XFileInfo.push_back(initinfo);

	//Xファイルの読み込み
	HRESULT hr = D3DXLoadMeshFromX(
		file.c_str(),
		D3DXMESH_SYSTEMMEM,
		pDevice,
		nullptr,
		&m_XFileInfo[nIdx].pBuffMat,
		nullptr,
		&m_XFileInfo[nIdx].dwNumMat,
		&m_XFileInfo[nIdx].pMesh);

	if (FAILED(hr))
	{
		// 要素削除
		m_XFileInfo.erase(m_XFileInfo.end() - 1);

		std::string error = "モデル読み込み失敗！" + file;
		MyAssert::CustomAssert(false, error);
		return E_FAIL;
	}

	// ファイル名と長さ保存
	m_XFileInfo[nIdx].filename = file;
	m_XFileInfo[nIdx].nFileNameLen = file.length();

	// 読み込み用にも保存
	m_ImageNames.emplace_back();
	m_ImageNames.back() = file;

	// テクスチャのインデックス番号
	if (m_XFileInfo[nIdx].nIdxTexture == nullptr)
	{
		m_XFileInfo[nIdx].nIdxTexture = DEBUG_NEW int[(int)m_XFileInfo[nIdx].dwNumMat];

		if (m_XFileInfo[nIdx].nIdxTexture == nullptr)
		{// 確保できていない場合
			return E_FAIL;
		}
	}

	// 頂点数取得
	m_XFileInfo[nIdx].nVtxNum = m_XFileInfo[nIdx].pMesh->GetNumVertices();

	// 頂点数分でメモリ確保
	m_XFileInfo[nIdx].pVtxPos = DEBUG_NEW MyLib::Vector3[m_XFileInfo[nIdx].nVtxNum];

	// 面の数取得
	m_XFileInfo[nIdx].nFaceNum = m_XFileInfo[nIdx].pMesh->GetNumFaces();

	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_XFileInfo[nIdx].pBuffMat->GetBufferPointer();

	// 頂点数分繰り返し
	for (int nCntMat = 0; nCntMat < (int)m_XFileInfo[nIdx].dwNumMat; nCntMat++)
	{
		m_XFileInfo[nIdx].nIdxTexture[nCntMat] = 0;

		if (pMat[nCntMat].pTextureFilename != nullptr)
		{// ファイルが存在する

			// テクスチャの読み込み
			m_XFileInfo[nIdx].nIdxTexture[nCntMat] = CTexture::GetInstance()->Regist(pMat[nCntMat].pTextureFilename);

			if (FAILED(hr))
			{// 失敗していたら
				return E_FAIL;
			}
		}
	}

	BYTE* pVtxBuff;

	// 頂点バッファをロック
	m_XFileInfo[nIdx].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// 頂点フォーマットのサイズを取得
	DWORD dwSizeFVF = D3DXGetFVFVertexSize(m_XFileInfo[nIdx].pMesh->GetFVF());

	// 頂点座標
	for (int nCntVtx = 0; nCntVtx < m_XFileInfo[nIdx].nVtxNum; nCntVtx++)
	{
		// 頂点座標代入
		m_XFileInfo[nIdx].pVtxPos[nCntVtx] = *(MyLib::Vector3*)pVtxBuff;

		// サイズ分ポインタ移動
		pVtxBuff += dwSizeFVF;
	}

	// 頂点バッファをアンロック
	m_XFileInfo[nIdx].pMesh->UnlockVertexBuffer();

	// 全頂点チェック
	UtilFunc::Calculation::CalModelVtx(0.0f, &m_XFileInfo[nIdx].vtxMax, &m_XFileInfo[nIdx].vtxMin, m_XFileInfo[nIdx].pMesh, m_XFileInfo[nIdx].pVtxBuff);

	// インデックスバッファをロック
	WORD* pIndexBuff;
	m_XFileInfo[nIdx].pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndexBuff);

	for (int nCntIdx = 0; nCntIdx < m_XFileInfo[nIdx].nFaceNum; nCntIdx++)
	{
		// 三角形を形成するインデックス, 面毎だから3つずつ見る
		int nIdx1 = (int)pIndexBuff[nCntIdx * 3];
		int nIdx2 = (int)pIndexBuff[nCntIdx * 3 + 1];
		int nIdx3 = (int)pIndexBuff[nCntIdx * 3 + 2];

		// 一時代入
		MyLib::Vector3 pos1 = m_XFileInfo[nIdx].pVtxPos[nIdx1];
		MyLib::Vector3 pos2 = m_XFileInfo[nIdx].pVtxPos[nIdx2];
		MyLib::Vector3 pos3 = m_XFileInfo[nIdx].pVtxPos[nIdx3];

		// 頂点間の最大距離
		float fVtxDistance = 0.0f;

		// 2点の距離
		fVtxDistance = sqrtf(
			((pos1.x - pos2.x) * (pos1.x - pos2.x)) +
			((pos1.z - pos2.z) * (pos1.z - pos2.z)));

		if (fVtxDistance > m_XFileInfo[nIdx].fMaxVtxDistance)
		{
			// 最大距離保存
			m_XFileInfo[nIdx].fMaxVtxDistance = fVtxDistance;
		}

		// 2点の距離
		fVtxDistance = sqrtf(
			((pos2.x - pos3.x) * (pos2.x - pos3.x)) +
			((pos2.z - pos3.z) * (pos2.z - pos3.z)));
		if (fVtxDistance > m_XFileInfo[nIdx].fMaxVtxDistance)
		{
			// 最大距離保存
			m_XFileInfo[nIdx].fMaxVtxDistance = fVtxDistance;
		}

		// 2点の距離
		fVtxDistance = sqrtf(
			((pos3.x - pos1.x) * (pos3.x - pos1.x)) +
			((pos3.z - pos1.z) * (pos3.z - pos1.z)));
		if (fVtxDistance > m_XFileInfo[nIdx].fMaxVtxDistance)
		{
			// 最大距離保存
			m_XFileInfo[nIdx].fMaxVtxDistance = fVtxDistance;
		}
	}

	// インデックスバッファをアンロック
	m_XFileInfo[nIdx].pMesh->UnlockIndexBuffer();

	return S_OK;
}

//==========================================================================
// Xファイルのデータ取得
//==========================================================================
CXLoad::SXFile *CXLoad::GetMyObject(int nIdx)
{
	return &m_XFileInfo[nIdx];
}

//==========================================================================
// Xファイルの総数取得
//==========================================================================
int CXLoad::GetNumAll()
{
	return static_cast<int>(m_XFileInfo.size());
}
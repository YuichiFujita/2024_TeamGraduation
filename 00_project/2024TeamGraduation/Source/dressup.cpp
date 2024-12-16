//=============================================================================
// 
// 着せ替え(髪)処理 [dressup.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "dressup.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"

// 派生先
#include "dressup_hair.h"		// 髪
#include "dressup_accessory.h"	// アクセ
#include "dressup_face.h"		// 顔

//==========================================================================
// コンストラクタ
//==========================================================================
CDressup::CDressup()
{
	m_nSwitchIdx = 0;		// 切り替えるインデックス
	m_nNowIdx = 0;			// 現在のインデックス
	m_nControllIdx = 0;		// 操作するインデックス
	m_pObjChara = nullptr;	// キャラクターのポインタ
	m_vecModelName.clear();	// モデル名
}

//==========================================================================
// デストラクタ
//==========================================================================
CDressup::~CDressup()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CDressup* CDressup::Create(EType type, CObjectChara* pObjChara, int nSwitchType)
{
	// メモリの確保
	CDressup* pObj = nullptr;

	switch (type)
	{
	case CDressup::TYPE_HAIR:
		pObj = DEBUG_NEW CDressup_Hair;
		break;

	case CDressup::TYPE_ACCESSORY:
		pObj = DEBUG_NEW CDressup_Accessory;
		break;

	case CDressup::TYPE_FACE:
		pObj = DEBUG_NEW CDressup_Face;
		break;

	default:
		return nullptr;
		break;
	}

	if (pObj != nullptr)
	{
		// 引数情報
		pObj->m_nSwitchIdx = nSwitchType;
		pObj->m_pObjChara = pObjChara;

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CDressup::Init()
{
	return S_OK;
}

//==========================================================================
// モデル全て読み込み
//==========================================================================
void CDressup::LoadAllModel(const std::wstring& folder)
{
	// 階層フォルダ追加
	std::stack<std::wstring> folderStack;
	folderStack.push(folder);

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

		do 
		{// 終端のフォルダまで確認

			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{// ファイルパス格納
				std::string fileName = UtilFunc::Transformation::WideToMultiByte((currentFolder + L"\\" + findFileData.cFileName).c_str());
				if (fileName.find(".x") != std::string::npos)
				{// .xのみ追加
					m_vecModelName.push_back(fileName);
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
// テクスチャ全読み込み
//==========================================================================
void CDressup::LoadAllTexture(const std::wstring& folder)
{
	// 階層フォルダ追加
	std::stack<std::wstring> folderStack;
	folderStack.push(folder);

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

		do
		{// 終端のフォルダまで確認

			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{// ファイルパス格納
				std::string fileName = UtilFunc::Transformation::WideToMultiByte((currentFolder + L"\\" + findFileData.cFileName).c_str());
				if (fileName.find(".png") != std::string::npos || fileName.find(".jpg") != std::string::npos || fileName.find(".tga") != std::string::npos)
				{
					m_vecModelName.push_back(fileName);
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
// 終了処理
//==========================================================================
void CDressup::Uninit()
{
}

//==========================================================================
// 更新処理
//==========================================================================
void CDressup::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 再割り当て
//==========================================================================
void CDressup::ReRegist(int idx)
{
	m_nNowIdx = idx;

	// 再割り当て
	ReRegist();
}

//==========================================================================
// ランダム設定
//==========================================================================
void CDressup::RandSet()
{
	m_nNowIdx = UtilFunc::Transformation::Random(0, static_cast<int>(m_vecModelName.size()) - 1);
	ReRegist();
}

//==========================================================================
// デバッグ処理
//==========================================================================
void CDressup::Debug()
{
	
}

//=============================================================================
// 
// 着せ替え(髪)処理 [dressup_hair_Mii.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "dressup_hair_Mii.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"
#include "dressup_hair_MiiLow.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::wstring FOLDERNAME = L"data\\MODEL\\audience\\hair";	// 読み込むフォルダ名
}

//==========================================================================
// コンストラクタ
//==========================================================================
CDressup_Hair_Mii::CDressup_Hair_Mii() : CDressup_Hair()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CDressup_Hair_Mii::~CDressup_Hair_Mii()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CDressup_Hair_Mii* CDressup_Hair_Mii::Create(CDressup::EType type)
{
	// インスタンス生成
	CDressup_Hair_Mii* pAudience = nullptr;

	switch (type)
	{
	case CDressup::EType::TYPE_HAIR_MII:
		pAudience = DEBUG_NEW CDressup_Hair_Mii;
		break;

	case CDressup::EType::TYPE_HAIR_MIILOW:
		pAudience = DEBUG_NEW CDressup_Hair_MiiLow;
		break;

	default:
		break;
	}

	return pAudience;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CDressup_Hair_Mii::Init()
{
	// 全て読み込み
	LoadAllModel(FOLDERNAME);

	// 親の初期化
	CDressup::Init();
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CDressup_Hair_Mii::Uninit()
{
	// 親の終了
	CDressup::Uninit();
	delete this;
}

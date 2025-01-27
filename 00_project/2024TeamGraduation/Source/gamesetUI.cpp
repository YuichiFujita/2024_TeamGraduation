//============================================================
//
//	ゲームセットUI処理 [gamesetUI.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "gamesetUI.h"
#include "manager.h"
#include "object2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// UIの優先順位
	namespace gameset
	{
		const char* TEXTURE = "data\\TEXTURE\\gameset000.png";	// 操作表記の背景
		const MyLib::Vector3 INIT_POS = MyLib::Vector3(-VEC3_SCREEN_SIZE.x, 360.0f, 0.0f);		// 初期位置
		const MyLib::Vector3 DEST_POS = MyLib::Vector3(VEC3_SCREEN_CENT.x, INIT_POS.y, 0.0f);	// 目標位置
		const float MOVE_TIME = 0.75f;	// 移動時間
		const float WIDTH = 580.0f;		// 横幅
	}
}

//************************************************************
//	子クラス [CGameSetUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGameSetUI::CGameSetUI() : CObject(PRIORITY, LAYER::LAYER_2D),
	m_pGameSet	(nullptr),	// ゲームセット情報
	m_fCurTime	(0.0f)		// 経過時間
{

}

//============================================================
//	デストラクタ
//============================================================
CGameSetUI::~CGameSetUI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGameSetUI::Init()
{
	// UIの生成
	if (FAILED(CreateUI()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類をオブジェクト2Dにする
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 世界停止中に動けるようにする
	SetEnablePosibleMove_WorldPause(true);

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGameSetUI::Uninit()
{
	// 既に破棄済みの場合抜ける
	if (IsDeath()) { return; }

	// ゲームセットの終了
	SAFE_UNINIT(m_pGameSet);

	// オブジェクトを破棄
	Release();
}

//============================================================
//	削除処理
//============================================================
void CGameSetUI::Kill()
{
	// 自身の終了
	CGameSetUI::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CGameSetUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 経過時間を加算
	m_fCurTime += fDeltaTime * fSlowRate;

	// 背景の位置を移動
	MyLib::Vector3 posGameSet = UtilFunc::Correction::EaseOutBack(gameset::INIT_POS, gameset::DEST_POS, 0.0f, gameset::MOVE_TIME, m_fCurTime, 1.0f);
	m_pGameSet->SetPosition(posGameSet);
}

//============================================================
//	描画処理
//============================================================
void CGameSetUI::Draw()
{

}

//============================================================
//	描画状況の設定処理
//============================================================
void CGameSetUI::SetEnableDisp(const bool bDisp)
{
	// 自身の描画状況を設定
	CObject::SetEnableDisp(bDisp);

	// ゲームセットの描画状況を設定
	m_pGameSet->SetEnableDisp(bDisp);
}

//============================================================
//	生成処理
//============================================================
CGameSetUI *CGameSetUI::Create()
{
	// インスタンスの生成
	CGameSetUI* pTrans = DEBUG_NEW CGameSetUI;
	if (pTrans != nullptr)
	{ // 生成に成功した場合

		// クラスの初期化
		if (FAILED(pTrans->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pTrans);
			return nullptr;
		}
	}

	// インスタンスを返す
	return pTrans;
}

//============================================================
//	演出終了の確認処理
//============================================================
bool CGameSetUI::IsEnd()
{
	// 演出終了フラグを返す
	return (m_fCurTime >= gameset::MOVE_TIME);
}

//============================================================
//	UIの生成処理
//============================================================
HRESULT CGameSetUI::CreateUI()
{
	CTexture* pTexture = CTexture::GetInstance();	// テクスチャ情報

	// 背景の生成
	m_pGameSet = CObject2D::Create(PRIORITY);
	if (m_pGameSet == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// テクスチャの割当
	int texID = pTexture->Regist(gameset::TEXTURE);
	m_pGameSet->BindTexture(texID);

	// 位置を設定
	m_pGameSet->SetPosition(gameset::INIT_POS);

	// 横幅を元にサイズ計算
	MyLib::Vector2 size = pTexture->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, gameset::WIDTH);
	m_pGameSet->SetSize(size);

	// 世界停止中に動けるようにする
	m_pGameSet->SetEnablePosibleMove_WorldPause(true);

	return S_OK;
}

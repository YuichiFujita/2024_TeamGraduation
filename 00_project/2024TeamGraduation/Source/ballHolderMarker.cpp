//==========================================================================
// 
//  ボール所持マーカー処理 [ballHolderMarker.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "ballHolderMarker.h"
#include "gameManager.h"
#include "player.h"
#include "object3D.h"

// TODO：ボールマーカー表示
#if 0
#define DISP
#endif

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_CIRCLE = "data\\TEXTURE\\ballmarker\\circle.png";	// 円のテクスチャ
	const std::string TEXTURE_ARROW = "data\\TEXTURE\\ballmarker\\arrow.png";	// 矢印のテクスチャ
}

//==========================================================================
// コンストラクタ
//==========================================================================
CBallHolderMarker::CBallHolderMarker(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_pPlayer(nullptr),	// ボールを所持してるプレイヤーのポインタ
	m_fArrowTime(0.0f)	// 矢印のタイマー
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CBallHolderMarker::~CBallHolderMarker()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CBallHolderMarker* CBallHolderMarker::Create(CPlayer* pPlayer)
{
	// メモリの確保
	CBallHolderMarker* pMarker = DEBUG_NEW CBallHolderMarker;

	if (pMarker != nullptr)
	{
		// プレイヤー割り当て
		pMarker->m_pPlayer = pPlayer;

		// クラスの初期化
		if (FAILED(pMarker->Init()))
		{ // 初期化に失敗した場合
			SAFE_UNINIT(pMarker);
			return nullptr;
		}
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CBallHolderMarker::Init()
{
	// オブジェクトの種類設定
	CObject::SetType(CObject::TYPE::TYPE_OBJECT3D);

	// 円生成
	if (FAILED(CreateCircle()))
	{
		return E_FAIL;
	}

	// 矢印生成
	if (FAILED(CreateArrow()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 円生成
//==========================================================================
HRESULT CBallHolderMarker::CreateCircle()
{
	// 生成処理
	m_pCircle = CObject3D::Create(GetPriority());
	if (m_pCircle == nullptr) return E_FAIL;

	// オブジェクトの種類設定
	m_pCircle->CObject::SetType(CObject::TYPE::TYPE_OBJECT3D);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_CIRCLE);
	m_pCircle->BindTexture(texID);

	// サイズ設定
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 135.0f);
	m_pCircle->SetSize(MyLib::Vector3(size.x, 0.0f, size.y));
	m_pCircle->SetSizeOrigin(m_pCircle->GetSize());

	return S_OK;
}

//==========================================================================
// 矢印生成
//==========================================================================
HRESULT CBallHolderMarker::CreateArrow()
{
	// 生成処理
	m_pArrow = CObject3D::Create(GetPriority());
	if (m_pArrow == nullptr) return E_FAIL;

	// オブジェクトの種類設定
	m_pArrow->CObject::SetType(CObject::TYPE::TYPE_OBJECT3D);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_ARROW);
	m_pArrow->BindTexture(texID);

	// サイズ設定
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 60.0f);
	m_pArrow->SetSize(MyLib::Vector3(size.x, 0.0f, size.y));
	m_pArrow->SetSizeOrigin(m_pArrow->GetSize());

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CBallHolderMarker::Uninit()
{
	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CBallHolderMarker::Kill()
{
	// 自身の終了
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CBallHolderMarker::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#ifdef DISP
	// プレイヤーいないと描画切る
	bool bDisp = (m_pPlayer == nullptr) ? false : true;
	SetEnableDisp(bDisp);

	// 描画しないときは更新無し
	if (!bDisp) return;

	// プレイヤーの位置に設定
	MyLib::Vector3 pos = m_pPlayer->GetPosition();
	pos.y = 0.01f;
	SetPosition(pos);

	// 円と矢印の位置設定
	m_pCircle->SetPosition(pos);
	m_pArrow->SetPosition(pos);
#else
	SetEnableDisp(false);
#endif
}

//==========================================================================
// 描画処理
//==========================================================================
void CBallHolderMarker::Draw()
{

}

//==========================================================================
// 描画状況の設定処理
//==========================================================================
void CBallHolderMarker::SetEnableDisp(bool bDisp)
{
	// 基底クラスの描画状況設定
	CObject::SetEnableDisp(bDisp);

	// 円と矢印の描画状況設定
	m_pCircle->SetEnableDisp(bDisp);
	m_pArrow->SetEnableDisp(bDisp);
}

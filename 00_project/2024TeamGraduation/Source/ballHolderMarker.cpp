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
#if 1
#define DISP
#endif

// 外周円だけの更新
#if 1
#define ONLY_OUT
#endif

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_CIRCLE = "data\\TEXTURE\\ballmarker\\circle.png";			// 円のテクスチャ
	const std::string TEXTURE_OUTCIRCLE = "data\\TEXTURE\\ballmarker\\outcircle.png";	// 外周円のテクスチャ
	const std::string TEXTURE_ARROW = "data\\TEXTURE\\ballmarker\\arrow.png";			// 矢印のテクスチャ
	const float SIZE_CIRCLE = 60.0f;	// 円のサイズ

#ifdef ONLY_OUT
	const float SIZE_OUTCIRCLE = SIZE_CIRCLE;	// 外周円のサイズ
#else
	const float SIZE_OUTCIRCLE = SIZE_CIRCLE * 1.2f;	// 外周円のサイズ
#endif // ONLY_OUT

	const float SIZE_ARROW = 60.0f;	// 矢印のサイズ
	float MULTIPLY_SIZE_CIRCLE = 1.2f;			// 円のサイズの倍率
}

namespace Timer
{
	float CIRCLE_LOOP = 0.5f;	// 円のループ時間
}

//==========================================================================
// コンストラクタ
//==========================================================================
CBallHolderMarker::CBallHolderMarker(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_pCircle		(nullptr),	// 円
	m_pOutCircle	(nullptr),	// 外周円
	m_pArrow		(nullptr),	// 矢印
	m_pPlayer		(nullptr),	// ボールを所持してるプレイヤーのポインタ
	m_fCircleTime	(0.0f),		// 円のタイマー
	m_fArrowTime	(0.0f)		// 矢印のタイマー
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

	// 外周円生成
	if (FAILED(CreateOutCircle()))
	{
		return E_FAIL;
	}

	// 円生成
	if (FAILED(CreateCircle()))
	{
		return E_FAIL;
	}

	// TODO : チームの色に設定する
	m_pCircle->SetColor(MyLib::color::Cyan());
	m_pOutCircle->SetColor(MyLib::color::Cyan());

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

	// 向き設定
	m_pCircle->SetRotation(MyLib::Vector3(D3DX_PI * 0.5f, 0.0f, 0.0f));

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_CIRCLE);
	m_pCircle->BindTexture(texID);

	// サイズ設定
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, SIZE_CIRCLE);
	m_pCircle->SetSize(size);
	m_pCircle->SetSizeOrigin(m_pCircle->GetSize());

	return S_OK;
}

//==========================================================================
// 外周円生成
//==========================================================================
HRESULT CBallHolderMarker::CreateOutCircle()
{
	// 生成処理
	m_pOutCircle = CObject3D::Create(GetPriority());
	if (m_pOutCircle == nullptr) return E_FAIL;

	// オブジェクトの種類設定
	m_pOutCircle->CObject::SetType(CObject::TYPE::TYPE_OBJECT3D);

	// 向き設定
	m_pOutCircle->SetRotation(MyLib::Vector3(D3DX_PI * 0.5f, 0.0f, 0.0f));

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_OUTCIRCLE);
	m_pOutCircle->BindTexture(texID);

	// サイズ設定
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, SIZE_OUTCIRCLE);
	m_pOutCircle->SetSize(size);
	m_pOutCircle->SetSizeOrigin(m_pOutCircle->GetSize());

	return S_OK;
}

//==========================================================================
// 矢印生成
//==========================================================================
HRESULT CBallHolderMarker::CreateArrow()
{
	// 生成処理
	m_pArrow = CObject3D::Create(GetPriority() + 1);
	if (m_pArrow == nullptr) return E_FAIL;

	// オブジェクトの種類設定
	m_pArrow->CObject::SetType(CObject::TYPE::TYPE_OBJECT3D);

	// アンカーポイント
	m_pArrow->SetAnchorType(CObject3D::EAnchorPoint::UNDER_CENTER);

	// アンカーの割合
	m_pArrow->SetAnchorRate(0.4f);

	// 向き設定
	m_pArrow->SetRotation(MyLib::Vector3(D3DX_PI * 0.5f, 0.0f, 0.0f));

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_ARROW);
	m_pArrow->BindTexture(texID);

	// サイズ設定
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, SIZE_ARROW);
	m_pArrow->SetSize(size);
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
	// 円
	SAFE_KILL(m_pCircle);

	// 外周円
	SAFE_KILL(m_pOutCircle);

	// 矢印
	SAFE_KILL(m_pArrow);

	// 自身の終了
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CBallHolderMarker::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#if _DEBUG

	if (ImGui::TreeNode("BallHolderMarker"))
	{
		// 周期
		ImGui::DragFloat("CIRCLE_LOOP", &Timer::CIRCLE_LOOP, 0.01f);

		// 拡縮の倍率
		ImGui::DragFloat("MULTIPLY_SIZE_CIRCLE", &MULTIPLY_SIZE_CIRCLE, 0.01f);

		// アンカーの割合
		float anchorRate = m_pArrow->GetAnchorRate();
		ImGui::DragFloat("ANCHOR_RATE", &anchorRate, 0.01f);
		m_pArrow->SetAnchorRate(anchorRate);

		ImGui::TreePop();
	}
#endif // _DEBUG



	// 拡縮の更新
	UpdateScaling(fDeltaTime, fDeltaTime, fSlowRate);

#ifdef DISP
	// プレイヤーいないと描画切る
	bool bDisp = (m_pPlayer == nullptr) ? false : true;

	// 関与しないキャラでも描画切る
	if (m_pPlayer != nullptr)
	{
		bDisp = (m_pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_NONE) ? false : bDisp;
	}

	// 描画状況設定処理
	SetEnableDisp(bDisp);

	// 描画しないときは更新無し
	if (!bDisp) return;


	// プレイヤーの位置に設定
	MyLib::Vector3 pos = m_pPlayer->GetPosition();
	pos.y = 2.0f;
	SetPosition(pos);

	// 円と矢印の位置設定
	m_pOutCircle->SetPosition(pos);
	m_pCircle->SetPosition(pos + MyLib::Vector3(0.0f, 0.1f, 0.0f));	// 円は少し上
	m_pArrow->SetPosition(pos + MyLib::Vector3(0.0f, 0.5f, 0.0f));	// 矢印は円のさらに上

	// 矢印の向き設定
	MyLib::Vector3 rot = m_pArrow->GetRotation();
	rot.z = -m_pPlayer->GetRotation().y + D3DX_PI;
	UtilFunc::Transformation::RotNormalize(rot.z);
	m_pArrow->SetRotation(rot);

#else
	SetEnableDisp(false);
#endif
}

//==========================================================================
// 拡縮の更新
//==========================================================================
void CBallHolderMarker::UpdateScaling(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 円のタイマー加算
	m_fCircleTime += fDeltaTime * fSlowRate;

	// 元のサイズ取得
	MyLib::Vector3 sizeCircle = m_pCircle->GetSizeOrigin(), sizeOutCircle = m_pOutCircle->GetSizeOrigin();

	float multiply = 1.0f;	// 倍率
	if (m_fCircleTime <= Timer::CIRCLE_LOOP * 0.5f)
	{// 拡大
		multiply = UtilFunc::Correction::EaseOutExpo(1.0f, MULTIPLY_SIZE_CIRCLE, 0.0f, Timer::CIRCLE_LOOP * 0.5f, m_fCircleTime);
	}
	else
	{// 縮小
		multiply = UtilFunc::Correction::EasingEaseIn(MULTIPLY_SIZE_CIRCLE, 1.0f, Timer::CIRCLE_LOOP * 0.5f, Timer::CIRCLE_LOOP, m_fCircleTime);
	}

	// サイズ設定
#ifndef ONLY_OUT
	m_pCircle->SetSize(sizeCircle * multiply);
#endif // !ONLY_OUT
	m_pOutCircle->SetSize(sizeOutCircle * multiply);

	if (m_fCircleTime >= Timer::CIRCLE_LOOP)
	{// 1周でリセット
		m_fCircleTime = 0.0f;
	}
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
	m_pOutCircle->SetEnableDisp(bDisp);
	m_pArrow->SetEnableDisp(bDisp);
}

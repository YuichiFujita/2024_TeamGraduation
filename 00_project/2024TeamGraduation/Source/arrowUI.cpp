//==========================================================================
// 
//  矢印UI処理 [arrowUI.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
//==========================================================================
// インクルードファイル
//==========================================================================
#include "arrowUI.h"
#include "manager.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\entry\\Arrow_Twin.png";	// テクスチャ
	const float DISTANCE_SELECTMOVE = 15.0f;	// 選択時移動量
	const float VALUE_FLOAT = 3.0f;				// ふわふわ量

	namespace StateTime
	{
		const float LOOP = 1.0f;		// ループ周期
		const float SELECTMOVE = 0.1f;	// 選択時移動
	}
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CArrowUI::STATE_FUNC CArrowUI::m_StateFunc[] =	// 状態関数
{
	&CArrowUI::StateNone,		// なし
	&CArrowUI::StateNormal,		// 通常
	&CArrowUI::StateSelectMove,	// 選択時移動
};

//==========================================================================
//	コンストラクタ
//==========================================================================
CArrowUI::CArrowUI(int nPriority) : CObject2D_Anim(nPriority),
	m_state			(EState::STATE_NONE),		// 状態
	m_fStateTime	(0.0f),						// 状態タイマー
	m_direction		(EDirection::DIRECTION_L),	// 方向
	m_fValueFloat	(VALUE_FLOAT)				// ふわふわ量
{

}

//==========================================================================
//	デストラクタ
//==========================================================================
CArrowUI::~CArrowUI()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CArrowUI* CArrowUI::Create(
	EDirection dir,				// 方向
	const MyLib::Vector3& pos,	// 位置
	const float width,			// 横幅
	const MyLib::Color& color,	// 色
	int nPriority				// 優先順位
)
{
	// メモリの確保
	CArrowUI* pObj = DEBUG_NEW CArrowUI(nPriority);

	if (pObj != nullptr)
	{
		// 引数情報
		pObj->m_direction = dir;		// 方向
		pObj->SetPosition(pos);			// 位置
		pObj->SetOriginPosition(pos);	// 原点位置
		pObj->SetColor(color);			// 色

		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合
			SAFE_UNINIT(pObj);
			return nullptr;
		}

		// サイズ設定
		pObj->SetSizeByWidth(width);	// サイズ
	}

	return pObj;
}

//==========================================================================
//	初期化処理
//==========================================================================
HRESULT CArrowUI::Init()
{
	// UV分割数設定
	SetDivisionUV((int)EDirection::DIRECTION_MAX, 1);

	//	初期化処理
	CObject2D_Anim::Init();

	// 種類をオブジェクト2Dにする
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist(TEXTURE);
	BindTexture(nTexID);

	// パターンを設定
	SetPatternAnim((int)m_direction);

	// 自動再生をOFFにする
	SetEnableAutoPlay(false);

	// 頂点情報設定
	SetVtx();

	// 状態設定
	SetState(EState::STATE_NORMAL);

	return S_OK;
}

//==========================================================================
//	終了処理
//==========================================================================
void CArrowUI::Uninit()
{
	// オブジェクトを破棄
	Release();
}

//==========================================================================
//	削除処理
//==========================================================================
void CArrowUI::Kill()
{
	// 自身の終了
	CArrowUI::Uninit();
}

//==========================================================================
//	更新処理
//==========================================================================
void CArrowUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// 親の更新
	CObject2D_Anim::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 状態更新
//==========================================================================
void CArrowUI::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイマー更新
	m_fStateTime += fDeltaTime * fDeltaRate * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 通常
//==========================================================================
void CArrowUI::StateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#if 0	// ふわふわさせたら木も買った
	// 割合
	float ratio = m_fStateTime / StateTime::LOOP;

	// 位置更新
	MyLib::Vector3 pos = GetOriginPosition();

	// ふわふわ量算出
	float moveValue = sinf(D3DX_PI * (m_fStateTime / StateTime::LOOP)) * m_fValueFloat;

	// 移動
	pos.x += (m_direction == EDirection::DIRECTION_L) ? -moveValue : moveValue;

	// 位置設定
	SetPosition(pos);
#endif
}

//==========================================================================
// 選択時移動
//==========================================================================
void CArrowUI::StateSelectMove(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 位置更新
	MyLib::Vector3 posDest = GetOriginPosition();
	posDest.x += (m_direction == EDirection::DIRECTION_L) ? -DISTANCE_SELECTMOVE : DISTANCE_SELECTMOVE;

	// イージング移動
	MyLib::Vector3 pos;
	if (m_fStateTime <= StateTime::SELECTMOVE * 0.5f)
	{// 遠のける
		pos = UtilFunc::Correction::EasingLinear(GetOriginPosition(), posDest, 0.0f, StateTime::SELECTMOVE * 0.5f, m_fStateTime);
	}
	else
	{// 戻す
		pos = UtilFunc::Correction::EasingLinear(posDest, GetOriginPosition(), StateTime::SELECTMOVE * 0.5f, StateTime::SELECTMOVE, m_fStateTime);
	}
	SetPosition(pos);

	if (m_fStateTime >= StateTime::SELECTMOVE)
	{// 完了
		SetState(EState::STATE_NORMAL);
	}
}

//========================================================================================
// 状態設定
//========================================================================================
void CArrowUI::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//========================================================================================
// 横幅からサイズ設定
//========================================================================================
void CArrowUI::SetSizeByWidth(const float width)
{
	// 画像サイズ取得
	CTexture* pTexture = CTexture::GetInstance();
	MyLib::Vector2 size = pTexture->GetImageSize(GetIdxTexture());

	// 縦幅を元にサイズ計算
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, width);
	SetSize(size);
}

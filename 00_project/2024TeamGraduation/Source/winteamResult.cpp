//==========================================================================
// 
//  勝利チーム処理 [winteamResult.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
//==========================================================================
// インクルードファイル
//==========================================================================
#include "winteamResult.h"
#include "manager.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\entry\\area.png";	// テクスチャ
	const std::string TEXTURE_AUDIENCE = "data\\TEXTURE\\result\\audience.png";	// テクスチャ
	const MyLib::Vector3 SPAWNPOS_AUDIENCE = MyLib::Vector3(640.0f, 900.0f, 0.0f);
	const float JUMPVALUE = 50.0f;
}

namespace StateTime
{
	const float SPAWN = 0.5f;	// 登場
	const float LOOP = 0.5f;	// ループ周期
	const float FADEOUT = 0.5f;	// 開始
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CWinTeamResult::STATE_FUNC CWinTeamResult::m_StateFunc[] =	// 状態関数
{
	&CWinTeamResult::StateNone,		// なし
	&CWinTeamResult::StateSpawn,	// 登場
	&CWinTeamResult::StateLoop,		// ループ
	&CWinTeamResult::StateFadeOut,	// フェードアウト
};

//==========================================================================
//	コンストラクタ
//==========================================================================
CWinTeamResult::CWinTeamResult(int nPriority) : CObject2D_Anim(nPriority)
{

}

//==========================================================================
//	デストラクタ
//==========================================================================
CWinTeamResult::~CWinTeamResult()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CWinTeamResult* CWinTeamResult::Create(CGameManager::ETeamSide winSIde)
{
	// メモリの確保
	CWinTeamResult* pObj = DEBUG_NEW CWinTeamResult;

	if (pObj != nullptr)
	{
		// 勝利チーム
		pObj->m_winTeam = winSIde;

		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
//	初期化処理
//==========================================================================
HRESULT CWinTeamResult::Init()
{
	// UV分割数設定
	SetDivisionUV(1, 2);

	//	初期化処理
	CObject2D_Anim::Init();

	// 種類をオブジェクト2Dにする
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist(TEXTURE);
	BindTexture(nTexID);

	// 大きさ設定
	SetSizeByWidth(400.0f);
	SetSizeOrigin(GetSize());

	// パターン設定
	SetPatternAnim(m_winTeam);

	// 自動再生をOFFにする
	SetEnableAutoPlay(false);

	// 頂点情報設定
	SetVtx();

	// 位置設定
	SetPosition(VEC3_SCREEN_CENT);

	// 状態設定
	SetState(EState::STATE_SPAWN);

	// 観客生成
	if (FAILED(CreateAudience()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 観客の生成
//==========================================================================
HRESULT CWinTeamResult::CreateAudience()
{
	// 観客の生成
	m_pAudience = CObject2D_Anim::Create(
		MyLib::Vector3(640.0f, 480.0f, 0.0f),	// 位置
		2,				// U
		2,				// V
		0.1f,			// インターバル
		false,
		GetPriority() - 1
	);

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist(TEXTURE_AUDIENCE);
	m_pAudience->BindTexture(nTexID);

	// サイズ設定
	// 画像サイズ取得
	MyLib::Vector2 size = pTexture->GetImageSize(GetIdxTexture());

	// 分割数で割る
	size.x *= (1.0f / (float)m_pAudience->GetDivisionU());
	size.y *= (1.0f / (float)m_pAudience->GetDivisionV());

	// 縦幅を元にサイズ計算
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 640.0f);
	m_pAudience->SetSize(size);

	// 不透明度設定
	m_pAudience->SetAlpha(0.8f);

	return S_OK;
}

//==========================================================================
//	終了処理
//==========================================================================
void CWinTeamResult::Uninit()
{
	// 終了処理
	CObject2D_Anim::Uninit();

	// 観客
	SAFE_UNINIT(m_pAudience);
}

//==========================================================================
//	削除処理
//==========================================================================
void CWinTeamResult::Kill()
{
	// 観客
	SAFE_KILL(m_pAudience);

	// 自身の終了
	CWinTeamResult::Uninit();
}

//==========================================================================
//	更新処理
//==========================================================================
void CWinTeamResult::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	if (IsDeath()) return;

	// 親の更新
	CObject2D_Anim::Update(fDeltaTime, fDeltaRate, fSlowRate);
}


//==========================================================================
// 状態更新
//==========================================================================
void CWinTeamResult::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイマー更新
	m_fStateTime += fDeltaTime * fDeltaRate * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 登場
//==========================================================================
void CWinTeamResult::StateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 割合計算
	float ratio = UtilFunc::Correction::EasingCubicOut(0.3f, 1.0f, 0.0f, StateTime::SPAWN, m_fStateTime);

	// サイズ
	SetSize(GetSizeOrigin() * ratio);

	// 不透明度
	SetAlpha(ratio);


	// 観客登場
	MyLib::Vector3 setpos = SPAWNPOS_AUDIENCE + (m_pAudience->GetOriginPosition() - SPAWNPOS_AUDIENCE) * ratio;
	m_pAudience->SetPosition(setpos);

	if (m_fStateTime >= StateTime::SPAWN)
	{// 遷移
		SetState(EState::STATE_LOOP);
	}
}

//==========================================================================
// ループ
//==========================================================================
void CWinTeamResult::StateLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 割合計算
	float cos = cosf(D3DX_PI * (m_fStateTime / StateTime::LOOP));
	float ratio = 0.8f + (1.0f - 0.8f) * cos;

	// 観客登場
	MyLib::Vector3 setpos = m_pAudience->GetOriginPosition();
	setpos.y += JUMPVALUE;

	const float loopTime = StateTime::LOOP * 0.5f;
	cos = cosf(D3DX_PI * (m_fStateTime / loopTime));
	setpos.y += JUMPVALUE * cos;
	m_pAudience->SetPosition(setpos);
	
	if (m_fStateTime >= StateTime::LOOP * 2.0f)
	{
		m_fStateTime = 0.0f;
	}

	// サイズ
	SetSize(GetSizeOrigin() * ratio);
}

//==========================================================================
// フェードアウト
//==========================================================================
void CWinTeamResult::StateFadeOut(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 割合計算
	float ratio = UtilFunc::Correction::EaseInExpo(1.0f, 0.3f, 0.0f, StateTime::FADEOUT, m_fStateTime);

	// サイズ
	SetSize(GetSizeOrigin() * ratio);

	// 不透明度
	SetAlpha(ratio);


	// 観客退場
	ratio = UtilFunc::Correction::EaseOutExpo(0.0f, 1.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);
	MyLib::Vector3 setpos = m_pAudience->GetOriginPosition() + (SPAWNPOS_AUDIENCE - m_pAudience->GetOriginPosition()) * ratio;
	m_pAudience->SetPosition(setpos);

	if (m_fStateTime >= StateTime::FADEOUT)
	{// 遷移
		Kill();
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CWinTeamResult::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;

	if (state == EState::STATE_FADEOUT)
	{
		m_pAudience->SetOriginPosition(m_pAudience->GetPosition());
	}
}

//==========================================================================
// 横幅からサイズ設定
//==========================================================================
void CWinTeamResult::SetSizeByWidth(const float width)
{
	// 画像サイズ取得
	CTexture* pTexture = CTexture::GetInstance();
	MyLib::Vector2 size = pTexture->GetImageSize(GetIdxTexture());

	// 分割数で割る
	size.x *= (1.0f / (float)GetDivisionU());
	size.y *= (1.0f / (float)GetDivisionV());

	// 縦幅を元にサイズ計算
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, width);
	SetSize(size);
}

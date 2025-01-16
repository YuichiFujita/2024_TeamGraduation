//=============================================================================
// 
//  担架処理 [stretcher.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "stretcher.h"
#include "manager.h"
#include "calculation.h"
#include "schoolnurse.h"
#include "player.h"
#include "shadow.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\komono\\stretcher.x";
	const float VELOCITY_MOVE = 10.0f;	// 移動速度
	const MyLib::Vector3 STARTPOS = MyLib::Vector3(-3000.0f, 0.0f, 0.0f);	// 開始地点
	const float DISTANCE_UP = 50.0f;	// 積む間隔
}

namespace StateTime
{
	const float GO = 1.0f;		// 向かう
	const float BACK = 2.0f;	// 戻る
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CStretcher::STATE_FUNC CStretcher::m_StateFunc[] =	// 状態関数
{
	&CStretcher::StateNone,		// なし
	&CStretcher::StateGo,		// 向かう
	&CStretcher::StateCollect,	// 回収
	&CStretcher::StateBack,		// 戻る
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CStretcher* CStretcher::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CStretcher::CStretcher(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_fStateTime = 0.0f;			// 状態カウンター
	m_state = EState::STATE_NONE;	// 状態

	// その他変数
	m_pCollectPlayer.clear();		// 回収するプレイヤー
	m_pCarryPlayer.clear();			// 運ぶプレイヤー
	m_pLeftNurse = nullptr;			// 左の先生
	m_pRightNurse = nullptr;		// 右の先生
	m_pShadow = nullptr;			// 影
	m_bEndSetting = false;			// 終了のセッティング

}

//==========================================================================
// デストラクタ
//==========================================================================
CStretcher::~CStretcher()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CStretcher *CStretcher::Create(CPlayer* pPlayer)
{
	if (m_pThisPtr != nullptr)
	{
		// プレイヤー追加
		m_pThisPtr->AddCollectPlayer(pPlayer);
		return m_pThisPtr;
	}

	// メモリの確保
	m_pThisPtr = DEBUG_NEW CStretcher;

	if (m_pThisPtr != nullptr)
	{
		// 初期化処理
		m_pThisPtr->Init();

		// プレイヤー追加
		m_pThisPtr->AddCollectPlayer(pPlayer);
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CStretcher::Init()
{
	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 状態
	SetState(EState::STATE_NONE);

	// 初期化処理
	HRESULT hr = CObjectX::Init(MODEL);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// 幅
	m_fWidth = GetVtxMax().z;

	// 影生成
	if (m_pShadow == nullptr)
	{
		m_pShadow = CShadow::Create(this, 50.0f);
	}

	// 左の先生
	if (m_pLeftNurse == nullptr)
	{
		m_pLeftNurse = CSchoolNurse::Create(this);
		m_pLeftNurse->SetPosition(STARTPOS);
	}

	// 右の先生
	if (m_pRightNurse == nullptr)
	{
		m_pRightNurse = CSchoolNurse::Create(this);
		m_pRightNurse->SetPosition(STARTPOS);
	}

	// 位置設定
	SetPosition(STARTPOS);
	m_startPos = STARTPOS;

	// 半回転
	SetRotation(MyLib::Vector3(0.0f, D3DX_PI * 0.5f, 0.0f));

	// 出動
	SetState(EState::STATE_GO);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CStretcher::Uninit()
{
	m_pThisPtr = nullptr;

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CStretcher::Kill()
{
	// 影を消す
	SAFE_UNINIT(m_pShadow);

	// 終了処理
	CStretcher::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CStretcher::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 回収終了の設定
	CollectEndSetting();

	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// 先生の位置補正
	PosAdjNurse();

	// プレイヤー運搬
	MyLib::Vector3 pos = GetPosition();
	for (const auto& player : m_pCarryPlayer)
	{
		player->SetPosition(pos);
		pos.y += DISTANCE_UP;
	}
}

//==========================================================================
// 先生の位置補正
//==========================================================================
void CStretcher::PosAdjNurse()
{
	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 setpos = MyLib::Vector3(pos.x, 0.0f, pos.z);

	// 位置割り出し
	setpos.x += sinf(rot.y) * m_fWidth;
	setpos.z += cosf(rot.y) * m_fWidth;
	if (m_pLeftNurse != nullptr)
	{
		m_pLeftNurse->SetPosition(setpos);

		// 向き設定
		m_pLeftNurse->SetRotation(MyLib::Vector3(0.0f, setpos.AngleXZ(pos), 0.0f));
	}

	// リセット
	setpos = MyLib::Vector3(pos.x, 0.0f, pos.z);

	// 位置割り出し
	setpos.x += sinf(rot.y) * -m_fWidth;
	setpos.z += cosf(rot.y) * -m_fWidth;
	if (m_pRightNurse != nullptr)
	{
		m_pRightNurse->SetPosition(setpos);

		// 向き設定
		m_pRightNurse->SetRotation(MyLib::Vector3(0.0f, setpos.AngleXZ(pos), 0.0f));
	}
}

//==========================================================================
// 回収終了の設定
//==========================================================================
void CStretcher::CollectEndSetting()
{
	// 設定しないときは抜ける
	if (!m_bEndSetting) return;

	// 開始位置更新
	m_startPos = GetPosition();

	// 要素削除
	m_pCarryPlayer.push_back((*m_pCollectPlayer.begin()));	// 運ぶプレイヤー
	m_pCollectPlayer.erase(m_pCollectPlayer.begin());

	// 追加された運搬プレイヤーに回収時処理
	m_pCarryPlayer.back()->DeadCollectSetting();

	if (m_pCollectPlayer.empty())
	{// 要素全員いなくなってたら

		// 戻る
		SetState(EState::STATE_BACK);

		// 先生も戻る
		m_pLeftNurse->SetState(CSchoolNurse::EState::STATE_BACK);
		m_pRightNurse->SetState(CSchoolNurse::EState::STATE_BACK);
	}
	else
	{
		// 再出動
		SetState(EState::STATE_GO);

		// 先生もGO
		m_pLeftNurse->SetState(CSchoolNurse::EState::STATE_GO);
		m_pRightNurse->SetState(CSchoolNurse::EState::STATE_GO);
	}

	// 終了のセッティング
	m_bEndSetting = false;

}

//==========================================================================
// 回収するプレイヤー取得
//==========================================================================
CPlayer* CStretcher::GetCollectPlayer()
{
	if (m_pCollectPlayer.empty())
	{// 要素全員いなくなってたら
		return nullptr;
	}

	return (*m_pCollectPlayer.begin());
}

//==========================================================================
// 状態更新
//==========================================================================
void CStretcher::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態タイマー加算
	m_fStateTime += fDeltaTime * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// なし
//==========================================================================
void CStretcher::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 向かう
//==========================================================================
void CStretcher::StateGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 先頭取得
	std::vector<CPlayer*>::iterator itr = m_pCollectPlayer.begin();
	CPlayer* pPlayer = (*itr);

	// 回収に向かう
	MyLib::Vector3 playerPos = pPlayer->GetPosition();

	// 担架から見たプレイヤーへの向き
	float fRotY = GetPosition().AngleXZ(playerPos);
	SetRotation(MyLib::Vector3(0.0f, fRotY, 0.0f));

	// プレイヤーに向けて移動
	MyLib::Vector3 pos = UtilFunc::Correction::EasingEaseOut(m_startPos, playerPos, 0.0f, StateTime::GO, m_fStateTime);
	SetPosition(pos);

	if (m_fStateTime >= StateTime::GO)
	{// 時間経過

		// 先生も回収にする
		m_pLeftNurse->SetState(CSchoolNurse::EState::STATE_COLLECT);
		m_pRightNurse->SetState(CSchoolNurse::EState::STATE_COLLECT);

		// 状態遷移
		SetState(EState::STATE_COLLECT);
	}
}

//==========================================================================
// 回収
//==========================================================================
void CStretcher::StateCollect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// 戻る
//==========================================================================
void CStretcher::StateBack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

	// 戻ってる途中でも、患者が追加されたら向かう
	if (!m_pCollectPlayer.empty())
	{
		// 再出動
		SetState(EState::STATE_GO);

		// 先生も再出動
		m_pLeftNurse->SetState(CSchoolNurse::EState::STATE_BACK);
		m_pRightNurse->SetState(CSchoolNurse::EState::STATE_BACK);

		// 開始位置更新
		m_startPos = GetPosition();
		return;
	}


	// 担架から見た終了地点
	float fRotY = GetPosition().AngleXZ(STARTPOS);
	SetRotation(MyLib::Vector3(0.0f, fRotY, 0.0f));

	// プレイヤーに向けて移動
	MyLib::Vector3 pos = UtilFunc::Correction::EasingEaseOut(m_startPos, STARTPOS, 0.0f, StateTime::BACK, m_fStateTime);
	SetPosition(pos);

	if (m_fStateTime >= StateTime::BACK)
	{// 時間経過

		// 先生も消す
		m_pLeftNurse->Kill();
		m_pRightNurse->Kill();

		// 消す
		Kill();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CStretcher::Draw()
{
	// 描画
	CObjectX::Draw();
}

//==========================================================================
// 状態設定
//==========================================================================
void CStretcher::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 回収時設定
//==========================================================================
void CStretcher::CollectSetting()
{
	// 先頭取得
	std::vector<CPlayer*>::iterator itr = m_pCollectPlayer.begin();
	CPlayer* pPlayer = (*itr);
	if (pPlayer == nullptr) return;

	// 回収モーション設定
	pPlayer->GetMotion()->Set(CPlayer::EMotion::MOTION_DEAD_COLLECT);

	// プレイヤー運搬
	MyLib::Vector3 pos = GetPosition();
	for (const auto& player : m_pCarryPlayer)
	{
		pos.y += DISTANCE_UP;
	}
	pPlayer->SetPosition(pos);
}

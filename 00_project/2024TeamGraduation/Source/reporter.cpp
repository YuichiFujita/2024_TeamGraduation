//=============================================================================
// 
//  実況者処理 [reporter.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "reporter.h"
#include "manager.h"
#include "calculation.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "stretcher.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string CHARAFILE = "data\\TEXT\\character\\nurse\\setup_player.txt";	// キャラクターファイル
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CReporter::STATE_FUNC CReporter::m_StateFunc[] =	// 状態関数
{
	&CReporter::StateNone,		// なし
	&CReporter::StateGo,			// 向かう
	&CReporter::StateCollect,	// 回収
	&CReporter::StateBack,		// 戻る
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CReporter> CReporter::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CReporter::CReporter(int nPriority) : CObjectChara(nPriority)
{
	// 値のクリア
	// 状態
	m_state = EState::STATE_NONE;		// 状態
	m_fStateTime = 0.0f;				// 状態時間

	// その他変数
	m_TeamSide = CGameManager::ETeamSide::SIDE_NONE;	// チームサイド
	m_pShadow = nullptr;								// 影
}

//==========================================================================
// デストラクタ
//==========================================================================
CReporter::~CReporter()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CReporter* CReporter::Create(const MyLib::Vector3& pos, const CGameManager::ETeamSide& side)
{
	// メモリの確保
	CReporter* pObj = DEBUG_NEW CReporter;
	if (pObj != nullptr)
	{
		// 引数情報
		pObj->SetPosition(pos);		// 位置
		pObj->m_TeamSide = side;	// チームサイド

		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CReporter::Init()
{
	// 種類の設定
	SetType(CObject::TYPE::TYPE_PLAYER);

	// 状態
	SetState(EState::STATE_NONE);

	// キャラ作成
	HRESULT hr = SetCharacter(CHARAFILE);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// リストに割当
	m_List.Regist(this);

	// 影生成
	if (m_pShadow == nullptr)
	{
		m_pShadow = CShadow::Create(this, 50.0f);
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CReporter::Uninit()
{
	// 影
	m_pShadow = nullptr;

	// 終了処理
	CObjectChara::Uninit();

	// プレイヤーリストから削除
	m_List.Delete(this);
}

//==========================================================================
// 動的削除処理
//==========================================================================
void CReporter::Kill()
{
	// 影を消す
	SAFE_UNINIT(m_pShadow);

	// 終了処理
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CReporter::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsDeath()) return;

	// 過去の位置保存
	SetOldPosition(GetPosition());

	// 親の更新処理
	CObjectChara::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 攻撃時処理
//==========================================================================
void CReporter::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	int nType = pMotion->GetType();

	// 武器の位置
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), ATKInfo);
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	switch (nType)
	{
	case EMotion::MOTION_DEF:
		break;

	default:
		break;
	}
}

//==========================================================================
// 攻撃判定中処理
//==========================================================================
void CReporter::AttackInDicision(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;
	int nType = pMotion->GetType();

	switch (nType)
	{
	case EMotion::MOTION_DEF:
		break;

	default:
		break;
	}

	// 武器の位置
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), ATKInfo);

	if (ATKInfo.fRangeSize == 0.0f)
	{
		return;
	}

#if _DEBUG
	CEffect3D::Create(
		weponpos,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		ATKInfo.fRangeSize, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

	if (ATKInfo.bEndAtk)
	{// 終了してたら抜ける
		return;
	}
}

//==========================================================================
// 状態更新
//==========================================================================
void CReporter::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態タイマー加算
	m_fStateTime += fDeltaTime * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// なし
//==========================================================================
void CReporter::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
}

//==========================================================================
// 向かう
//==========================================================================
void CReporter::StateGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
}

//==========================================================================
// 回収
//==========================================================================
void CReporter::StateCollect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 戻る
//==========================================================================
void CReporter::StateBack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 描画処理
//==========================================================================
void CReporter::Draw()
{
	// 描画
	CObjectChara::Draw();
}

//==========================================================================
// 状態設定
//==========================================================================
void CReporter::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

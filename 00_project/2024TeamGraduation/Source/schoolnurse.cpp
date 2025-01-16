//=============================================================================
// 
//  保険の先生処理 [schoolnurse.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "schoolnurse.h"
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
CSchoolNurse::STATE_FUNC CSchoolNurse::m_StateFunc[] =	// 状態関数
{
	&CSchoolNurse::StateNone,		// なし
	&CSchoolNurse::StateGo,			// 向かう
	&CSchoolNurse::StateCollect,	// 回収
	&CSchoolNurse::StateBack,		// 戻る
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CSchoolNurse> CSchoolNurse::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CSchoolNurse::CSchoolNurse(CStretcher* pStretcher, int nPriority) : CObjectChara(nPriority),
	m_pStretcher(pStretcher)	// 担架
{
	// 値のクリア
	// 状態
	m_state = EState::STATE_NONE;		// 状態
	m_Oldstate = EState::STATE_NONE;	// 前回の状態
	m_fStateTime = 0.0f;				// 状態時間

	// その他変数
	m_pCollectPlayer.clear();		// 回収するプレイヤー
	m_pShadow = nullptr;			// 影
}

//==========================================================================
// デストラクタ
//==========================================================================
CSchoolNurse::~CSchoolNurse()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CSchoolNurse* CSchoolNurse::Create(CStretcher* pStretcher)
{
	// メモリの確保
	CSchoolNurse* pObj = DEBUG_NEW CSchoolNurse(pStretcher);
	if (pObj != nullptr)
	{
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
HRESULT CSchoolNurse::Init()
{
	// 種類の設定
	SetType(CObject::TYPE::TYPE_PLAYER);

	// 状態
	SetState(EState::STATE_GO);

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
void CSchoolNurse::Uninit()
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
void CSchoolNurse::Kill()
{
	// 影を消す
	SAFE_UNINIT(m_pShadow);

	// 終了処理
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CSchoolNurse::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CSchoolNurse::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
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

	case EMotion::MOTION_WALK:
		// 歩きのエフェクト
		CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_RUN,
			weponpos,
			MyLib::Vector3(),	// 向き
			MyLib::Vector3(),
			15.0f, true);
		break;

	default:
		break;
	}
}

//==========================================================================
// 攻撃判定中処理
//==========================================================================
void CSchoolNurse::AttackInDicision(CMotion::AttackInfo ATKInfo, int nCntATK)
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
void CSchoolNurse::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態タイマー加算
	m_fStateTime += fDeltaTime * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// なし
//==========================================================================
void CSchoolNurse::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
}

//==========================================================================
// 向かう
//==========================================================================
void CSchoolNurse::StateGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 歩き
	GetMotion()->Set(EMotion::MOTION_WALK);
}

//==========================================================================
// 回収
//==========================================================================
void CSchoolNurse::StateCollect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モーション取得
	CMotion* pMotion = GetMotion();

	if (pMotion->GetType() != EMotion::MOTION_COLLECT)
	{// 回収してない場合設定
		pMotion->Set(EMotion::MOTION_COLLECT);
	}

	if (pMotion->IsFinish())
	{// モーションが終わってたら

		// 担架に終了信号
		m_pStretcher->SetEnableEnd(true);
		SetState(EState::STATE_BACK);
	}

}

//==========================================================================
// 戻る
//==========================================================================
void CSchoolNurse::StateBack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 歩き
	GetMotion()->Set(EMotion::MOTION_WALK);
}

//==========================================================================
// 描画処理
//==========================================================================
void CSchoolNurse::Draw()
{
	// 描画
	CObjectChara::Draw();
}

//==========================================================================
// 状態設定
//==========================================================================
void CSchoolNurse::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

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
#include "reporterSet.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string CHARAFILE = "data\\TEXT\\character\\reporter\\setup_player.txt";	// キャラクターファイル
	const MyLib::Vector3 DEFAULTPOS[CGameManager::ETeamSide::SIDE_MAX] =
	{
		MyLib::Vector3(-200.0f, -17.0f, 909.0f),	// 左
		MyLib::Vector3(200.0f, -17.0f, 909.0f),	// 右
	};

	namespace StateTime
	{
		const float WAIT = 1.0f;	// 待機
	}
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CReporter::STATE_FUNC CReporter::m_StateFunc[] =	// 状態関数
{
	&CReporter::StateNone,		// なし
	&CReporter::StateThout,		// 実況
	&CReporter::StateWait,		// 待機
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CReporter> CReporter::m_List[CGameManager::ETeamSide::SIDE_MAX] = {};	// リスト

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
CReporter* CReporter::Create(const CGameManager::ETeamSide& side)
{
	// メモリの確保
	CReporter* pObj = DEBUG_NEW CReporter;
	if (pObj != nullptr)
	{
		// 引数情報
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

	// 初期位置
	SetPosition(DEFAULTPOS[m_TeamSide]);

	// キャラ作成
	HRESULT hr = SetCharacter(CHARAFILE);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// リストに割当
	m_List[m_TeamSide].Regist(this);

	// 影生成
	if (m_pShadow == nullptr)
	{
		m_pShadow = CShadow::Create(this, 50.0f);
	}

	// 実況セット
	MyLib::Vector3 setpos = GetPosition();
	setpos.y = 0.0f;
	m_pReporterSet = CReporterSet::Create(setpos, m_TeamSide);

	// デフォモーション設定
	SetDefMotion();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CReporter::Uninit()
{
	// リストから削除
	m_List[m_TeamSide].Delete(this);

	// 終了処理
	CObjectChara::Uninit();
}

//==========================================================================
// 動的削除処理
//==========================================================================
void CReporter::Kill()
{
	// 影を消す
	SAFE_UNINIT(m_pShadow);

	// 実況セット
	SAFE_UNINIT(m_pReporterSet);

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

#if _DEBUG
	// デバッグ処理
	Debug();
#endif
}

//==========================================================================
// 攻撃時処理
//==========================================================================
void CReporter::AttackAction(CMotionManager::AttackInfo ATKInfo, int nCntATK)
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
void CReporter::AttackInDicision(CMotionManager::AttackInfo ATKInfo, int nCntATK)
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
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	// ループした瞬間に待機させる
	if (pMotion->IsLoopMoment())
	{
		SetState(EState::STATE_WAIT);
		pMotion->Set(EMotion::MOTION_WAIT);

		// 待機時間設定
		m_fWaitTime = StateTime::WAIT + UtilFunc::Transformation::Random(-4, 4) * 0.1f;
	}
}

//==========================================================================
// 実況
//==========================================================================
void CReporter::StateThout(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	if (pMotion->GetType() != EMotion::MOTION_THOUT)
	{// 実況設定
		pMotion->Set(EMotion::MOTION_THOUT);
	}

	// 終了時にニュートラルへ
	if (pMotion->IsFinish())
	{
		// 状態設定
		SetState(EState::STATE_NONE);

		// デフォモーション設定
		SetDefMotion();
	}
}

//==========================================================================
// 待機
//==========================================================================
void CReporter::StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fWaitTime <= m_fStateTime)
	{// 待機時間終了

		// 状態設定
		SetState(EState::STATE_NONE);

		// デフォモーション設定
		SetDefMotion();
	}
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

//==========================================================================
// デフォモーション設定
//==========================================================================
void CReporter::SetDefMotion()
{
	// モーション設定
	CMotion* pMotion = GetMotion();
	if (pMotion != nullptr)
	{
		int setIdx = UtilFunc::Transformation::Random(EMotion::MOTION_DEF, EMotion::MOTION_DEF_INV);
		pMotion->Set(setIdx);
	}
}

//==========================================================================
// デバッグ処理
//==========================================================================
void CReporter::Debug()
{
#if _DEBUG

	//-----------------------------
	// 位置
	//-----------------------------
	if (ImGui::TreeNode("SetPosition"))
	{
		// 位置取得
		MyLib::Vector3 pos = GetPosition();

		if (ImGui::Button("Reset"))
		{// リセット
			pos = MyLib::Vector3();
		}
		ImGui::DragFloat3("pos", (float*)&pos, 1.0f, 0.0f, 0.0f, "%.2f");

		// 位置設定
		SetPosition(pos);
		ImGui::TreePop();
	}

	//-----------------------------
	// パラメーター
	//-----------------------------
	if (ImGui::TreeNode("Parameter"))
	{
		// 拡大率の調整
		float scale = GetScale();
		ImGui::DragFloat("Scale", &scale, 0.001f, 0.01f, 100.0f, "%.3f");
		SetScale(scale);

		ImGui::TreePop();
	}
#endif
}
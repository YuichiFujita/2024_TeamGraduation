//=============================================================================
// 
//  実況者セット処理 [reporterSet.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "reporterSet.h"
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
	const std::string MODEL_DESK = "data\\MODEL\\komono\\Livedesk.x";	// 実況机
	const std::string MODEL_CHAIR = "data\\MODEL\\komono\\Liveseat.x";	// 実況席
	const std::string MODEL_MIC = "data\\MODEL\\komono\\stamdmic.x";	// 実況マイク
	const float VELOCITY_MOVE = 10.0f;	// 移動速度
	const MyLib::Vector3 STARTPOS = MyLib::Vector3(-3000.0f, 0.0f, 0.0f);	// 開始地点
	const float DISTANCE_UP = 50.0f;	// 積む間隔
}

namespace OffSet
{
	const MyLib::Vector3 DESK = MyLib::Vector3(0.0f, 0.0f, -44.0f);	// 机から見た椅子のオフセット
	const MyLib::Vector3 CHAIR = MyLib::Vector3(0.0f, 0.0f, 60.0f);	// 机から見た椅子のオフセット
	const MyLib::Vector3 MIC = MyLib::Vector3(0.0f, 73.0f, 0.0f);	// 机から見たマイクのオフセット
}

namespace StateTime
{
	const float GO = 1.0f;		// 向かう
	const float BACK = 2.0f;	// 戻る
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CReporterSet::STATE_FUNC CReporterSet::m_StateFunc[] =	// 状態関数
{
	&CReporterSet::StateNone,		// なし
	&CReporterSet::StateGo,		// 向かう
	&CReporterSet::StateCollect,	// 回収
	&CReporterSet::StateBack,		// 戻る
};

//==========================================================================
// 静的メンバ変数
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CReporterSet::CReporterSet(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_fStateTime = 0.0f;			// 状態カウンター
	m_state = EState::STATE_NONE;	// 状態

	// その他変数
	m_pShadow = nullptr;			// 影

}

//==========================================================================
// デストラクタ
//==========================================================================
CReporterSet::~CReporterSet()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CReporterSet *CReporterSet::Create(const MyLib::Vector3& pos, const CGameManager::ETeamSide& side)
{
	
	// メモリの確保
	CReporterSet* pObj = DEBUG_NEW CReporterSet;

	if (pObj != nullptr)
	{
		// 引数情報
		pObj->SetPosition(pos + OffSet::DESK);		// 位置設定
		pObj->m_TeamSide = side;	// チームサイド

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CReporterSet::Init()
{
	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 初期化処理
	HRESULT hr = CObjectX::Init(MODEL_DESK);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// 椅子生成
	CreateChair();

	// マイク生成
	CreateMic();

	// 影生成
	if (m_pShadow == nullptr)
	{
		m_pShadow = CShadow::Create(this, 50.0f);
	}

	// 出動
	SetState(EState::STATE_GO);

	return S_OK;
}

//==========================================================================
// 椅子生成
//==========================================================================
void CReporterSet::CreateChair()
{
	// 生成処理
	m_pChair = CObjectX::Create(MODEL_CHAIR, GetPosition() + OffSet::CHAIR, MyLib::Vector3());

	// 種類の設定
	m_pChair->CObject::SetType(TYPE_OBJECTX);
}

//==========================================================================
// マイク生成
//==========================================================================
void CReporterSet::CreateMic()
{
	// 生成処理
	m_pMic = CObjectX::Create(MODEL_MIC, GetPosition() + OffSet::MIC, MyLib::Vector3());

	// 種類の設定
	m_pMic->CObject::SetType(TYPE_OBJECTX);
}

//==========================================================================
// 終了処理
//==========================================================================
void CReporterSet::Uninit()
{
	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CReporterSet::Kill()
{
	// 影を消す
	SAFE_UNINIT(m_pShadow);

	// 実況セット
	SAFE_UNINIT(m_pChair);
	SAFE_UNINIT(m_pMic);

	// 終了処理
	CReporterSet::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CReporterSet::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 状態更新
//==========================================================================
void CReporterSet::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態タイマー加算
	m_fStateTime += fDeltaTime * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// なし
//==========================================================================
void CReporterSet::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 向かう
//==========================================================================
void CReporterSet::StateGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
	if (m_fStateTime >= StateTime::GO)
	{// 時間経過

		// 状態遷移
		SetState(EState::STATE_COLLECT);
	}
}

//==========================================================================
// 回収
//==========================================================================
void CReporterSet::StateCollect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// 戻る
//==========================================================================
void CReporterSet::StateBack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
	if (m_fStateTime >= StateTime::BACK)
	{// 時間経過


		// 消す
		Kill();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CReporterSet::Draw()
{
	// 描画
	CObjectX::Draw();
}

//==========================================================================
// 状態設定
//==========================================================================
void CReporterSet::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

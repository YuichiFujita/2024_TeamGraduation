//==========================================================================
// 
//  リザルト王冠処理 [resultCrown.cpp]
//  Author : Kai Takada
// 
//==========================================================================
#include "resultCrown.h"
#include "object2D.h"
#include "objectX.h"
#include "manager.h"
#include "camera.h"
#include "instantfade.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string MODEL[] =	// モデル
	{
		"data\\MODEL\\result\\crown.x",
		"data\\MODEL\\result\\draw.x",
	};
}

namespace StateTime
{
	const float SPAWN = 1.2f;	// 登場
	const float LOOP_ROTATION = 6.0f;	// ループ周期
	const float LOOP = 1.0f;	// ループ周期
	const float START = 1.0f;	// 開始
}

namespace Rotate
{
	const float INTERVAL = 5.0f;	// 間隔
	const float TIME = 2.0f;	// 回転する時間
}

namespace Logo
{
	const MyLib::Vector3 POS_ORIGIN = MyLib::Vector3(0.0f, 600.0f, 600.0f);				// 初期位置
	const MyLib::Vector3 POS_DEFAULT = MyLib::Vector3(0.0f, 250.0f, 0.0f);				// 通常位置
	const MyLib::Vector3 VALUE_ROTATION = MyLib::Vector3(0.0f, D3DX_PI * 0.05f, 0.0f);	// 向き
	const float VALUE_FLOAT = 10.0f;													// 浮上値
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CResultCrown::STATE_FUNC CResultCrown::m_StateFunc[] =	// 状態関数
{
	&CResultCrown::StateNone,		// なし
	&CResultCrown::StateSpawn,	// 登場
	&CResultCrown::StateLoop,		// ループ
	&CResultCrown::StateStart,	// 開始
	&CResultCrown::StateWait,		// 待機
};

//==========================================================================
// コンストラクタ
//==========================================================================
CResultCrown::CResultCrown(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_state				(EState::STATE_NONE),	// 状態
	m_fStateTime		(0.0f),					// 状態タイマー
	m_fRotationTime		(0.0f),					// 回転タイマー
	m_fIntervalRotate	(0.0f),					// 回転までの間隔
	m_fRotationY		(0.0f),					// Y軸回転量
	m_fTime				(0.0f),					// 副のタイマー
	m_result			(EResult::RESULT_WIN)	// 結果
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CResultCrown::~CResultCrown()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CResultCrown* CResultCrown::Create(EResult result)
{
	// メモリの確保
	CResultCrown* pLogo = DEBUG_NEW CResultCrown;

	if (pLogo != nullptr)
	{
		// クラスの初期化
		if (FAILED(pLogo->Init(result)))
		{ // 初期化に失敗した場合
			SAFE_UNINIT(pLogo);
			return nullptr;
		}
	}

	return pLogo;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CResultCrown::Init()
{
	// オブジェクトの種類設定
	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 主生成
	if (FAILED(CreateMain()))
	{
		return E_FAIL;
	}

	// 状態設定
	SetState(EState::STATE_SPAWN);

	// カメラ取得
	CCamera* pCamera = GET_MANAGER->GetCamera();

	// カメラの向き同期
	MyLib::Vector3 cameraRot = pCamera->GetRotation();
	cameraRot.y = 0.0f;
	pCamera->SetRotation(cameraRot);

	return S_OK;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CResultCrown::Init(EResult result)
{
	SetResult(result);

	return Init();
}

//==========================================================================
// 主生成
//==========================================================================
HRESULT CResultCrown::CreateMain()
{
	// 生成処理
	m_pMain = CObjectX::Create(MODEL[m_result], Logo::POS_ORIGIN);
	if (m_pMain == nullptr) return E_FAIL;

	// オブジェクトの種類設定
	m_pMain->CObject::SetType(CObject::TYPE::TYPE_OBJECTX);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CResultCrown::Uninit()
{
	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CResultCrown::Kill()
{
	// ロゴ部分
	SAFE_KILL(m_pMain);

	// 自身の終了
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CResultCrown::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (GET_MANAGER->GetInstantFade()->GetState() == CInstantFade::EState::STATE_FADEIN)
	{// フェードINは抜ける
		return;
	}

	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// ポリゴン更新
	UpdateMain();
}

//==========================================================================
// 状態更新
//==========================================================================
void CResultCrown::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイマー更新
	m_fStateTime += fDeltaTime * fDeltaRate * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 登場
//==========================================================================
void CResultCrown::StateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 位置更新
	MyLib::Vector3 pos = UtilFunc::Correction::EaseOutBack(GetOriginPosition(), Logo::POS_DEFAULT, 0.0f, StateTime::SPAWN, m_fStateTime, 3.0f);
	m_pMain->SetPosition(pos);

	if (m_fStateTime >= StateTime::SPAWN)
	{// 遷移
		SetState(EState::STATE_LOOP);
	}
}

//==========================================================================
// ループ
//==========================================================================
void CResultCrown::StateLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 回転タイマー加算
	m_fRotationTime += fDeltaTime * fSlowRate;
	if (m_fIntervalRotate <= m_fRotationTime)
	{
		// 2回転
		m_fRotationY = UtilFunc::Correction::EasingCubicInOut(0.0f, D3DX_PI * 2.0f, m_fIntervalRotate, m_fIntervalRotate + Rotate::TIME, m_fRotationTime);

		if (m_fRotationTime >= m_fIntervalRotate + Rotate::TIME)
		{// 終了

			// 間隔計算
			m_fIntervalRotate = Rotate::INTERVAL;
			m_fIntervalRotate += UtilFunc::Transformation::Random(0, 50) * 0.1f;

			// タイマーリセット
			m_fRotationTime = 0.0f;
		}
	}

	// 割合
	float ratio = m_fStateTime / StateTime::LOOP;

	// 位置更新
	MyLib::Vector3 pos = m_pMain->GetPosition();
	pos.y = Logo::POS_DEFAULT.y;

	// 浮上
	pos.y += sinf(D3DX_PI * (m_fStateTime / StateTime::LOOP)) * Logo::VALUE_FLOAT;
	m_pMain->SetPosition(pos);

	// 回転
	MyLib::Vector3 rot = sinf(D3DX_PI * m_fStateTime / StateTime::LOOP_ROTATION) * Logo::VALUE_ROTATION;
	MyLib::Vector3 setrot = rot + MyLib::Vector3(0.0f, m_fRotationY, 0.0f);
	m_pMain->SetRotation(setrot);

	//// カメラ取得
	//CCamera* pCamera = GET_MANAGER->GetCamera();

	//// カメラの向き同期
	//MyLib::Vector3 cameraRot = pCamera->GetRotation();
	//cameraRot.y = rot.y;
	//pCamera->SetRotation(cameraRot);
}

//==========================================================================
// 開始
//==========================================================================
void CResultCrown::StateStart(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 待機
//==========================================================================
void CResultCrown::StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 主更新
//==========================================================================
void CResultCrown::UpdateMain()
{
	
}

//==========================================================================
// 描画処理
//==========================================================================
void CResultCrown::Draw()
{

}

//==========================================================================
// 描画状況の設定処理
//==========================================================================
void CResultCrown::SetEnableDisp(bool bDisp)
{
	// 基底クラスの描画状況設定
	CObject::SetEnableDisp(bDisp);

	// 主と副の描画状況設定
	m_pMain->SetEnableDisp(bDisp);
}

//==========================================================================
// 状態設定
//==========================================================================
void CResultCrown::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 位置設定
//==========================================================================
void CResultCrown::SetPosition(const MyLib::Vector3& pos)
{
	if (m_pMain != nullptr)
	{
		m_pMain->SetPosition(pos);
	}
}

//==========================================================================
// Xモデル
//==========================================================================
void CResultCrown::BindXData(std::string filepass)
{
}

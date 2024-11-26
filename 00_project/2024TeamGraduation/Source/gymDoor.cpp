//==========================================================================
// 
//  体育館ドア処理 [gymDoor.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "gymDoor.h"
#include "manager.h"
#include "objectX.h"
#include "debugproc.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string MODEL_DOOR[] =	// ドアモデル
	{
		"data\\MODEL\\gym\\3D_Gym_DoorL.x",
		"data\\MODEL\\gym\\3D_Gym_DoorR.x",
	};

	const float OFFSET_CLOSE = 60.0f;	// 閉じている際のドアオフセット
	const float OFFSET_OPEN = 175.0f;	// 開いている際のドアオフセット
}

//==========================================================================
// コンストラクタ
//==========================================================================
CGymDoor::CGymDoor(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_state			(STATE_CLOSE),	// 状態
	m_fCurMoveTime	(0.0f),			// 現在のドア開閉タイマー
	m_fEndMoveTime	(0.0f),			// 終了のドア開閉タイマー
	m_fCurOffset	(0.0f),			// 現在のドア配置オフセット
	m_fInitOffset	(0.0f),			// 開始のドア配置オフセット
	m_fDestOffset	(0.0f),			// 目標のドア配置オフセット
	m_bOpen			(false)			// 開閉フラグ
{
	// メンバ変数をクリア
	memset(&m_apDoor[0], 0, sizeof(m_apDoor));	// ドア情報
}

//==========================================================================
// デストラクタ
//==========================================================================
CGymDoor::~CGymDoor()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CGymDoor* CGymDoor::Create(const MyLib::Vector3& rPos)
{
	// メモリの確保
	CGymDoor* pGymDoor = DEBUG_NEW CGymDoor;
	if (pGymDoor != nullptr)
	{
		// クラスの初期化
		if (FAILED(pGymDoor->Init()))
		{ // 初期化に失敗した場合

			SAFE_UNINIT(pGymDoor);
			return nullptr;
		}

		// 位置の設定
		pGymDoor->SetPosition(rPos);
	}

	return pGymDoor;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CGymDoor::Init()
{
	// オブジェクトの種類設定
	CObject::SetType(CObject::TYPE::TYPE_OBJECT3D);

	for (int i = 0; i < EDoor::DOOR_MAX; i++)
	{ // ドアの枚数分繰り返す

		// ドアの生成
		m_apDoor[i] = CObjectX::Create(MODEL_DOOR[i]);
		if (m_apDoor[i] == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}
	}

	// ドアを閉じる
	SetEnableOpen(false);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CGymDoor::Uninit()
{
	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CGymDoor::Kill()
{
	// 自身の終了
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CGymDoor::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 開閉中じゃない場合は抜ける
	if (m_state != EState::STATE_MOVE) { return; }

	// 経過時間を加算
	m_fCurMoveTime += fDeltaTime * fSlowRate;
	if (m_fCurMoveTime >= m_fEndMoveTime)
	{ // 終了時間に到達した場合

		// 開閉状況を補正
		SetEnableOpen(m_bOpen);
		return;
	}

	// ドアをオフセットを計算
	m_fCurOffset = UtilFunc::Correction::EasingCubicIn(m_fInitOffset, m_fDestOffset, 0.0f, m_fEndMoveTime, m_fCurMoveTime);

	// 相対位置の設定
	SetPositionRelative();
}

//==========================================================================
// 描画処理
//==========================================================================
void CGymDoor::Draw()
{

}

//==========================================================================
// 描画状況の設定処理
//==========================================================================
void CGymDoor::SetEnableDisp(bool bDisp)
{
	// 基底クラスの描画状況設定
	CObject::SetEnableDisp(bDisp);

	for (int i = 0; i < EDoor::DOOR_MAX; i++)
	{ // ドアの枚数分繰り返す

		// ドアの描画状況設定
		m_apDoor[i]->SetEnableDisp(bDisp);
	}
}

//==========================================================================
// 位置の設定処理
//==========================================================================
void CGymDoor::SetPosition(const MyLib::Vector3& pos)
{
	// 基底クラスの位置設定
	CObject::SetPosition(pos);

	// 相対位置の設定
	SetPositionRelative();
}

//==========================================================================
// ドア開放フラグの設定処理
//==========================================================================
void CGymDoor::SetEnableOpen(const bool bOpen, const float fMoveTime)
{
	// 開閉フラグを保存
	m_bOpen = bOpen;

	// 目標のドア配置オフセットを設定
	m_fDestOffset = (bOpen) ? OFFSET_OPEN : OFFSET_CLOSE;

	if (fMoveTime <= 0.0f)
	{ // 開閉時間が未設定の場合

		// 開閉ごとの状態にする
		m_state = (bOpen) ? STATE_OPEN : STATE_CLOSE;

		// 現在のドア配置オフセットを目標オフセットに補正
		m_fCurOffset = m_fDestOffset;
	}
	else
	{ // 開閉時間が設定されている場合

		// 開閉中状態にする
		m_state = STATE_MOVE;
	}

	// 情報の設定
	m_fInitOffset = m_fCurOffset;	// 開始のドア配置オフセット
	m_fEndMoveTime = fMoveTime;		// 終了のドア開閉タイマー

	// 情報の初期化
	m_fCurMoveTime = 0.0f;	// 現在のドア開閉タイマー

	// 相対位置の設定
	SetPositionRelative();
}

//==========================================================================
// 相対位置の設定処理
//==========================================================================
void CGymDoor::SetPositionRelative(void)
{
	MyLib::Vector3 posThis = GetPosition();	// 自身の位置
	for (int i = 0; i < EDoor::DOOR_MAX; i++)
	{ // ドアの枚数分繰り返す

		// オフセットの計算
		float fDir = (i == 0) ? -1.0f : 1.0f;	// 方向の計算係数
		MyLib::Vector3 offset = MyLib::Vector3(m_fCurOffset * fDir, 0.0f, 0.0f);	// オフセット

		// ドアの位置設定
		m_apDoor[i]->SetPosition(posThis + offset);
	}
}

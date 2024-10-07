//=============================================================================
// 
//  タイマー処理 [timer.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "timer.h"
#include "manager.h"
#include "multinumber.h"
#include "debugproc.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"
#include "camera.h"
#include "baggage.h"
#include "baggageManager.h"

// 派生先
#include "timer_result.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\number\\number_oradano03.png";	// テクスチャのファイル
	const std::string BGTEXTURE = "data\\TEXTURE\\timer\\bg.png";	// テクスチャのファイル
	const float DISTANCE = 1300.0f;	// 距離
	const MyLib::Vector3 DEFAULT_POSITION = MyLib::Vector3(1070.0f, 70.0f, 0.0f);	// 初期位置
	const D3DXVECTOR2 SIZE_NUMBER = D3DXVECTOR2(35.0f, 35.0f);
	const float DSTANCE_MULTIPLAY = 2.5f;
	const float ROTY = D3DX_PI * 0.0f;	// Y軸角度
	MyLib::Vector3 CAMERA_POS = MyLib::Vector3(-845.0f, -390.0f, 0.0f);
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CTimer *CTimer::m_pTimer = nullptr;	// 自身のポインタ

//==========================================================================
// 関数リスト
//==========================================================================
CTimer::STATE_FUNC CTimer::m_StateFuncList[] =
{
	&CTimer::StateWait,		// 待機
	&CTimer::StateGoal,		// ゴール
};

//==========================================================================
// コンストラクタ
//==========================================================================
CTimer::CTimer(int nPriority)
{
	// 値のクリア
	m_nPriority = nPriority;
	m_state = STATE_WAIT;		// 状態
	m_fStateTime = 0.0f;		// 状態時間
	m_fTime = 0.0f;				// 時間
	m_pos = mylib_const::DEFAULT_VECTOR3;	// 位置
	m_posOrigin = mylib_const::DEFAULT_VECTOR3;	// 元の位置
	m_bAddTime = false;			// タイマー加算のフラグ
	m_pMyCamera = nullptr;
	m_pBg = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CTimer::~CTimer()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTimer* CTimer::Create(Type type, int nPriority)
{
	if (m_pTimer != nullptr) return m_pTimer;

	// メモリの確保

	switch (type)
	{
	case TYPE_NORMAL:
		m_pTimer = DEBUG_NEW CTimer;
		break;

	case Type::TYPE_RESULT:
		m_pTimer = DEBUG_NEW CTimer_Result(nPriority);
		break;
	}

	if (m_pTimer != nullptr)
	{
		// 初期化処理
		m_pTimer->Init();
	}

	return m_pTimer;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTimer::Init()
{
	// 各種変数初期化
	m_pos = DEFAULT_POSITION;
	m_posOrigin = m_pos;	// 元の位置
	m_fTime = 0.0f;	// 時間
	m_state = STATE_WAIT;			// 状態
	m_bAddTime = false;			// タイマー加算のフラグ

	// 背景を生成するお
	{
		MyLib::Vector3 bgpos = DEFAULT_POSITION;
		bgpos.x -= 10.0f;
		m_pBg = CObject2D::Create(6);
		m_pBg->SetPosition(bgpos);
		m_pBg->SetType(CObject::TYPE::TYPE_UI);

		// テクスチャ設定
		m_pBg->BindTexture(CTexture::GetInstance()->Regist(BGTEXTURE));
		D3DXVECTOR2 texture = CTexture::GetInstance()->GetImageSize(m_pBg->GetIdxTexture());
		m_pBg->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, SIZE_NUMBER.y * 2.0f));
		m_pBg->SetAlpha(0.7f);
	}

	// 分、秒、ミリ秒の計算
	int time[3];
	time[0] = static_cast<int>(m_fTime) / (60);
	time[1] = static_cast<int>(m_fTime) % (60);
	time[2] = static_cast<int>(m_fTime) % 1000;

	for (int i = 0; i < 3; i++)
	{
		// 生成
		m_pClearTime[i] = CMultiNumber::Create(
			m_pos,
			SIZE_NUMBER,
			2,
			CNumber::EObjectType::OBJECTTYPE_2D,
			TEXTURE, false, m_nPriority);
		if (m_pClearTime[i] == nullptr){
			continue;
		}
		CMultiNumber* pNumber = m_pClearTime[i];

		// 位置設定
		MyLib::Vector3 pos = m_pos;
		pos.x -= (SIZE_NUMBER.x * DSTANCE_MULTIPLAY) * i;
		pNumber->SetPosition(pos);

		// 右寄せに設定
		pNumber->SetAlignmentType(CMultiNumber::AlignmentType::ALIGNMENT_RIGHT);

		// 値の設定
		pNumber->SetValue(time[i]);
		pNumber->SetType(CObject::TYPE::TYPE_UI);

		for (int i = 0; i < pNumber->GetDigit(); i++)
		{
			CNumber* pThisNum = pNumber->GetNumber(i);
			pThisNum->SetType(CObject::TYPE::TYPE_UI);
		}

	}

	// 描画用カメラの生成と設定
	m_pMyCamera = DEBUG_NEW CCamera;
	m_pMyCamera->Init();
	m_pMyCamera->SetRotation(MyLib::Vector3(0.0f, ROTY, 0.0f));
	m_pMyCamera->SetDistance(DISTANCE);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTimer::Uninit()
{
	if (m_pMyCamera != nullptr)
	{
		m_pMyCamera->Uninit();
		delete m_pMyCamera;
		m_pMyCamera = nullptr;
	}

	delete m_pTimer;
	m_pTimer = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CTimer::Update()
{
	// 状態別処理
	(this->*(m_StateFuncList[m_state]))();

	// カメラ更新
	SetCamera();

	if (!m_bAddTime)
	{
		return;
	}

	// タイマー加算
	m_fTime += CManager::GetInstance()->GetDeltaTime();

	// タイマーを分、秒、ミリ秒に変換
	ApplyTimer();
}

//==========================================================================
// 待機状態
//==========================================================================
void CTimer::StateWait()
{
	// 時間リセット
	m_fStateTime = 0.0f;
}

//==========================================================================
// ゴール状態
//==========================================================================
void CTimer::StateGoal()
{
	m_bAddTime = false;
}

//==========================================================================
// タイマー反映
//==========================================================================
void CTimer::ApplyTimer()
{
	// タイマーを分、秒、ミリ秒に変換
	int time[3];
	time[2] = static_cast<int>(m_fTime / 60);
	time[1] = static_cast<int>(m_fTime) % 60;
	time[0] = static_cast<int>((m_fTime - static_cast<int>(m_fTime)) * 1000);
	time[0] /= 10;

	for (int i = 0; i < 3; i++)
	{
		if (m_pClearTime[i] == nullptr) {
			continue;
		}

		// 値の設定
		m_pClearTime[i]->SetValue(time[i]);
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CTimer::Draw()
{

}

//==========================================================================
// タイマー設定と反映
//==========================================================================
void CTimer::SetTime(const float time)
{
	m_fTime = time;
	ApplyTimer();
}

//==========================================================================
// カメラ
//==========================================================================
void CTimer::SetCamera()
{
	// カメラがない
	if (m_pMyCamera == nullptr) { return; }

	CBaggage* pBag = CBaggage::GetListObj().GetData(0);

	// 荷物がない
	if (pBag == nullptr) { return; }

	// 座標を更新
	MyLib::Vector3 pos = CAMERA_POS;
	m_pMyCamera->SetPositionR(pos);
	m_pMyCamera->SetPositionV();
}
//=============================================================================
// 
//  カウントダウン処理 [countdown_start.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "countdown_start.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"
#include "player.h"
#include "baggage.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE[] =	// テクスチャのファイル
	{
		"data\\TEXTURE\\countdown\\countdown_00.png",
		"data\\TEXTURE\\countdown\\countdown_01.png",
		"data\\TEXTURE\\countdown\\countdown_02.png",
		"data\\TEXTURE\\countdown\\countdown_03.png",
	};
	const int MAX_COUNT = 3;	// カウントの最大値
}

// 状態別タイマー
namespace StateTime
{
	const float DROP_UP = 0.35f;	// 落下_上
	const float DROP_WAIT = 0.3f;	// 落下_待機
	const float DROP_DOWN = 0.35f;	// 落下_下
	const float COMPLETE = 1.2f;	// 完了
	const float FADEOUT = 0.5f;		// フェードアウト
}

// 状態別位置
namespace StatePos
{
	const MyLib::Vector3 ORIGIN_DROP_UP = MyLib::Vector3(640.0f, -120.0f, 0.0f);	// 落下(上)の初期値
	const MyLib::Vector3 DEST_DROP_UP = MyLib::Vector3(640.0f, 360.0f, 0.0f);		// 落下(上)の目標値
	const MyLib::Vector3 DEST_DROP_DOWN = MyLib::Vector3(640.0f, 880.0f, 0.0f);		// 落下(下)の目標値
	const MyLib::Vector3 DEST_COMPLETE = MyLib::Vector3(640.0f, 360.0f, 0.0f);		// 完了の目標値
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CCountdown_Start::STATE_FUNC CCountdown_Start::m_StateFuncList[] =
{
	&CCountdown_Start::StateNone,		// なし
	&CCountdown_Start::StateDrop_UP,	// 落下(上)
	&CCountdown_Start::StateDrop_Wait,	// 落下(待機)
	&CCountdown_Start::StateDrop_DOWN,	// 落下(下)
	&CCountdown_Start::StateComplete,	// 完了
	&CCountdown_Start::StateFadeout,	// フェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CCountdown_Start::CCountdown_Start(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_state = State::STATE_NONE;	// 状態
	m_fStateTime = 0.0f;	// 状態カウンター
	m_nCount = 0;			// カウント
}

//==========================================================================
// デストラクタ
//==========================================================================
CCountdown_Start::~CCountdown_Start()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCountdown_Start* CCountdown_Start::Create()
{
	// メモリの確保
	CCountdown_Start* pObj = DEBUG_NEW CCountdown_Start;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCountdown_Start::Init()
{

	// オブジェクト2Dの初期化
	CObject2D::Init();

	// 各種変数初期化
	m_nCount = MAX_COUNT;	// カウント
	m_state = State::STATE_DROP_UP;	// 状態
	m_fStateTime = 0.0f;	// 状態カウンター

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE[m_nCount]);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
#endif
	SetSize(size);
	SetSizeOrigin(size);


	// 位置、向き設定は必要があれば追加

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// サウンド再生
	CSound::GetInstance()->PlaySound(CSound::LABEL_SE_COUNTDOWN1);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCountdown_Start::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CCountdown_Start::Update()
{
	// 状態更新
	UpdateState();

	if (IsDeath())
	{
		return;
	}

	// 更新処理
	CObject2D::Update();
}

//==========================================================================
// 状態更新
//==========================================================================
void CCountdown_Start::UpdateState()
{
	// 状態タイマー加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 実行
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// なし
//==========================================================================
void CCountdown_Start::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// 落下(上)
//==========================================================================
void CCountdown_Start::StateDrop_UP()
{
	MyLib::Vector3 pos = GetPosition();

	// 補正、徐々に加速
	pos = UtilFunc::Correction::EaseOutExpo(StatePos::ORIGIN_DROP_UP, StatePos::DEST_DROP_UP, 0.0f, StateTime::DROP_UP, m_fStateTime);
	SetPosition(pos);

	if (m_fStateTime >= StateTime::DROP_UP)
	{
		// 目標値に設定
		SetPosition(StatePos::DEST_DROP_UP);

		// 状態タイマーリセット
		m_fStateTime = 0.0f;

		// 待機状態へ遷移
		m_state = State::STATE_DROP_WAIT;
	}
}

//==========================================================================
// 落下(待機)
//==========================================================================
void CCountdown_Start::StateDrop_Wait()
{
	if (m_fStateTime >= StateTime::DROP_WAIT)
	{
		// 状態タイマーリセット
		m_fStateTime = 0.0f;

		// 落下状態へ遷移
		m_state = State::STATE_DROP_DOWN;
	}
}

//==========================================================================
// 落下(下)
//==========================================================================
void CCountdown_Start::StateDrop_DOWN()
{
	MyLib::Vector3 pos = GetPosition();

	// 補正、徐々に加速
	pos = UtilFunc::Correction::EaseInExpo(StatePos::DEST_DROP_UP, StatePos::DEST_DROP_DOWN, 0.0f, StateTime::DROP_DOWN, m_fStateTime);
	SetPosition(pos);

	if (m_fStateTime >= StateTime::DROP_DOWN)
	{
		// 目標値に設定
		SetPosition(StatePos::DEST_DROP_DOWN);

		// カウント
		m_nCount--;

		// 状態タイマーリセット
		m_fStateTime = 0.0f;

		// 落下状態へ遷移
		m_state = State::STATE_DROP_UP;

		if (m_nCount <= 0)
		{
			// 完了状態へ遷移
			m_state = State::STATE_COMPLETE;

			// 目標値に設定
			SetPosition(StatePos::DEST_COMPLETE);

			// サウンド再生
			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_COUNTDOWN2);
		}
		else
		{
			// サウンド再生
			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_COUNTDOWN1);
		}

		// テクスチャ設定
		int texID = CTexture::GetInstance()->Regist(TEXTURE[m_nCount]);
		BindTexture(texID);

		// サイズ設定
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

		// 縦幅を元にサイズ設定
		if (m_nCount != 0)
		{
			size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
			SetSize(size);
			SetSizeOrigin(size);
		}
		else
		{// 開始
			size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
			SetSize(size * 8.0f);
			SetSizeOrigin(size);
			SetAlpha(0.0f);
		}
	}
}

//==========================================================================
// 完了
//==========================================================================
void CCountdown_Start::StateComplete()
{

	float ratio = UtilFunc::Correction::EaseInExpo(0.0f, 1.0f, 0.0f, StateTime::COMPLETE * 0.8f, m_fStateTime);

	// 透明度設定
	SetAlpha(ratio);

	// サイズ設定
	D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();
	size.x = (sizeOrigin.x * 5.0f) + (sizeOrigin.x - (sizeOrigin.x * 5.0f)) * ratio;
	size.y = (sizeOrigin.y * 5.0f) + (sizeOrigin.y - (sizeOrigin.y * 5.0f)) * ratio;
	SetSize(size);

	if (m_fStateTime >= StateTime::COMPLETE)
	{
		// 状態タイマーリセット
		m_fStateTime = 0.0f;

		// フェードアウト状態へ遷移
		m_state = State::STATE_FADEOUT;

		CBaggage* pBaggage = CPlayer::GetListObj().GetData(0)->GetBaggage();
		pBaggage->SetState(CBaggage::STATE::STATE_NONE);

		// 空気待ち
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_WAIT_AIRPUSH);
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CCountdown_Start::StateFadeout()
{
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		// 状態タイマーリセット
		m_fStateTime = 0.0f;

		Uninit();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CCountdown_Start::Draw()
{
	// 描画処理
	CObject2D::Draw();
}


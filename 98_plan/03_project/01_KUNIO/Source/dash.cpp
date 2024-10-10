//=============================================================================
// 
//  ダッシュ処理 [dash.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "dash.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\key\\A.png";	// テクスチャのファイル
	const float TIME_INTERVAL = 0.3f;	// ダッシュ猶予
	const float TIME_DASH = 0.2f;		// ダッシュ時間
	const float MOVE_VELOCITY = 5.0f;	// 移動速度
	const float INTERVAL_INPUT = (2.0f / 60.0f);	// 入力
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CDash::STATE_FUNC CDash::m_StateFuncList[] =
{
	&CDash::StateNone,	// なし
	&CDash::StateDash,	// ダッシュ
};

//==========================================================================
// コンストラクタ
//==========================================================================
CDash::CDash(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_HoldDashAngle = Angle::ANGLE_UP;			// 保持してるダッシュの移動方向
	m_fInputInterval = 0.0f;	// 入力の受け付け猶予
	m_fDashInterval = 0.0f;		// ダッシュのインターバル
	m_state = State::STATE_NONE;
	memset(m_nCntTrigger, 0, sizeof(m_nCntTrigger));	// トリガーのカウント

}

//==========================================================================
// デストラクタ
//==========================================================================
CDash::~CDash()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CDash* CDash::Create()
{
	// メモリの確保
	CDash* pObj = DEBUG_NEW CDash;

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
HRESULT CDash::Init()
{

	// オブジェクト2Dの初期化
	CObject2D::Init();

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SAMPLE);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 50.0f);
#endif
	SetSize(size);
	SetSizeOrigin(size);


	// 位置、向き設定は必要があれば追加
	SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CDash::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CDash::Update()
{
	// 状態更新
	UpdateState();

	// 取得
	MyLib::Vector3 pos = GetPosition(), move = GetMove();

	// 移動
	pos += move;
	move += (MyLib::Vector3() - move) * 0.15f;

	// 情報設定
	SetPosition(pos);
	SetMove(move);

	for (int i = 0; i < Angle::ANGLE_MAX; i++)
	{
		ImGui::Text("[%d] : %d", i, m_nCntTrigger[i]);
	}

	// 更新処理
	CObject2D::Update();
}

//==========================================================================
// 状態更新
//==========================================================================
void CDash::UpdateState()
{
	// 状態タイマー加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 実行
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// なし
//==========================================================================
void CDash::StateNone()
{
	m_fStateTime = 0.0f;

	// インプット取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// ダッシュ判定
	MyStruct info;

	// スティックの向き
	float stickAngle = pPad->GetStickRotL(0);

	// 移動フラグ
	bool bMove = false;

	if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) || pPad->GetStickMoveL(0).y > 0)
	{// 上
		bMove = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) || pPad->GetStickMoveL(0).x < 0)
		{// 左上
			info.angle = Angle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) || pPad->GetStickMoveL(0).x > 0)
		{// 右上
			info.angle = Angle::ANGLE_RIGHTUP;
		}
		else
		{// 上
			info.angle = Angle::ANGLE_UP;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) || pPad->GetStickMoveL(0).y < 0)
	{// 下
		bMove = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) || pPad->GetStickMoveL(0).x < 0)
		{// 左下
			info.angle = Angle::ANGLE_LEFTDW;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) || pPad->GetStickMoveL(0).x > 0)
		{// 右下
			info.angle = Angle::ANGLE_RIGHTDW;
		}
		else
		{// 下
			info.angle = Angle::ANGLE_DOWN;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) || pPad->GetStickMoveL(0).x > 0)
	{// 右
		bMove = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) || pPad->GetStickMoveL(0).y < 0)
		{// 右上
			info.angle = Angle::ANGLE_RIGHTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) || pPad->GetStickMoveL(0).y > 0)
		{// 右下
			info.angle = Angle::ANGLE_RIGHTDW;
		}
		else
		{// 右
			info.angle = Angle::ANGLE_RIGHT;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) || pPad->GetStickMoveL(0).x < 0)
	{// 左
		bMove = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) || pPad->GetStickMoveL(0).y < 0)
		{// 左上
			info.angle = Angle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) || pPad->GetStickMoveL(0).y > 0)
		{// 左下
			info.angle = Angle::ANGLE_LEFTDW;
		}
		else
		{// 左
			info.angle = Angle::ANGLE_LEFT;
		}
	}

	if (bMove)
	{// 移動中
		MyLib::Vector3 move;
		float division = (D3DX_PI * 2.0f) / Angle::ANGLE_MAX;
		move.x += cosf((D3DX_PI * -0.5f) + division * info.angle) * MOVE_VELOCITY;
		move.y += sinf((D3DX_PI * -0.5f) + division * info.angle) * MOVE_VELOCITY;
		SetMove(move);
	}






	// 入力フラグ
	bool bInput = false;

	if (m_fInputInterval <= 0.0f)
	{// 猶予受け付け終了中

		info = MyStruct();
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0))
		{// 上
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				(/*pPad->GetLStickTrigger(CInputGamepad::STICK_X) && */pPad->GetStickMoveL(0).x < 0))
			{// 左上
				m_HoldDashAngle = Angle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				(/*pPad->GetLStickTrigger(CInputGamepad::STICK_X) && */pPad->GetStickMoveL(0).x > 0))
			{// 右上
				m_HoldDashAngle = Angle::ANGLE_RIGHTUP;
			}
			else
			{// 上
				m_HoldDashAngle = Angle::ANGLE_UP;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y < 0))
		{// 下
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				(/*pPad->GetLStickTrigger(CInputGamepad::STICK_X) && */pPad->GetStickMoveL(0).x < 0))
			{// 左下
				m_HoldDashAngle = Angle::ANGLE_LEFTDW;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				(/*pPad->GetLStickTrigger(CInputGamepad::STICK_X) && */pPad->GetStickMoveL(0).x > 0))
			{// 右下
				m_HoldDashAngle = Angle::ANGLE_RIGHTDW;
			}
			else
			{// 下
				m_HoldDashAngle = Angle::ANGLE_DOWN;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0))
		{// 右
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				(/*pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && */pPad->GetStickMoveL(0).y < 0))
			{// 右上
				m_HoldDashAngle = Angle::ANGLE_RIGHTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				(/*pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && */pPad->GetStickMoveL(0).y > 0))
			{// 右下
				m_HoldDashAngle = Angle::ANGLE_RIGHTDW;
			}
			else
			{// 右
				m_HoldDashAngle = Angle::ANGLE_RIGHT;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0))
		{// 左
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				(/*pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && */pPad->GetStickMoveL(0).y < 0))
			{// 左上
				m_HoldDashAngle = Angle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				(/*pPad->GetLStickTrigger(CInputGamepad::STICK_Y) &&*/ pPad->GetStickMoveL(0).y > 0))
			{// 左下
				m_HoldDashAngle = Angle::ANGLE_LEFTDW;
			}
			else
			{// 左
				m_HoldDashAngle = Angle::ANGLE_LEFT;
			}
		}
	}


	if (bInput)
	{// 入力された

		// 入力のインターバル
		m_fInputInterval = INTERVAL_INPUT;
	}

	if (m_fInputInterval > 0.0f)
	{// 受け付け猶予中

		// 次の入力判定
		bool bNextInput = false;
		switch (m_HoldDashAngle)
		{
		case Angle::ANGLE_LEFT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y < 0))
			{// 上
				info = Trigger(Angle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0))
			{// 下
				info = Trigger(Angle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			break;

		case Angle::ANGLE_RIGHT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y < 0))
			{// 上
				info = Trigger(Angle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0))
			{// 下
				info = Trigger(Angle::ANGLE_RIGHTDW);
				bNextInput = true;
			}
			break;

		case Angle::ANGLE_UP:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0))
			{// 左
				info = Trigger(Angle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0))
			{// 右
				info = Trigger(Angle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			break;

		case Angle::ANGLE_DOWN:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0))
			{// 左
				info = Trigger(Angle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0))
			{// 右
				info = Trigger(Angle::ANGLE_RIGHTDW);
				bNextInput = true;
			}
			break;

		default:
			info = Trigger(m_HoldDashAngle);
			bNextInput = true;
			break;
		}

		// 受け付けされてるよ
		if (bNextInput)
		{
			// 受け付け猶予終了
			m_fInputInterval = 0.0f;
		}
	}

	// 受け付け猶予減算
	float oldTime = m_fInputInterval;
	m_fInputInterval -= CManager::GetInstance()->GetDeltaTime();
	m_fInputInterval = UtilFunc::Transformation::Clamp(m_fInputInterval, 0.0f, INTERVAL_INPUT);

	if (oldTime > 0.0f &&
		m_fInputInterval <= 0.0f)
	{// 前回まだ猶予中 && 今は終了
		info = Trigger(m_HoldDashAngle);
	}

	// ダッシュする
	if (info.bDash)
	{
		MyLib::Vector3 move;
		float division = (D3DX_PI * 2.0f) / Angle::ANGLE_MAX;
		move.x += cosf((D3DX_PI * -0.5f) + division * info.angle) * 25.0f;
		move.y += sinf((D3DX_PI * -0.5f) + division * info.angle) * 25.0f;
		SetMove(move);
		memset(m_nCntTrigger, 0, sizeof(m_nCntTrigger));
	}

	// 猶予減らしていく
	m_fDashInterval -= CManager::GetInstance()->GetDeltaTime();
	if (m_fDashInterval <= 0.0f)
	{
		// トリガーのカウント
		memset(m_nCntTrigger, 0, sizeof(m_nCntTrigger));
	}
}

//==========================================================================
// ダッシュ
//==========================================================================
void CDash::StateDash()
{
	if (m_fStateTime >= TIME_DASH)
	{
		m_fStateTime = 0.0f;
		m_state = State::STATE_NONE;
	}
}


//==========================================================================
// トリガー
//==========================================================================
CDash::MyStruct CDash::Trigger(Angle angle)
{
	MyStruct info;
	info.bDash = false;

	if (m_nCntTrigger[angle] == 1)
	{// ダッシュ
		info.bDash = true;
		info.angle = angle;

		m_fStateTime = 0.0f;
		m_state = State::STATE_DASH;
	}

	// タイマーリセット
	m_fDashInterval = TIME_INTERVAL;

	// トリガーのカウント
	m_nCntTrigger[angle] = (m_nCntTrigger[angle] + 1) % 2;

	return info;
}

//==========================================================================
// 描画処理
//==========================================================================
void CDash::Draw()
{
	// 描画処理
	CObject2D::Draw();
}

//=============================================================================
// 
//  タイマー処理(真ん中) [timer_result.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "timer_result.h"
#include "manager.h"
#include "multinumber.h"
#include "debugproc.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"

#include "2D_effect.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\number\\number_oradano.png";	// テクスチャのファイル
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\result\\cleartime.png";

	const MyLib::Vector3 DEFAULT_POSITION = MyLib::Vector3(1100.0f, 100.0f, 0.0f);	// 初期位置
	const D3DXVECTOR2 SIZE_NUMBER = D3DXVECTOR2(30.0f, 30.0f);
	const float DSTANCE_TIMER = SIZE_NUMBER.x * 2.25f;

	const float SIZE_HEIGHT = 50.0f;	// 縦幅のサイズ
	const float MOVEVALUE_TEXT = 3.0f;	// テキストの移動量
	const float MOVEVALUE_TIME = 9.0f;	// タイムの移動量
	const float DSTANCE_MULTIPLAY = 2.25f;
}

namespace StateTime
{
	const float WAIT = 0.5f;	// 待機
	const float EMPHASIZE = 1.0f;	// 強調
	const int SCROLLEND_WAITFRAME = 60;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CTimer_Result::STATE_FUNC CTimer_Result::m_StateFunc[] =
{
	&CTimer_Result::StateScrollText,	// 文字送り
	&CTimer_Result::StateSrollVoid,		// 空間送り
	&CTimer_Result::StateScrollTime,	// タイム送り
	&CTimer_Result::StateFinish,		// 終了
	&CTimer_Result::StateEmphasize,		// 強調
	&CTimer_Result::StateNone,			// なにもなし

};

//==========================================================================
// コンストラクタ
//==========================================================================
CTimer_Result::CTimer_Result(int nPriority) : CTimer(nPriority)
{
	// 値のクリア
	m_fStateTime = 0.0f;		// 状態カウンター
	m_state = State::STATE_SCROLL_TEXT;			// 状態
	m_fMoveTextLen = 0.0f;	// テキストの移動距離
	m_fMoveTimeLen = 0.0f;	// タイムの移動距離
	m_bFinish = false;		// 終了
}

//==========================================================================
// デストラクタ
//==========================================================================
CTimer_Result::~CTimer_Result()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTimer_Result::Init()
{
	//=============================
	// 文字生成
	//=============================
	CreateText();

	// 初期化
	CTimer::Init();
	m_pos = m_pText->GetPosition() + MyLib::Vector3(m_pText->GetSizeOrigin().x * 2.0f + 30.0f, 0.0f, 0.0f);

	// アンカーポイントを左にする
	for (int i = 0; i < 3; i++)
	{
		CMultiNumber* pMultiNumber = m_pClearTime[i];

		// UV自動設定OFF
		pMultiNumber->SetEnableAutoUVSetting(true);

		CNumber** pNumber = pMultiNumber->GetNumber();

		for (int j = 0; j < pMultiNumber->GetDigit(); j++)
		{
			pNumber[j]->SetType(CObject::TYPE_OBJECT2D);
			CObject2D* pObj2D = pNumber[j]->GetObject2D();
			pObj2D->SetSize(D3DXVECTOR2(0.0f, pObj2D->GetSizeOrigin().y));
			pObj2D->SetAnchorType(CObject2D::AnchorPoint::LEFT);
		}
	}

	// タイマー
	ApplyTimer();


	// 状態遷移
	SetState(State::STATE_NONE);

	return S_OK;
}


//==========================================================================
// 文字生成
//==========================================================================
void CTimer_Result::CreateText()
{
	// 生成
	m_pText = CObject2D::Create(6);
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	m_pText->BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_HEIGHT);
	m_pText->SetSize(D3DXVECTOR2(0.0f, size.y));
	m_pText->SetSizeOrigin(size);

	// 位置設定
	m_pText->SetPosition(MyLib::Vector3(200.0f, 350.0f, 0.0f));
	
	// アンカーポイントの設定
	m_pText->SetAnchorType(CObject2D::AnchorPoint::LEFT);
}

//==========================================================================
// 終了処理
//==========================================================================
void CTimer_Result::Uninit()
{
	CTimer::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTimer_Result::Update()
{
	// 更新処理
	CTimer::Update();

	// 状態更新
	UpdateState();
}

//==========================================================================
// タイマー反映
//==========================================================================
void CTimer_Result::ApplyTimer()
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

		// 位置設定
		MyLib::Vector3 pos = m_pos;
		pos.x += (m_pClearTime[0]->GetNumber()[0]->GetSize().x * DSTANCE_MULTIPLAY) * (2 - i);
		m_pClearTime[i]->SetPosition(pos);

	}
}

//==========================================================================
// 状態更新
//==========================================================================
void CTimer_Result::UpdateState()
{

	// 状態タイマー
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// スキップ
//==========================================================================
void CTimer_Result::Skip()
{
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{
		// モード設定
		SetState(CTimer_Result::State::STATE_FINISH);
	}
}

//==========================================================================
// 文字送り
//==========================================================================
void CTimer_Result::StateScrollText()
{
	// サイズ取得
	D3DXVECTOR2 size = m_pText->GetSize(), sizeOrigin = m_pText->GetSizeOrigin();

	// テキスト移動距離加算
	m_fMoveTextLen += MOVEVALUE_TEXT;
	m_fMoveTextLen = UtilFunc::Transformation::Clamp(m_fMoveTextLen, 0.0f, sizeOrigin.x);

	if (m_fMoveTextLen >= sizeOrigin.x)
	{
		// 状態遷移
		SetState(State::STATE_SCROLL_VOID);
	}

	// サイズ設定
	size.x = m_fMoveTextLen;
	m_pText->SetSize(size);

	// テクスチャ座標設定
	D3DXVECTOR2* pTex = m_pText->GetTex();
	pTex[1].x = pTex[3].x = (size.x / sizeOrigin.x);

	// スキップ
	Skip();
}

//==========================================================================
// 空間送り
//==========================================================================
void CTimer_Result::StateSrollVoid()
{
	if (m_fStateTime >= StateTime::WAIT)
	{
		// 状態遷移
		SetState(State::STATE_SCROLL_TIME);

		// サウンド再生
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
	}

	// スキップ
	Skip();
}

//==========================================================================
// タイム送り
//==========================================================================
void CTimer_Result::StateScrollTime()
{
	// アンカーポイントを左にする
	D3DXVECTOR2 size, sizeOrigin;
	MyLib::Vector3 pos;
	D3DXVECTOR2* pTex = nullptr;

	// テキスト移動距離加算
	m_fMoveTimeLen += MOVEVALUE_TIME;


	// 基点の位置
	float basePoint = (m_fMoveTimeLen + m_pClearTime[0]->GetNumber()[0]->GetPosition().x);


	for (int i = 0; i < 3; i++)
	{
		// ナンバー取得
		CMultiNumber* pMultiNumber = m_pClearTime[i];
		CNumber** pNumber = pMultiNumber->GetNumber();

		for (int j = 0; j < pMultiNumber->GetDigit(); j++)
		{
			// 数字の2Dオブジェクト取得
			CObject2D* pObj2D = pNumber[j]->GetObject2D();
			size = pObj2D->GetSize();
			sizeOrigin = pObj2D->GetSizeOrigin();
			pos = pObj2D->GetPosition();
			pTex = pObj2D->GetTex();

			// 位置によっての割合計算
			float ratio = 0.0f;
			if (basePoint > pos.x)
			{
				ratio = (basePoint - pos.x) / sizeOrigin.x;
				ratio = UtilFunc::Transformation::Clamp(ratio, 0.0f, 1.0f);
			}

			// 割合によってサイズ変更
			size.x = ratio * sizeOrigin.x;
			pObj2D->SetSize(size);

			// テクスチャ設定
			float fNum = pNumber[j]->GetNum() * 0.1f;
			pTex[0].x = pTex[2].x = fNum;
			pTex[1].x = pTex[3].x = (fNum + 0.1f) * ratio;
		}
	}


	// 最後の文字が送り終わった
	if (m_pClearTime[0]->GetNumber()[1]->GetPosition().x + (StateTime::SCROLLEND_WAITFRAME * MOVEVALUE_TEXT) < basePoint)
	{
		// 状態遷移
		SetState(State::STATE_FINISH);
	}

	// スキップ
	Skip();
}

//==========================================================================
// 終了
//==========================================================================
void CTimer_Result::StateFinish()
{
	// 終了フラグ
	m_bFinish = true;

	//=============================
	// テキスト変更
	//=============================
	{
		// サイズ設定
		m_pText->SetSize(m_pText->GetSizeOrigin());

		// テクスチャ設定
		D3DXVECTOR2* pTex = m_pText->GetTex();
		pTex[1].x = pTex[3].x = 1.0f;
	}

	//=============================
	// タイム変更
	//=============================
	for (int i = 0; i < 3; i++)
	{
		// ナンバー取得
		CMultiNumber* pMultiNumber = m_pClearTime[i];
		CNumber** pNumber = pMultiNumber->GetNumber();

		for (int j = 0; j < pMultiNumber->GetDigit(); j++)
		{
			// 数字の2Dオブジェクト取得
			CObject2D* pObj2D = pNumber[j]->GetObject2D();
			D3DXVECTOR2* pTex = pObj2D->GetTex();
			pObj2D->SetSize(pObj2D->GetSizeOrigin());

			// テクスチャ設定
			float fNum = pNumber[j]->GetNum() * 0.1f;
			pTex[0].x = pTex[2].x = fNum;
			pTex[1].x = pTex[3].x = fNum + 0.1f;
		}
	}


	// 状態遷移
	SetState(State::STATE_EMPHASIZE);
}

//==========================================================================
// 強調
//==========================================================================
void CTimer_Result::StateEmphasize()
{
	D3DXVECTOR2 size, sizeOrigin;

	for (int i = 0; i < 3; i++)
	{
		// ナンバー取得
		CMultiNumber* pMultiNumber = m_pClearTime[i];
		CNumber** pNumber = pMultiNumber->GetNumber();

		for (int j = 0; j < pMultiNumber->GetDigit(); j++)
		{
			// 数字の2Dオブジェクト取得
			CObject2D* pObj2D = pNumber[j]->GetObject2D();
			size = pObj2D->GetSize();
			sizeOrigin = pObj2D->GetSizeOrigin();

			float ratio = m_fStateTime / StateTime::EMPHASIZE;
			size.x = sizeOrigin.x + sinf(D3DX_PI * ratio) * (sizeOrigin.x * 0.5f);
			size.y = sizeOrigin.y + sinf(D3DX_PI * ratio) * (sizeOrigin.y * 0.5f);

			pMultiNumber->SetKerning(size.y);
			pObj2D->SetSize(size);
		}
	}


	if (m_fStateTime >= StateTime::EMPHASIZE)
	{
		// 状態遷移
		SetState(State::STATE_NONE);
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CTimer_Result::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 描画処理
//==========================================================================
void CTimer_Result::Draw()
{

}

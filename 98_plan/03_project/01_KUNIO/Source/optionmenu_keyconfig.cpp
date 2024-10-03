//=============================================================================
// 
//  サウンドメニュー処理 [optionmenu_keyconfig.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "optionmenu_keyconfig.h"
#include "manager.h"
#include "keyconfig.h"
#include "keyconfig_gamepad.h"
#include "keyconfig_keyboard.h"
#include "input_gamepad.h"
#include "controlkeydisp.h"
#include "selectdraw.h"


//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const MyLib::Vector3 DEFAULT_POS = MyLib::Vector3(640.0f, -50.0f, 0.0f);	// 基本座標
	const float DOWN_POSY = 100.0f;
	const float KEY_SIZE = 50.0f;
	const float FONT_WIDTH = 150.0f;
	const float TITLE_HEIGHT = KEY_SIZE * 1.3f;
	const D3DXCOLOR FRONT_COL = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.0f);
	const float ADD_ALPHA = (1.0f / 20.0f);
}

// ファイル名
namespace FILENAME
{
	const std::string CONFIG[INGAME::ACTION::ACT_MAX] =
	{
		"data\\TEXTURE\\ui_setting\\decide.png",
		"data\\TEXTURE\\ui_setting\\back.png",
		"data\\TEXTURE\\ui_setting\\updown.png",
		"data\\TEXTURE\\ui_setting\\air.png",
		"data\\TEXTURE\\ui_setting\\checkpoint.png",
		"data\\TEXTURE\\ui_setting\\retry.png",
		"data\\TEXTURE\\ui_setting\\pause.png",
	};

	const std::string FRONT = "data\\TEXTURE\\ui_setting\\front.png";
}

//==========================================================================
// コンストラクタ
//==========================================================================
COptionMenu_Keyconfig::COptionMenu_Keyconfig(int nPriority) : COptionMenu(nPriority)
{
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		m_aKeyConfig[i].s_p2D = nullptr;
		m_aKeyConfig[i].s_pKeyDisp = nullptr;

		// クリア
		m_aKeyConfig[i] = KEYINFO();
	}

	// クリア
	m_checkconfig = CHENGEINFO();

	m_pTitle2D = nullptr;
	m_pSelect = nullptr;
	m_bNowChange = false;
	m_SelectKey = 0;
	m_Alpha = 0.0f;
	m_fTime = 0.0f;
}

//==========================================================================
// デストラクタ
//==========================================================================
COptionMenu_Keyconfig::~COptionMenu_Keyconfig()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT COptionMenu_Keyconfig::Init()
{
	// 初期化処理
	COptionMenu::Init();

	CKeyConfig* pConfigPad = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CTexture* pTexture = CTexture::GetInstance();
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 説明文字生成
	{
		MyLib::Vector3 pos = DEFAULT_POS;
		pos.y += DOWN_POSY;
		m_pTitle2D = CObject2D::Create(11);
		m_pTitle2D->SetType(CObject::TYPE_OBJECT2D);
		m_pTitle2D->SetPosition(pos);
		m_pTitle2D->BindTexture(pTexture->Regist("data\\TEXTURE\\ui_setting\\setting.png"));
		D3DXVECTOR2 texture = pTexture->GetImageSize(m_pTitle2D->GetIdxTexture());
		m_pTitle2D->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, TITLE_HEIGHT));
		m_pTitle2D->SetColor(col);
	}

	// ポリゴンを生成する
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		if (i <= INGAME::ACT_BACK) { continue; }

		// キーを生成
		MyLib::Vector3 pos = DEFAULT_POS;
		pos.y += i * DOWN_POSY;
		pos.x += FONT_WIDTH;
		m_aKeyConfig[i].s_pKeyDisp = CControlKeyDisp::Create(pos, 0.0f, KEY_SIZE, KEY_SIZE, pConfigPad->GetKey(i));
		m_aKeyConfig[i].s_pKeyDisp->SetColor(col);

		// 文字を生成
		pos.x = DEFAULT_POS.x - FONT_WIDTH;
		m_aKeyConfig[i].s_p2D = CObject2D::Create(11);
		m_aKeyConfig[i].s_p2D->SetType(CObject::TYPE_OBJECT2D);
		m_aKeyConfig[i].s_p2D->SetPosition(pos);
		m_aKeyConfig[i].s_p2D->BindTexture(pTexture->Regist(FILENAME::CONFIG[i]));
		D3DXVECTOR2 texture = pTexture->GetImageSize(m_aKeyConfig[i].s_p2D->GetIdxTexture());

		// サイズ設定
		D3DXVECTOR2 setsize = UtilFunc::Transformation::AdjustSizeByHeight(texture, KEY_SIZE);
		m_aKeyConfig[i].s_p2D->SetSize(setsize);
		m_aKeyConfig[i].s_p2D->SetSizeOrigin(setsize);
		m_aKeyConfig[i].s_p2D->SetColor(col);

		// 暗くする用のポリゴン生成
		pos.x = DEFAULT_POS.x;
		
#if 1
		// 筆のアドレス渡し
		KEYINFO* pNowInfo = &m_aKeyConfig[i];
#else
		m_aKeyConfig[i].s_p2DFront = CObject2D::Create(10);
		m_aKeyConfig[i].s_p2DFront->SetSize(D3DXVECTOR2(0.0f, KEY_SIZE));
		m_aKeyConfig[i].s_p2DFront->SetSizeOrigin(D3DXVECTOR2(KEY_SIZE * 6, KEY_SIZE));
		m_aKeyConfig[i].s_p2DFront->SetColor(FRONT_COL);
		m_aKeyConfig[i].s_p2DFront->BindTexture(pTexture->Regist(FILENAME::FRONT));

		m_aKeyConfig[i].s_p2DFront->SetAnchorType(CObject2D::AnchorPoint::LEFT);
		m_aKeyConfig[i].s_p2DFront->SetPosition(pos - MyLib::Vector3(KEY_SIZE * 6.0f, 0.0f, 0.0f));
#endif
	}

	{
		MyLib::Vector3 pos = DEFAULT_POS;
		pos.y += DOWN_POSY * (INGAME::ACT_BACK + 1);
		// 筆ポリゴン生成
		m_pSelect = CSelectDraw::Create(pos);
		m_pSelect->SetSizeOrigin(D3DXVECTOR2(KEY_SIZE * 3, KEY_SIZE));
		m_pSelect->SetSize(m_pSelect->GetSizeOrigin());

		// 初期状態設定
		m_pSelect->SetState(CSelectDraw::State::STATE_NONE);
		m_pSelect->SetAlpha(0.0f);
	}

	// 決定戻る変更用ポリゴン生成
	{
		MyLib::Vector3 pos = DEFAULT_POS;
		pos.y += DOWN_POSY * INGAME::ACTION::ACT_MAX;

		// 文字の生成
		m_checkconfig.s_p2D = CObject2D::Create(11);
		m_checkconfig.s_p2D->SetType(CObject::TYPE_OBJECT2D);
		m_checkconfig.s_p2D->SetPosition(pos);
		m_checkconfig.s_p2D->BindTexture(pTexture->Regist("data\\TEXTURE\\ui_setting\\change.png"));
		D3DXVECTOR2 texture = pTexture->GetImageSize(m_checkconfig.s_p2D->GetIdxTexture());
		m_checkconfig.s_p2D->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, KEY_SIZE * 0.8f));
		m_checkconfig.s_p2D->SetColor(col);

		// キーコンフィグを生成
		float size = KEY_SIZE * 0.75f;
		pos.x += (size * 3);

		// OK
		m_checkconfig.s_pKeyDispOK = CControlKeyDisp::Create(pos, 0.0f, size, size,
			pConfigPad->GetKey(INGAME::ACT_OK));
		pos.x += (size * 3);
		m_checkconfig.s_pKeyDispOK->SetColor(col);

		// NO
		m_checkconfig.s_pKeyDispNO = CControlKeyDisp::Create(pos, 0.0f, size, size,
			pConfigPad->GetKey(INGAME::ACT_BACK));
		m_checkconfig.s_pKeyDispNO->SetColor(col);

		// 暗くする用のポリゴン生成
		pos = DEFAULT_POS;
		pos.y += DOWN_POSY * INGAME::ACTION::ACT_MAX;

#if 0
		m_checkconfig.s_p2DFront = CObject2D::Create(10);
		m_checkconfig.s_p2DFront->SetPosition(pos);
		m_checkconfig.s_p2DFront->SetSize(D3DXVECTOR2(KEY_SIZE * 6, KEY_SIZE));
		m_checkconfig.s_p2DFront->SetColor(FRONT_COL);
		m_checkconfig.s_p2DFront->BindTexture(pTexture->Regist(FILENAME::FRONT));
#else

		// 筆ポリゴン生成
		m_checkconfig.pSelect = CSelectDraw::Create(pos - MyLib::Vector3(KEY_SIZE * 6, 0.0f, 0.0f));
		m_checkconfig.pSelect->SetSizeOrigin(D3DXVECTOR2(KEY_SIZE * 6, KEY_SIZE));
		m_checkconfig.pSelect->SetSize(m_checkconfig.pSelect->GetSizeOrigin());

		// 初期状態設定
		m_checkconfig.pSelect->SetState(CSelectDraw::State::STATE_NONE);
		m_checkconfig.pSelect->SetAlpha(0.0f);

#endif
	}

	m_SelectKey = INGAME::ACT_BACK + 1;


	// 初期選択肢の筆状態設定
	m_pSelect->SetState(CSelectDraw::State::STATE_FADEIN);

	// 不透明度初期値1.0f
	m_Alpha = 1.0f;


	// 選択肢リセット
	ResetSelect();
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void COptionMenu_Keyconfig::Uninit()
{
	// 終了処理
	COptionMenu::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void COptionMenu_Keyconfig::Kill()
{
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		// 2Dポリゴンの終了
		if (m_aKeyConfig[i].s_p2D != nullptr)
		{
			m_aKeyConfig[i].s_p2D->Uninit();
			m_aKeyConfig[i].s_p2D = nullptr;
		}

		// 2Dポリゴンの終了
#if 0
		if (m_aKeyConfig[i].s_p2DFront != nullptr)
		{
			m_aKeyConfig[i].s_p2DFront->Uninit();
			m_aKeyConfig[i].s_p2DFront = nullptr;
		}
#else
		
#endif

		// キーコンフィグ表示の終了
		if (m_aKeyConfig[i].s_pKeyDisp != nullptr)
		{
			m_aKeyConfig[i].s_pKeyDisp->Uninit();
			m_aKeyConfig[i].s_pKeyDisp = nullptr;
		}
	}

	{
		// 2Dポリゴンの終了
		if (m_checkconfig.s_p2D != nullptr)
		{
			m_checkconfig.s_p2D->Uninit();
			m_checkconfig.s_p2D = nullptr;
		}

		// 2Dポリゴンの終了
		// 選択肢の筆の終了
		if (m_checkconfig.pSelect != nullptr)
		{
			m_checkconfig.pSelect->Uninit();
			m_checkconfig.pSelect = nullptr;
		}

		// キーコンフィグ表示の終了
		if (m_checkconfig.s_pKeyDispOK != nullptr)
		{
			m_checkconfig.s_pKeyDispOK->Uninit();
			m_checkconfig.s_pKeyDispOK = nullptr;
		}

		if (m_checkconfig.s_pKeyDispNO != nullptr)
		{
			m_checkconfig.s_pKeyDispNO->Uninit();
			m_checkconfig.s_pKeyDispNO = nullptr;
		}
	}

	// タイトル
	if (m_pTitle2D != nullptr)
	{
		m_pTitle2D->Uninit();
		m_pTitle2D = nullptr;
	}

	if (m_pSelect != nullptr)
	{
		m_pSelect->Uninit();
		m_pSelect = nullptr;
	}

	// 削除処理
	COptionMenu::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void COptionMenu_Keyconfig::Update()
{
	// 更新処理
	COptionMenu::Update();
}

//==========================================================================
// 編集
//==========================================================================
void COptionMenu_Keyconfig::StateEdit()
{
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();
	CKeyConfig* pKeyConfig = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);

	// 不透明度設定
	//SetAlpha();


	// 元のサイズに戻す
	for (const auto& key : m_aKeyConfig)
	{
		if (key.s_pKeyDisp == nullptr) continue;
		key.s_pKeyDisp->SetSize(key.s_pKeyDisp->GetSizeOrigin());
	}

	// 選択中のものを少し拡大
	if (m_bNowChange &&
		m_SelectKey < INGAME::ACTION::ACT_MAX &&
		m_aKeyConfig[m_SelectKey].s_pKeyDisp != nullptr)
	{
		m_aKeyConfig[m_SelectKey].s_pKeyDisp->SetSize(m_aKeyConfig[m_SelectKey].s_pKeyDisp->GetSizeOrigin() * 1.15f);
	}

	if (m_bNowChange) { return; }

	// 入力待ち状態にする
	if (pKeyConfig->GetTrigger(INGAME::ACT_OK))
	{
		if (m_SelectKey == INGAME::ACTION::ACT_MAX)
		{
			// キーを取得
			int oktype = pKeyConfig->GetKey(INGAME::ACT_OK);
			int backtype = pKeyConfig->GetKey(INGAME::ACT_BACK);

			// キーの入れ替え
			pKeyConfig->Join(INGAME::ACT_OK, backtype);
			pKeyConfig->Join(INGAME::ACT_BACK, oktype);

			// テクスチャ種類を設定
			m_checkconfig.s_pKeyDispOK->SetType(backtype);
			m_checkconfig.s_pKeyDispNO->SetType(oktype);

			return;
		}
		// 決定キーと戻るキー以外
		else if (m_SelectKey != INGAME::ACT_OK && m_SelectKey != INGAME::ACT_BACK)
		{
			// 変更状態に
			m_bNowChange = true;

			// SE再生
			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_DICTION);

			// マルチスレッドで入力受付
			std::thread th(&COptionMenu_Keyconfig::Chenge, this);
			th.detach();

			return;
		}
	}

	int oldkey = m_SelectKey;

	// 選択場所変更
	if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y > 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
		pKey->GetTrigger(DIK_W))
	{
		// 上に移動
		m_SelectKey = (m_SelectKey + INGAME::ACTION::ACT_MAX) % (INGAME::ACTION::ACT_MAX + 1);

		if (m_SelectKey < 2)
		{
			m_SelectKey = INGAME::ACTION::ACT_MAX;
		}

		// サウンド再生
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
	}
	else if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y < 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
		pKey->GetTrigger(DIK_S))
	{
		// 下に移動
		m_SelectKey = (m_SelectKey + 1) % (INGAME::ACTION::ACT_MAX + 1);
		m_SelectKey = UtilFunc::Transformation::Clamp(m_SelectKey, 2, static_cast<int>(INGAME::ACTION::ACT_MAX) + 1);

		// サウンド再生
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
	}

	// 変更していた
	if (oldkey != m_SelectKey)
	{
		if (m_SelectKey < INGAME::ACT_MAX)
		{// 選択肢
			m_pSelect->SetState(CSelectDraw::State::STATE_DRAWING);
			MyLib::Vector3 pos = m_aKeyConfig[m_SelectKey].s_pKeyDisp->GetPosition();
			m_pSelect->SetPosition(pos);
			m_pSelect->SetAlpha(1.0f);
			m_checkconfig.pSelect->SetAlpha(0.0f);
		}
		else
		{// 決定戻る切り替え
			m_checkconfig.pSelect->SetState(CSelectDraw::State::STATE_DRAWING);
			m_checkconfig.pSelect->SetAlpha(1.0f);
			m_pSelect->SetAlpha(0.0f);
		}
	}


	// 選択肢リセット
	ResetSelect();
}


//==========================================================================
// 変更
//==========================================================================
void COptionMenu_Keyconfig::Chenge()
{
	CKeyConfig* pKeyConfig = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);

	while (1)
	{
		if (pKeyConfig->GetRelease(INGAME::ACT_OK))
		{
			break;
		}
	}

	// 変更前を保存
	int oldkey = pKeyConfig->GetKey(m_SelectKey);

	// 変更
	pKeyConfig->Setting(m_SelectKey);

	// 元のサイズに戻す
	m_aKeyConfig[m_SelectKey].s_pKeyDisp->SetSize(m_aKeyConfig[m_SelectKey].s_pKeyDisp->GetSizeOrigin());

	if (m_aKeyConfig[m_SelectKey].s_pKeyDisp != nullptr)
	{
		m_aKeyConfig[m_SelectKey].s_pKeyDisp->SetType(pKeyConfig->GetKey(m_SelectKey));
	}

	// 変更後を保存
	int selectkey = pKeyConfig->GetKey(m_SelectKey);

	// 被りを確認する	
	for (int i = 0; i < INGAME::ACT_MAX; i++)
	{
		// 選択中
		if (m_SelectKey == i) { continue; }

		// 決定戻るの場合
		if (i <= INGAME::ACT_BACK) { continue; }

		// キー取得
		int key = pKeyConfig->GetKey(i);

		// 同じキーにしてしまっている場合
		if (selectkey == key)
		{
			// 変更前のと入れ替える
			pKeyConfig->Join(i, oldkey);
			if (m_aKeyConfig[i].s_pKeyDisp != nullptr)
			{
				m_aKeyConfig[i].s_pKeyDisp->SetType(pKeyConfig->GetKey(i));
			}
		}
	}

	while (1)
	{
		if (!pKeyConfig->GetPress(m_SelectKey))
		{
			break;
		}
	}

	m_bNowChange = false;
}

//==========================================================================
// 透明度調整
//==========================================================================
void COptionMenu_Keyconfig::SetAlpha()
{
	if (m_Alpha >= 1.0f) { return; }

	m_fTime += ADD_ALPHA;
	m_Alpha = UtilFunc::Correction::EaseInOutExpo(0.0f, 1.0f, 0, 2.0f, m_fTime);
	m_Alpha = UtilFunc::Transformation::Clamp(m_Alpha, 0.0f, 1.0f);

	// 色を調整する

	// タイトル
	D3DXCOLOR col = m_pTitle2D->GetColor();
	col.a = m_Alpha;
	m_pTitle2D->SetColor(col);

	// 色を調整する
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		// 文字
		col = m_aKeyConfig[i].s_p2D->GetColor();
		col.a = m_Alpha;
		m_aKeyConfig[i].s_p2D->SetColor(col);

		// キーコンフィグ
		col = m_aKeyConfig[i].s_pKeyDisp->GetColor();
		col.a = m_Alpha;
		m_aKeyConfig[i].s_pKeyDisp->SetColor(col);
	}

	// 文字の生成
	col = m_checkconfig.s_p2D->GetColor();
	col.a = m_Alpha;
	m_checkconfig.s_p2D->SetColor(col);

	// キーコンフィグを生成
	col = m_checkconfig.s_pKeyDispOK->GetColor();
	col.a = m_Alpha;
	m_checkconfig.s_pKeyDispOK->SetColor(col);
	col = m_checkconfig.s_pKeyDispNO->GetColor();
	col.a = m_Alpha;
	m_checkconfig.s_pKeyDispNO->SetColor(col);
}


//==========================================================================
// 選択肢リセット
//==========================================================================
void COptionMenu_Keyconfig::ResetSelect()
{
	// 選択状況に応じた色変更
	for (int i = 0; i < INGAME::ACT_MAX; i++)
	{
		if (m_aKeyConfig[i].s_p2D == nullptr) continue;
		if (m_aKeyConfig[i].s_pKeyDisp == nullptr) continue;

		// 選択外は暗く
		D3DXCOLOR setcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		if (i != m_SelectKey)
		{
			setcol = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
		}
		m_aKeyConfig[i].s_p2D->SetColor(setcol);
		m_aKeyConfig[i].s_pKeyDisp->SetColor(setcol);
	}

	// 決定戻る変更中
	D3DXCOLOR setcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	if (m_SelectKey != INGAME::ACT_MAX)
	{
		setcol = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	}
	m_checkconfig.s_p2D->SetColor(setcol);
	m_checkconfig.s_pKeyDispNO->SetColor(setcol);
	m_checkconfig.s_pKeyDispOK->SetColor(setcol);

}

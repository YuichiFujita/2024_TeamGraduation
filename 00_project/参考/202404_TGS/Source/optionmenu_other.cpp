//=============================================================================
// 
//  その他メニュー処理 [optionmenu_other.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "optionmenu_other.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "object2D.h"
#include "selectdraw.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const MyLib::Vector3 BASEPOINT = MyLib::Vector3(850.0f, 200.0f, 0.0f);
	const float DISTANCE_Y = 120.0f;		// 選択肢同士の間隔
	const float MULTIPLY_SELECT = 1.25f;	// 選択時の倍率
	const std::string TEXTURE = "data\\TEXTURE\\number\\number_oradano03.png";	// テクスチャのファイル
	const std::string TEXTURE_TEXT[] =											// テキスト部分のファイル
	{
		"data\\TEXTURE\\option\\windowmode.png",
		"data\\TEXTURE\\option\\vib.png",

	};
	const std::string TEXTURE_ACTIVE[COptionMenu_Other::Select::SELECT_MAX][COptionMenu_Other::Active::MAX] =	// アクティブ部分のファイル
	{
		{// ウィンドウモード
			"data\\TEXTURE\\option\\window_window.png",
			"data\\TEXTURE\\option\\window_full.png",
		},
		{// バイブレーション
			"data\\TEXTURE\\option\\vib_ON.png",
			"data\\TEXTURE\\option\\vib_OFF.png",
		},
	};

	const float HEIGHT = 50.0f;
}

//==========================================================================
// コンストラクタ
//==========================================================================
COptionMenu_Other::COptionMenu_Other(int nPriority) : COptionMenu(nPriority)
{
	m_selectType = Select::SELECT_WINDOW;	// 選択中の種類
	m_bNowChange = false;						// 変更のフラグ
	m_pDrawing = nullptr;					// 選択肢筆

	memset(m_pText, 0, sizeof(m_pText));	// テキスト
	memset(m_switchInfo, 0, sizeof(m_switchInfo));	// 切り替え情報
	m_pDrawing = nullptr;					// 選択肢筆
}

//==========================================================================
// デストラクタ
//==========================================================================
COptionMenu_Other::~COptionMenu_Other()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT COptionMenu_Other::Init()
{
	// 初期化処理
	COptionMenu::Init();

	// 切り替え生成
	CreateSwitch();

	// テキスト生成
	CreateText();

	// 設定位置
	MyLib::Vector3 setpos = BASEPOINT;

	// 選択肢筆
	m_pDrawing = CSelectDraw::Create(BASEPOINT);
	m_pDrawing->SetSizeOrigin(D3DXVECTOR2(200.0f, HEIGHT));
	m_pDrawing->SetSize(m_pDrawing->GetSizeOrigin());

	// 選択肢筆の初期位置設定
	m_pDrawing->SetPosition(setpos);


	// バイブ状況に応じて状況設定
	CInputGamepad* pPad = CInputGamepad::GetInstance();
	m_switchInfo[m_selectType].active = pPad->IsEnableVibration() ? Active::ON : Active::OFF;

	// アクティブ状態別テクスチャ
	ActiveByTexture(Select::SELECT_WINDOW, m_switchInfo[m_selectType].active);
	ActiveByTexture(Select::SELECT_VIB, m_switchInfo[m_selectType].active);


	// 選択肢リセット
	ResetSelect();

	return S_OK;
}

//==========================================================================
// 切り替え生成
//==========================================================================
void COptionMenu_Other::CreateSwitch()
{
	// 設定位置
	MyLib::Vector3 setpos = BASEPOINT;

	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		// アクティブへ変更
		m_switchInfo[i].active = Active::ON;

		// 切り替え生成
		m_switchInfo[i].pSwitch = CObject2D::Create();

		// アドレス渡し
		CObject2D** pObj2D = &m_switchInfo[i].pSwitch;
		
		// 位置設定
		(*pObj2D)->SetPosition(setpos);
		(*pObj2D)->SetOriginPosition(setpos);

		// テクスチャ設定
		int texID = CTexture::GetInstance()->Regist(TEXTURE_TEXT[i]);
		(*pObj2D)->BindTexture(texID);

		// 縦幅を元にサイズ設定
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, HEIGHT);
		(*pObj2D)->SetSize(size);
		(*pObj2D)->SetSizeOrigin(size);

		// 位置ずれ
		setpos.y += DISTANCE_Y;
	}

}

//==========================================================================
// テキスト生成
//==========================================================================
void COptionMenu_Other::CreateText()
{
	// 設定位置
	MyLib::Vector3 setpos = BASEPOINT;
	setpos.x -= 450.0f;

	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		// テキスト生成
		m_pText[i] = CObject2D::Create(GetPriority());

		CObject2D* pObj2D = m_pText[i];
		pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

		// 位置設定
		pObj2D->SetPosition(setpos);
		setpos.y += DISTANCE_Y;


		// テクスチャ設定
		int texID = CTexture::GetInstance()->Regist(TEXTURE_TEXT[i]);
		pObj2D->BindTexture(texID);

		// 縦幅を元にサイズ設定
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, HEIGHT);

		// サイズ設定
		pObj2D->SetSize(size);
		pObj2D->SetSizeOrigin(size);
	}

}

//==========================================================================
// 終了処理
//==========================================================================
void COptionMenu_Other::Uninit()
{
	// 終了処理
	COptionMenu::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void COptionMenu_Other::Kill()
{
	// テキスト
	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		if (m_pText[i] == nullptr) continue;
		m_pText[i]->Uninit();
		m_pText[i] = nullptr;
	}

	// 切り替え
	for (const auto& obj : m_switchInfo)
	{
		if (obj.pSwitch == nullptr) continue;
		obj.pSwitch->Uninit();
	}
	
	// 選択肢筆
	if (m_pDrawing != nullptr)
	{
		m_pDrawing->Uninit();
		m_pDrawing = nullptr;
	}

	// 削除処理
	COptionMenu::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void COptionMenu_Other::Update()
{
	// 更新処理
	COptionMenu::Update();
}

//==========================================================================
// 編集
//==========================================================================
void COptionMenu_Other::StateEdit()
{

	// 入力情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (!m_bNowChange)
	{// 変更時以外選択肢切り替え

		// 切り替え判定
		bool bSwitch = false;

		// 上
		if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y > 0) ||
			pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
			pKey->GetTrigger(DIK_W))
		{
			// パターンNo.を更新
			m_selectType = static_cast<Select>((m_selectType + (Select::SELECT_MAX - 1)) % Select::SELECT_MAX);
			m_pDrawing->SetState(CSelectDraw::State::STATE_DRAWING);
			bSwitch = true;

			// サウンド再生
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
		}

		// 下
		if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y < 0) ||
			pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
			pKey->GetTrigger(DIK_S))
		{
			// パターンNo.を更新
			m_selectType = static_cast<Select>(((int)m_selectType + 1) % Select::SELECT_MAX);
			m_pDrawing->SetState(CSelectDraw::State::STATE_DRAWING);
			bSwitch = true;

			// サウンド再生
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
		}

		if (bSwitch)
		{// 筆の位置更新
			m_pDrawing->SetPosition(m_switchInfo[m_selectType].pSwitch->GetPosition());
		}
	}


	// キーコンフィグ取得
	CKeyConfig* pKeyConfigPad = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKeyConfigKey = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INKEY);

	if (pKeyConfigPad->GetTrigger(INGAME::ACT_OK) ||
		pKeyConfigKey->GetTrigger(INGAME::ACT_OK))
	{
		// 変更のフラグ切り替え
		m_bNowChange = m_bNowChange ? false : true;

		// SE再生
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_DICTION);
	}
	else if (pKeyConfigPad->GetTrigger(INGAME::ACT_BACK) ||
		pKeyConfigKey->GetTrigger(INGAME::ACT_BACK))
	{
		m_bNowChange = false;

		// SE再生
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_DICTION);
	}

	// 変更中のみ切り替え
	if (m_bNowChange)
	{
		if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pPad->GetStickMoveL(0).x > 0) ||
			pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
			pKey->GetTrigger(DIK_D))
		{// 切り替え
			m_switchInfo[m_selectType].active = m_switchInfo[m_selectType].active ? Active::ON : Active::OFF;
			
			// SE再生
			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_SELECT);

			// ウィンドウモード切替
			if (m_selectType == Select::SELECT_WINDOW)
			{
				bool bUse = (m_switchInfo[m_selectType].active == Active::ON) ? true : false;
				CManager::GetInstance()->ChangeDisplayMode(bUse);
			}
		}
		else if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pPad->GetStickMoveL(0).x < 0) ||
			pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
			pKey->GetTrigger(DIK_A))
		{// 切り替え
			m_switchInfo[m_selectType].active = m_switchInfo[m_selectType].active ? Active::ON : Active::OFF;
			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_SELECT);

			// ウィンドウモード切替
			if (m_selectType == Select::SELECT_WINDOW)
			{
				bool bUse = (m_switchInfo[m_selectType].active == Active::ON) ? true : false;
				CManager::GetInstance()->ChangeDisplayMode(bUse);
			}
		}

		// アクティブ状態別テクスチャ
		ActiveByTexture(m_selectType, m_switchInfo[m_selectType].active);
	}

	// バイブの時
	if (m_selectType == Select::SELECT_VIB)
	{
		bool bUse = (m_switchInfo[m_selectType].active == Active::ON) ? true : false;
		pPad->SetEnableVibration(bUse);
	}

	// 選択肢リセット
	ResetSelect();
}

//==========================================================================
// 選択肢リセット
//==========================================================================
void COptionMenu_Other::ResetSelect()
{
	// 選択状況に応じたサイズ変更
	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		D3DXVECTOR2 setsize = m_switchInfo[i].pSwitch->GetSizeOrigin();
		D3DXCOLOR setcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		if (m_bNowChange &&
			i == m_selectType)
		{// 選択中は拡大
			setsize *= 1.15f;
		}

		if (i != m_selectType)
		{// 選択外は暗く
			setcol = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
		}

		m_switchInfo[i].pSwitch->SetSize(setsize);
		m_switchInfo[i].pSwitch->SetColor(setcol);
		m_pText[i]->SetColor(setcol);	// テキスト
	}
}

//==========================================================================
// アクティブ状態別テクスチャ
//==========================================================================
void COptionMenu_Other::ActiveByTexture(Select _select, Active _active)
{
	// アドレス渡し
	CObject2D** pObj2D = &m_switchInfo[_select].pSwitch;

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE_ACTIVE[_select][_active]);
	(*pObj2D)->BindTexture(texID);

	// 縦幅を元にサイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, HEIGHT);
	(*pObj2D)->SetSize(size);
	(*pObj2D)->SetSizeOrigin(size);
}

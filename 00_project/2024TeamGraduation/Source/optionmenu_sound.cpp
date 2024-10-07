//=============================================================================
// 
//  サウンドメニュー処理 [optionmenu_sound.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "optionmenu_sound.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "object2D.h"
#include "multinumber.h"
#include "sound.h"
#include "selectdraw.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const MyLib::Vector3 BASEPOINT = MyLib::Vector3(850.0f, 200.0f, 0.0f);
	const D3DXVECTOR2 SIZE_NUMBER = D3DXVECTOR2(40.0f, 40.0f);
	const float DISTANCE_Y = 120.0f;
	const float MULTIPLY_SELECT = 1.25f;
	const char* TEXTURE = "data\\TEXTURE\\number\\number_oradano03.png";	// テクスチャのファイル
	const char* TEXTURE_TEXT[] =
	{
		"data\\TEXTURE\\option\\mastervolume.png",
		"data\\TEXTURE\\option\\SE.png",
		"data\\TEXTURE\\option\\BGM.png",

	};// テクスチャのファイル
	const float HEIGHT = 50.0f;
}

//==========================================================================
// コンストラクタ
//==========================================================================
COptionMenu_Sound::COptionMenu_Sound(int nPriority) : COptionMenu(nPriority)
{
	m_selectType = Select::SELECT_MASTER;	// 選択中の種類
	m_bNowChange = false;						// 変更のフラグ
	m_pDrawing = nullptr;		// 選択肢筆


	m_pNumber_Master = nullptr;			// マスターボリュームの数字
	m_pNumber_SE = nullptr;				// SEの数字
	m_pNumber_BGM = nullptr;			// BGMの数字
	memset(m_pText, 0, sizeof(m_pText));// テキスト
	m_pDrawing = nullptr;				// 選択肢筆
}

//==========================================================================
// デストラクタ
//==========================================================================
COptionMenu_Sound::~COptionMenu_Sound()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT COptionMenu_Sound::Init()
{
	// 初期化処理
	COptionMenu::Init();

	// テキスト生成
	CreateText();

	// 設定位置
	MyLib::Vector3 setpos = BASEPOINT;

	// マスターボリュームの数字
	m_pNumber_Master = CMultiNumber::Create(
		setpos,
		SIZE_NUMBER,
		3,
		CNumber::EObjectType::OBJECTTYPE_2D,
		TEXTURE, true, 4);
	m_pNumber_Master->SetAlignmentType(CMultiNumber::ALIGNMENT_RIGHT);
	m_pNumber_Master->SetType(CObject::TYPE::TYPE_NUMBER);
	m_pNumber_Master->SetPosition(m_pNumber_Master->GetPosition());

	// SEの数字
	setpos.y += DISTANCE_Y;
	m_pNumber_SE = CMultiNumber::Create(
		setpos,
		SIZE_NUMBER,
		3,
		CNumber::EObjectType::OBJECTTYPE_2D,
		TEXTURE, true, 4);
	m_pNumber_SE->SetAlignmentType(CMultiNumber::ALIGNMENT_RIGHT);
	m_pNumber_SE->SetType(CObject::TYPE::TYPE_NUMBER);
	m_pNumber_SE->SetPosition(m_pNumber_SE->GetPosition());

	// BGMの数字
	setpos.y += DISTANCE_Y;
	m_pNumber_BGM = CMultiNumber::Create(
		setpos,
		SIZE_NUMBER,
		3,
		CNumber::EObjectType::OBJECTTYPE_2D,
		TEXTURE, true, 4);
	m_pNumber_BGM->SetAlignmentType(CMultiNumber::ALIGNMENT_RIGHT);
	m_pNumber_BGM->SetType(CObject::TYPE::TYPE_NUMBER);
	m_pNumber_BGM->SetPosition(m_pNumber_BGM->GetPosition());



	// 選択肢筆
	m_pDrawing = CSelectDraw::Create(BASEPOINT);
	m_pDrawing->SetSizeOrigin(D3DXVECTOR2(200.0f, HEIGHT));
	m_pDrawing->SetSize(m_pDrawing->GetSizeOrigin());


	// サウンド取得
	CSound* pSound = CSound::GetInstance();

	// マスターボリューム
	float masterVolume = pSound->GetVolume();

	// 各ボリューム
	float volumeSE = pSound->GetVolume(CSound::TYPE::TYPE_SE), volumeBGM = pSound->GetVolume(CSound::TYPE::TYPE_BGM);

	// 値の設定
	m_pNumber_Master->SetValue(pSound->GetVolumeNum() / 2);
	m_pNumber_SE->SetValue(pSound->GetVolumeNum(CSound::TYPE::TYPE_SE) / 2);
	m_pNumber_BGM->SetValue(pSound->GetVolumeNum(CSound::TYPE::TYPE_BGM) / 2);



	// 選択肢筆の初期位置設定
	m_pDrawing->SetPosition(m_pNumber_Master->GetPosition());


	// 選択肢リセット
	ResetSelect();

	return S_OK;
}

//==========================================================================
// テキスト生成
//==========================================================================
void COptionMenu_Sound::CreateText()
{
	// 設定位置
	MyLib::Vector3 setpos = BASEPOINT;
	setpos.x -= 400.0f;

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
void COptionMenu_Sound::Uninit()
{
	// 終了処理
	COptionMenu::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void COptionMenu_Sound::Kill()
{
	// マスターボリュームの数字
	if (m_pNumber_Master != nullptr)
	{
		m_pNumber_Master->Kill();
		m_pNumber_Master = nullptr;
	}

	// マスターボリュームの数字
	if (m_pNumber_SE != nullptr)
	{
		m_pNumber_SE->Kill();
		m_pNumber_SE = nullptr;
	}

	// マスターボリュームの数字
	if (m_pNumber_BGM != nullptr)
	{
		m_pNumber_BGM->Kill();
		m_pNumber_BGM = nullptr;
	}

	// テキスト
	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		if (m_pText[i] == nullptr) continue;
		m_pText[i]->Uninit();
		m_pText[i] = nullptr;
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
void COptionMenu_Sound::Update()
{
	// 更新処理
	COptionMenu::Update();
}

//==========================================================================
// 編集
//==========================================================================
void COptionMenu_Sound::StateEdit()
{
	// サウンド取得
	CSound* pSound = CSound::GetInstance();

	// マスターボリューム
	float masterVolume = pSound->GetVolume();

	// 各ボリューム
	float volumeSE = pSound->GetVolume(CSound::TYPE::TYPE_SE), volumeBGM = pSound->GetVolume(CSound::TYPE::TYPE_BGM);


	// 入力情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (!m_bNowChange)
	{
		// サイズリセット
		{
			m_pNumber_Master->SetSize(m_pNumber_Master->GetSizeOrigin());
			m_pNumber_Master->SetKerning(m_pNumber_Master->GetSizeOrigin().y);
			m_pNumber_Master->SetPosition(m_pNumber_Master->GetPosition());

			m_pNumber_SE->SetSize(m_pNumber_SE->GetSizeOrigin());
			m_pNumber_SE->SetKerning(m_pNumber_SE->GetSizeOrigin().y);
			m_pNumber_SE->SetPosition(m_pNumber_SE->GetPosition());

			m_pNumber_BGM->SetSize(m_pNumber_BGM->GetSizeOrigin());
			m_pNumber_BGM->SetKerning(m_pNumber_BGM->GetSizeOrigin().y);
			m_pNumber_BGM->SetPosition(m_pNumber_BGM->GetPosition());
		}

		// 上
		if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y > 0) ||
			pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
			pKey->GetTrigger(DIK_W))
		{
			// パターンNo.を更新
			m_selectType = static_cast<Select>((m_selectType + (Select::SELECT_MAX - 1)) % Select::SELECT_MAX);
			m_pDrawing->SetState(CSelectDraw::State::STATE_DRAWING);

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

			// サウンド再生
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
		}

		// 選択肢別音量設定
		switch (m_selectType)
		{
		case COptionMenu_Sound::SELECT_MASTER:
			m_pDrawing->SetPosition(m_pNumber_Master->GetPosition());
			break;

		case COptionMenu_Sound::SELECT_SE:
			m_pDrawing->SetPosition(m_pNumber_SE->GetPosition());
			break;

		case COptionMenu_Sound::SELECT_BGM:
			m_pDrawing->SetPosition(m_pNumber_BGM->GetPosition());
			break;

		default:
			break;
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


	// 変更中のみ音量変更
	if (m_bNowChange)
	{
		float fVolume = 1.0f;

		m_pNumber_Master->SetSize(m_pNumber_Master->GetSizeOrigin());
		m_pNumber_SE->SetSize(m_pNumber_SE->GetSizeOrigin());
		m_pNumber_BGM->SetSize(m_pNumber_BGM->GetSizeOrigin());

		// 選択肢別音量取得
		switch (m_selectType)
		{
		case COptionMenu_Sound::SELECT_MASTER:
			fVolume = pSound->GetVolume();
			m_pNumber_Master->SetSize(m_pNumber_Master->GetSizeOrigin() * MULTIPLY_SELECT);
			m_pNumber_Master->SetKerning(m_pNumber_Master->GetSize().y);
			m_pNumber_Master->SetPosition(m_pNumber_Master->GetPosition());
			break;

		case COptionMenu_Sound::SELECT_SE:
			fVolume = pSound->GetVolume(CSound::TYPE::TYPE_SE);
			m_pNumber_SE->SetSize(m_pNumber_SE->GetSizeOrigin() * MULTIPLY_SELECT);
			m_pNumber_SE->SetKerning(m_pNumber_SE->GetSize().y);
			m_pNumber_SE->SetPosition(m_pNumber_SE->GetPosition());
			break;

		case COptionMenu_Sound::SELECT_BGM:
			fVolume = pSound->GetVolume(CSound::TYPE::TYPE_BGM);
			m_pNumber_BGM->SetSize(m_pNumber_BGM->GetSizeOrigin() * MULTIPLY_SELECT);
			m_pNumber_BGM->SetKerning(m_pNumber_BGM->GetSize().y);
			m_pNumber_BGM->SetPosition(m_pNumber_BGM->GetPosition());
			break;

		default:
			break;
		}

		if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pPad->GetStickMoveL(0).x > 0) ||
			pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
			pKey->GetTrigger(DIK_D))
		{// 増量

			fVolume += 0.1f;
			pSound->PlaySound(CSound::LABEL_SE_SELECT);
		}
		else if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pPad->GetStickMoveL(0).x < 0) ||
			pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
			pKey->GetTrigger(DIK_A))
		{// 減少

			fVolume -= 0.1f;
			pSound->PlaySound(CSound::LABEL_SE_SELECT);
		}

		// 選択肢別音量設定
		switch (m_selectType)
		{
		case COptionMenu_Sound::SELECT_MASTER:
			pSound->VolumeChange(fVolume);
			break;

		case COptionMenu_Sound::SELECT_SE:
			pSound->VolumeChange(CSound::TYPE::TYPE_SE, fVolume);
			break;

		case COptionMenu_Sound::SELECT_BGM:
			pSound->VolumeChange(CSound::TYPE::TYPE_BGM, fVolume);
			break;

		default:
			break;
		}
	}

	// 値の設定
	m_pNumber_Master->SetValue(pSound->GetVolumeNum() / 2);
	m_pNumber_SE->SetValue(pSound->GetVolumeNum(CSound::TYPE::TYPE_SE) / 2);
	m_pNumber_BGM->SetValue(pSound->GetVolumeNum(CSound::TYPE::TYPE_BGM) / 2);

	// 選択肢リセット
	ResetSelect();
}


//==========================================================================
// 選択肢リセット
//==========================================================================
void COptionMenu_Sound::ResetSelect()
{
	// 選択状況に応じたサイズ変更
	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		D3DXVECTOR2 setsize = m_pNumber_Master->GetSizeOrigin();
		D3DXCOLOR setcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		if (m_bNowChange &&
			i == m_selectType)
		{// 選択中は拡大
			setsize *= MULTIPLY_SELECT;
		}

		if (i != m_selectType)
		{// 選択外は暗く
			setcol = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
		}


		// 選択肢別音量取得
		switch (i)
		{
		case COptionMenu_Sound::SELECT_MASTER:
			m_pNumber_Master->SetSize(setsize);
			m_pNumber_Master->SetColor(setcol);
			break;

		case COptionMenu_Sound::SELECT_SE:
			m_pNumber_SE->SetSize(setsize);
			m_pNumber_SE->SetColor(setcol);
			break;

		case COptionMenu_Sound::SELECT_BGM:
			m_pNumber_BGM->SetSize(setsize);
			m_pNumber_BGM->SetColor(setcol);
			break;

		default:
			break;
		}

		// テキストの色設定
		m_pText[i]->SetColor(setcol);
	}
}

//==========================================================================
// 
//  オプションメニュー処理 [optionMenu.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "optionMenu.h"
#include "object2D.h"
#include "objectX.h"
#include "multinumber.h"
#include "number.h"
#include "manager.h"
#include "camera.h"
#include "instantfade.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_BOARD = "data\\TEXTURE\\title\\board.jpg";	// ボードのテクスチャ
	const std::string TEXTURE_PAPER = "data\\TEXTURE\\title\\paper.png";	// 紙のテクスチャ
	const std::string TEXTURE_SOUND = "data\\TEXTURE\\title\\soundOption.png";	// サウンド見出しのテクスチャ
	const std::string TEXTURE_PAD = "data\\TEXTURE\\title\\controllOption.png";	// コントローラー見出しのテクスチャ
	const std::string TEXTURE_SELECT[COptionMenu::ESelect::SELECT_MAX] =	// 選択肢のテクスチャ
	{
		"data\\TEXTURE\\title\\master.png",		// マスターボリューム
		"data\\TEXTURE\\title\\SE.png"	,		// SE
		"data\\TEXTURE\\title\\BGM.png",		// BGM
		"data\\TEXTURE\\title\\vib.png",		// バイブレーション
		"data\\TEXTURE\\title\\back.png",		// 戻る
		"data\\TEXTURE\\title\\reset.png",	// リセット
	};
	const std::string TEXTURE_NUMBER = "data\\TEXTURE\\number\\school02.png";	// 数字のテクスチャ
	const std::string TEXTURE_PIN = "data\\TEXTURE\\title\\pin.png";		// ピンのテクスチャ
	const std::string TEXTURE_VIB[2] =										// バイブフラグのテクスチャ
	{
		"data\\TEXTURE\\title\\vib_on.png",
		"data\\TEXTURE\\title\\vib_off.png",
	};
	const std::string TEXTURE_DESIGN01 = "data\\TEXTURE\\title\\design01.png";		// デザインのテクスチャ
	const std::string TEXTURE_DESIGN02 = "data\\TEXTURE\\title\\design02.png";		// デザインのテクスチャ
	const std::string TEXTURE_MARKER = "data\\TEXTURE\\title\\selectmarker.png";	// 選択肢のマーカーのテクスチャ

	float SIZEHEIGHT_SELECT[COptionMenu::ESelect::SELECT_MAX] =	// 選択肢の高さサイズ
	{
		45.0f,	// マスターボリューム
		45.0f,	// SE
		45.0f,	// BGM
		45.0f,	// バイブレーション
		55.0f,	// 戻る
		100.0f,	// リセット
	};

	float SIZEHEIGHT_TITLE = 60.0f;	// 見出しの高さサイズ
	const MyLib::Vector3 POSITION_SOUNDTITLE = MyLib::Vector3(635.0f, 94.0f, 0.0f);	// サウンド見出しの高さサイズ
	const MyLib::Vector3 POSITION_PADTITLE = MyLib::Vector3(635.0f, 522.0f, 0.0f);	// コントローラー見出しの高さサイズ
	const MyLib::Vector3 OFFSET_MARKER = MyLib::Vector3(-130.0f, 10.0f, 0.0f);	// マーカーのオフセット

	const MyLib::Vector3 POSITION_SELECT[COptionMenu::ESelect::SELECT_MAX] =	// 選択肢の高さサイズ
	{
		MyLib::Vector3(500.0f, 215.0f, 0.0f),	// マスターボリューム
		MyLib::Vector3(500.0f, 315.0f, 0.0f),	// SE
		MyLib::Vector3(500.0f, 415.0f, 0.0f),	// BGM
		MyLib::Vector3(500.0f, 620.0f, 0.0f),	// バイブレーション
		MyLib::Vector3(150.0f, 653.0f, 0.0f),	// 戻る
		MyLib::Vector3(1100.0f, 615.0f, 0.0f),	// リセット
	};

	const float NUMBER_POSITION_X = 730.0f;		// 数字のX軸
	const float NUMBER_POSITION_Y = 12.0f;		// 数字のY軸ズレ
	const D3DXCOLOR COLOR_NOTSELECT = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);	// 選択してない色
	const float MARKERTIME = 0.2f;	// 選択マーカーの時間
}

namespace StateTime
{
	const float SPAWN = 1.2f;	// 登場
	const float LOOP_ROTATION = 6.0f;	// ループ周期
	const float LOOP = 1.0f;	// ループ周期
	const float START = 1.0f;	// 開始
}


//==========================================================================
// 関数ポインタ
//==========================================================================
COptionMenu::STATE_FUNC COptionMenu::m_StateFunc[] =	// 状態関数
{
	&COptionMenu::StateNone,		// なし
	&COptionMenu::StateSelect,	// 選択
	&COptionMenu::StateEdit,	// 編集
};

//==========================================================================
// コンストラクタ
//==========================================================================
COptionMenu::COptionMenu(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_state			(EState::STATE_NONE),	// 状態
	m_fStateTime	(0.0f),					// 状態タイマー
	m_select		(ESelect::SELECT_MASTERVOLUME),		// 選択肢
	m_bBack			(false),				// 戻るフラグ
	m_fMarkerTime	(0.0f),					// マーカーのタイマー
	m_pBoard		(nullptr),				// ボード
	m_pPaper		(nullptr),				// 紙
	m_pTitleSound	(nullptr),				// サウンドの見出し
	m_pTitlePad		(nullptr),				// コントローラーの見出し
	m_pNumberMaster	(nullptr),				// マスターボリューム
	m_pNumberSE		(nullptr),				// SE
	m_pNumberBGM	(nullptr),				// BGM
	m_pVibFrag		(nullptr),				// バイブフラグ
	m_pPin			(nullptr),				// ピン
	m_pDesign_01	(nullptr),				// デザイン01
	m_pDesign_02	(nullptr),				// デザイン02
	m_pSelectMarker	(nullptr)				// 選択肢のマーカー
{
	memset(m_pSelectUI, 0, sizeof(m_pSelectUI));	// 選択肢のUI
}

//==========================================================================
// デストラクタ
//==========================================================================
COptionMenu::~COptionMenu()
{

}

//==========================================================================
// 生成処理
//==========================================================================
COptionMenu* COptionMenu::Create()
{
	// メモリの確保
	COptionMenu* pLogo = DEBUG_NEW COptionMenu;

	if (pLogo != nullptr)
	{
		// クラスの初期化
		if (FAILED(pLogo->Init()))
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
HRESULT COptionMenu::Init()
{
	// オブジェクトの種類設定
	CObject::SetType(CObject::TYPE::TYPE_NONE);

	// ボード生成
	if (FAILED(CreateBoard()))
	{
		return E_FAIL;
	}

	// 紙生成
	if (FAILED(CreatePaper()))
	{
		return E_FAIL;
	}

	// 見出し生成
	if (FAILED(CreateTitle(&m_pTitleSound, TEXTURE_SOUND, POSITION_SOUNDTITLE)))
	{
		return E_FAIL;
	}
	if (FAILED(CreateTitle(&m_pTitlePad, TEXTURE_PAD, POSITION_PADTITLE)))
	{
		return E_FAIL;
	}

	// 選択肢生成
	if (FAILED(CreateSelect()))
	{
		return E_FAIL;
	}

	// 数字生成
	if (FAILED(CreateNumber()))
	{
		return E_FAIL;
	}

	// バイブフラグ生成
	if (FAILED(CreateVibFrag()))
	{
		return E_FAIL;
	}

	// ピン生成
	if (FAILED(CreatePin()))
	{
		return E_FAIL;
	}

	// デザイン生成
	if (FAILED(CreateDesign(&m_pDesign_01, TEXTURE_DESIGN01, MyLib::Vector3(1000.0f, 330.0f, 0.0f), MyLib::Vector3(0.0f, 0.0f, -D3DX_PI * 0.05f))))
	{
		return E_FAIL;
	}
	if (FAILED(CreateDesign(&m_pDesign_02, TEXTURE_DESIGN02, MyLib::Vector3(200.0f, 400.0f, 0.0f), MyLib::Vector3(0.0f, 0.0f, D3DX_PI * 0.08f))))
	{
		return E_FAIL;
	}

	// マーカー生成
	if (FAILED(CreateSelectMarker()))
	{
		return E_FAIL;
	}

	// 選択肢更新
	UpdateSelect(0.0f, 0.0f, 0.0f);

	// 色更新
	m_fMarkerTime = MARKERTIME;
	UpdateColor(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//==========================================================================
// ボード生成
//==========================================================================
HRESULT COptionMenu::CreateBoard()
{
	// 生成処理
	m_pBoard = CObject2D::Create(GetPriority());
	if (m_pBoard == nullptr) return E_FAIL;

	// オブジェクトの種類設定
	m_pBoard->CObject::SetType(CObject::TYPE::TYPE_NONE);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_BOARD);
	m_pBoard->BindTexture(texID);

	// サイズ設定
	m_pBoard->SetSize(MyLib::Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
	m_pBoard->SetSizeOrigin(m_pBoard->GetSize());

	// 位置設定
	m_pBoard->SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	return S_OK;
}

//==========================================================================
// 紙生成
//==========================================================================
HRESULT COptionMenu::CreatePaper()
{
	// 生成処理
	m_pPaper = CObject2D::Create(GetPriority());
	if (m_pPaper == nullptr) return E_FAIL;

	// オブジェクトの種類設定
	m_pPaper->CObject::SetType(CObject::TYPE::TYPE_NONE);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_PAPER);
	m_pPaper->BindTexture(texID);

	// 画像サイズ取得
	MyLib::Vector2 size = pTexture->GetImageSize(texID);

	// 縦幅を元にサイズ計算
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SCREEN_HEIGHT * 0.5f);
	m_pPaper->SetSize(size);
	m_pPaper->SetSizeOrigin(size);

	// 位置設定
	m_pPaper->SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	return S_OK;
}

//==========================================================================
// 見出し生成
//==========================================================================
HRESULT COptionMenu::CreateTitle(CObject2D** ppObj2D, const std::string& texture, const MyLib::Vector3& pos)
{
	// 生成処理
	(*ppObj2D) = CObject2D::Create(GetPriority());
	if ((*ppObj2D) == nullptr) return E_FAIL;

	// オブジェクトの種類設定
	(*ppObj2D)->CObject::SetType(CObject::TYPE::TYPE_NONE);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(texture);
	(*ppObj2D)->BindTexture(texID);


	// 画像サイズ取得
	MyLib::Vector2 size = pTexture->GetImageSize(texID);

	// 縦幅を元にサイズ計算
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_TITLE);
	(*ppObj2D)->SetSize(size);
	(*ppObj2D)->SetSizeOrigin(size);

	// 位置設定
	(*ppObj2D)->SetPosition(pos);
	return S_OK;
}

//==========================================================================
// 選択肢生成
//==========================================================================
HRESULT COptionMenu::CreateSelect()
{
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		// 生成処理
		m_pSelectUI[i] = CObject2D::Create(GetPriority());
		if (m_pSelectUI[i] == nullptr) return E_FAIL;

		// オブジェクトの種類設定
		m_pSelectUI[i]->CObject::SetType(CObject::TYPE::TYPE_NONE);

		// テクスチャ設定
		CTexture* pTexture = CTexture::GetInstance();
		int texID = CTexture::GetInstance()->Regist(TEXTURE_SELECT[i]);
		m_pSelectUI[i]->BindTexture(texID);


		// 画像サイズ取得
		MyLib::Vector2 size = pTexture->GetImageSize(texID);

		// 縦幅を元にサイズ計算
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_SELECT[i]);
		m_pSelectUI[i]->SetSize(MyLib::Vector2(size.x, size.y));
		m_pSelectUI[i]->SetSizeOrigin(m_pSelectUI[i]->GetSize());

		// 位置設定
		m_pSelectUI[i]->SetPosition(POSITION_SELECT[i]);
	}

	return S_OK;
}

//==========================================================================
// 数字生成
//==========================================================================
HRESULT COptionMenu::CreateNumber()
{
	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_NUMBER);

	// 画像サイズの縦幅を元にサイズ計算
	MyLib::Vector2 size = pTexture->GetImageSize(texID);
	size.x *= 0.1f;
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_SELECT[ESelect::SELECT_MASTERVOLUME] * 0.8f);

	std::vector<CNumber*> vecNumber;	// 数字のオブジェクト

	//--------------------------
	// マスターボリューム
	//--------------------------
	// 選択肢の一時ポインタ
	CObject2D* pSelect = m_pSelectUI[ESelect::SELECT_MASTERVOLUME];

	// 生成
	m_pNumberMaster = CMultiNumber::Create(
		50,
		texID,
		MyLib::Vector3(NUMBER_POSITION_X, pSelect->GetPosition().y + NUMBER_POSITION_Y, 0.0f),
		size,
		3,
		CMultiNumber::EObjType::OBJTYPE_2D,
		true,
		GetPriority());

	// オブジェクトの種類設定
	m_pNumberMaster->SetType(CObject::TYPE::TYPE_NONE);

	// 数字のオブジェクト取得
	vecNumber = m_pNumberMaster->GetNumberObj();

	for (const auto& num : vecNumber)
	{
		// オブジェクトの種類設定
		num->SetType(CObject::TYPE::TYPE_NONE);
	}


	//--------------------------
	// SE
	//--------------------------
	// 選択肢の一時ポインタ
	pSelect = m_pSelectUI[ESelect::SELECT_SE];

	// 生成
	m_pNumberSE = CMultiNumber::Create(
		50,
		texID,
		MyLib::Vector3(NUMBER_POSITION_X, pSelect->GetPosition().y + NUMBER_POSITION_Y, 0.0f),
		size,
		3,
		CMultiNumber::EObjType::OBJTYPE_2D,
		true,
		GetPriority());

	// オブジェクトの種類設定
	m_pNumberSE->SetType(CObject::TYPE::TYPE_NONE);

	// 数字のオブジェクト取得
	vecNumber = m_pNumberSE->GetNumberObj();

	for (const auto& num : vecNumber)
	{
		// オブジェクトの種類設定
		num->SetType(CObject::TYPE::TYPE_NONE);
	}


	//--------------------------
	// BGM
	//--------------------------
	// 選択肢の一時ポインタ
	pSelect = m_pSelectUI[ESelect::SELECT_BGM];

	// 生成
	m_pNumberBGM = CMultiNumber::Create(
		50,
		texID,
		MyLib::Vector3(NUMBER_POSITION_X, pSelect->GetPosition().y + NUMBER_POSITION_Y, 0.0f),
		size,
		3,
		CMultiNumber::EObjType::OBJTYPE_2D,
		true,
		GetPriority());

	// オブジェクトの種類設定
	m_pNumberBGM->SetType(CObject::TYPE::TYPE_NONE);

	// 数字のオブジェクト取得
	vecNumber = m_pNumberBGM->GetNumberObj();

	for (const auto& num : vecNumber)
	{
		// オブジェクトの種類設定
		num->SetType(CObject::TYPE::TYPE_NONE);
	}

	return S_OK;
}

//==========================================================================
// ピン生成
//==========================================================================
HRESULT COptionMenu::CreatePin()
{
	// 生成処理
	m_pPin = CObject2D::Create(GetPriority());
	if (m_pPin == nullptr) return E_FAIL;

	// オブジェクトの種類設定
	m_pPin->CObject::SetType(CObject::TYPE::TYPE_NONE);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_PIN);
	m_pPin->BindTexture(texID);


	// 画像サイズ取得
	MyLib::Vector2 size = pTexture->GetImageSize(texID);

	// 縦幅を元にサイズ計算
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, 80.0f);
	m_pPin->SetSize(size);
	m_pPin->SetSizeOrigin(size);

	// 位置設定
	m_pPin->SetPosition(MyLib::Vector3(1150.0f, 130.0f, 0.0f));
	return S_OK;
}

//==========================================================================
// バイブフラグ生成
//==========================================================================
HRESULT COptionMenu::CreateVibFrag()
{
	// 生成処理
	m_pVibFrag = CObject2D::Create(GetPriority());
	if (m_pVibFrag == nullptr) return E_FAIL;

	// オブジェクトの種類設定
	m_pVibFrag->CObject::SetType(CObject::TYPE::TYPE_NONE);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_VIB[0]);
	m_pVibFrag->BindTexture(texID);


	// 画像サイズ取得
	MyLib::Vector2 size = pTexture->GetImageSize(texID);

	// 縦幅を元にサイズ計算
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_SELECT[ESelect::SELECT_MASTERVOLUME] * 0.8f);
	m_pVibFrag->SetSize(size);
	m_pVibFrag->SetSizeOrigin(size);

	// 位置設定
	m_pVibFrag->SetPosition(MyLib::Vector3(
		NUMBER_POSITION_X,
		m_pSelectUI[ESelect::SELECT_VIB]->GetPosition().y + NUMBER_POSITION_Y,
		0.0f));
	return S_OK;
}

//==========================================================================
// デザイン生成
//==========================================================================
HRESULT COptionMenu::CreateDesign(CObject2D** ppObj2D, const std::string& texture, const MyLib::Vector3& pos, const MyLib::Vector3& rot)
{
	// 生成処理
	(*ppObj2D) = CObject2D::Create(GetPriority());
	if ((*ppObj2D) == nullptr) return E_FAIL;

	// オブジェクトの種類設定
	(*ppObj2D)->CObject::SetType(CObject::TYPE::TYPE_NONE);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(texture);
	(*ppObj2D)->BindTexture(texID);


	// 画像サイズ取得
	MyLib::Vector2 size = pTexture->GetImageSize(texID);

	// 縦幅を元にサイズ計算
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, 200.0f);
	(*ppObj2D)->SetSize(size);
	(*ppObj2D)->SetSizeOrigin(size);

	// 位置設定
	(*ppObj2D)->SetPosition(pos);

	// 向き設定
	(*ppObj2D)->SetRotation(rot);
	return S_OK;
}

//==========================================================================
// 選択肢のマーカー生成
//==========================================================================
HRESULT COptionMenu::CreateSelectMarker()
{
	// 生成処理
	m_pSelectMarker = CObject2D::Create(GetPriority());
	if (m_pSelectMarker == nullptr) return E_FAIL;

	// オブジェクトの種類設定
	m_pSelectMarker->CObject::SetType(CObject::TYPE::TYPE_NONE);

	// アンカーポイント設定
	m_pSelectMarker->SetAnchorType(CObject2D::AnchorPoint::LEFT);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_MARKER);
	m_pSelectMarker->BindTexture(texID);


	// 画像サイズ取得
	MyLib::Vector2 size = pTexture->GetImageSize(texID);

	// 縦幅を元にサイズ計算
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_SELECT[ESelect::SELECT_MASTERVOLUME] * 1.5f);
	m_pSelectMarker->SetSize(MyLib::Vector2(0.0f, size.y));
	m_pSelectMarker->SetSizeOrigin(size);

	// 位置設定
	m_pSelectMarker->SetPosition(m_pSelectUI[0]->GetPosition());
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void COptionMenu::Uninit()
{
	// ボード削除
	SAFE_UNINIT(m_pBoard);

	// 紙
	SAFE_UNINIT(m_pPaper);

	// マーカー
	SAFE_UNINIT(m_pSelectMarker);

	// 見出し
	SAFE_UNINIT(m_pTitleSound);
	SAFE_UNINIT(m_pTitlePad);

	// 選択肢UI削除
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		SAFE_UNINIT(m_pSelectUI[i]);
	}

	// 数字削除
	SAFE_KILL(m_pNumberMaster);
	SAFE_KILL(m_pNumberSE);
	SAFE_KILL(m_pNumberBGM);

	// ピン
	SAFE_UNINIT(m_pPin);

	// デザイン
	SAFE_UNINIT(m_pDesign_01);
	SAFE_UNINIT(m_pDesign_02);

	// バイブのフラグ
	SAFE_UNINIT(m_pVibFrag);

	// 選択肢のマーカー
	SAFE_UNINIT(m_pSelectMarker);

	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void COptionMenu::Kill()
{
	// ボード削除
	SAFE_KILL(m_pBoard);

	// 紙
	SAFE_KILL(m_pPaper);

	// マーカー
	SAFE_KILL(m_pSelectMarker);

	// 見出し
	SAFE_KILL(m_pTitleSound);
	SAFE_KILL(m_pTitlePad);

	// 選択肢UI削除
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		SAFE_KILL(m_pSelectUI[i]);
	}

	// 数字削除
	SAFE_KILL(m_pNumberMaster);
	SAFE_KILL(m_pNumberSE);
	SAFE_KILL(m_pNumberBGM);

	// ピン
	SAFE_KILL(m_pPin);

	// デザイン
	SAFE_KILL(m_pDesign_01);
	SAFE_KILL(m_pDesign_02);

	// バイブのフラグ
	SAFE_KILL(m_pVibFrag);

	// 選択肢のマーカー
	SAFE_KILL(m_pSelectMarker);

	// 自身の終了
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void COptionMenu::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// マーカー更新
	UpdateMarker(fDeltaTime, fDeltaRate, fSlowRate);

	// オブジェクト更新
	UpdateObj(fDeltaTime, fDeltaRate, fSlowRate);

#if _DEBUG
	// デバッグ処理
	Debug();
#endif
}

//==========================================================================
// 選択肢更新
//==========================================================================
void COptionMenu::UpdateSelect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// パッド情報
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// キーボード情報

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
		pKey->GetTrigger(DIK_W))
	{// 上(逆ループ)
		int nSelect = (m_select + (ESelect::SELECT_BACK - 1)) % ESelect::SELECT_BACK;
		m_select = (ESelect)nSelect;
		m_oldSelect = m_select;

		// マーカータイマーリセット
		m_fMarkerTime = 0.0f;

		// 書き音
		WriteSound();
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
		pKey->GetTrigger(DIK_S))
	{// 下(ループ)
		int nSelect = (m_select + 1) % ESelect::SELECT_BACK;
		m_select = (ESelect)nSelect;
		m_oldSelect = m_select;

		// マーカータイマーリセット
		m_fMarkerTime = 0.0f;

		// 書き音
		WriteSound();
	}

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		pKey->GetTrigger(DIK_A))
	{// 左

		// リセットなら過去へ、それ以外は戻るへ
		ESelect old = m_select;
		m_select = (m_select == ESelect::SELECT_RESET) ? m_oldSelect : ESelect::SELECT_BACK;

		if (old != m_select)
		{
			m_oldSelect = old;

			// マーカータイマーリセット
			m_fMarkerTime = 0.0f;
		}
	}
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		pKey->GetTrigger(DIK_D))
	{// 右

		// 戻るなら過去へ、それ以外はリセットへ
		ESelect old = m_select;
		m_select = (m_select == ESelect::SELECT_BACK) ? m_oldSelect : ESelect::SELECT_RESET;
		
		if (old != m_select)
		{
			m_oldSelect = old;

			// マーカータイマーリセット
			m_fMarkerTime = 0.0f;
		}
	}

	// マーカー
	m_pSelectMarker->SetPosition(OFFSET_MARKER + m_pSelectUI[m_select]->GetPosition());

	// 戻るフラグリセット
	m_bBack = false;

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pKey->GetTrigger(DIK_RETURN))
	{// 決定

		// 戻るフラグ設定
		m_bBack = m_select == ESelect::SELECT_BACK;

		switch (m_select)
		{
		case COptionMenu::SELECT_MASTERVOLUME:
		case COptionMenu::SELECT_SE:
		case COptionMenu::SELECT_BGM:
		case COptionMenu::SELECT_VIB:

			// 編集状態設定
			SetState(EState::STATE_EDIT);
			break;

		case COptionMenu::SELECT_BACK:

			// サウンドの再生
			PLAY_SOUND(CSound::ELabel::LABEL_SE_OPTION_BACK);
			break;

		case COptionMenu::SELECT_RESET:

			// リセット
			Reset();		
			break;

		default:
			assert(false);
			break;
		}
	}

}

//==========================================================================
// マーカー更新
//==========================================================================
void COptionMenu::UpdateMarker(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// マーカーのタイマー加算
	m_fMarkerTime += fDeltaTime * fSlowRate;

	// サイズ取得
	MyLib::Vector2 size = m_pSelectMarker->GetSize();

	// サイズ設定
	size.x = UtilFunc::Correction::EasingEaseOut(0.0f, m_pSelectMarker->GetSizeOrigin().x, 0.0f, MARKERTIME, m_fMarkerTime);
	m_pSelectMarker->SetSize(size);

	// UV設定
	std::vector<D3DXVECTOR2> uv = m_pSelectMarker->GetVecTexUV();
	uv[1].x = uv[3].x = size.x / m_pSelectMarker->GetSizeOrigin().x;
	m_pSelectMarker->SetTexUV(uv);
}

//==========================================================================
// サウンド更新
//==========================================================================
void COptionMenu::UpdateSound(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CMultiNumber* pMultiNum = nullptr;
	switch (m_select)
	{
	case COptionMenu::SELECT_MASTERVOLUME:
		pMultiNum = m_pNumberMaster;
		break;

	case COptionMenu::SELECT_SE:
		pMultiNum = m_pNumberSE;
		break;

	case COptionMenu::SELECT_BGM:
		pMultiNum = m_pNumberBGM;
		break;
	}
	if (pMultiNum == nullptr) return;

	// 入力情報取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// パッド情報
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// キーボード情報

	// 値取得
	int num = pMultiNum->GetNum();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		pKey->GetTrigger(DIK_A))
	{// 左, 減少

		// 値設定
		pMultiNum->SetNum(UtilFunc::Transformation::Clamp(num - 5, 0, 100));

		// サウンドの再生
		PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW_DOWN);
	}
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		pKey->GetTrigger(DIK_D))
	{// 右, 増加

		// 値設定
		pMultiNum->SetNum(UtilFunc::Transformation::Clamp(num + 5, 0, 100));

		// サウンドの再生
		PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW_UP);
	}
	num = pMultiNum->GetNum();

	// 変更した値を反映
	CSound* pSound = CSound::GetInstance();
	float volume = ((float)num / 100.0f) * 2.0f;
	switch (m_select)
	{
	case COptionMenu::SELECT_MASTERVOLUME:
		pSound->VolumeChange(volume);
		break;

	case COptionMenu::SELECT_SE:
		pSound->VolumeChange(CSound::EType::TYPE_SE, volume);
		break;

	case COptionMenu::SELECT_BGM:
		pSound->VolumeChange(CSound::EType::TYPE_BGM, volume);
		break;
	}
}

//==========================================================================
// バイブフラグ更新
//==========================================================================
void COptionMenu::UpdateVib(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 入力情報取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// パッド情報
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// キーボード情報

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		pKey->GetTrigger(DIK_A) ||
		pKey->GetTrigger(DIK_D))
	{// 左右, 切り替え

		// バイブフラグ
		bool bVib = pPad->IsEnableVibration();

		// 反転
		pPad->SetEnableVibration(!bVib);

		// 振動させる
		if (!bVib)
		{
			pPad->SetVibration(CInputGamepad::EVibType::VIBTYPE_THROW_FAST, 0);
		}

		// サウンドの再生
		PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW);
	}
	
	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_VIB[!pPad->IsEnableVibration()]);
	m_pVibFrag->BindTexture(texID);
}

//==========================================================================
// 色更新
//==========================================================================
void COptionMenu::UpdateColor(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	float color = UtilFunc::Correction::EasingLinear(COLOR_NOTSELECT.r, 1.0f, 0.0f, MARKERTIME, m_fMarkerTime);
	D3DXCOLOR selectColor = D3DXCOLOR(color, color, color, 1.0f);

	// 選択肢のUI
	for (const auto& select : m_pSelectUI)
	{
		select->SetColor(COLOR_NOTSELECT);
	}
	m_pSelectUI[m_select]->SetColor(selectColor);

	// マーカー
	m_pSelectMarker->SetColor(selectColor);

	// バイブフラグ
	m_pVibFrag->SetColor(COLOR_NOTSELECT);

	// 数字
	m_pNumberMaster->SetColor(COLOR_NOTSELECT);
	m_pNumberSE->SetColor(COLOR_NOTSELECT);
	m_pNumberBGM->SetColor(COLOR_NOTSELECT);

	switch (m_select)
	{
	case COptionMenu::SELECT_MASTERVOLUME:
		m_pNumberMaster->SetColor(selectColor);
		break;

	case COptionMenu::SELECT_SE:
		m_pNumberSE->SetColor(selectColor);
		break;

	case COptionMenu::SELECT_BGM:
		m_pNumberBGM->SetColor(selectColor);
		break;

	case ESelect::SELECT_VIB:
		m_pVibFrag->SetColor(selectColor);
		break;

	default:
		m_pSelectMarker->SetColor(D3DXCOLOR(MyLib::color::White(0.0f)));
		break;
	}
}

//==========================================================================
// オブジェクト更新
//==========================================================================
void COptionMenu::UpdateObj(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ボードの更新
	assert(m_pBoard != nullptr);
	m_pBoard->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 紙
	m_pPaper->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// マーカー
	m_pSelectMarker->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 見出し
	m_pTitleSound->Update(fDeltaTime, fDeltaRate, fSlowRate);
	m_pTitlePad->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 選択肢のUI
	for (const auto& select : m_pSelectUI)
	{
		assert(select != nullptr);
		select->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 数字
	m_pNumberMaster->Update(fDeltaTime, fDeltaRate, fSlowRate);
	m_pNumberSE->Update(fDeltaTime, fDeltaRate, fSlowRate);
	m_pNumberBGM->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// バイブのフラグ
	m_pVibFrag->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ピン
	m_pPin->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// デザイン
	m_pDesign_01->Update(fDeltaTime, fDeltaRate, fSlowRate);
	m_pDesign_02->Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 状態更新
//==========================================================================
void COptionMenu::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイマー更新
	m_fStateTime += fDeltaTime * fDeltaRate * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 選択
//==========================================================================
void COptionMenu::StateSelect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 選択肢更新
	UpdateSelect(fDeltaTime, fDeltaRate, fSlowRate);

	// 色更新
	UpdateColor(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 編集
//==========================================================================
void COptionMenu::StateEdit(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 入力情報取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// パッド情報
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// キーボード情報

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, 0) ||
		pKey->GetTrigger(DIK_BACKSPACE))
	{// キャンセル

		// 編集状態設定
		SetState(EState::STATE_SELECT);
		return;
	}

	// サウンド更新
	switch (m_select)
	{
	case COptionMenu::SELECT_MASTERVOLUME:
	case COptionMenu::SELECT_SE:
	case COptionMenu::SELECT_BGM:
		UpdateSound(fDeltaTime, fDeltaRate, fSlowRate);
		break;

	case COptionMenu::SELECT_VIB:
		UpdateVib(fDeltaTime, fDeltaRate, fSlowRate);
		break;

	default:
		break;
	}

	// 色更新
	UpdateColor(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 書き音
//==========================================================================
void COptionMenu::WriteSound()
{
	if (m_select == ESelect::SELECT_BACK) return;

	// サウンドの再生
	int soundLabel = UtilFunc::Transformation::Random(CSound::ELabel::LABEL_SE_WRITE01, CSound::ELabel::LABEL_SE_WRITE02);
	PLAY_SOUND((CSound::ELabel)soundLabel);
}

//==========================================================================
// リセット
//==========================================================================
void COptionMenu::Reset()
{
	// 入力情報取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// パッド情報

	// バイブON
	pPad->SetEnableVibration(true);
	pPad->SetVibration(CInputGamepad::EVibType::VIBTYPE_THROW_FAST, 0);

	// 表示切替
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_VIB[!pPad->IsEnableVibration()]);
	m_pVibFrag->BindTexture(texID);

	// 数字リセット
	m_pNumberMaster->SetNum(50);
	m_pNumberSE->SetNum(50);
	m_pNumberBGM->SetNum(50);

	// 1.0fにする
	CSound* pSound = CSound::GetInstance();
	pSound->VolumeChange(1.0f);
	pSound->VolumeChange(CSound::EType::TYPE_SE, 1.0f);
	pSound->VolumeChange(CSound::EType::TYPE_BGM, 1.0f);
}

//==========================================================================
// 描画処理
//==========================================================================
void COptionMenu::Draw()
{
	// ボードの描画
	assert(m_pBoard != nullptr);
	m_pBoard->Draw();

	// 紙
	m_pPaper->Draw();

	// マーカー
	m_pSelectMarker->Draw();

	// 見出し描画
	m_pTitleSound->Draw();
	m_pTitlePad->Draw();

	// 選択肢のUI
	for (const auto& select : m_pSelectUI)
	{
		assert(select != nullptr);
		select->Draw();
	}

	// 数字
	m_pNumberMaster->Draw();
	m_pNumberSE->Draw();
	m_pNumberBGM->Draw();

	// バイブのフラグ
	m_pVibFrag->Draw();

	// ピン
	m_pPin->Draw();

	// デザイン
	m_pDesign_01->Draw();
	m_pDesign_02->Draw();
}

//==========================================================================
// デバッグ処理
//==========================================================================
void COptionMenu::Debug()
{
	if (ImGui::TreeNode("OptionMenu"))
	{
		// 選択肢のUI
		for (int i = 0; i < ESelect::SELECT_MAX; i++)
		{
			if (ImGui::TreeNode(magic_enum::enum_name((ESelect)i).data()))
			{
				//--------------------------
				// サイズ設定
				//--------------------------
				// 画像サイズ取得
				CTexture* pTexture = CTexture::GetInstance();
				MyLib::Vector2 size = pTexture->GetImageSize(m_pSelectUI[i]->GetIdxTexture());

				// 縦幅を元にサイズ計算
				std::string str = "SIZEHEIGHT_SELECT[" + std::to_string(i) + "]";
				ImGui::DragFloat(str.c_str(), &SIZEHEIGHT_SELECT[i], 0.1f, 0.0f, 0.0f, "%.2f");

				size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_SELECT[i]);
				m_pSelectUI[i]->SetSize(MyLib::Vector2(size.x, size.y));

				//--------------------------
				// 位置設定
				//--------------------------
				str = "SIZEHEIGHT_SELECT[" + std::to_string(i) + "]";
				ImGui::DragFloat3(str.c_str(), (float*)&POSITION_SELECT[i], 1.0f, 0.0f, 0.0f, "%.2f");
				m_pSelectUI[i]->SetPosition(POSITION_SELECT[i]);


				//--------------------------
				// 色
				//--------------------------
				str = "Color[" + std::to_string(i) + "]";

				D3DXCOLOR col = m_pSelectUI[i]->GetColor();
				ImVec4 editCol = ImVec4(col.r, col.g, col.b, col.a);
				ImGui::ColorEdit4(str.c_str(), (float*)&editCol);
				m_pSelectUI[i]->SetColor(D3DXCOLOR(editCol.x, editCol.y, editCol.z, editCol.w));

				ImGui::TreePop();
			}
		}

		// 見出し
		if (ImGui::TreeNode("itle"))
		{
			//--------------------------
			// サイズ設定
			//--------------------------
			// 画像サイズ取得
			CTexture* pTexture = CTexture::GetInstance();
			MyLib::Vector2 size = pTexture->GetImageSize(m_pTitleSound->GetIdxTexture());

			// 縦幅を元にサイズ計算
			ImGui::DragFloat("SIZEHEIGHT_SOUNDTITLE]", &SIZEHEIGHT_TITLE, 0.1f, 0.0f, 0.0f, "%.2f");
			size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_TITLE);
			m_pTitleSound->SetSize(size);
			m_pTitlePad->SetSize(size);


			if (ImGui::TreeNode("SoundTitle"))
			{
				//--------------------------
				// 位置設定
				//--------------------------
				ImGui::DragFloat3("pos", (float*)&POSITION_SOUNDTITLE, 1.0f, 0.0f, 0.0f, "%.2f");
				m_pTitleSound->SetPosition(POSITION_SOUNDTITLE);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("PadTitle"))
			{
				//--------------------------
				// 位置設定
				//--------------------------
				ImGui::DragFloat3("pos", (float*)&POSITION_PADTITLE, 1.0f, 0.0f, 0.0f, "%.2f");
				m_pTitlePad->SetPosition(POSITION_PADTITLE);

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}


		ImGui::TreePop();
	}

	
}

//==========================================================================
// 状態設定
//==========================================================================
void COptionMenu::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 選択肢設定
//==========================================================================
void COptionMenu::SetSelect(ESelect select)
{
	m_oldSelect = m_select;
	m_select = select;
}

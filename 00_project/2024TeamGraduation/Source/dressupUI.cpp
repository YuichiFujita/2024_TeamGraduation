//============================================================
//
//	ドレスアップUI処理 [dressupUI.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "dressupUI.h"
#include "dressup.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "entry_setupTeam.h"
#include "object2D.h"
#include "object2D_Anim.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// ドレスアップUIの優先順位
	const D3DXCOLOR COL_CHOICE	= MyLib::color::White();				// 選択中色
	const D3DXCOLOR COL_DEFAULT	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// 非選択中色

	namespace change
	{
		const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(CDressupUI::EChangeType::TYPE_MAX, 1);	// テクスチャ分割数
		const std::string TEXTURE = "data\\TEXTURE\\entry\\ChangeType000.png";	// 変更種類アイコンテクスチャ
		const float WIDTH = 60.0f;	// 横幅
	}

	namespace frame
	{
		namespace player
		{
			const std::string TEXTURE = "data\\TEXTURE\\entry\\PlayerFrame000.png";	// 変更種類アイコンテクスチャ
			const float WIDTH = 100.0f;	// 横幅
		}
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
int CDressupUI::m_nNumAI = 0;	// AI総数

//************************************************************
//	子クラス [CDressupUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CDressupUI::CDressupUI(const int nPlayerIdx) : CObject(PRIORITY, LAYER::LAYER_2D),
	m_pChangeIcon	(nullptr),		// 変更種類アイコン情報
	m_nPlayerIdx	(nPlayerIdx),	// プレイヤーインデックス
	m_nOrdinalAI	(-1),			// 自身が生成された順番 (AIのみ)
	m_pPlayer		(nullptr),		// プレイヤー
	m_pHair			(nullptr),		// 髪着せ替え
	m_pAccessory	(nullptr),		// アクセ着せ替え
	m_pFace			(nullptr),		// 顔着せ替え
	m_typeEdit		(EEditType::EDIT_PROCESS),	// エディットする種類
	m_typeChange	(EChangeType::TYPE_HAIR)	// 変更する種類
{

}

//============================================================
//	デストラクタ
//============================================================
CDressupUI::~CDressupUI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CDressupUI::Init()
{
	// セットアップの生成
	if (FAILED(CreateSetup()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// UIの生成
	if (FAILED(CreateUI()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CDressupUI::Uninit()
{
	// チームセットアップ情報の取得
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	assert(pSetupTeam != nullptr);

	// 破棄したプレイヤーがAIの場合はAI総数を減算
	if (pSetupTeam->GetEntryIdx(m_nPlayerIdx) <= -1) { m_nNumAI--; }

	// 髪着せ替えの終了
	SAFE_UNINIT(m_pHair);

	// アクセ切り替えの終了
	SAFE_UNINIT(m_pAccessory);

	// 顔切り替えの終了
	SAFE_UNINIT(m_pFace);

	// オブジェクトを破棄
	Release();
}

//============================================================
//	削除処理
//============================================================
void CDressupUI::Kill()
{
	// 自身の終了
	CDressupUI::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CDressupUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// チームセットアップ情報の取得
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) { return; }

	// エントリーインデックスを取得
	const int nEntryIdx = pSetupTeam->GetEntryIdx(m_nPlayerIdx);
	if (nEntryIdx < 0) { return; }
	if (nEntryIdx >= mylib_const::MAX_PLAYER) { return; }

	// エディットする種類変更
	ChangeEditType(nEntryIdx);

	switch (m_typeEdit)
	{
	case EEditType::EDIT_PROCESS:

		switch (m_typeChange)
		{
		case EChangeType::TYPE_HAIR:

			// 髪更新
			m_pHair->Update(fDeltaTime, fDeltaRate, fSlowRate);
			break;

		case EChangeType::TYPE_ACCESSORY:

			// アクセ更新
			m_pAccessory->Update(fDeltaTime, fDeltaRate, fSlowRate);
			break;

		case EChangeType::TYPE_FACE:

			// 顔更新
			m_pFace->Update(fDeltaTime, fDeltaRate, fSlowRate);
			break;

		case EChangeType::TYPE_BODY:

			// 体型変更
			ChangeBodyType(nEntryIdx);
			break;

		case EChangeType::TYPE_HANDEDNESS:

			// 利き手変更
			ChangeHandedness(nEntryIdx);
			break;

		default:
			assert(false);
			break;
		}
		break;

	case EEditType::EDIT_CHANGETYPE:

		// 変更する箇所の種類変更
		ChangeChangeType(nEntryIdx);
		break;

	default:
		assert(false);
		break;
	}

	// UIの更新
	UpdateUI();

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	描画処理
//============================================================
void CDressupUI::Draw()
{

}

//============================================================
//	種類の設定処理
//============================================================
void CDressupUI::SetType(const TYPE type)
{
	// 自身の種類を設定
	CObject::SetType(type);
}

//============================================================
//	描画状況の設定処理
//============================================================
void CDressupUI::SetEnableDisp(const bool bDraw)
{
	// 自身の描画状況を設定
	CObject::SetEnableDisp(bDraw);
}

//============================================================
//	位置の設定処理
//============================================================
void CDressupUI::SetPosition(const MyLib::Vector3& pos)
{
	// 設定する位置を保存
	CObject::SetPosition(pos);

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	生成処理
//============================================================
CDressupUI *CDressupUI::Create
(
	const int nPlayerIdx,		// プレイヤーインデックス
	const MyLib::Vector3 &rPos	// 原点位置
)
{
	// ドレスアップUIの生成
	CDressupUI* pDressupUI = DEBUG_NEW CDressupUI(nPlayerIdx);
	if (pDressupUI == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ドレスアップUIの初期化
		if (FAILED(pDressupUI->Init()))
		{ // 初期化に失敗した場合

			// ドレスアップUIの破棄
			SAFE_DELETE(pDressupUI);
			return nullptr;
		}

		// 原点位置を設定
		pDressupUI->SetPosition(rPos);

		// 確保したアドレスを返す
		return pDressupUI;
	}
}

//============================================================
// 操作権インデックスの取得処理
//============================================================
int CDressupUI::GetMyPlayerIdx() const
{
	// 操作権インデックスを返す
	return m_pPlayer->GetMyPlayerIdx();
}

//============================================================
// チームサイドの取得処理
//============================================================
CGameManager::ETeamSide CDressupUI::GetTeam() const
{
	// チームサイドを返す
	return m_pPlayer->GetTeam();
}

//============================================================
// 髪のインデックス番号の取得処理
//============================================================
int CDressupUI::GetHairNowIdx() const
{
	// 髪のインデックス番号を返す
	return m_pHair->GetNowIdx();
}

//============================================================
// アクセのインデックス番号の取得処理
//============================================================
int CDressupUI::GetAccessoryNowIdx() const
{
	// アクセのインデックス番号を返す
	return m_pAccessory->GetNowIdx();
}

//============================================================
// 顔のインデックス番号の取得処理
//============================================================
int CDressupUI::GetFaceNowIdx() const
{
	// 顔のインデックス番号を返す
	return m_pFace->GetNowIdx();
}

//============================================================
// 体型の取得処理
//============================================================
CPlayer::EBody CDressupUI::GetBodyType() const
{
	// 体型を返す
	return m_pPlayer->GetBodyType();
}

//============================================================
// 利き手の取得処理
//============================================================
CPlayer::EHandedness CDressupUI::GetHandedness() const
{
	// 利き手を返す
	return m_pPlayer->GetHandedness();
}

//============================================================
// UIの生成処理
//============================================================
HRESULT CDressupUI::CreateUI()
{
	// 変更種類アイコンの生成
	if (FAILED(CreateChangeIcon()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// プレイヤーフレームの生成
	if (FAILED(CreatePlayerFrame()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 相対位置の設定
	SetPositionRelative();

	return S_OK;
}

//============================================================
// 変更種類アイコンの生成処理
//============================================================
HRESULT CDressupUI::CreateChangeIcon()
{
	// 変更種類アイコンの生成
	m_pChangeIcon = CObject2D_Anim::Create
	( // 引数
		VEC3_ZERO,		// 位置
		change::PTRN.x,	// テクスチャ横分割数
		change::PTRN.y,	// テクスチャ縦分割数
		0.0f,			// 再生時間
		false,			// 自動破棄
		PRIORITY		// 優先順位
	);
	if (m_pChangeIcon == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 自動再生をOFFにする
	m_pChangeIcon->SetEnableAutoPlay(false);

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(change::TEXTURE);
	m_pChangeIcon->BindTexture(nTexID);

	// 横幅を元にサイズを設定
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, change::WIDTH * (float)change::PTRN.x);
	size.x /= (float)change::PTRN.x;
	m_pChangeIcon->SetSize(size);
	m_pChangeIcon->SetSizeOrigin(m_pChangeIcon->GetSize());

	return S_OK;
}

//============================================================
// プレイヤーフレームの生成処理
//============================================================
HRESULT CDressupUI::CreatePlayerFrame()
{
	// プレイヤーフレームの生成
	m_pPlayerFrame = CObject2D::Create(PRIORITY);
	if (m_pPlayerFrame == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(frame::player::TEXTURE);
	m_pPlayerFrame->BindTexture(nTexID);

	// 横幅を元にサイズを設定
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, frame::player::WIDTH);
	m_pPlayerFrame->SetSize(size);
	m_pPlayerFrame->SetSizeOrigin(m_pPlayerFrame->GetSize());

	return S_OK;
}

//============================================================
// セットアップの生成処理
//============================================================
HRESULT CDressupUI::CreateSetup()
{
	// チームセットアップ情報の取得
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) { return E_FAIL; }

	// エントリーインデックスを取得
	const int nEntryIdx = pSetupTeam->GetEntryIdx(m_nPlayerIdx);
	CGameManager::ETeamSide side;
	if (nEntryIdx > -1)
	{ // ユーザーの場合

		// エントリーインデックスからチームサイドを取得
		side = pSetupTeam->GetTeamSideUser(nEntryIdx);
	}
	else
	{ // AIの場合

		// 自身のAI生成順を保存
		m_nOrdinalAI = m_nNumAI;

		// AI生成順からチームサイドを取得
		side = pSetupTeam->GetTeamSideAI(m_nOrdinalAI);

		// AI生成数を加算
		m_nNumAI++;
	}

	// プレイヤー生成
	MyLib::Vector3 pos = MyLib::Vector3(-400.0f, 0.0f, 0.0f);
	MyLib::Vector3 offset = MyLib::Vector3(200.0f * m_nPlayerIdx, 0.0f, 0.0f);
	m_pPlayer = CPlayer::Create
	( // 引数
		pos + offset, 					// 位置
		side,							// チームサイド
		CPlayer::EHuman::HUMAN_ENTRY,	// ポジション
		CPlayer::EBody::BODY_NORMAL,	// 体系
		CPlayer::EHandedness::HAND_R	// 利き手
	);
	if (m_pPlayer == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 元の位置設定
	m_pPlayer->CObject::SetOriginPosition(pos + offset);

	// インデックスの上書き
	m_pPlayer->SetMyPlayerIdx(nEntryIdx);

	// 髪着せ替え生成
	m_pHair = CDressup::Create
	( // 引数
		CDressup::EType::TYPE_HAIR,	// 着せ替えの種類
		m_pPlayer,					// 変更するプレイヤー
		CPlayer::ID_HAIR			// 変更箇所のインデックス
	);
	if (m_pHair == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}
	
	// アクセ切り替え生成
	m_pAccessory = CDressup::Create
	( // 引数
		CDressup::EType::TYPE_ACCESSORY,	// 着せ替えの種類
		m_pPlayer,							// 変更するプレイヤー
		CPlayer::ID_ACCESSORY				// 変更箇所のインデックス
	);
	if (m_pAccessory == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 顔切り替え生成
	m_pFace = CDressup::Create
	( // 引数
		CDressup::EType::TYPE_FACE,	// 着せ替えの種類
		m_pPlayer,					// 変更するプレイヤー
		CPlayer::ID_FACE			// 変更箇所のインデックス
	);
	if (m_pFace == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 操作するインデックス設定
	m_pHair->SetControllIdx(nEntryIdx);
	m_pAccessory->SetControllIdx(nEntryIdx);
	m_pFace->SetControllIdx(nEntryIdx);

	return S_OK;
}

//============================================================
//	相対位置の設定処理
//============================================================
void CDressupUI::SetPositionRelative()
{
	// 自身の位置を取得
	MyLib::Vector3 posThis = GetPosition();

	// 変更種類アイコンの位置設定
	m_pChangeIcon->SetPosition(posThis + MyLib::Vector3(0.0f, -220.0f, 0.0f));

	// プレイヤーフレームの位置設定
	m_pPlayerFrame->SetPosition(posThis);
}

//============================================================
// UIの更新処理
//============================================================
void CDressupUI::UpdateUI()
{
	// テクスチャ情報の取得
	CTexture* pTexture = CTexture::GetInstance();
	assert(pTexture != nullptr);

	// 変更種類をアイコンに反映
	m_pChangeIcon->SetPatternAnim(m_typeChange);

	switch (m_typeEdit)
	{ // エディット種類ごとの処理
	case EEditType::EDIT_PROCESS:
	{
		// 変更種類アイコンの色をグレーにする
		m_pChangeIcon->SetColor(COL_DEFAULT);

		// プレイヤーフレームの色を白にする
		m_pPlayerFrame->SetColor(COL_CHOICE);
		break;
	}
	case EEditType::EDIT_CHANGETYPE:
	{
		// 変更種類アイコンの色を白にする
		m_pChangeIcon->SetColor(COL_CHOICE);

		// プレイヤーフレームの色をグレーにする
		m_pPlayerFrame->SetColor(COL_DEFAULT);
		break;
	}
	default:
		assert(false);
		break;
	}
}

//============================================================
// エディットする種類変更
//============================================================
void CDressupUI::ChangeEditType(int nPadIdx)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, nPadIdx))
	{// 変更する箇所の種類変更へ変更

		m_typeEdit = EEditType::EDIT_PROCESS;
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, nPadIdx))
	{// 実際の変更画面へ

		m_typeEdit = EEditType::EDIT_CHANGETYPE;
	}
}

//============================================================
// 変更する箇所の種類変更
//============================================================
void CDressupUI::ChangeChangeType(int nPadIdx)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nPadIdx))
	{// ループ

		// 次へ変更
		int changeType = (m_typeChange + 1) % EChangeType::TYPE_MAX;
		m_typeChange = static_cast<EChangeType>(changeType);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nPadIdx))
	{// 逆ループ

		// 前へ変更
		int changeType = (m_typeChange + (EChangeType::TYPE_MAX - 1)) % EChangeType::TYPE_MAX;
		m_typeChange = static_cast<EChangeType>(changeType);
	}
}

//============================================================
// 体型変更
//============================================================
void CDressupUI::ChangeBodyType(int nPadIdx)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// 体型取得
	CPlayer::EBody body = m_pPlayer->GetBodyType();
	CPlayer::EBody oldBody = body;

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nPadIdx))
	{// ループ

		// 次へ変更
		int afterBody = (body + 1) % CPlayer::EBody::BODY_MAX;
		body = static_cast<CPlayer::EBody>(afterBody);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nPadIdx))
	{// 逆ループ

		// 前へ変更
		int deforeBody = (body + (CPlayer::EBody::BODY_MAX - 1)) % CPlayer::EBody::BODY_MAX;
		body = static_cast<CPlayer::EBody>(deforeBody);
	}

	if (body != oldBody)
	{// 変更されていたら

		// プレイヤー再生成
		ReCreatePlayer(m_pPlayer->GetHandedness(), body);
	}
}

//============================================================
// 利き手変更
//============================================================
void CDressupUI::ChangeHandedness(int nPadIdx)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// 利き手取得
	CPlayer::EHandedness handedness = m_pPlayer->GetHandedness();
	CPlayer::EHandedness oldHandedness = handedness;

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, nPadIdx))
	{// ループ

		// 次へ変更
		int afterHandedness = (handedness + 1) % CPlayer::EHandedness::HAND_MAX;
		handedness = static_cast<CPlayer::EHandedness>(afterHandedness);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, nPadIdx))
	{// 逆ループ

		// 前へ変更
		int deforeHandedness = (handedness + (CPlayer::EHandedness::HAND_MAX - 1)) % CPlayer::EHandedness::HAND_MAX;
		handedness = static_cast<CPlayer::EHandedness>(deforeHandedness);
	}

	if (handedness != oldHandedness)
	{// 変更されていたら

		// プレイヤー再生成
		ReCreatePlayer(handedness, m_pPlayer->GetBodyType());
	}
}

//============================================================
// プレイヤー再生成
//============================================================
HRESULT CDressupUI::ReCreatePlayer(CPlayer::EHandedness handedness, CPlayer::EBody body)
{
	// チームセットアップ情報の取得
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) { return E_FAIL; }

	// プレイヤー位置の保存
	MyLib::Vector3 posPlayer = m_pPlayer->CObject::GetOriginPosition();

	// プレイヤーの削除
	SAFE_KILL(m_pPlayer);

	// エントリーインデックスを取得
	int nEntryIdx = pSetupTeam->GetEntryIdx(m_nPlayerIdx);
	CGameManager::ETeamSide side;
	if (nEntryIdx > -1)
	{ // ユーザーの場合

		// エントリーインデックスからチームサイドを取得
		side = pSetupTeam->GetTeamSideUser(nEntryIdx);
	}
	else
	{ // AIの場合

		// AI生成順からチームサイドを取得
		side = pSetupTeam->GetTeamSideAI(m_nOrdinalAI);
	}

	// 再生成
	m_pPlayer = CPlayer::Create
	( // 引数
		posPlayer, 						// 位置
		side,							// チームサイド
		CPlayer::EHuman::HUMAN_ENTRY,	// ポジション
		body,							// 体系
		handedness						// 利き手
	);
	if (m_pPlayer == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// インデックスの上書き
	m_pPlayer->SetMyPlayerIdx(nEntryIdx);

	// 元の位置設定
	m_pPlayer->CObject::SetPosition(posPlayer);
	m_pPlayer->CObject::SetOriginPosition(posPlayer);

	// ドレスアップに再割当
	m_pHair->BindObjectCharacter(m_pPlayer);
	m_pAccessory->BindObjectCharacter(m_pPlayer);
	m_pFace->BindObjectCharacter(m_pPlayer);
	m_pHair->ReRegist();
	m_pAccessory->ReRegist();
	m_pFace->ReRegist();

	return S_OK;
}

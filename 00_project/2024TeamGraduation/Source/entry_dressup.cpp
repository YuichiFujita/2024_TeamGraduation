//=============================================================================
// 
//  ドレスアップ設定処理 [entry_dressup.cpp]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//=============================================================================
#include "entry_dressup.h"
#include "manager.h"
#include "fade.h"
#include "entry_setupTeam.h"
#include "playerManager.h"
#include "inputKeyButton.h"
#include "string2D.h"
#include "dressupUI.h"
#include "selectUI.h"
#include "object2D.h"
#include "object2D_Anim.h"
#include "entryRuleManager.h"
#include "loadtext.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTFILE	= "data\\TEXT\\entry\\setupTeam.txt";
	const std::string TOP_LINE	= "#==============================================================================";	// テキストのライン
	const std::string TEXT_LINE	= "#------------------------------------------------------------------------------";	// テキストのライン
	const int PRIORITY = 6;	// 優先順位

	namespace ui
	{
		namespace left
		{
			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, VEC3_SCREEN_CENT.y - 80.0f, 0.0f);	// 左中心位置
			const float OFFSET_X = 210.0f;	// X座標オフセット
		}

		namespace right
		{
			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, VEC3_SCREEN_CENT.y - 80.0f, 0.0f);	// 右中心位置
			const float OFFSET_X = 210.0f;	// X座標オフセット
		}

		namespace area
		{
			const MyLib::Vector3 POS	= MyLib::Vector3(VEC3_SCREEN_CENT.x - 240.0f, 570.0f, 0.0f);	// 左位置
			const std::string TEXTURE	= "data\\TEXTURE\\entry\\inout.png";	// 変更種類アイコンテクスチャ
			const MyLib::Vector3 OFFSET	= MyLib::Vector3(480.0f, 0.0f, 0.0f);	// オフセット
			const MyLib::PosGrid2 PTRN	= MyLib::PosGrid2(1, 2);	// テクスチャ分割数
			const float WIDTH			= 40.0f;	// 横幅
		}

		namespace name
		{
			const MyLib::Vector3 POS[] = { MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, 60.0f, 0.0f), MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, 60.0f, 0.0f) };	// 原点位置
			const char*	FONT		= "data\\FONT\\チョークS.otf";	// フォントパス
			const bool	ITALIC		= false;						// イタリック
			const float	HEIGHT		= 42.0f;						// 文字縦幅
			const EAlignX ALIGN_X	= XALIGN_CENTER;				// 横配置
			const D3DXCOLOR COL		= MyLib::color::Black();		// 色
		}

		namespace back
		{
			const std::string TEXTURE	= "data\\TEXTURE\\entry\\back.png";	// テクスチャ
			const MyLib::Vector3 POS	= MyLib::Vector3(VEC3_SCREEN_CENT.x - 500.0f, 645.0f, 0.0f);	// 位置
			const float HEGHT = 55.0f;	// 大きさ
		}

		namespace enter
		{
			const std::string TEXTURE = "data\\TEXTURE\\entry\\next.png";	// テクスチャ
			const MyLib::Vector3 POS	= MyLib::Vector3(VEC3_SCREEN_CENT.x + 500.0f, 645.0f, 0.0f);	// 位置
			const float HEGHT = 55.0f;	// 大きさ
		}
	}
}

//==========================================================================
// コンストラクタ
//==========================================================================
CEntry_Dressup::CEntry_Dressup() : CEntryScene(),
	m_pRuleManager	(nullptr),		// ルールマネージャー
	m_state			(STATE_DRESSUP)	// 状態
{
	// メンバ変数をクリア
	memset(&m_apTeamName[0], 0, sizeof(m_apTeamName));		// チーム名情報
	memset(&m_apDressInfo[0], 0, sizeof(m_apDressInfo));	// 外野着せ替え情報
	memset(&m_apAreaUI[0], 0, sizeof(m_apAreaUI));			// ポジション変更UI情報
	memset(&m_apTransUI[0], 0, sizeof(m_apTransUI));		// 遷移UI情報
}

//==========================================================================
// デストラクタ
//==========================================================================
CEntry_Dressup::~CEntry_Dressup()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEntry_Dressup::Init()
{
	CTexture* pTexture = CTexture::GetInstance();

	// 前回の着せ替え読込
	Load();

	// チームセットアップ情報の取得
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) { return E_FAIL; }

	for (int i = 0; i < CGameManager::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		// ポジション変更UIの生成
		m_apAreaUI[i] = CObject2D_Anim::Create
		( // 引数
			ui::area::POS + (ui::area::OFFSET * (float)i),	// 位置
			ui::area::PTRN.x,	// テクスチャ横分割数
			ui::area::PTRN.y,	// テクスチャ縦分割数
			0.0f,				// 待機時間
			false,				// 自動破棄
			PRIORITY			// 優先順位
		);
		if (m_apAreaUI[i] == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}

		// 自動再生をOFFにする
		m_apAreaUI[i]->SetEnableAutoPlay(false);

		// テクスチャの割当
		int nTexID = CTexture::GetInstance()->Regist(ui::area::TEXTURE);
		m_apAreaUI[i]->BindTexture(nTexID);

		// 横幅を元にサイズを設定
		MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, ui::area::WIDTH);
		size.x *= ui::area::PTRN.y;
		m_apAreaUI[i]->SetSize(size);
		m_apAreaUI[i]->SetSizeOrigin(m_apAreaUI[i]->GetSize());
	}

	// 戻るボタンの生成
	m_apTransUI[TRANS_BACK] = CObject2D::Create(PRIORITY);
	if (m_apTransUI[TRANS_BACK] == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// テクスチャの割当
	int nTexBackID = CTexture::GetInstance()->Regist(ui::back::TEXTURE);
	m_apTransUI[TRANS_BACK]->BindTexture(nTexBackID);

	// 位置を設定
	m_apTransUI[TRANS_BACK]->SetPosition(ui::back::POS);

	// 横幅を元にサイズを設定
	MyLib::Vector2 sizeBack = pTexture->GetImageSize(nTexBackID);
	sizeBack = UtilFunc::Transformation::AdjustSizeByHeight(sizeBack, ui::back::HEGHT);
	m_apTransUI[TRANS_BACK]->SetSize(sizeBack);
	m_apTransUI[TRANS_BACK]->SetSizeOrigin(m_apTransUI[TRANS_BACK]->GetSize());

	// 進むボタンの生成
	m_apTransUI[TRANS_NEXT] = CObject2D::Create(PRIORITY);
	if (m_apTransUI[TRANS_NEXT] == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// テクスチャの割当
	int nTexNextID = CTexture::GetInstance()->Regist(ui::enter::TEXTURE);
	m_apTransUI[TRANS_NEXT]->BindTexture(nTexNextID);

	// 位置を設定
	m_apTransUI[TRANS_NEXT]->SetPosition(ui::enter::POS);

	// 横幅を元にサイズを設定
	MyLib::Vector2 sizeNext = pTexture->GetImageSize(nTexNextID);
	sizeNext = UtilFunc::Transformation::AdjustSizeByHeight(sizeNext, ui::enter::HEGHT);
	m_apTransUI[TRANS_NEXT]->SetSize(sizeNext);
	m_apTransUI[TRANS_NEXT]->SetSizeOrigin(m_apTransUI[TRANS_NEXT]->GetSize());

	int nCurLeft = 0;	// 現在の左プレイヤー数
	int nMaxLeft = pSetupTeam->GetPlayerNum(CGameManager::ETeamSide::SIDE_LEFT);	// 左プレイヤー総数
	for (int nPlayerIdx = 0; nPlayerIdx < CGameManager::MAX_PLAYER; nPlayerIdx++)
	{ // プレイヤー人数分繰り返す

		// チーム指定がない場合次へ
		CGameManager::ETeamSide team = pSetupTeam->GetTeamSide(nPlayerIdx);	// チームサイド
		if (team != CGameManager::ETeamSide::SIDE_LEFT) { continue; }

		// UIの位置を計算
		MyLib::Vector3 posUI = ui::left::POS;	// UI位置
		posUI.x = ui::left::POS.x - (ui::left::OFFSET_X * (float)(nMaxLeft - 1)) * 0.5f + (ui::left::OFFSET_X * (float)nCurLeft);

		// チームメンバー数を加算
		nCurLeft++;

		// 着せ替えUIの生成
		CDressupUI* pDressup = CDressupUI::Create(this, CPlayer::FIELD_IN, nPlayerIdx, posUI);
		if (pDressup == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// 着せ替えUIの追加
		m_vecDressInfo.push_back(pDressup);

		const int nPadIdx = pSetupTeam->PlayerIdxToPadIdx(nPlayerIdx);	// 操作権インデックス
		if (nPadIdx > -1)
		{ // プレイヤーの場合

			// 選択UIの生成
			int nSelectX = nCurLeft - 1;	// X選択
			CSelectUI* pSelect = CSelectUI::Create(team, nSelectX, nPadIdx, posUI);
			if (pSelect == nullptr)
			{ // 生成に失敗した場合

				return E_FAIL;
			}

			// 着せ替えUI操作権の設定
			SetDressUIControl(nPadIdx, nSelectX);

			// 選択操作を停止
			pSelect->SetSelect(false);

			// 選択UIの追加
			m_vecSelect.push_back(pSelect);
		}
	}

	int nCurRight = 0;	// 現在の右プレイヤー数
	int nMaxRight = pSetupTeam->GetPlayerNum(CGameManager::ETeamSide::SIDE_RIGHT);	// 右プレイヤー総数
	for (int nPlayerIdx = 0; nPlayerIdx < CGameManager::MAX_PLAYER; nPlayerIdx++)
	{ // プレイヤー人数分繰り返す

		// チーム指定がない場合次へ
		CGameManager::ETeamSide team = pSetupTeam->GetTeamSide(nPlayerIdx);	// チームサイド
		if (team != CGameManager::ETeamSide::SIDE_RIGHT) { continue; }

		// UIの位置を計算
		MyLib::Vector3 posUI = ui::right::POS;	// UI位置
		posUI.x = ui::right::POS.x - (ui::right::OFFSET_X * (float)(nMaxRight - 1)) * 0.5f + (ui::right::OFFSET_X * (float)nCurRight);

		// チームメンバー数を加算
		nCurRight++;

		// 着せ替えUIの生成
		CDressupUI* pDressup = CDressupUI::Create(this, CPlayer::FIELD_IN, nPlayerIdx, posUI);
		if (pDressup == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// 着せ替えUIの追加
		m_vecDressInfo.push_back(pDressup);

		const int nPadIdx = pSetupTeam->PlayerIdxToPadIdx(nPlayerIdx);	// 操作権インデックス
		if (nPadIdx > -1)
		{ // プレイヤーの場合

			// 選択UIの生成
			int nSelectX = nCurLeft + nCurRight - 1;	// X選択
			CSelectUI* pSelect = CSelectUI::Create(team, nSelectX, nPadIdx, posUI);
			if (pSelect == nullptr)
			{ // 生成に失敗した場合

				return E_FAIL;
			}

			// 着せ替えUI操作権の設定
			SetDressUIControl(nPadIdx, nSelectX);

			// 選択操作を停止
			pSelect->SetSelect(false);

			// 選択UIの追加
			m_vecSelect.push_back(pSelect);
		}
	}

	for (int nPlayerIdx = 0; nPlayerIdx < CPlayerManager::OUT_MAX; nPlayerIdx++)
	{ // 外野人数分繰り返す

		int nMaxTeam = CPlayerManager::OUT_MAX / 2;	// チームごとの外野人数
		MyLib::Vector3 posUI;	// UI位置
		if (nPlayerIdx < nMaxTeam)
		{
			// UIの位置を計算
			posUI = ui::left::POS;
			posUI.x = ui::left::POS.x - (ui::left::OFFSET_X * (float)(nMaxTeam - 1)) * 0.5f + (ui::left::OFFSET_X * (float)nPlayerIdx);
		}
		else
		{
			// UIの位置を計算
			posUI = ui::right::POS;
			posUI.x = ui::right::POS.x - (ui::right::OFFSET_X * (float)(nMaxTeam - 1)) * 0.5f + (ui::right::OFFSET_X * (float)(nPlayerIdx - nMaxTeam));
		}

		// 着せ替えUIの生成
		m_apDressInfo[nPlayerIdx] = CDressupUI::Create(this, CPlayer::FIELD_OUT, -1, posUI);
		if (m_apDressInfo[nPlayerIdx] == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// 自動描画をOFFにする
		m_apDressInfo[nPlayerIdx]->SetEnableDisp(false);
	}

	for (int i = 0; i < CGameManager::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		// チーム名の生成
		m_apTeamName[i] = CString2D::Create
		( // 引数
			ui::name::FONT,		// フォントパス
			ui::name::ITALIC,	// イタリック
			L"",				// 指定文字列
			ui::name::POS[i],	// 原点位置
			ui::name::HEIGHT,	// 文字縦幅
			ui::name::ALIGN_X,	// 横配置
			VEC3_ZERO,			// 原点向き
			ui::name::COL		// 色
		);
		if (m_apTeamName[i] == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}

		// テキストを割当
		loadtext::BindString(m_apTeamName[i], loadtext::LoadText("data\\TEXT\\entry\\nameTeam.txt", UtilFunc::Transformation::Random(0, 9)));
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEntry_Dressup::Uninit()
{
	// エントリールールの破棄
	SAFE_REF_RELEASE(m_pRuleManager);

	for (int nPlayerIdx = 0; nPlayerIdx < CPlayerManager::OUT_MAX; nPlayerIdx++)
	{ // 外野人数分繰り返す

		// 着せ替えUIの終了
		SAFE_UNINIT(m_apDressInfo[nPlayerIdx]);
	}

	for (auto& rInfo : m_vecDressInfo)
	{ // 要素数分繰り返す

		// 着せ替えUIの終了
		SAFE_UNINIT(rInfo);
	}

	for (auto& rSelect : m_vecSelect)
	{ // 要素数分繰り返す

		// 選択UIの終了
		SAFE_UNINIT(rSelect);
	}

	for (int i = 0; i < CGameManager::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		// チーム名の終了
		SAFE_UNINIT(m_apTeamName[i]);

		// ポジション変更UIの終了
		SAFE_UNINIT(m_apAreaUI[i]);
	}

	for (int i = 0; i < TRANS_MAX; i++)
	{ // 遷移ボタンの総数分繰り返す

		// 遷移UIの終了
		SAFE_UNINIT(m_apTransUI[i]);
	}

	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CEntry_Dressup::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	switch (m_state)
	{ // 状態ごとの処理
	case EState::STATE_DRESSUP:
		break;

	case EState::STATE_SETTING:

		// ルールマネージャーの更新
		assert(m_pRuleManager != nullptr);
		m_pRuleManager->Update(fDeltaTime, fDeltaRate, fSlowRate);
		break;

	case EState::STATE_END:

		// セーブ処理
		Save();

		// ゲーム画面に遷移する
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_GAME);
		break;

	default:
		assert(false);
		break;
	}

	// 親の更新
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 状態設定
//==========================================================================
void CEntry_Dressup::SetState(const EState state)
{
	// 状態を設定
	m_state = state;
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_DRESSUP:

		// エントリールールの破棄
		SAFE_REF_RELEASE(m_pRuleManager);
		break;

	case STATE_SETTING:

		if (m_pRuleManager == nullptr)
		{
			// エントリールールの生成
			m_pRuleManager = CEntryRuleManager::Create(this);
		}
		break;

	case STATE_END:
		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// 着せ替えUI準備完了フラグの設定
//==========================================================================
void CEntry_Dressup::SetDressUIReady(const int nPlayerIdx, const bool bReady)
{
	// プレイヤーインデックスが範囲外の場合エラー
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumDressUI()) { assert(false); return; }

	// 準備完了フラグを返す
	GetPtrDressUI(nPlayerIdx)->SetReady(bReady);
}

//==========================================================================
// 着せ替えUIの準備完了済みかの確認
//==========================================================================
bool CEntry_Dressup::IsDressUIReady(const int nPlayerIdx)
{
	// プレイヤーインデックスが範囲外の場合エラー
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumDressUI()) { assert(false); return false; }

	// 準備完了フラグを返す
	return GetPtrDressUI(nPlayerIdx)->IsReady();
}

//==========================================================================
// 着せ替えUI操作権の設定
//==========================================================================
void CEntry_Dressup::SetDressUIControl(const int nPadIdx, const int nPlayerIdx)
{
	// プレイヤーインデックスが範囲外の場合エラー
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumDressUI()) { assert(false); return; }

	// 引数プレイヤーの着せ替えUI位置を返す
	GetPtrDressUI(nPlayerIdx)->SetPadIdx(nPadIdx);
}

//==========================================================================
// 着せ替えUI選択可能かの確認
//==========================================================================
bool CEntry_Dressup::IsDressSelectOK(const int nPadIdx, const int nPlayerIdx) const
{
	// 自分以外のユーザーの着せ替えUIの場合選択不可
	const int nSelectPadIdx = GetPtrDressUI(nPlayerIdx)->GetMyPlayerIdx();	// 選択予定先の操作権インデックス
	if (nSelectPadIdx > -1 && nSelectPadIdx != nPadIdx) { return false; }

	for (const auto& rSelect : m_vecSelect)
	{ // 要素数分繰り返す

		// 既に自分以外のユーザーが選択中の場合選択不可
		if (rSelect->GetSelectIdx().y == CSelectUI::SELECT_DRESSUP
		&&  rSelect->GetSelectIdx().x == nPlayerIdx
		&&  rSelect->GetPadIdx()	  != nPadIdx) { return false; }
	}

	return true;
}

//==========================================================================
// 選択UI選択操作フラグの設定
//==========================================================================
void CEntry_Dressup::SetSelectUISelect(const int nPadIdx, const bool bSelect)
{
	for (auto& rSelect : m_vecSelect)
	{ // 要素数分繰り返す

		// 選択UIの操作権インデックスが引数の操作権インデックスと一致しない場合次へ
		if (rSelect->GetPadIdx() != nPadIdx) { continue; }

		// 選択操作可能フラグの設定
		return rSelect->SetSelect(bSelect);
	}
}

//==========================================================================
// 着せ替えUI位置取得
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetDressUIPosition(const int nPlayerIdx) const
{
	// プレイヤーインデックスが範囲外の場合エラー
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumDressUI()) { assert(false); return VEC3_ZERO; }

	// 引数プレイヤーの着せ替えUI位置を返す
	return GetPtrDressUI(nPlayerIdx)->GetPosition();
}

//==========================================================================
// 名前UI位置取得
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetNameUIPosition(const CGameManager::ETeamSide team)
{
	// 文字列の位置を返す
	return m_apTeamName[team]->GetPosition();
}

//==========================================================================
// 名前UI大きさ取得
//==========================================================================
MyLib::Vector2 CEntry_Dressup::GetNameUISize(const CGameManager::ETeamSide team)
{
	// 文字列の大きさを返す
	return MyLib::Vector2(m_apTeamName[team]->GetStrWidth(), ui::name::HEIGHT);
}

//==========================================================================
// ポジション変更UI位置取得
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetAreaUIPosition(const CPlayer::EFieldArea area)
{
	// ポリゴンの位置を返す
	return m_apAreaUI[area]->GetPosition();
}

//==========================================================================
// ポジション変更UI大きさ取得
//==========================================================================
MyLib::Vector2 CEntry_Dressup::GetAreaUISize(const CPlayer::EFieldArea area)
{
	// ポリゴンの大きさを返す
	return m_apAreaUI[area]->GetSize();
}

//==========================================================================
// 遷移UI位置取得
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetTransUIPosition(const ETrans trans)
{
	// ポリゴンの位置を返す
	return m_apTransUI[trans]->GetPosition();
}

//==========================================================================
// 遷移UI大きさ取得
//==========================================================================
MyLib::Vector2 CEntry_Dressup::GetTransUISize(const ETrans trans)
{
	// ポリゴンの大きさを返す
	return m_apTransUI[trans]->GetSize();
}

//==========================================================================
// 表示着せ替えUI数取得 (全着せ替えUI)
//==========================================================================
int CEntry_Dressup::GetNumDressUI() const
{
	int nNumDressUI = 0;	// 着せ替えUI数
	for (int team = 0; team < CGameManager::SIDE_MAX; team++)
	{ // チーム数分繰り返す

		// 表示着せ替えUI数取得
		nNumDressUI += GetNumDressUI((CGameManager::ETeamSide)team);
	}
	return nNumDressUI;
}

//==========================================================================
// 表示着せ替えUI数取得 (チーム別着せ替えUI)
//==========================================================================
int CEntry_Dressup::GetNumDressUI(const CGameManager::ETeamSide team) const
{
	int nNumDressUI = 0;	// 着せ替えUI数
	switch (m_apAreaUI[team]->GetPatternAnim())
	{ // 現在パターンごとの処理
	case CPlayer::FIELD_IN:
	{
		for (auto& rInfo : m_vecDressInfo)
		{ // 要素数分繰り返す

			// 違うチームの場合次へ
			if (rInfo->GetTeam() != team) { continue; }

			// UI数を加算
			nNumDressUI++;
		}
		break;
	}
	case CPlayer::FIELD_OUT:
	{
		// 外野人数分UI数を加算
		nNumDressUI += CPlayerManager::OUT_MAX / 2;
		break;
	}
	default:
		assert(false);
		break;
	}
	return nNumDressUI;
}

//==========================================================================
// 着せ替えUIのポジション変更
//==========================================================================
void CEntry_Dressup::ChangeDressUIArea(const CGameManager::ETeamSide team)
{
	// チームの準備が完了していない場合変更不可
	if (!IsAreaChangeOK(team)) { return; }

	// TODO：ここで現在の選択インデックスが持つDressUIポインタを保存
	std::map<CSelectUI*, CDressupUI*> mapDressUI;	// 着せ替えUI一時保存マップ
	for (auto& rSelect : m_vecSelect)
	{ // 要素数分繰り返す

		MyLib::PosGrid2 select = rSelect->GetSelectIdx();	// 選択インデックス
		if (select.y == CSelectUI::SELECT_DRESSUP)
		{ // 着せ替え選択中の場合

			// ローカルマップに着せ替えUIを一時保存
			mapDressUI.insert(std::make_pair(rSelect, GetPtrDressUI(select.x)));
		}
	}

	// パターンを進める
	m_apAreaUI[team]->AddPatternAnim();

	switch (m_apAreaUI[team]->GetPatternAnim())
	{ // 現在パターンごとの処理
	case CPlayer::FIELD_IN:
	{
		int nSideMax = CPlayerManager::OUT_MAX / 2;	// チームごとの最大人数
		int nOffset = nSideMax * (int)team;			// インデックスオフセット
		for (int nPlayerIdx = nOffset; nPlayerIdx < nSideMax + nOffset; nPlayerIdx++)
		{ // 外野人数分繰り返す

			// 外野着せ替えUIの非表示
			m_apDressInfo[nPlayerIdx]->SetEnableDisp(false);

			// 準備完了させる
			m_apDressInfo[nPlayerIdx]->SetReady(true);
		}

		for (auto& rInfo : m_vecDressInfo)
		{ // 要素数分繰り返す

			// 違うチームの場合次へ
			if (rInfo->GetTeam() != team) { continue; }

			// 内野着せ替えUIの表示
			rInfo->SetEnableDisp(true);
		}
		break;
	}
	case CPlayer::FIELD_OUT:
	{
		int nSideMax = CPlayerManager::OUT_MAX / 2;	// チームごとの最大人数
		int nOffset = nSideMax * (int)team;			// インデックスオフセット
		for (int nPlayerIdx = nOffset; nPlayerIdx < nSideMax + nOffset; nPlayerIdx++)
		{ // 外野人数分繰り返す

			// 外野着せ替えUIの表示
			m_apDressInfo[nPlayerIdx]->SetEnableDisp(true);
		}

		for (auto& rInfo : m_vecDressInfo)
		{ // 要素数分繰り返す

			// 違うチームの場合次へ
			if (rInfo->GetTeam() != team) { continue; }

			// 内野着せ替えUIの非表示
			rInfo->SetEnableDisp(false);

			// 準備完了させる
			rInfo->SetReady(true);
		}
		break;
	}
	default:
		assert(false);
		break;
	}

	// TODO：ここでDressUIポインタが表示されている場合は選択インデックスを更新
	// ELSE：非表示の場合は全員選択肢をNEXTに移行
	for (auto& rSelect : m_vecSelect)
	{ // 要素数分繰り返す

		// 同一選択UIが見つからない場合次へ
		auto itr = mapDressUI.find(rSelect);	// 検索イテレーター
		if (itr == mapDressUI.end()) { continue; }

		CDressupUI* pDressUI = itr->second;	// 紐づけられた着せ替えUI
		if (pDressUI->IsDisp())
		{ // 表示中の場合

			// TODO：この着せ替えUIポインタの選択インデックスをselect.xに設定
			int nSelectX = GetIdxDressUI(pDressUI);
			assert(nSelectX != -1);

			// 選択中だった着せ替えUIに移動
			rSelect->SetSelectIdxX(nSelectX);
		}
		else
		{ // 非表示の場合

			// ポジション変更ボタンに移動
			rSelect->SetSelectIdxX(team);
			rSelect->SetSelectIdxY(CSelectUI::SELECT_AREA);
		}
	}
}

//==========================================================================
// 遷移UI選択数取得
//==========================================================================
int CEntry_Dressup::GetTransUINumSelect(const int nSelectX) const
{
	int nNumSelect = 0;	// 選択数
	for (auto& rSelect : m_vecSelect)
	{ // 要素数分繰り返す

		MyLib::PosGrid2 select = rSelect->GetSelectIdx();	// 選択インデックス
		if (select.y == CSelectUI::SELECT_TRANS && select.x == nSelectX)
		{ // 誰かが遷移を選択している場合

			// 選択数の加算
			nNumSelect++;
		}
	}

	return nNumSelect;
}

//==========================================================================
// ポジション変更UI選択数取得
//==========================================================================
int CEntry_Dressup::GetAreaUINumSelect(const int nSelectX) const
{
	int nNumSelect = 0;	// 選択数
	for (auto& rSelect : m_vecSelect)
	{ // 要素数分繰り返す

		MyLib::PosGrid2 select = rSelect->GetSelectIdx();	// 選択インデックス
		if (select.y == CSelectUI::SELECT_AREA && select.x == nSelectX)
		{ // 誰かが遷移を選択している場合

			// 選択数の加算
			nNumSelect++;
		}
	}

	return nNumSelect;
}

//==========================================================================
// チームの準備全完了確認
//==========================================================================
bool CEntry_Dressup::IsTeamReady(const CPlayer::EFieldArea area, const CGameManager::ETeamSide team)
{
	switch (area)
	{ // ポジションごとの処理
	case CPlayer::FIELD_IN:
	{
		for (auto& rInfo : m_vecDressInfo)
		{ // 要素数分繰り返す

			// 違うチームの場合次へ
			if (rInfo->GetTeam() != team) { continue; }

			// 準備が終わっている場合次へ
			if (rInfo->IsReady()) { continue; }

			// 準備未完了を返す
			return false;
		}

		// 準備全完了を返す
		return true;
	}
	case CPlayer::FIELD_OUT:
	{
		int nSideMax = CPlayerManager::OUT_MAX / 2;	// チームごとの最大人数
		int nOffset = nSideMax * (int)team;			// インデックスオフセット
		for (int nPlayerIdx = nOffset; nPlayerIdx < nSideMax + nOffset; nPlayerIdx++)
		{ // 外野人数分繰り返す

			// 準備が終わっている場合次へ
			if (m_apDressInfo[nPlayerIdx]->IsReady()) { continue; }

			// 準備未完了を返す
			return false;
		}

		// 準備全完了を返す
		return true;
	}
	default:
		assert(false);
		return false;
	}
}

//==========================================================================
// 名前選択可能かの確認
//==========================================================================
bool CEntry_Dressup::IsNameSelectOK(const CGameManager::ETeamSide team) const
{
	for (auto& rSelect : m_vecSelect)
	{ // 要素数分繰り返す

		MyLib::PosGrid2 select = rSelect->GetSelectIdx();	// 選択インデックス
		if (select.y == CSelectUI::SELECT_NAME && select.x == team)
		{ // 誰かが名前を選択している場合

			return false;
		}
	}

	return true;
}

//==========================================================================
// ポジション変更可能かの確認
//==========================================================================
bool CEntry_Dressup::IsAreaChangeOK(const CGameManager::ETeamSide team) const
{
	const int nMaxDressUI = GetNumDressUI(team);		// 着せ替えUI総数
	const int nSideMax = CPlayerManager::OUT_MAX / 2;	// チームごとの最大人数
	const int nOffset = nSideMax * (int)team;			// インデックスオフセット
	for (const auto& rSelect : m_vecSelect)
	{ // 要素数分繰り返す

		MyLib::PosGrid2 select = rSelect->GetSelectIdx();

		// 既に自分以外のユーザーが選択中の場合選択不可
		if (select.y != CSelectUI::SELECT_DRESSUP) { continue; }

		// 選択範囲が引数チームじゃない場合次へ
		if (!(select.x >= nOffset && select.x < nMaxDressUI + nOffset)) { continue; }

		// 操作していない場合次へ
		if (rSelect->IsSelect()) { continue; }

		return false;
	}

	return true;
}

//==========================================================================
// ゲーム設定遷移
//==========================================================================
bool CEntry_Dressup::TransSetting()
{
	// 準備完了できていない場合抜ける
	if (!IsAllReady()) { return false; }

	// ゲーム設定状態に遷移
	SetState(EState::STATE_SETTING);

	return true;
}

//==========================================================================
// 着せ替えUIの取得処理
//==========================================================================
CDressupUI* CEntry_Dressup::GetPtrDressUI(const int nIdx) const
{
	int nNumDressUI = 0;	// 着せ替えUI数
	for (int team = 0; team < CGameManager::SIDE_MAX; team++)
	{ // チーム数分繰り返す

		switch (m_apAreaUI[team]->GetPatternAnim())
		{ // 現在パターンごとの処理
		case CPlayer::FIELD_IN:
		{
			for (auto& rInfo : m_vecDressInfo)
			{ // 要素数分繰り返す

				// 違うチームの場合次へ
				if (rInfo->GetTeam() != team) { continue; }

				// 引数インデックスと一致した場合現在の着せ替えUIを返す
				if (nNumDressUI == nIdx) { return rInfo; }

				// UI数を加算
				nNumDressUI++;
			}
			break;
		}
		case CPlayer::FIELD_OUT:
		{
			int nSideMax = CPlayerManager::OUT_MAX / 2;	// チームごとの最大人数
			int nOffset = nSideMax * (int)team;			// インデックスオフセット
			for (int nPlayerIdx = nOffset; nPlayerIdx < nSideMax + nOffset; nPlayerIdx++)
			{ // 外野人数分繰り返す

				// 引数インデックスと一致した場合現在の着せ替えUIを返す
				if (nNumDressUI == nIdx) { return m_apDressInfo[nPlayerIdx]; }

				// UI数を加算
				nNumDressUI++;
			}
			break;
		}
		default:
			assert(false);
			break;
		}
	}

	return nullptr;
}

//==========================================================================
// X選択インデックスの取得処理
//==========================================================================
int CEntry_Dressup::GetIdxDressUI(const CDressupUI* pUI) const
{
	int nNumDressUI = GetNumDressUI();	// 着せ替えUI数
	for (int i = 0; i < nNumDressUI; i++)
	{ // 着せ替えUI数分繰り返す

		// 引数ポインタが見つかった場合インデックスを返す
		if (GetPtrDressUI(i) == pUI) { return i; }
	}

	return -1;
}

//==========================================================================
// 準備全完了フラグの取得処理
//==========================================================================
bool CEntry_Dressup::IsAllReady()
{
	const int nNumDressUI = GetNumDressUI(); // 着せ替えUI数
	for (int i = 0; i < nNumDressUI; i++)
	{ // 着せ替えUI数分繰り返す

		// 準備が終わっている場合次へ
		if (GetPtrDressUI(i)->IsReady()) { continue; }

		// 準備未完了を返す
		return false;
	}

	// 準備全完了を返す
	return true;
}

//==========================================================================
// セーブ処理
//==========================================================================
void CEntry_Dressup::Save()
{
	// プレイヤーマネージャ取得
	CPlayerManager* pPlayerMgr = CPlayerManager::GetInstance();

	// 読み込み情報
	std::vector<CPlayerManager::LoadInfo> vecInSaveInfo[CGameManager::ETeamSide::SIDE_MAX];
	std::vector<CPlayerManager::LoadInfo> vecOutSaveInfo[CGameManager::ETeamSide::SIDE_MAX];

	// 今回のサイズ
	int size = static_cast<int>(m_vecDressInfo.size());
	for (int i = 0; i < size; i++)
	{
		// 自身の操作するインデックス番号取得
		int nPadIdx = m_vecDressInfo[i]->GetMyPlayerIdx();
		CGameManager::ETeamSide side = m_vecDressInfo[i]->GetTeam();

		// 読み込み情報
		vecInSaveInfo[side].emplace_back();
		CPlayerManager::LoadInfo* pLoadInfo = &vecInSaveInfo[side].back();
		pLoadInfo->nControllIdx = nPadIdx;									// 自身の操作するインデックス番号取得
		pLoadInfo->nHair = m_vecDressInfo[i]->GetHairNowIdx();				// 髪のインデックス番号
		pLoadInfo->nAccessory = m_vecDressInfo[i]->GetAccessoryNowIdx();	// アクセのインデックス番号
		pLoadInfo->nFace = m_vecDressInfo[i]->GetFaceNowIdx();				// 顔のインデックス番号
		pLoadInfo->eBody = m_vecDressInfo[i]->GetBodyType();				// 体型
		pLoadInfo->eHanded = m_vecDressInfo[i]->GetHandedness();			// 利き手
	}

	for (int i = 0; i < CPlayerManager::OUT_MAX; i++)
	{ // 外野人数分繰り返す

		// 自身の操作するインデックス番号取得
		int nPadIdx = m_apDressInfo[i]->GetMyPlayerIdx();
		CGameManager::ETeamSide side = m_apDressInfo[i]->GetTeam();

		// 読み込み情報
		vecOutSaveInfo[side].emplace_back();
		CPlayerManager::LoadInfo* pLoadInfo = &vecOutSaveInfo[side].back();
		pLoadInfo->nControllIdx = nPadIdx;								// 自身の操作するインデックス番号取得
		pLoadInfo->nHair = m_apDressInfo[i]->GetHairNowIdx();			// 髪のインデックス番号
		pLoadInfo->nAccessory = m_apDressInfo[i]->GetAccessoryNowIdx();	// アクセのインデックス番号
		pLoadInfo->nFace = m_apDressInfo[i]->GetFaceNowIdx();			// 顔のインデックス番号
		pLoadInfo->eBody = m_apDressInfo[i]->GetBodyType();				// 体型
		pLoadInfo->eHanded = m_apDressInfo[i]->GetHandedness();			// 利き手
	}

	// セーブ処理
	pPlayerMgr->Save
	(
		vecInSaveInfo[CGameManager::ETeamSide::SIDE_LEFT],
		vecInSaveInfo[CGameManager::ETeamSide::SIDE_RIGHT],
		vecOutSaveInfo[CGameManager::ETeamSide::SIDE_LEFT],
		vecOutSaveInfo[CGameManager::ETeamSide::SIDE_RIGHT]
	);

	// ゲーム設定の保存
	assert(m_pRuleManager != nullptr);
	CEntryRuleManager::SRule rule = m_pRuleManager->GetRule();	// ルール
	CEntryRuleManager::SaveSetting(rule, m_apTeamName[CGameManager::ETeamSide::SIDE_LEFT]->GetStr(), m_apTeamName[CGameManager::ETeamSide::SIDE_RIGHT]->GetStr());
}

//==========================================================================
// ロード
//==========================================================================
void CEntry_Dressup::Load()
{
	// ファイルを開く
	std::ifstream File(TEXTFILE);
	if (!File.is_open()) {
		return;
	}

	// コメント用
	std::string hoge;

	// データ読み込み
	std::string line;
	while (std::getline(File, line))
	{
		// コメントはスキップ
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("PLAYERNUM") != std::string::npos)
		{// PLAYERNUMでチーム毎のプレイヤー数読み込み

			// ストリーム作成
			std::istringstream lineStream(line);
			
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// ファイルを閉じる
	File.close();
}

//==========================================================================
// デバッグ処理
//==========================================================================
void CEntry_Dressup::Debug()
{

	//--------------------------
	// シーン遷移
	//--------------------------
	if (IsAllReady() &&
		CInputKeyboard::GetInstance()->GetTrigger(DIK_X))
	{
		// ゲーム設定状態に遷移
		SetState(EState::STATE_SETTING);
	}

	if (ImGui::TreeNode("SetUpTeam"))
	{
		//=============================
		// セーブ
		//=============================s
		if (ImGui::Button("Save"))
		{
			Save();
		}

		// 今回のサイズ
		int size = static_cast<int>(m_vecDressInfo.size());
		for (int i = 0; i < size; i++)
		{
			std::string treename = "Info : " + std::to_string(i);	// ツリー名
			if (ImGui::TreeNode(treename.c_str()))
			{
				const auto& info = m_vecDressInfo[i];

				ImGui::Text("editType[%s]", magic_enum::enum_name(info->GetTypeEdit()));
				ImGui::Text("changeType[%s]", magic_enum::enum_name(info->GetTypeChange()));
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}

//============================================================
//
//	選択UI処理 [selectUI.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "selectUI.h"
#include "object2D.h"
#include "object2D_Anim.h"
#include "entry.h"
#include "entryscene.h"
#include "entry_dressup.h"
#include "inputKeyButton.h"
#include "string2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIO_BG = 4;		// 背景の優先順位
	const int PRIO_UI = 6;		// UIの優先順位
	const int NUM_SELECTX[] =	// X選択肢数
	{
		CGameManager::SIDE_MAX,		// 名前
		-1,							// 着せ替え
		CGameManager::SIDE_MAX,		// ポジション
		CEntry_Dressup::TRANS_MAX,	// 遷移
	};

	namespace pad
	{
		const std::string TEXTURE	= "data\\TEXTURE\\entry\\playerMarker001.png";	// コントローラーUIテクスチャ
		const MyLib::PosGrid2 PTRN	= MyLib::PosGrid2(4, 1);	// テクスチャ分割数
		const float NAME_WIDTH	= 22.0f;	// 名前選択時の横幅
		const float DRESS_WIDTH	= 55.0f;	// 着せ替え時の横幅
		const float OTHER_WIDTH	= 18.0f;	// 他選択時の横幅
		const MyLib::Vector3 NAME_OFFSET	= MyLib::Vector3(0.0f, -60.0f, 0.0f);	// 名前オフセット
		const MyLib::Vector3 DRESS_OFFSET	= MyLib::Vector3(0.0f, -140.0f, 0.0f);	// 着せ替えオフセット
	}

	namespace frame
	{
		namespace name
		{
			const float PLUS_EDGE = 10.0f;	// フレーム縁取り拡大
		}
		namespace dress
		{
			const MyLib::Vector3 OFFSET	= MyLib::Vector3(0.0f, 55.0f, 0.0f);	// オフセット
			const MyLib::Vector2 SIZE	= MyLib::Vector2(110.0f, 200.0f);			// 大きさ
		}
		namespace area
		{
			const float PLUS_EDGE	= 10.0f;	// フレーム縁取り拡大
			const float OFFSET_Y	= -58.0f;	// Yオフセット
		}
		namespace trans
		{
			const float PLUS_EDGE = 10.0f;	// フレーム縁取り拡大
			const float OFFSET_Y = -58.0f;	// Yオフセット
		}
	}
}

//************************************************************
//	関数ポインタ
//************************************************************
// 選択関数
CSelectUI::SELECT_FUNC CSelectUI::m_SelectFuncList[] =
{
	&CSelectUI::UpdateName,		// 名前の更新
	&CSelectUI::UpdateDressup,	// 着せ替えの更新
	&CSelectUI::UpdateArea,		// ポジションの更新
	&CSelectUI::UpdateTrans,	// 遷移の更新
};

//************************************************************
//	子クラス [CSelectUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSelectUI::CSelectUI(CGameManager::ETeamSide team, const int nPlayerIdx, const int nPadIdx) : CObject(PRIO_BG, LAYER::LAYER_2D),
	m_select	(MyLib::PosGrid2(nPlayerIdx, SELECT_DRESSUP)),	// 選択インデックス
	m_pPadUI	(nullptr),			// コントローラーUI情報
	m_pFrame	(nullptr),			// フレーム情報
	m_nPadIdx	(nPadIdx),			// 操作権インデックス
	m_bSelect	(true),				// 選択操作フラグ
	m_team		(team)				// チーム
{
	// スタティックアサート
	static_assert(NUM_ARRAY(m_SelectFuncList) == CSelectUI::SELECT_MAX, "ERROR : Select Count Mismatch");
	static_assert(NUM_ARRAY(NUM_SELECTX)	  == CSelectUI::SELECT_MAX, "ERROR : Select Count Mismatch");
}

//============================================================
//	デストラクタ
//============================================================
CSelectUI::~CSelectUI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSelectUI::Init()
{
	// UIの生成
	if (FAILED(CreateUI()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類をオブジェクト2Dにする
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSelectUI::Uninit()
{
	// 既に破棄済みの場合抜ける
	if (IsDeath()) { return; }

	// コントローラーUIの終了
	SAFE_UNINIT(m_pPadUI);

	// フレームの終了
	SAFE_UNINIT(m_pFrame);

	// オブジェクトを破棄
	Release();
}

//============================================================
//	削除処理
//============================================================
void CSelectUI::Kill()
{
	// 自身の終了
	CSelectUI::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CSelectUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 選択の更新
	UpdateSelect();

	// 相対位置の設定
	SetPositionRelative();

	if (m_SelectFuncList[m_select.y] != nullptr)
	{ // 選択更新関数がある場合

		// 選択別処理
		(this->*(m_SelectFuncList[m_select.y]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//============================================================
//	描画処理
//============================================================
void CSelectUI::Draw()
{

}

//============================================================
//	描画状況の設定処理
//============================================================
void CSelectUI::SetEnableDisp(const bool bDraw)
{
	// 自身の描画状況を設定
	CObject::SetEnableDisp(bDraw);

	// コントローラーUIの描画状況を設定
	m_pPadUI->SetEnableDisp(bDraw);

	// フレームの描画状況を設定
	m_pFrame->SetEnableDisp(bDraw);
}

//============================================================
//	位置の設定処理
//============================================================
void CSelectUI::SetPosition(const MyLib::Vector3& pos)
{
	// 設定する位置を保存
	CObject::SetPosition(pos);

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	生成処理
//============================================================
CSelectUI *CSelectUI::Create(CGameManager::ETeamSide team, const int nPlayerIdx, const int nPadIdx, const MyLib::Vector3& pos)
{
	// 選択UIの生成
	CSelectUI* pSelectUI = DEBUG_NEW CSelectUI(team, nPlayerIdx, nPadIdx);
	if (pSelectUI == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 選択UIの初期化
		if (FAILED(pSelectUI->Init()))
		{ // 初期化に失敗した場合

			// 選択UIの破棄
			SAFE_DELETE(pSelectUI);
			return nullptr;
		}

		// UI位置を反映
		pSelectUI->SetPosition(pos);

		// 確保したアドレスを返す
		return pSelectUI;
	}
}

//============================================================
//	名前の更新処理
//============================================================
void CSelectUI::UpdateName(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 着せ替えシーンの取得
	CEntry* pEntry = CEntry::GetInstance();						// エントリーモード情報
	if (pEntry == nullptr) { assert(false); return; }			// エントリーモードがない場合抜ける
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// エントリーシーン情報
	if (pEntryScene == nullptr) { assert(false); return; }		// エントリーシーンがない場合抜ける
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// 着せ替えシーン情報
	if (pDressup == nullptr) { assert(false); return; }			// 着せ替えシーンがない場合抜ける

	// X選択肢をチームインデックスにする
	m_select.x = m_team;

	// 選択操作ができない場合抜ける
	if (!IsSelectOK()) { return; }

	// 既に名前入力中の場合抜ける
	if (CInputKeyButton::GetInstance() != nullptr) { return; }

	// 決定の更新
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報
	if (pPad->GetTrigger(CInputGamepad::BUTTON_A, m_nPadIdx))
	{
		// 名前変更クラスの生成
		CString2D* pName = pDressup->GetNameString2D(m_team);	// 名前文字列
		CInputKeyButton::Create(m_nPadIdx, pName->GetStr(), pName);
	}
}

//============================================================
//	着せ替えの更新処理
//============================================================
void CSelectUI::UpdateDressup(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsSelectOK())
	{ // 選択操作中の場合

		// 選択プレイヤーの更新
		UpdateSelectPlayer();

		// 決定の更新
		UpdateDecideDressup();
	}
	else
	{ // 着せ替え操作中の場合

		// キャンセルの更新
		UpdateCancelDressup();
	}

	// 着せ替えシーンの取得
	CEntry* pEntry = CEntry::GetInstance();						// エントリーモード情報
	if (pEntry == nullptr) { assert(false); return; }			// エントリーモードがない場合抜ける
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// エントリーシーン情報
	if (pEntryScene == nullptr) { assert(false); return; }		// エントリーシーンがない場合抜ける
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// 着せ替えシーン情報
	if (pDressup == nullptr) { assert(false); return; }			// 着せ替えシーンがない場合抜ける

	// UI位置を反映
	SetPosition(pDressup->GetDressUIPosition(m_select.x));
}

//============================================================
//	ポジションの更新処理
//============================================================
void CSelectUI::UpdateArea(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 着せ替えシーンの取得
	CEntry* pEntry = CEntry::GetInstance();						// エントリーモード情報
	if (pEntry == nullptr) { assert(false); return; }			// エントリーモードがない場合抜ける
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// エントリーシーン情報
	if (pEntryScene == nullptr) { assert(false); return; }		// エントリーシーンがない場合抜ける
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// 着せ替えシーン情報
	if (pDressup == nullptr) { assert(false); return; }			// 着せ替えシーンがない場合抜ける

	// 選択操作ができない場合抜ける
	if (!IsSelectOK()) { return; }

	// X選択の更新
	UpdateSelectX(m_select.y);

	// 決定の更新
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報
	if (pPad->GetTrigger(CInputGamepad::BUTTON_A, m_nPadIdx))
	{
		// 着せ替えUIのポジション変更
		pDressup->ChangeDressUIArea((CGameManager::ETeamSide)m_select.x);
	}
}

//============================================================
//	遷移の更新処理
//============================================================
void CSelectUI::UpdateTrans(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 着せ替えシーンの取得
	CEntry* pEntry = CEntry::GetInstance();						// エントリーモード情報
	if (pEntry == nullptr) { assert(false); return; }			// エントリーモードがない場合抜ける
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// エントリーシーン情報
	if (pEntryScene == nullptr) { assert(false); return; }		// エントリーシーンがない場合抜ける
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// 着せ替えシーン情報
	if (pDressup == nullptr) { assert(false); return; }			// 着せ替えシーンがない場合抜ける

	// 選択操作ができない場合抜ける
	if (!IsSelectOK()) { return; }

	// X選択の更新
	UpdateSelectX(m_select.y);

	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報
	switch (m_select.x)
	{ // X選択ごとの処理
	case CEntry_Dressup::TRANS_BACK:	// 戻る選択
	{
		// 決定の更新
		if (pPad->GetTrigger(CInputGamepad::BUTTON_A, m_nPadIdx))
		{
			// チーム設定シーンへ遷移
			pEntry->ChangeEntryScene(CEntry::ESceneType::SCENETYPE_SETUPTEAM);
		}
		break;
	}
	case CEntry_Dressup::TRANS_NEXT:	// 進む選択
	{
		// 決定の更新
		if (pPad->GetTrigger(CInputGamepad::BUTTON_A, m_nPadIdx))
		{
			// ゲーム設定遷移
			pDressup->TransSetting();
		}
		break;
	}
	default:
		assert(false);
		break;
	}
}

//============================================================
//	UIの生成処理
//============================================================
HRESULT CSelectUI::CreateUI()
{
	// フレームの生成
	m_pFrame = CObject2D::Create(PRIO_BG);
	if (m_pFrame == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 大きさの設定
	m_pFrame->SetSize(frame::dress::SIZE);

	// コントローラーUIの生成
	m_pPadUI = CObject2D_Anim::Create
	( // 引数
		VEC3_ZERO,		// 位置
		pad::PTRN.x,	// テクスチャ横分割数
		pad::PTRN.y,	// テクスチャ縦分割数
		0.0f,			// 再生時間
		false,			// 自動破棄
		PRIO_UI			// 優先順位
	);
	if (m_pPadUI == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 自動再生をOFFにする
	m_pPadUI->SetEnableAutoPlay(false);

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist(pad::TEXTURE);
	m_pPadUI->BindTexture(nTexID);

	// テクスチャパターンの設定
	m_pPadUI->SetPatternAnim(m_nPadIdx);

	// 横幅を元にサイズを設定
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, pad::OTHER_WIDTH);
	size.y *= (float)pad::PTRN.x;
	m_pPadUI->SetSize(size);
	m_pPadUI->SetSizeOrigin(m_pPadUI->GetSize());

	return S_OK;
}

//============================================================
//	選択の更新処理
//============================================================
void CSelectUI::UpdateSelect()
{
	// 選択操作ができない場合抜ける
	if (!IsSelectOK()) { return; }

	// 選択の更新
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報
	if (pPad->GetTrigger(CInputGamepad::BUTTON_UP, m_nPadIdx))
	{
		// 着せ替えシーンの取得
		CEntry* pEntry = CEntry::GetInstance();						// エントリーモード情報
		if (pEntry == nullptr) { assert(false); return; }			// エントリーモードがない場合抜ける
		CEntryScene* pEntryScene = pEntry->GetEntryScene();			// エントリーシーン情報
		if (pEntryScene == nullptr) { assert(false); return; }		// エントリーシーンがない場合抜ける
		CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// 着せ替えシーン情報
		if (pDressup == nullptr) { assert(false); return; }			// 着せ替えシーンがない場合抜ける

		// これ以上上に行けない場合抜ける
		if (m_select.y <= 0) { return; }

		// Y移動時のX補正位置が取得できない場合抜ける
		int nSelectX = GetMoveYSelectX(m_select.y - 1);
		if (nSelectX == -1) { return; }

		// 誰かが名前にいる場合抜ける
		if (m_select.y - 1 == SELECT_NAME && !pDressup->IsNameSelectOK(m_team)) { return; }

		// X選択位置を補正
		m_select.x = nSelectX;

		// 上に移動
		m_select.y--;
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON_DOWN, m_nPadIdx))
	{
		// これ以上下に行けない場合抜ける
		if (m_select.y >= SELECT_MAX - 1) { return; }

		// Y移動時のX補正位置の取得
		int nSelectX = m_select.x = GetMoveYSelectX(m_select.y + 1);
		if (nSelectX == -1) { return; }

		// X選択位置を補正
		m_select.x = nSelectX;

		// 下に移動
		m_select.y++;
	}
}

//============================================================
//	決定の更新処理
//============================================================
void CSelectUI::UpdateDecideDressup()
{
	// 着せ替えシーンの取得
	CEntry* pEntry = CEntry::GetInstance();						// エントリーモード情報
	if (pEntry == nullptr) { assert(false); return; }			// エントリーモードがない場合抜ける
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// エントリーシーン情報
	if (pEntryScene == nullptr) { assert(false); return; }		// エントリーシーンがない場合抜ける
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// 着せ替えシーン情報
	if (pDressup == nullptr) { assert(false); return; }			// 着せ替えシーンがない場合抜ける

	// 決定の更新
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報
	if (pPad->GetTrigger(CInputGamepad::BUTTON_A, m_nPadIdx))
	{
		// 入力情報の初期化
		pPad->InitTrigger(m_nPadIdx);

		// 着せ替えUI準備完了フラグの初期化
		pDressup->SetDressUIReady(m_select.x, false);

		// 着せ替えUI操作権の設定
		pDressup->SetDressUIControl(m_nPadIdx, m_select.x);

		// 選択操作を停止
		m_bSelect = false;
	}
}

//============================================================
//	キャンセルの更新処理
//============================================================
void CSelectUI::UpdateCancelDressup()
{
	// 着せ替えシーンの取得
	CEntry* pEntry = CEntry::GetInstance();						// エントリーモード情報
	if (pEntry == nullptr) { assert(false); return; }			// エントリーモードがない場合抜ける
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// エントリーシーン情報
	if (pEntryScene == nullptr) { assert(false); return; }		// エントリーシーンがない場合抜ける
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// 着せ替えシーン情報
	if (pDressup == nullptr) { assert(false); return; }			// 着せ替えシーンがない場合抜ける

	// 準備完了済みの場合操作不可
	if (pDressup->IsDressUIReady(m_select.x)) { return; }

	// キャンセルの更新
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報
	if (pPad->GetTrigger(CInputGamepad::BUTTON_B, m_nPadIdx))
	{
		// 着せ替えUI操作権の初期化
		pDressup->SetDressUIControl(-1, m_select.x);

		// 選択操作を再開
		m_bSelect = true;
	}
}

//============================================================
//	選択プレイヤーの更新処理
//============================================================
void CSelectUI::UpdateSelectPlayer()
{
	// 着せ替えシーンの取得
	CEntry* pEntry = CEntry::GetInstance();						// エントリーモード情報
	if (pEntry == nullptr) { assert(false); return; }			// エントリーモードがない場合抜ける
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// エントリーシーン情報
	if (pEntryScene == nullptr) { assert(false); return; }		// エントリーシーンがない場合抜ける
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// 着せ替えシーン情報
	if (pDressup == nullptr) { assert(false); return; }			// 着せ替えシーンがない場合抜ける

	// 選択の更新
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報
	if (pPad->GetTrigger(CInputGamepad::BUTTON_LEFT, m_nPadIdx))
	{
		const int nNumPlayer = pDressup->GetNumDressUI();	// プレイヤー数
		int nLoop = 0;	// ループ回数
		do {
			// 左に選択をずらす
			m_select.x = (m_select.x + (nNumPlayer - 1)) % nNumPlayer;

			// 選択が一周した場合抜ける
			nLoop++;
			if (nLoop >= nNumPlayer) { break; }

		} while (!pDressup->IsDressSelectOK(m_nPadIdx, m_select.x));
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON_RIGHT, m_nPadIdx))
	{
		const int nNumPlayer = pDressup->GetNumDressUI();	// プレイヤー数
		int nLoop = 0;	// ループ回数
		do {
			// 右に選択をずらす
			m_select.x = (m_select.x + 1) % nNumPlayer;

			// 選択が一周した場合抜ける
			nLoop++;
			if (nLoop >= nNumPlayer) { break; }

		} while (!pDressup->IsDressSelectOK(m_nPadIdx, m_select.x));
	}
}

//============================================================
//	相対位置の設定処理
//============================================================
void CSelectUI::SetPositionRelative()
{
	// 着せ替えシーンの取得
	CEntry* pEntry = CEntry::GetInstance();						// エントリーモード情報
	if (pEntry == nullptr) { assert(false); return; }			// エントリーモードがない場合抜ける
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// エントリーシーン情報
	if (pEntryScene == nullptr) { assert(false); return; }		// エントリーシーンがない場合抜ける
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// 着せ替えシーン情報
	if (pDressup == nullptr) { return; }						// 着せ替えシーンがない場合抜ける

	switch (m_select.y)
	{ // 縦選択ごとの処理
	case SELECT_NAME:
	{
		// フレーム情報の設定
		MyLib::Vector3 posName = pDressup->GetNameUIPosition(m_team);	// 名前の位置
		m_pFrame->SetPosition(posName);									// 位置設定
		m_pFrame->SetSize(pDressup->GetNameUISize(m_team) + frame::name::PLUS_EDGE);	// 大きさ設定

		// コントローラーUIの位置設定
		m_pPadUI->SetPosition(posName + pad::NAME_OFFSET);

		// コントローラーUIの大きさ設定
		SetWidthPadUI(pad::NAME_WIDTH);
		break;
	}
	case SELECT_DRESSUP:
	{
		MyLib::Vector3 posThis = GetPosition();	// 自身の位置

		// フレーム情報の設定
		m_pFrame->SetPosition(posThis + frame::dress::OFFSET);	// 位置設定
		m_pFrame->SetSize(frame::dress::SIZE);	// 大きさ設定

		// コントローラーUIの位置設定
		m_pPadUI->SetPosition(posThis + pad::DRESS_OFFSET);

		// コントローラーUIの大きさ設定
		SetWidthPadUI(pad::DRESS_WIDTH);
		break;
	}
	case SELECT_AREA:
	{
		// フレーム情報の設定
		const MyLib::Vector3 posArea = pDressup->GetAreaUIPosition((CPlayer::EFieldArea)m_select.x);	// ポジションUI位置
		const MyLib::Vector2 sizeArea = pDressup->GetAreaUISize((CPlayer::EFieldArea)m_select.x);		// ポジションUI大きさ
		m_pFrame->SetPosition(posArea);							// 位置設定
		m_pFrame->SetSize(sizeArea + frame::area::PLUS_EDGE);	// 大きさ設定

		// コントローラーUIの位置設定
		const float	fHalfWidthPad = m_pPadUI->GetSize().x;			// パッド横幅
		const float	fMaxWidth = sizeArea.x * 2.0f - fHalfWidthPad;	// 使用横幅
		const int	nPadIdx = std::abs(m_nPadIdx + (-(mylib_const::MAX_PLAYER - 1) * m_select.x));	// 計算用パッドインデックス
		const float	fOffset = (fMaxWidth / (float)mylib_const::MAX_PLAYER) * nPadIdx - (fMaxWidth * 0.5f) + fHalfWidthPad;	// 位置オフセット
		m_pPadUI->SetPosition(posArea + MyLib::Vector3(fOffset, frame::area::OFFSET_Y, 0.0f));

		// コントローラーUIの大きさ設定
		SetWidthPadUI(pad::OTHER_WIDTH);
		break;
	}
	case SELECT_TRANS:
	{
		// フレーム情報の設定
		const MyLib::Vector3 posTrans = pDressup->GetTransUIPosition((CEntry_Dressup::ETrans)m_select.x);	// 遷移UI位置
		const MyLib::Vector2 sizeTrans = pDressup->GetTransUISize((CEntry_Dressup::ETrans)m_select.x);		// 遷移UI大きさ
		m_pFrame->SetPosition(posTrans);						// 位置設定
		m_pFrame->SetSize(sizeTrans + frame::trans::PLUS_EDGE);	// 大きさ設定

		// コントローラーUIの位置設定
		const float fSide = (m_select.x == 0) ? 1.0f : -1.0f;	// オフセット開始方向
		const float fWidthPad = (m_pPadUI->GetSize().x * 2.0f);	// パッド横幅
		const float fOffset = (fWidthPad * m_nPadIdx) - sizeTrans.x + (fWidthPad * 0.5f);	// パッド位置オフセット
		m_pPadUI->SetPosition(posTrans + MyLib::Vector3(fOffset * fSide, frame::trans::OFFSET_Y, 0.0f));	// パッド位置反映

		// コントローラーUIの大きさ設定
		SetWidthPadUI(pad::OTHER_WIDTH);
		break;
	}
	default:
		assert(false);
		break;
	}
}

//============================================================
//	選択操作可能かの確認処理
//============================================================
bool CSelectUI::IsSelectOK() const
{
	// 着せ替えシーンの取得
	CEntry* pEntry = CEntry::GetInstance();							// エントリーモード情報
	if (pEntry == nullptr) { assert(false); return false; }			// エントリーモードがない場合抜ける
	CEntryScene* pEntryScene = pEntry->GetEntryScene();				// エントリーシーン情報
	if (pEntryScene == nullptr) { assert(false); return false; }	// エントリーシーンがない場合抜ける
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();		// 着せ替えシーン情報
	if (pDressup == nullptr) { assert(false); return false; }		// 着せ替えシーンがない場合抜ける

	// 選択操作が可能且つ、着せ替え状態の場合
	return (m_bSelect && pDressup->GetState() == CEntry_Dressup::STATE_DRESSUP);
}

//============================================================
//	パッドUIの大きさ設定処理
//============================================================
void CSelectUI::SetWidthPadUI(const float fWidth)
{
	// コントローラーUIの大きさ計算
	CTexture* pTexture = CTexture::GetInstance();								// テクスチャ情報
	MyLib::Vector2 size = pTexture->GetImageSize(m_pPadUI->GetIdxTexture());	// テクスチャ大きさ
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, fWidth);			// 横幅調整
	size.y *= (float)pad::PTRN.x;												// 縦幅調整

	// コントローラーUIの大きさ反映
	m_pPadUI->SetSize(size);
}

//============================================================
//	X選択肢数の取得処理
//============================================================
int CSelectUI::GetNumSelectX(const int nSelectY) const
{
	switch (nSelectY)
	{ // Y選択ごとの処理
	case SELECT_DRESSUP:
	{
		// 着せ替えシーンの取得
		CEntry* pEntry = CEntry::GetInstance();						// エントリーモード情報
		if (pEntry == nullptr) { assert(false); return -1; }		// エントリーモードがない場合抜ける
		CEntryScene* pEntryScene = pEntry->GetEntryScene();			// エントリーシーン情報
		if (pEntryScene == nullptr) { assert(false); return -1; }	// エントリーシーンがない場合抜ける
		CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// 着せ替えシーン情報
		if (pDressup == nullptr) { assert(false); return -1; }		// 着せ替えシーンがない場合抜ける

		// プレイヤー数を返す
		return pDressup->GetNumDressUI();
	}
	case SELECT_NAME:
	case SELECT_AREA:
	case SELECT_TRANS:
	{
		// 定数を返す
		return NUM_SELECTX[nSelectY];
	}
	default:
		assert(false);
		return -1;
	}
}

//============================================================
//	Y移動時のX補正位置の取得処理
//============================================================
int CSelectUI::GetMoveYSelectX(const int nNextSelectY)
{
	const int nCurNumX = GetNumSelectX(m_select.y);		// 現在のX選択数
	const int nNextNumX = GetNumSelectX(nNextSelectY);	// 次のX選択数
	const float fCurRate = (float)m_select.x / (float)nCurNumX;	// 選択割合

	if (nNextSelectY == SELECT_DRESSUP)
	{ // 次の選択が着せ替えの場合

		// 着せ替えシーンの取得
		CEntry* pEntry = CEntry::GetInstance();						// エントリーモード情報
		if (pEntry == nullptr) { assert(false); return -1; }		// エントリーモードがない場合抜ける
		CEntryScene* pEntryScene = pEntry->GetEntryScene();			// エントリーシーン情報
		if (pEntryScene == nullptr) { assert(false); return -1; }	// エントリーシーンがない場合抜ける
		CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// 着せ替えシーン情報
		if (pDressup == nullptr) { assert(false); return -1; }		// 着せ替えシーンがない場合抜ける

		int nNumDressUI = pDressup->GetNumDressUI();
		switch ((int)((float)CGameManager::SIDE_MAX * fCurRate))
		{ // チームごとの処理
		case CGameManager::SIDE_LEFT:

			for (int i = 0; i < nNumDressUI; i++)
			{ // 着せ替えUI数分左から繰り返す

				// 選択可能な着せ替えUIがあった場合返す
				if (pDressup->IsDressSelectOK(m_nPadIdx, i)) { return i; }
			}
			return -1;

		case CGameManager::SIDE_RIGHT:

			for (int i = nNumDressUI - 1; i >= 0; i--)
			{ // 着せ替えUI数分右から繰り返す

				// 選択可能な着せ替えUIがあった場合返す
				if (pDressup->IsDressSelectOK(m_nPadIdx, i)) { return i; }
			}
			return -1;

		default:
			assert(false);
			break;
		}
	}

	// 次のX選択位置を返す
	return (int)((float)nNextNumX * fCurRate);
}

//============================================================
//	X選択の更新処理
//============================================================
void CSelectUI::UpdateSelectX(const int nSelectY)
{
	// 選択の更新
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報
	if (pPad->GetTrigger(CInputGamepad::BUTTON_LEFT, m_nPadIdx))
	{
		// 左に選択をずらす
		const int nCurNumX = GetNumSelectX(nSelectY);	// 現在のX選択数
		m_select.x = (m_select.x + (nCurNumX - 1)) % nCurNumX;
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON_RIGHT, m_nPadIdx))
	{
		// 右に選択をずらす
		const int nCurNumX = GetNumSelectX(nSelectY);	// 現在のX選択数
		m_select.x = (m_select.x + 1) % nCurNumX;
	}
}

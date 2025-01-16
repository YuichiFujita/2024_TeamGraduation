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

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIO_BG = 4;	// 背景の優先順位
	const int PRIO_UI = 6;	// UIの優先順位

	namespace pad
	{
		const std::string TEXTURE = "data\\TEXTURE\\entry\\playerMarker000.png";	// コントローラーUIテクスチャ
		const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(4, 1);	// テクスチャ分割数
		const float WIDTH = 55.0f;	// 横幅
	}

	namespace frame
	{
		const MyLib::Vector2 SIZE = MyLib::Vector2(110.0f, 200.0f);	// 大きさ
	}
}

//************************************************************
//	子クラス [CSelectUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSelectUI::CSelectUI(const int nPlayerIdx, const int nPadIdx) : CObject(PRIO_BG, LAYER::LAYER_2D),
	m_pPadUI			(nullptr),		// コントローラーUI情報
	m_pFrame			(nullptr),		// フレーム情報
	m_nSelectPlayerIdx	(nPlayerIdx),	// 選択プレイヤーインデックス
	m_nPadIdx			(nPadIdx),		// 操作権インデックス
	m_bSelect			(true)			// 選択操作フラグ
{

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
	if (m_bSelect)
	{ // 選択操作中の場合

		// 選択の更新
		UpdateSelect();

		// 決定の更新
		UpdateDecide();
	}
	else
	{ // 着せ替え操作中の場合

		// キャンセルの更新
		UpdateCancel();
	}

	// 相対位置の設定
	SetPositionRelative();
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
CSelectUI *CSelectUI::Create(const int nPlayerIdx, const int nPadIdx, const MyLib::Vector3& pos)
{
	// 選択UIの生成
	CSelectUI* pSelectUI = DEBUG_NEW CSelectUI(nPlayerIdx, nPadIdx);
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
	m_pFrame->SetSize(frame::SIZE);

	// 種類の設定
	m_pFrame->SetType(CObject::TYPE::TYPE_OBJECT2D);

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
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, pad::WIDTH);
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
		const int nNumPlayer = pDressup->GetNumPlayer();	// プレイヤー数
		int nLoop = 0;	// ループ回数
		do {
			// 左に選択をずらす
			m_nSelectPlayerIdx = (m_nSelectPlayerIdx + (nNumPlayer - 1)) % nNumPlayer;

			// 選択が一周した場合抜ける
			nLoop++;
			if (nLoop >= nNumPlayer) { break; }

		} while (!pDressup->IsSelectOK(m_nPadIdx, m_nSelectPlayerIdx));

		// UI位置を反映
		SetPosition(pDressup->GetDressUIPosition(m_nSelectPlayerIdx));
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON_RIGHT, m_nPadIdx))
	{
		const int nNumPlayer = pDressup->GetNumPlayer();	// プレイヤー数
		int nLoop = 0;	// ループ回数
		do {
			// 右に選択をずらす
			m_nSelectPlayerIdx = (m_nSelectPlayerIdx + 1) % nNumPlayer;

			// 選択が一周した場合抜ける
			nLoop++;
			if (nLoop >= nNumPlayer) { break; }

		} while (!pDressup->IsSelectOK(m_nPadIdx, m_nSelectPlayerIdx));

		// UI位置を反映
		SetPosition(pDressup->GetDressUIPosition(m_nSelectPlayerIdx));
	}
}

//============================================================
//	決定の更新処理
//============================================================
void CSelectUI::UpdateDecide()
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
		pDressup->SetDressUIReady(m_nSelectPlayerIdx, false);

		// 着せ替えUI操作権の設定
		pDressup->SetDressUIControl(m_nPadIdx, m_nSelectPlayerIdx);

		// 選択操作を停止
		m_bSelect = false;
	}
}

//============================================================
//	キャンセルの更新処理
//============================================================
void CSelectUI::UpdateCancel()
{
	// 着せ替えシーンの取得
	CEntry* pEntry = CEntry::GetInstance();						// エントリーモード情報
	if (pEntry == nullptr) { assert(false); return; }			// エントリーモードがない場合抜ける
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// エントリーシーン情報
	if (pEntryScene == nullptr) { assert(false); return; }		// エントリーシーンがない場合抜ける
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// 着せ替えシーン情報
	if (pDressup == nullptr) { assert(false); return; }			// 着せ替えシーンがない場合抜ける

	// 準備完了済みの場合操作不可
	if (pDressup->IsDressUIReady(m_nSelectPlayerIdx)) { return; }

	// キャンセルの更新
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報
	if (pPad->GetTrigger(CInputGamepad::BUTTON_B, m_nPadIdx))
	{
		// 着せ替えUI操作権の初期化
		pDressup->SetDressUIControl(-1, m_nSelectPlayerIdx);

		// 選択操作を再開
		m_bSelect = true;
	}
}

//============================================================
//	相対位置の設定処理
//============================================================
void CSelectUI::SetPositionRelative()
{
	// 自身の位置を取得
	MyLib::Vector3 posThis = GetPosition();

	// コントローラーUIの位置設定
	m_pPadUI->SetPosition(posThis + MyLib::Vector3(0.0f, -155.0f, 0.0f));

	// フレームの位置設定
	m_pFrame->SetPosition(posThis + MyLib::Vector3(0.0f, 55.0f, 0.0f));
}

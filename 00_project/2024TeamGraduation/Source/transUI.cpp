//============================================================
//
//	遷移UI処理 [transUI.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "transUI.h"
#include "manager.h"
#include "object2D.h"
#include "entry.h"
#include "entryscene.h"
#include "entry_setupTeam.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY	= 6;		// UIの優先順位
	const float	MOVE_TIME	= 2.0f;		// 移動時間
	const float	MIN_ALPHA	= 0.35f;	// 最小透明度

	namespace gameset
	{
		const char* TEXTURE = "data\\TEXTURE\\entry\\PressStart_BG.png";	// 操作表記の背景
		const MyLib::Vector3 INIT_POS = MyLib::Vector3(-VEC3_SCREEN_SIZE.x, 360.0f, 0.0f);		// 初期位置
		const MyLib::Vector3 DEST_POS = MyLib::Vector3(VEC3_SCREEN_CENT.x, INIT_POS.y, 0.0f);	// 目標位置
		const float MOVE_TIME = 0.35f;	// 移動時間
		const float WIDTH = 450.0f;		// 横幅
	}

	namespace string
	{
		const char* TEXTURE	= "data\\TEXTURE\\entry\\PressStart.png";	// 操作表記テクスチャ
		const float HEIGHT	= 150.0f;	// 縦幅
		const float INIT_ALPHA	= 0.0f;	// 初期透明度
		const float DEST_ALPHA	= 1.0f;	// 目標透明度
		const float MOVE_TIME	= 0.4f;	// 移動時間
		const float WAIT_TIME	= 0.4f;	// 待機時間
		const MyLib::Vector2 INIT_SIZE = MyLib::Vector2(0.0f, 0.0f);	// 初期大きさ
		const MyLib::Vector3 POS = MyLib::Vector3(610.0f, 450.0f, 0.0f);	// 位置
	}
}

//************************************************************
//	関数ポインタ
//************************************************************
// 状態関数
CTransUI::STATE_FUNC CTransUI::m_StateFuncList[] =
{
	&CTransUI::UpdateDispOFF,		// 表示OFFの更新
	&CTransUI::UpdateSpawnBG,		// 背景生成の更新
	&CTransUI::UpdateSpawnString,	// 文字生成の更新
	&CTransUI::UpdateDispON,		// 表示ONの更新
};

//************************************************************
//	子クラス [CTransUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTransUI::CTransUI() : CObject(PRIORITY, LAYER::LAYER_2D),
	m_pBG		(nullptr),			// 背景情報
	m_pString	(nullptr),			// 文字情報
	m_state		(STATE_DISP_OFF),	// 状態
	m_fCurTime	(0.0f)				// 経過時間
{
	// スタティックアサート
	static_assert(NUM_ARRAY(m_StateFuncList) == CTransUI::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	デストラクタ
//============================================================
CTransUI::~CTransUI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTransUI::Init()
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
void CTransUI::Uninit()
{
	// 既に破棄済みの場合抜ける
	if (IsDeath()) { return; }

	// 背景の終了
	SAFE_UNINIT(m_pBG);

	// 文字の終了
	SAFE_UNINIT(m_pString);

	// オブジェクトを破棄
	Release();
}

//============================================================
//	削除処理
//============================================================
void CTransUI::Kill()
{
	// 自身の終了
	CTransUI::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CTransUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_StateFuncList[m_state] != nullptr)
	{ // 状態更新関数がある場合

		// 状態別処理
		(this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//============================================================
//	描画処理
//============================================================
void CTransUI::Draw()
{

}

//============================================================
//	描画状況の設定処理
//============================================================
void CTransUI::SetEnableDisp(const bool bDisp)
{
	// 自身の描画状況を設定
	CObject::SetEnableDisp(bDisp);

	// 背景の描画状況を設定
	m_pBG->SetEnableDisp(bDisp);

	// 文字の描画状況を設定
	m_pString->SetEnableDisp(bDisp);
}

//============================================================
//	生成処理
//============================================================
CTransUI *CTransUI::Create()
{
	// インスタンスの生成
	CTransUI* pTrans = DEBUG_NEW CTransUI;
	if (pTrans != nullptr)
	{ // 生成に成功した場合

		// クラスの初期化
		if (FAILED(pTrans->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pTrans);
			return nullptr;
		}
	}

	// インスタンスを返す
	return pTrans;
}

//============================================================
//	決定操作の更新処理
//============================================================
bool CTransUI::UpdateDecide(const bool bAutoUninit)
{
	CInputKeyboard*	pKey = GET_INPUTKEY;	// キーボード情報
	CInputGamepad*	pPad = GET_INPUTPAD;	// パッド情報
	bool bInput = false;	// 入力フラグ

	// セットアップシーンの取得
	CEntry* pEntry = CEntry::GetInstance();							// エントリーモード情報
	if (pEntry == nullptr) { assert(false); return false; }			// エントリーモードがない場合抜ける
	CEntryScene* pEntryScene = pEntry->GetEntryScene();				// エントリーシーン情報
	if (pEntryScene == nullptr) { assert(false); return false; }	// エントリーシーンがない場合抜ける
	CEntry_SetUpTeam* pSetupTeam = pEntryScene->GetSetupTeam();		// セットアップシーン情報
	if (pSetupTeam == nullptr) { assert(false); return false; }		// セットアップシーンがない場合抜ける

	// エントリー中プレイヤーインデックス取得
	std::vector<int> vecIdx = pSetupTeam->GetEntryPlayerIdx();
	for (const auto& rIdx : vecIdx)
	{
		// パッド入力結果を保存
		bInput = (bInput || pPad->GetTrigger(CInputGamepad::BUTTON_A, rIdx));
	}
	
#ifdef _DEBUG
	// キー入力結果を保存
	bInput = (bInput || pKey->GetTrigger(DIK_RETURN) || pKey->GetTrigger(DIK_SPACE));
#endif

	// キー入力がない場合抜ける
	if (!bInput) { return false; }

	// 表示されていない場合抜ける
	if (m_state == STATE_DISP_OFF) { return false; }

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // パッド数分繰り返す

		// トリガー入力初期化
		pPad->InitTrigger(i);
	}

	if (bAutoUninit)
	{ // 自動破棄が有効の場合

		// 自身の破棄
		Uninit();
	}

	return true;
}

//============================================================
//	UI表示状況の設定処理
//============================================================
void CTransUI::SetEnableDispUI(const bool bDisp)
{
	if (bDisp)
	{ // 表示を開始する場合

		if (!IsDispTransState()
		&&  m_state != STATE_DISP_ON)
		{ // 表示状態じゃない場合

			// UI表示状況の設定
			SetDisp(false);

			// 経過時間を初期化
			m_fCurTime = 0.0f;

			// 表示開始状態にする
			m_state = STATE_SPAWN_BG;

			PLAY_SOUND(CSound::ELabel::LABEL_SE_NEXT);
		}
	}
	else
	{ // 表示を終了する場合

		// UI表示状況の設定
		SetDisp(false);

		// 経過時間を初期化
		m_fCurTime = 0.0f;

		// 表示終了状態にする
		m_state = STATE_DISP_OFF;
	}
}

//============================================================
//	表示OFFの更新処理
//============================================================
void CTransUI::UpdateDispOFF(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//============================================================
//	背景生成の更新処理
//============================================================
void CTransUI::UpdateSpawnBG(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 経過時間を加算
	m_fCurTime += fDeltaTime * fSlowRate;

	// 背景の位置を移動
	MyLib::Vector3 posBG = UtilFunc::Correction::EaseOutBack(gameset::INIT_POS, gameset::DEST_POS, 0.0f, gameset::MOVE_TIME, m_fCurTime);
	m_pBG->SetPosition(posBG);

	if (m_fCurTime >= gameset::MOVE_TIME)
	{ // 経過しきった場合

		// 経過時間を初期化
		m_fCurTime = 0.0f;

		// 文字生成状態にする
		m_state = STATE_SPAWN_STRING;
	}
}

//============================================================
//	文字生成の更新処理
//============================================================
void CTransUI::UpdateSpawnString(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 経過時間を加算
	m_fCurTime += fDeltaTime * fSlowRate;

	// 縦幅を元に目標大きさを計算
	CTexture* pTexture = CTexture::GetInstance();	// テクスチャ情報
	MyLib::Vector2 sizeDest = pTexture->GetImageSize(pTexture->Regist(string::TEXTURE));	// 画像サイズを取得
	sizeDest = UtilFunc::Transformation::AdjustSizeByHeight(sizeDest, string::HEIGHT);		// 画像サイズを縦幅指定で調整

	// 文字を拡大
	MyLib::Vector2 sizeStr = UtilFunc::Correction::EaseOutBack(string::INIT_SIZE, sizeDest, 0.0f, string::MOVE_TIME, m_fCurTime);
	m_pString->SetSize(sizeStr);

	// 文字を不透明にする
	float fAlphaStr = UtilFunc::Correction::EasingCubicIn(string::INIT_ALPHA, string::DEST_ALPHA, 0.0f, string::MOVE_TIME, m_fCurTime);
	m_pString->SetAlpha(fAlphaStr);

	if (m_fCurTime >= string::MOVE_TIME + string::WAIT_TIME)
	{ // 経過しきった場合

		// 経過時間を初期化
		m_fCurTime = 0.0f;

		// 表示ON状態にする
		m_state = STATE_DISP_ON;
	}
}

//============================================================
//	表示ONの更新処理
//============================================================
void CTransUI::UpdateDispON(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	float fAlphaStr = 0.0f;	// 文字透明度

	// 経過時間を加算
	m_fCurTime += fDeltaTime * fSlowRate;
	if (m_fCurTime < MOVE_TIME * 0.5f)
	{ // 経過時間が半分未満の場合

		// 文字を透明度を計算
		fAlphaStr = UtilFunc::Correction::EasingEaseIn(string::DEST_ALPHA, MIN_ALPHA, 0.0f, MOVE_TIME * 0.5f, m_fCurTime);
	}
	else
	{ // 経過時間が半分以上の場合

		// 文字を透明度を計算
		fAlphaStr = UtilFunc::Correction::EasingEaseOut(MIN_ALPHA, string::DEST_ALPHA, MOVE_TIME * 0.5f, MOVE_TIME, m_fCurTime);
	}

	if (m_fCurTime > MOVE_TIME)
	{ // 経過しきった場合

		// 経過時間分減算
		m_fCurTime -= MOVE_TIME;
	}

	// 文字の透明度を設定
	m_pString->SetAlpha(fAlphaStr);
}

//============================================================
//	表示状況の設定処理
//============================================================
void CTransUI::SetDisp(const bool bDisp)
{
	if (bDisp)
	{ // 表示する場合

		// 縦幅を元に文字の目標大きさを計算
		CTexture* pTexture = CTexture::GetInstance();	// テクスチャ情報
		MyLib::Vector2 sizeDest = pTexture->GetImageSize(pTexture->Regist(string::TEXTURE));	// 画像サイズを取得
		sizeDest = UtilFunc::Transformation::AdjustSizeByHeight(sizeDest, string::HEIGHT);		// 画像サイズを縦幅指定で調整

		// 背景の位置の設定
		m_pBG->SetPosition(gameset::DEST_POS);

		// 文字の大きさの設定
		m_pString->SetSize(sizeDest);

		// 文字の透明度の設定
		m_pString->SetAlpha(string::DEST_ALPHA);
	}
	else
	{ // 表示しない場合

		// 背景の位置の設定
		m_pBG->SetPosition(gameset::INIT_POS);

		// 文字の大きさの設定
		m_pString->SetSize(string::INIT_SIZE);

		// 文字の透明度の設定
		m_pString->SetAlpha(string::INIT_ALPHA);
	}
}

//============================================================
//	表示遷移状態かの確認処理
//============================================================
bool CTransUI::IsDispTransState()
{
	// 表示状態かの確認
	return (m_state == STATE_SPAWN_BG || m_state == STATE_SPAWN_STRING);
}

//============================================================
//	UIの生成処理
//============================================================
HRESULT CTransUI::CreateUI()
{
	CTexture* pTexture = CTexture::GetInstance();	// テクスチャ情報

	// 背景の生成
	m_pBG = CObject2D::Create(PRIORITY);
	if (m_pBG == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// テクスチャの割当
	int texID = pTexture->Regist(gameset::TEXTURE);
	m_pBG->BindTexture(texID);

	// 横幅を元にサイズ計算
	MyLib::Vector2 size = pTexture->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, gameset::WIDTH);
	m_pBG->SetSize(size);

	// 文字の生成
	m_pString = CObject2D::Create(PRIORITY);
	if (m_pString == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// テクスチャの割当
	m_pString->BindTexture(pTexture->Regist(string::TEXTURE));

	// 位置の設定
	m_pString->SetPosition(string::POS);

	// UI表示を初期化
	SetDisp(false);

	return S_OK;
}

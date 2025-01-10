//============================================================
//
//	エントリールールマネージャー処理 [entryRuleManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "entryRuleManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"
#include "object2D_Anim.h"
#include "arrowUI.h"
#include "timeUI.h"
#include "fade.h"
#include "entry.h"
#include "entry_dressup.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *TEXTURE[] =	// テクスチャパス
	{
		"data\\TEXTURE\\entry\\Choices.png",		// ルールタイトルテクスチャ
		"data\\TEXTURE\\entry\\lifeRule.png",		// 撃破条件テクスチャ
		"data\\TEXTURE\\entry\\Battle_Start.png",	// 開始ボタンテクスチャ
		"data\\TEXTURE\\entry\\B_Back.png",			// 操作表示テクスチャ
		"data\\TEXTURE\\entry\\Arrow_Twin.png",		// 矢印テクスチャ
		"data\\TEXTURE\\entry\\flame_choice.png",	// 選択テクスチャ
	};

	const std::string SET_PATH = "data\\TEXT\\entry\\game_setting.txt";
	const int PRIORITY		= 6;	// エントリーの優先順位
	const int MAX_SELECT	= CEntryRuleManager::RULE_MAX + 1;	// ルール選択の総数 (完了操作も含む)
	const int SELECT_GAME	= CEntryRuleManager::RULE_MAX;		// ゲーム遷移選択番号

	// フェード情報
	namespace fade
	{
		const D3DXCOLOR INIT_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 初期化色
		const D3DXCOLOR SET_COL	 = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.84f);	// 設定色
		const float	ADD_ALPHA = 0.025f;	// 透明度の加算量
		const float	SUB_ALPHA = 0.025f;	// 透明度の減算量
	}

	// ルールタイトル情報
	namespace rule
	{
		const MyLib::PosGrid2	PART	= MyLib::PosGrid2(1, CEntryRuleManager::RULE_MAX);	// テクスチャ分割数
		const MyLib::Vector3	POS		= MyLib::Vector3(320.0f, 150.0f, 0.0f);		// 位置
		const MyLib::Vector2	SIZE	= MyLib::Vector2(588.0f, 130.0f) * 0.5f;	// 大きさ
		const MyLib::Vector3	SPACE	= MyLib::Vector3(0.0f, 140.0f, 0.0f);		// 空白
	}

	// 開始ボタン情報
	namespace start
	{
		const MyLib::Vector3 POS	= MyLib::Vector3(VEC3_SCREEN_CENT.x, 595.0f, 0.0f);	// 位置
		const MyLib::Vector2 SIZE	= MyLib::Vector2(484.0f, 112.5f) * 0.5f;			// 大きさ
	}

	// タイム情報
	namespace time
	{
		const MyLib::Vector3 POS		= MyLib::Vector3(930.0f, 150.0f, 0.0f);	// タイマー位置
		const MyLib::Vector2 VAL_SIZE	= MyLib::Vector2(40.0f, 40.0f);			// タイマー数字大きさ
		const MyLib::Vector2 PART_SIZE	= MyLib::Vector2(30.0f, 40.0f);			// タイマー区切り大きさ
		const MyLib::Vector2 VAL_SPACE	= MyLib::Vector2(40.0f, 0.0f);			// タイマー数字空白
		const MyLib::Vector2 PART_SPACE	= MyLib::Vector2(60.0f, 0.0f);			// タイマー区切り空白
	}

	// 体力情報
	namespace life
	{
		const MyLib::PosGrid2	PART	= MyLib::PosGrid2(1, CEntryRuleManager::LIFE_MAX);				// テクスチャ分割数
		const MyLib::Vector3	POS		= MyLib::Vector3(930.0f, time::POS.y + rule::SPACE.y, 0.0f);	// 位置
		const MyLib::Vector2	SIZE	= MyLib::Vector2(444.0f, 96.0f) * 0.5f;							// 大きさ
	}

	// 選択情報
	namespace select
	{
		const float	ADD_SIZE = 17.5f;	// 大きさ加算量

		const MyLib::Vector3	POS	= MyLib::Vector3(930.0f, rule::POS.y, 0.0f);	// 位置
		const D3DXCOLOR			COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);			// 色
		const MyLib::Vector2	SIZE_RULE	= MyLib::Vector2(life::SIZE.x + ADD_SIZE, life::SIZE.y + ADD_SIZE);		// ルール選択時の大きさ
		const MyLib::Vector2	SIZE_START	= MyLib::Vector2(start::SIZE.x + ADD_SIZE, start::SIZE.y + ADD_SIZE);	// 開始選択時の大きさ

		const D3DXCOLOR CHOICE_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 選択中カラー
		const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);	// 非選択中カラー
	}

	// 矢印情報
	namespace arrow
	{
		const float	ADD_ALPHA		= 0.02f;	// 透明度の加算量
		const float	ADD_SINROT		= 0.04f;	// 透明度ふわふわさせる際のサインカーブ向き加算量
		const float	MAX_ADD_ALPHA	= 0.25f;	// 透明度の最大加算量
		const float	BASIC_ALPHA		= 0.95f;	// 基準の透明度
		const float	SPACE_EDGE		= 40.0f;	// 縁の空白

		const MyLib::PosGrid2	PART	= MyLib::PosGrid2(MAX_RULE_ARROW, 1);	// テクスチャ分割数
		const float				SIZE	= 40.0f;								// 大きさ
		const MyLib::Vector3	POS		= MyLib::Vector3(select::POS.x - select::SIZE_RULE.x - SPACE_EDGE, select::POS.y, 0.0f);	// 位置
		const MyLib::Vector3	SPACE	= MyLib::Vector3(select::SIZE_RULE.x * 2.0f + (SPACE_EDGE * 2.0f), 0.0f, 0.0f);	// 空白
		const D3DXCOLOR			MIN_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, BASIC_ALPHA - MAX_ADD_ALPHA);	// 色
	}

	// 操作情報
	namespace control
	{
		const float	ADD_ALPHA		= 0.0045f;	// 透明度の加算量
		const float	ADD_SINROT		= 0.04f;	// 透明度ふわふわさせる際のサインカーブ向き加算量
		const float	MAX_ADD_ALPHA	= 0.45f;	// 透明度の最大加算量
		const float	BASIC_ALPHA		= 0.85f;	// 基準の透明度

		const MyLib::Vector3	POS		= MyLib::Vector3(1080.0f, 660.0f, 0.0f);	// 位置
		const MyLib::Vector2	SIZE	= MyLib::Vector2(302.0f, 73.0f) * 0.5f;		// 大きさ
		const D3DXCOLOR			MIN_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, BASIC_ALPHA - MAX_ADD_ALPHA);	// 色
	}
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TEXTURE) == CEntryRuleManager::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	親クラス [CEntryRuleManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEntryRuleManager::CEntryRuleManager(CEntry_Dressup* pParent) :
	m_pLife		 (nullptr),		// 体力の情報
	m_pTime		 (nullptr),		// 時間の情報
	m_pSelect	 (nullptr),		// 選択の情報
	m_pStart	 (nullptr),		// 開始ボタンの情報
	m_pControl	 (nullptr),		// 操作表示の情報
	m_pFade		 (nullptr),		// フェードの情報
	m_pParent	 (pParent),		// 親の情報
	m_state		 (STATE_INIT),	// 状態
	m_nSelect	 (RULE_TIME),	// 現在の選択
	m_nOldSelect (RULE_TIME),	// 前回の選択
	m_fSinControlAlpha	(0.0f),	// 操作表示の透明向き
	m_fSinArrowAlpha	(0.0f)	// 矢印表示の透明向き
{
	// メンバ変数をクリア
	memset(&m_apRuleTitle[0], 0, sizeof(m_apRuleTitle));	// ルールタイトルの情報
	memset(&m_apArrow[0], 0, sizeof(m_apArrow));			// 矢印の情報
}

//============================================================
//	デストラクタ
//============================================================
CEntryRuleManager::~CEntryRuleManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEntryRuleManager::Init()
{
	CTexture* pTexture = CTexture::GetInstance();	// テクスチャ情報

	// メンバ変数を初期化
	memset(&m_apRuleTitle[0], 0, sizeof(m_apRuleTitle));	// ルールタイトルの情報
	memset(&m_apArrow[0], 0, sizeof(m_apArrow));			// 矢印の情報
	m_pLife		 = nullptr;			// 体力の情報
	m_pTime		 = nullptr;			// 時間の情報
	m_pSelect	 = nullptr;			// 選択の情報
	m_pStart	 = nullptr;			// 開始ボタンの情報
	m_pControl	 = nullptr;			// 操作表示の情報
	m_pFade		 = nullptr;			// フェードの情報
	m_state		 = STATE_INIT;		// 状態
	m_nSelect	 = RULE_TIME;		// 現在の選択
	m_nOldSelect = RULE_TIME;		// 前回の選択
	m_fSinControlAlpha	= -HALF_PI;	// 操作表示の透明向き
	m_fSinArrowAlpha	= -HALF_PI;	// 矢印表示の透明向き

	// フェードの生成
	{
		m_pFade = CObject2D::Create(PRIORITY);
		if (m_pFade == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}

		// 位置を設定
		m_pFade->SetPosition(VEC3_SCREEN_CENT);

		// 大きさを設定
		m_pFade->SetSize(VEC2_SCREEN_SIZE);

		// 色を設定
		m_pFade->SetColor(fade::INIT_COL);
	}

	// ルールタイトルの生成
	{
		for (int i = 0; i < RULE_MAX; i++)
		{ // ルールの総数分繰り返す

			// ルールタイトルの生成
			m_apRuleTitle[i] = CObject2D_Anim::Create
			( // 引数
				rule::POS + ((float)i * rule::SPACE),	// 位置
				rule::PART.x,	// テクスチャ横分割数
				rule::PART.y,	// テクスチャ縦分割数
				0.0f,			// 待機時間
				false,			// 自動破棄
				PRIORITY		// 優先順位
			);
			if (m_apRuleTitle[i] == nullptr)
			{ // 生成に失敗した場合

				assert(false);
				return E_FAIL;
			}

			// テクスチャを登録・割当
			m_apRuleTitle[i]->BindTexture(pTexture->Regist(TEXTURE[TEXTURE_RULE_TITLE]));

			// 大きさを設定
			m_apRuleTitle[i]->SetSize(rule::SIZE);

			// パターンを設定
			m_apRuleTitle[i]->SetPatternAnim(i);

			// 自動再生をOFFにする
			m_apRuleTitle[i]->SetEnableAutoPlay(false);
		}
	}

	// 矢印の生成
	{
		for (int i = 0; i < MAX_RULE_ARROW; i++)
		{ // 矢印の総数分繰り返す

			// 矢印の生成
			m_apArrow[i] = CArrowUI::Create
			( // 引数
				(CArrowUI::EDirection)i,	// 方向
				arrow::POS + ((float)i * arrow::SPACE),	// 位置
				arrow::SIZE,							// サイズ
				MyLib::color::White(0.0f),				// 色
				PRIORITY		// 優先順位
			);
			if (m_apArrow[i] == nullptr)
			{ // 生成に失敗した場合

				assert(false);
				return E_FAIL;
			}
		}
	}

	// 選択の生成
	{
		m_pSelect = CObject2D::Create(PRIORITY);
		if (m_pSelect == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録・割当
		m_pSelect->BindTexture(pTexture->Regist(TEXTURE[TEXTURE_SELECT]));

		// 位置を設定
		m_pSelect->SetPosition(select::POS);

		// 大きさを設定
		m_pSelect->SetSize(select::SIZE_RULE);

		// 色を設定
		m_pSelect->SetColor(select::COL);
	}

	// 開始ボタンの生成
	{
		m_pStart = CObject2D::Create(PRIORITY);
		if (m_pStart == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録・割当
		m_pStart->BindTexture(pTexture->Regist(TEXTURE[TEXTURE_START]));

		// 位置を設定
		m_pStart->SetPosition(start::POS);

		// 大きさを設定
		m_pStart->SetSize(start::SIZE);

		// 色を設定
		m_pStart->SetColor(select::DEFAULT_COL);
	}

	// 操作表示の生成
	{
		m_pControl = CObject2D::Create(PRIORITY);
		if (m_pControl == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録・割当
		m_pControl->BindTexture(pTexture->Regist(TEXTURE[TEXTURE_CONTROL]));

		// 位置を設定
		m_pControl->SetPosition(control::POS);

		// 大きさを設定
		m_pControl->SetSize(control::SIZE);

		// 色を設定
		m_pControl->SetColor(MyLib::color::White(0.0f));
	}

	// 体力の生成
	{
		m_pLife = CObject2D_Anim::Create
		( // 引数
			life::POS,		// 位置
			life::PART.x,	// テクスチャ横分割数
			life::PART.y,	// テクスチャ縦分割数
			0.0f,			// 待機時間
			false,			// 自動破棄
			PRIORITY		// 優先順位
		);
		if (m_pLife == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録・割当
		m_pLife->BindTexture(pTexture->Regist(TEXTURE[TEXTURE_LIFE]));

		// 大きさを設定
		m_pLife->SetSize(life::SIZE);

		// 色を設定
		m_pLife->SetColor(select::DEFAULT_COL);

		// 自動再生をOFFにする
		m_pLife->SetEnableAutoPlay(false);
	}

	// 時間の生成
	{
		m_pTime = CTimeUI::Create
		( // 引数
			0.0f,				// 表示時間
			time::POS,			// 位置
			time::VAL_SIZE,		// 数字の大きさ
			time::PART_SIZE,	// 区切りの大きさ
			time::VAL_SPACE,	// 数字の空白
			time::PART_SPACE	// 区切りの空白
		);
		if (m_pTime == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}
	}

	// UIの透明度の設定
	SetAlphaUI(0.0f, false);

	// ゲーム設定の読込
	SRule rule;	// ルール
	if (FAILED(LoadSetting(&rule)))
	{ // 読込に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// パターンを設定
	m_pLife->SetPatternAnim(rule.life);

	// 時間を設定
	m_pTime->SetTime(rule.fTime);

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CEntryRuleManager::Uninit()
{
	for (int i = 0; i < RULE_MAX; i++)
	{ // ルールの総数分繰り返す

		// ルールタイトルの終了
		SAFE_UNINIT(m_apRuleTitle[i]);
	}

	for (int i = 0; i < MAX_RULE_ARROW; i++)
	{ // 矢印の総数分繰り返す

		// 矢印の終了
		SAFE_UNINIT(m_apArrow[i]);
	}

	// 選択の終了
	SAFE_UNINIT(m_pSelect);

	// 開始ボタンの終了
	SAFE_UNINIT(m_pStart);

	// 操作表示の終了
	SAFE_UNINIT(m_pControl);

	// 体力の終了
	SAFE_UNINIT(m_pLife);

	// 時間の終了
	SAFE_UNINIT(m_pTime);

	// フェードの終了
	SAFE_UNINIT(m_pFade);

	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CEntryRuleManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// フェード中の場合抜ける
	if (GET_MANAGER->GetFade()->GetState() != CFade::STATE_NONE) { return; }

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_INIT:

		// UIの自動描画をONにする
		SetEnableDispUI(true);

		// フェードイン状態にする
		m_state = STATE_FADEIN;

		// 処理を抜けずにフェードインに移行

	case STATE_FADEIN:

		// フェードインの更新
		UpdateFadeIn();
		break;

	case STATE_RULESET:

		// 選択操作
		Select();

		// 決定操作
		Decide();

		// ルール変更操作
		ChangeRule();

		// 操作更新
		UpdateControl();

		// 矢印更新
		UpdateArrow();
		break;

	case STATE_FADEOUT:

		// フェードアウトの更新
		UpdateFadeOut();
		break;

	case STATE_ENTRYBACK:

		// UIの自動描画をOFFにする
		SetEnableDispUI(false);

		// 着せ替え状態に戻る
		m_pParent->SetState(CEntry_Dressup::EState::STATE_DRESSUP);
		return;	// 関数を抜ける

	default:
		assert(false);
		break;
	}

	// TODO：自動
#if 0
	for (int i = 0; i < RULE_MAX; i++)
	{ // ルールの総数分繰り返す

		// ルールタイトルの更新
		m_apRuleTitle[i]->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	for (int i = 0; i < MAX_RULE_ARROW; i++)
	{ // 矢印の総数分繰り返す

		// 矢印の更新
		m_apArrow[i]->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 選択の更新
	m_pSelect->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 開始ボタンの更新
	m_pStart->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 操作表示の更新
	m_pControl->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 時間の更新
	m_pTime->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 体力の更新
	m_pLife->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// フェードの更新
	m_pFade->Update(fDeltaTime, fDeltaRate, fSlowRate);
#endif
}

//============================================================
//	ルール取得処理
//============================================================
CEntryRuleManager::SRule CEntryRuleManager::GetRule() const
{
	SRule rule;	// ルール

	// パターンを取得
	rule.life = (ELife)m_pLife->GetPatternAnim();

	// 時間を取得
	rule.fTime = m_pTime->GetTime();

	return rule;
}

//============================================================
//	生成処理
//============================================================
CEntryRuleManager* CEntryRuleManager::Create(CEntry_Dressup* pParent)
{
	// エントリールールマネージャーの生成
	CEntryRuleManager* pEntryRuleManager = DEBUG_NEW CEntryRuleManager(pParent);
	if (pEntryRuleManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// エントリールールマネージャーの初期化
		if (FAILED(pEntryRuleManager->Init()))
		{ // 初期化に失敗した場合

			// エントリールールマネージャーの破棄
			SAFE_DELETE(pEntryRuleManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pEntryRuleManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CEntryRuleManager::Release(CEntryRuleManager*& prEntryRuleManager)
{
	// エントリールールマネージャーの終了
	assert(prEntryRuleManager != nullptr);
	prEntryRuleManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prEntryRuleManager);
}

//============================================================
//	ゲーム設定の保存処理
//============================================================
HRESULT CEntryRuleManager::SaveSetting(SRule* pRule)
{
	// ファイルを開く
	std::ofstream file(SET_PATH);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ゲーム設定の書き出しに失敗！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// 見出しの書き出し
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	ゲーム設定テキスト [game_setting.txt]" << std::endl;
	file << "#	Author : 藤田 勇一" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;

	// ゲーム設定の書き出し
	file << "#------------------------------------------------------------------------------" << std::endl;
	file << "#	ゲーム設定情報" << std::endl;
	file << "#------------------------------------------------------------------------------" << std::endl;
	file << "GAMESET" << std::endl;
	file << "	TIME = " << pRule->fTime << std::endl;
	file << "	LIFE = " << pRule->life << std::endl;
	file << "END_GAMESET" << std::endl;

	// ファイルを閉じる
	file.close();

	return S_OK;
}

//============================================================
//	ゲーム設定の読込処理
//============================================================
HRESULT CEntryRuleManager::LoadSetting(SRule* pRule)
{
	// ファイルを開く
	std::ifstream file(SET_PATH);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ゲーム設定の読み込みに失敗！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// ファイルを読込
	std::string str;	// 読込文字列
	while (file >> str)
	{ // ファイルの終端ではない場合ループ

		if (str.front() == '#') { std::getline(file, str); }	// コメントアウト
		else if (str == "GAMESET")
		{
			do { // END_GAMESETを読み込むまでループ

				// 文字列を読み込む
				file >> str;

				if (str.front() == '#') { std::getline(file, str); }	// コメントアウト
				else if (str == "TIME")
				{
					file >> str;			// ＝を読込
					file >> pRule->fTime;	// 時間を読込
				}
				else if (str == "LIFE")
				{
					int nLife = 0;	// 体力
					file >> str;	// ＝を読込
					file >> nLife;	// 体力を読込

					// 列挙に変換
					pRule->life = (ELife)nLife;
				}
			} while (str != "END_GAMESET");	// END_GAMESETを読み込むまでループ
		}
	}

	// ファイルを閉じる
	file.close();

	return S_OK;
}

//============================================================
//	フェードインの更新処理
//============================================================
void CEntryRuleManager::UpdateFadeIn()
{
	float fAlphaFade = m_pFade->GetAlpha();	// フェード透明度

	// 透明度を加算
	fAlphaFade += fade::ADD_ALPHA;
	if (fAlphaFade >= fade::SET_COL.a)
	{ // 透明度が上がりきった場合

		// 透明度を補正
		fAlphaFade = fade::SET_COL.a;

		// ルール設定状態にする
		m_state = STATE_RULESET;
	}

	// UIの透明度の設定
	SetAlphaUI(fAlphaFade * 2.0f, false);

	// 透明度を反映
	m_pFade->SetAlpha(fAlphaFade);
}

//============================================================
//	フェードアウトの更新処理
//============================================================
void CEntryRuleManager::UpdateFadeOut()
{
	float fAlphaFade = m_pFade->GetAlpha();	// フェード透明度

	// 透明度を減算
	fAlphaFade -= fade::SUB_ALPHA;
	if (fAlphaFade <= fade::INIT_COL.a)
	{ // 透明度が下がりきった場合

		// 透明度を補正
		fAlphaFade = fade::INIT_COL.a;

		// エントリー戻し状態にする
		m_state = STATE_ENTRYBACK;
	}

	// UIの透明度の設定
	SetAlphaUI(fAlphaFade * 2.0f, true);

	// 透明度を反映
	m_pFade->SetAlpha(fAlphaFade);
}

//============================================================
//	操作の更新処理
//============================================================
void CEntryRuleManager::UpdateControl()
{
	float fAlphaControl = m_pControl->GetAlpha();	// 操作表示透明度
	if (fAlphaControl < control::MIN_COL.a)
	{ // 透明度が最低限より低い場合

		// 透明度を加算
		fAlphaControl += control::ADD_ALPHA;
		if (fAlphaControl > control::MIN_COL.a)
		{ // 透明度が超過した場合

			// 透明度を補正
			fAlphaControl = control::MIN_COL.a;
		}

		// 操作表示透明度を設定
		m_pControl->SetAlpha(fAlphaControl);
	}
	else
	{ // 透明度が最低限以上の場合

		// 透明度を上げる
		m_fSinControlAlpha += control::ADD_SINROT;
		UtilFunc::Transformation::RotNormalize(m_fSinControlAlpha);	// 向き正規化

		// 透明度加算量を求める
		float fAddAlpha = (control::MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinControlAlpha) - 1.0f);

		// 操作表示透明度を設定
		m_pControl->SetAlpha(control::BASIC_ALPHA + fAddAlpha);
	}
}

//============================================================
//	矢印の更新処理
//============================================================
void CEntryRuleManager::UpdateArrow()
{
	for (int i = 0; i < MAX_RULE_ARROW; i++)
	{ // 矢印の総数分繰り返す

		float fAlphaArrow = m_apArrow[i]->GetAlpha();	// 矢印透明度
		if (fAlphaArrow < arrow::MIN_COL.a)
		{ // 透明度が最低限より低い場合

			// 透明度を加算
			fAlphaArrow += arrow::ADD_ALPHA;
			if (fAlphaArrow > arrow::MIN_COL.a)
			{ // 透明度が超過した場合

				// 透明度を補正
				fAlphaArrow = arrow::MIN_COL.a;
			}

			// 矢印透明度を設定
			m_apArrow[i]->SetAlpha(fAlphaArrow);
		}
		else
		{ // 透明度が最低限以上の場合

			// 透明度を上げる
			m_fSinArrowAlpha += arrow::ADD_SINROT;
			UtilFunc::Transformation::RotNormalize(m_fSinArrowAlpha);	// 向き正規化

			// 透明度加算量を求める
			float fAddAlpha = (arrow::MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinArrowAlpha) - 1.0f);

			// 矢印透明度を設定
			m_apArrow[i]->SetAlpha(arrow::BASIC_ALPHA + fAddAlpha);
		}
	}
}

//============================================================
//	選択操作処理
//============================================================
void CEntryRuleManager::Select()
{
	CObject *apSelect[MAX_SELECT] =	// 選択オブジェクト
	{
		m_pTime,	// 時間
		m_pLife,	// 体力
		m_pStart	// 開始ボタン
	};
	CInputKeyboard* pKey = GET_INPUTKEY;	// キーボード
	CInputGamepad* pPad = GET_INPUTPAD;		// パッド
	MyLib::Vector3 posSelect = m_pSelect->GetPosition();	// 選択位置
	MyLib::Vector2 sizeSelect = m_pSelect->GetSize();		// 選択大きさ

	if (pKey->GetTrigger(DIK_W)
	||  pKey->GetTrigger(DIK_UP)
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_UP))
	{ // 上移動の操作が行われた場合

		// 上に選択をずらす
		m_nSelect = (m_nSelect + (MAX_SELECT - 1)) % MAX_SELECT;

		if (m_nSelect != SELECT_GAME)
		{ // 選択がゲーム遷移ではない場合

			// 位置を変更
			posSelect.x = select::POS.x;
			posSelect.y = select::POS.y + rule::SPACE.y * (float)m_nSelect;

			// 大きさを変更
			sizeSelect = select::SIZE_RULE;
			for (int i = 0; i < MAX_RULE_ARROW; i++)
			{ // 矢印の総数分繰り返す

				// 矢印の位置を変更
				MyLib::Vector3 setpos = MyLib::Vector3(arrow::POS.x + (arrow::SPACE.x * (float)i), arrow::POS.y + rule::SPACE.y * (float)m_nSelect, 0.0f);
				m_apArrow[i]->SetPosition(setpos);
				m_apArrow[i]->SetOriginPosition(setpos);

				// 自動描画をONにする
				m_apArrow[i]->SetEnableDisp(true);
			}
		}
		else
		{ // 選択がゲーム遷移の場合

			// 位置を変更
			posSelect = start::POS;

			// 大きさを変更
			sizeSelect = select::SIZE_START;
			for (int i = 0; i < MAX_RULE_ARROW; i++)
			{ // 矢印の総数分繰り返す

				// 自動描画をOFFにする
				m_apArrow[i]->SetEnableDisp(false);
			}
		}

		// サウンドの再生
		PLAY_SOUND(CSound::LABEL_SE_GRIP01);
	}

	if (pKey->GetTrigger(DIK_S)
	||  pKey->GetTrigger(DIK_DOWN)
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_DOWN))
	{ // 下移動の操作が行われた場合

		// 下に選択をずらす
		m_nSelect = (m_nSelect + 1) % MAX_SELECT;

		if (m_nSelect != SELECT_GAME)
		{ // 選択がゲーム遷移ではない場合

			// 位置を変更
			posSelect.x = select::POS.x;
			posSelect.y = select::POS.y + rule::SPACE.y * (float)m_nSelect;

			// 大きさを変更
			sizeSelect = select::SIZE_RULE;
			for (int i = 0; i < MAX_RULE_ARROW; i++)
			{ // 矢印の総数分繰り返す

				// 矢印の位置を変更
				MyLib::Vector3 setpos = MyLib::Vector3(arrow::POS.x + (arrow::SPACE.x * (float)i), arrow::POS.y + rule::SPACE.y * (float)m_nSelect, 0.0f);
				m_apArrow[i]->SetPosition(setpos);
				m_apArrow[i]->SetOriginPosition(setpos);


				// 自動描画をONにする
				m_apArrow[i]->SetEnableDisp(true);
			}
		}
		else
		{ // 選択がゲーム遷移の場合

			// 位置を変更
			posSelect = start::POS;

			// 大きさを変更
			sizeSelect = select::SIZE_START;
			for (int i = 0; i < MAX_RULE_ARROW; i++)
			{ // 矢印の総数分繰り返す

				// 自動描画をOFFにする
				m_apArrow[i]->SetEnableDisp(false);
			}
		}

		// サウンドの再生
		PLAY_SOUND(CSound::LABEL_SE_GRIP01);
	}

	// 前回の選択要素の色を黒に設定
	apSelect[m_nOldSelect]->SetColor(select::DEFAULT_COL);

	// 現在の選択要素の色を白に設定
	apSelect[m_nSelect]->SetColor(select::CHOICE_COL);

	// 現在の選択要素を代入
	m_nOldSelect = m_nSelect;

	// 選択位置を反映
	m_pSelect->SetPosition(posSelect);

	// 選択大きさを反映
	m_pSelect->SetSize(sizeSelect);
}

//============================================================
//	決定操作処理
//============================================================
void CEntryRuleManager::Decide()
{
	CInputKeyboard* pKey = GET_INPUTKEY;	// キーボード
	CInputGamepad* pPad = GET_INPUTPAD;		// パッド
	if (pKey->GetTrigger(DIK_RETURN)
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_A))
	{ // 決定の操作が行われた場合

		switch (m_nSelect)
		{ // 選択ごとの処理
		case RULE_TIME:
		case RULE_LIFE:
			break;

		case SELECT_GAME:

			// ルール変更反映
			SetRule();

			// 終了状態に戻る
			m_pParent->SetState(CEntry_Dressup::EState::STATE_END);

			// サウンドの再生
			PLAY_SOUND(CSound::LABEL_SE_PUSH);
			break;

		default:
			assert(false);
			break;
		}
	}

	if (pKey->GetTrigger(DIK_SPACE)
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_B))
	{ // 戻る操作が行われた場合

		// フェードアウト状態にする
		m_state = STATE_FADEOUT;

		// サウンドの再生
		PLAY_SOUND(CSound::LABEL_SE_GRIP01);
	}
}

//============================================================
//	ルール変更操作処理
//============================================================
void CEntryRuleManager::ChangeRule()
{
	CInputKeyboard* pKey = GET_INPUTKEY;	// キーボード
	CInputGamepad* pPad = GET_INPUTPAD;		// パッド
	int nLife = m_pLife->GetPatternAnim();	// 体力

	if (pKey->GetTrigger(DIK_A)
	||  pKey->GetTrigger(DIK_LEFT)
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_LEFT))
	{ // 左移動の操作が行われた場合

		// 選択時移動状態へ遷移
		m_apArrow[CArrowUI::EDirection::DIRECTION_L]->SetState(CArrowUI::EState::STATE_SELECTMOVE);

		switch (m_nSelect)
		{ // 選択ごとの処理
		case RULE_TIME:
		{
			float fCurTime = m_pTime->GetTime();	// 時間

			// 時間を減算
			m_pTime->SetTime(fCurTime - 15.0f);

			// サウンドの再生
			PLAY_SOUND(CSound::LABEL_SE_GRIP01);
			break;
		}
		case RULE_LIFE:
		{
			// 左に選択をずらす
			nLife = (nLife + (CEntryRuleManager::LIFE_MAX - 1)) % CEntryRuleManager::LIFE_MAX;

			// サウンドの再生
			PLAY_SOUND(CSound::LABEL_SE_GRIP01);
			break;
		}
		case SELECT_GAME:
		{
			break;
		}
		default:
			assert(false);
			break;
		}
	}
	if (pKey->GetTrigger(DIK_D)
	||  pKey->GetTrigger(DIK_RIGHT)
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_RIGHT))
	{ // 右移動の操作が行われた場合

		// 選択時移動状態へ遷移
		m_apArrow[CArrowUI::EDirection::DIRECTION_R]->SetState(CArrowUI::EState::STATE_SELECTMOVE);

		switch (m_nSelect)
		{ // 選択ごとの処理
		case RULE_TIME:
		{
			float fCurTime = m_pTime->GetTime();	// 時間

			// 時間を減算
			m_pTime->SetTime(fCurTime + 15.0f);

			// サウンドの再生
			PLAY_SOUND(CSound::LABEL_SE_GRIP01);
			break;
		}
		case RULE_LIFE:
		{
			// 右に選択をずらす
			nLife = (nLife + 1) % CEntryRuleManager::LIFE_MAX;

			// サウンドの再生
			PLAY_SOUND(CSound::LABEL_SE_GRIP01);
			break;
		}
		case SELECT_GAME:
		{
			break;
		}
		default:
			assert(false);
			break;
		}
	}

	// 体力を反映
	m_pLife->SetPatternAnim(nLife);
}

//============================================================
//	ルール変更反映処理
//============================================================
void CEntryRuleManager::SetRule()
{
	// TODO：外部ファイルに書き出す
#if 0
	// ポインタを宣言
	CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// データ保存情報

	// 勝利ポイント数を設定
	pRetention->SetWinPoint(m_pWinPoint->GetNum());

	// 撃破条件を設定
	pRetention->SetKillState((CRetentionManager::EKill)m_pKill->GetPattern());

	// 勝利条件を設定
	pRetention->SetWinState((CRetentionManager::EWin)m_pWin->GetPattern());
#endif
}

//============================================================
//	UIの描画状況の設定処理
//============================================================
void CEntryRuleManager::SetEnableDispUI(const bool bDisp)
{
	for (int i = 0; i < RULE_MAX; i++)
	{ // ルールの総数分繰り返す

		// ルールタイトルの描画を設定
		m_apRuleTitle[i]->SetEnableDisp(bDisp);
	}

	for (int i = 0; i < MAX_RULE_ARROW; i++)
	{ // 矢印の総数分繰り返す

		// 矢印の描画を設定
		m_apArrow[i]->SetEnableDisp(bDisp);
	}

	// 選択の描画を設定
	m_pSelect->SetEnableDisp(bDisp);

	// 開始ボタンの描画を設定
	m_pStart->SetEnableDisp(bDisp);

	// 操作表示の描画を設定
	m_pControl->SetEnableDisp(bDisp);

	// 体力の描画を設定
	m_pLife->SetEnableDisp(bDisp);

	// 時間の描画を設定
	m_pTime->SetEnableDisp(bDisp);
}

//============================================================
//	UIの透明度の設定処理
//============================================================
void CEntryRuleManager::SetAlphaUI(const float fAlpha, const bool bFadeOut)
{
	for (int i = 0; i < RULE_MAX; i++)
	{ // ルールの総数分繰り返す

		// ルールタイトルの透明度を設定
		m_apRuleTitle[i]->SetAlpha(fAlpha);
	}

	// 開始ボタンの透明度を設定
	m_pStart->SetAlpha(fAlpha);

	// 体力の透明度を設定
	m_pLife->SetAlpha(fAlpha);

	// 時間の透明度を設定
	m_pTime->SetAlpha(fAlpha);

	if (bFadeOut)
	{ // フェードアウト時の場合

		for (int i = 0; i < MAX_RULE_ARROW; i++)
		{ // 矢印の総数分繰り返す

			// 透明度情報を設定
			float fMaxAlpha = m_apArrow[i]->GetAlpha();	// 最大透明度
			float fSetAlpha = fAlpha;	// 設定透明度

			// 透明度を制限
			UtilFunc::Transformation::ValueNormalize(fSetAlpha, fMaxAlpha, 0.0f);

			// 矢印の透明度を設定
			m_apArrow[i]->SetAlpha(fSetAlpha);
		}

		// 選択の透明度を設定
		{
			// 透明度情報を設定
			float fMaxAlpha = m_pSelect->GetAlpha();	// 最大透明度
			float fSetAlpha = fAlpha;	// 設定透明度

			// 透明度を制限
			UtilFunc::Transformation::ValueNormalize(fSetAlpha, fMaxAlpha, 0.0f);

			// 選択の透明度を設定
			m_pSelect->SetAlpha(fSetAlpha);
		}

		// 操作表示の透明度を設定
		{
			// 透明度情報を設定
			float fMaxAlpha = m_pControl->GetAlpha();	// 最大透明度
			float fSetAlpha = fAlpha;	// 設定透明度

			// 透明度を制限
			UtilFunc::Transformation::ValueNormalize(fSetAlpha, fMaxAlpha, 0.0f);

			// 操作表示の透明度を設定
			m_pControl->SetAlpha(fSetAlpha);
		}
	}
}

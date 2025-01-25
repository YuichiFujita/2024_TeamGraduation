//============================================================
//
//	文字列入力処理 [inputKeyButton.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "inputKeyButton.h"
#include "manager.h"
#include "object2D.h"
#include "string2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* PATH_CHAR[] =	// 文字配置情報の相対パス
	{
		"data\\CSV\\char_hiragana.csv",	// ひらがな配置情報
		"data\\CSV\\char_katakana.csv",	// カタカナ配置情報
		"data\\CSV\\char_alphabet.csv",	// アルファベット配置情報
	};

	const char* STR_BUTTON[] =	// ボタンの文字列
	{
		"ひらがな",			// ひらがな
		"カタカナ",			// カタカナ
		"アルファベット",	// アルファベット
		"やめる",			// やめる
		"さくじょ",			// さくじょ
		"かくてい",			// かくてい
	};

	const char* PATH		= "data\\TEXT\\start.txt";	// テキストパス
	const int PRIORITY		= 7;	// 優先順位
	const int MAX_STR_NAME	= 10;	// 名前の最大文字数

	namespace title
	{	
		const char*	FONT	= "data\\FONT\\チョークS.otf";	// フォントパス
		const bool	ITALIC	= false;	// イタリック
		const float	HEIGHT	= 28.0f;	// 文字縦幅

		const MyLib::Vector3	POS = MyLib::Vector3(VEC3_SCREEN_CENT.x, 80.0f, 0.0f);	// 位置
		const MyLib::Vector3	ROT = VEC3_ZERO;		// 向き
		const D3DXCOLOR	COL = MyLib::color::White();	// 色
		const EAlignX	ALIGN_X = XALIGN_CENTER;		// 横配置
	}
	
	namespace name
	{
		const char*	FONT = "data\\FONT\\チョークS.otf";	// フォントパス
		const bool	ITALIC = false;	// イタリック
		const float	HEIGHT = 48.0f;	// 文字縦幅

		const MyLib::Vector3	POS = MyLib::Vector3(VEC3_SCREEN_CENT.x, 150.0f, 0.0f);	// 位置
		const MyLib::Vector3	ROT = VEC3_ZERO;		// 向き
		const D3DXCOLOR	COL = MyLib::color::White();	// 色
		const EAlignX	ALIGN_X = XALIGN_CENTER;		// 横配置
	}

	namespace select
	{	
		const MyLib::Vector3 POS[CInputKeyButton::YSELECT_MAX][CInputKeyButton::XSELECT_MAX] =	// 位置配列
		{
			{ MyLib::Vector3(180.0f + 145.0f, 560.0f, 0.0f), MyLib::Vector3(440.0f + 145.0f, 560.0f, 0.0f), MyLib::Vector3(770.0f + 145.0f, 560.0f, 0.0f) },
			{ MyLib::Vector3(220.0f + 145.0f, 630.0f, 0.0f), MyLib::Vector3(460.0f + 145.0f, 630.0f, 0.0f), MyLib::Vector3(720.0f + 145.0f, 630.0f, 0.0f) },
		};

		const CInputKeyButton::ETypeChar INIT_TYPE = CInputKeyButton::TYPECHAR_HIRAGANA;	// 初期文字セット
		const MyLib::PosGrid2 INIT_SELECT = MyLib::PosGrid2(0, 2);	// 初期選択位置

		const char*	FONT		= "data\\FONT\\チョークS.otf";	// フォントパス
		const bool	ITALIC		= false;	// イタリック
		const float	HEIGHT		= 28.0f;	// 文字縦幅
		const float	HEIGHT_BIG	= 34.0f;	// 文字縦幅
		const float	NEXT_TIME	= 0.035f;	// 文字振動の待機時間
		const float	MOVE		= 1.0f;		// 振動移動量
		const MyLib::Vector3 ROT	= VEC3_ZERO;				// 向き
		const D3DXCOLOR COL_DEFAULT	= MyLib::color::White();	// 通常色
		const D3DXCOLOR COL_CHOICE	= MyLib::color::Yellow();	// 選択色
		const EAlignX ALIGN_X		= XALIGN_CENTER;			// 横配置
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CInputKeyButton* CInputKeyButton::m_pInstance = nullptr;	// 自身のインスタンス

//************************************************************
//	子クラス [CInputKeyButton] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CInputKeyButton::CInputKeyButton(const int nPadIdx, const std::string& rOrigin, CString2D* pChange) :
	m_nPadIdx	(nPadIdx),				// 操作権インデックス
	m_sOrigin	(rOrigin),				// 元の文字列
	m_pChange	(pChange),				// 変更する文字列
	m_pBG		(nullptr),				// 背景
	m_pTitle	(nullptr),				// タイトル
	m_pName		(nullptr),				// 名前
	m_typeChar	((ETypeChar)0),			// 文字種類
	m_curSelect	(MyLib::PosGrid2()),	// 現在の選択肢
	m_oldSelect	(MyLib::PosGrid2())		// 前回の選択肢
{
	// 選択文字配列をクリア
	m_vecSelect.clear();
}

//============================================================
//	デストラクタ
//============================================================
CInputKeyButton::~CInputKeyButton()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CInputKeyButton::Init()
{
	// メンバ変数を初期化
	m_pBG		= nullptr;				// 背景
	m_pTitle	= nullptr;				// タイトル
	m_pName		= nullptr;				// 名前
	m_typeChar	= (ETypeChar)0;			// 文字種類
	m_curSelect	= select::INIT_SELECT;	// 現在の選択肢
	m_oldSelect	= select::INIT_SELECT;	// 前回の選択肢

	// 選択文字配列を初期化
	m_vecSelect.clear();

	// 背景の生成
	m_pBG = CObject2D::Create(6);
	if (m_pBG == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// 黒板テクスチャを割当
	CTexture* pTexture = CTexture::GetInstance();
	m_pBG->BindTexture(pTexture->Regist("data\\TEXTURE\\entry\\blackboard.png"));

	// 位置を設定
	m_pBG->SetPosition(VEC3_SCREEN_CENT);

	// 大きさを設定
	m_pBG->SetSize(VEC2_SCREEN_SIZE * 0.48f);

	// タイトルの生成
	m_pTitle = CString2D::Create
	( // 引数
		title::FONT,		// フォントパス
		title::ITALIC,		// イタリック
		L"チーム名",		// 指定文字列
		title::POS,			// 原点位置
		title::HEIGHT,		// 文字縦幅
		title::ALIGN_X,		// 横配置
		title::ROT,			// 原点向き
		title::COL,			// 色
		PRIORITY			// 優先順位
	);
	if (m_pTitle == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// 名前の生成
	m_pName = CString2D::Create
	( // 引数
		name::FONT,		// フォントパス
		name::ITALIC,	// イタリック
		m_sOrigin,		// 指定文字列
		name::POS,		// 原点位置
		name::HEIGHT,	// 文字縦幅
		name::ALIGN_X,	// 横配置
		name::ROT,		// 原点向き
		name::COL,		// 色
		PRIORITY		// 優先順位
	);
	if (m_pName == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	for (int i = 0; i < YSELECT_MAX; i++)
	{ // 行の固定選択肢の総数分繰り返す

		// 横一行分の配列を拡張
		m_vecSelect.emplace_back();

		for (int j = 0; j < XSELECT_MAX; j++)
		{ // 列の固定選択肢の総数分繰り返す

			// 選択肢の生成
			int nTextIdx = j + (i * XSELECT_MAX);	// テキストインデックス
			CString2D* pSelect = CString2D::Create
			( // 引数
				select::FONT,			// フォントパス
				select::ITALIC,			// イタリック
				STR_BUTTON[nTextIdx],	// 指定文字列
				select::POS[i][j],		// 原点位置
				select::HEIGHT_BIG,		// 文字縦幅
				select::ALIGN_X,		// 横配置
				select::ROT,			// 原点向き
				select::COL_DEFAULT,	// 色
				PRIORITY				// 優先順位
			);
			if (pSelect == nullptr)
			{ // 生成に失敗した場合

				assert(false);
				return E_FAIL;
			}

			// 現在の行列の最後尾に生成した文字を追加
			m_vecSelect.back().push_back(pSelect);
		}
	}

	// 配置の読込
	if (FAILED(ChangeChar(select::INIT_TYPE)))
	{ // 読込に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// 世界停止中に動けるようにする
	SetEnablePosibleMove_WorldPause(true);

	// 世界の時を止める
	GET_MANAGER->SetEnableWorldPaused(true);

	// マネージャー種類に変更
	SetType(CObject::TYPE::TYPE_MANAGER);

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CInputKeyButton::Uninit()
{
	// 自身の保存インスタンスを初期化
	m_pInstance = nullptr;

	// 世界の時を動かす
	GET_MANAGER->SetEnableWorldPaused(false);

	// 背景の終了
	SAFE_UNINIT(m_pBG);

	// タイトルの終了
	SAFE_UNINIT(m_pTitle);

	// 名前の終了
	SAFE_UNINIT(m_pName);

	for (int i = 0; i < (int)m_vecSelect.size(); i++)
	{ // 行の総数分繰り返す

		for (int j = 0; j < (int)m_vecSelect[i].size(); j++)
		{ // 列の総数分繰り返す

			// 選択文字の終了
			SAFE_UNINIT(m_vecSelect[i][j]);
		}
	}

	// 選択文字配列をクリア
	m_vecSelect.clear();

	// オブジェクトの破棄
	Release();
}

//============================================================
//	削除処理
//============================================================
void CInputKeyButton::Kill()
{
	// 自身の終了
	CInputKeyButton::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CInputKeyButton::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 選択の更新
	UpdateSelect();

	// 決定の更新
	UpdateDecide();

	// キャンセルの更新
	UpdateChancel();

	if (m_pBG != nullptr)
	{
		// 背景の更新
		m_pBG->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	if (m_pTitle != nullptr)
	{
		// タイトルの更新
		m_pTitle->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	if (m_pName != nullptr)
	{
		// 名前の更新
		m_pName->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	for (int i = 0; i < (int)m_vecSelect.size(); i++)
	{ // 行の総数分繰り返す

		for (int j = 0; j < (int)m_vecSelect[i].size(); j++)
		{ // 列の総数分繰り返す

			if (m_vecSelect[i][j] != nullptr)
			{
				// 選択文字の更新
				m_vecSelect[i][j]->Update(fDeltaTime, fDeltaRate, fSlowRate);
			}
		}
	}
}

//============================================================
//	描画処理
//============================================================
void CInputKeyButton::Draw()
{

}

//============================================================
//	生成処理
//============================================================
CInputKeyButton* CInputKeyButton::Create
(
	const int nPadIdx,			// 操作権インデックス
	const std::string& rOrigin,	// 変更前の文字列
	CString2D* pChange			// 変更する文字列
)
{
	// インスタンスの生成
	assert(m_pInstance == nullptr);
	m_pInstance = DEBUG_NEW CInputKeyButton(nPadIdx, rOrigin, pChange);
	if (m_pInstance != nullptr)
	{ // 生成に成功した場合

		// クラスの初期化
		if (FAILED(m_pInstance->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(m_pInstance);
			return nullptr;
		}
	}

	// インスタンスを返す
	return m_pInstance;
}

//============================================================
//	インスタンス取得処理
//============================================================
CInputKeyButton* CInputKeyButton::GetInstance()
{
	// インスタンスを返す
	return m_pInstance;
}

//============================================================
//	選択の操作処理
//============================================================
void CInputKeyButton::ControlSelect()
{
	CInputKeyboard*	pKey = GET_INPUTKEY;	// キーボード情報
	CInputGamepad*	pPad = GET_INPUTPAD;	// パッド情報
	if (pKey->GetTrigger(DIK_LEFT) || pPad->GetTrigger(CInputGamepad::BUTTON_LEFT, m_nPadIdx))
	{
		do { // 選択先がない場合さらに動かす

			// 左に選択をずらす
			int nMaxWidth = (int)m_vecSelect[m_curSelect.y].size();	// 列の最大数
			m_curSelect.x = (m_curSelect.x + (nMaxWidth - 1)) % nMaxWidth;

		} while (m_vecSelect[m_curSelect.y][m_curSelect.x] == nullptr);
	}
	if (pKey->GetTrigger(DIK_RIGHT) || pPad->GetTrigger(CInputGamepad::BUTTON_RIGHT, m_nPadIdx))
	{
		do { // 選択先がない場合さらに動かす

			// 右に選択をずらす
			int nMaxWidth = (int)m_vecSelect[m_curSelect.y].size();	// 列の最大数
			m_curSelect.x = (m_curSelect.x + 1) % nMaxWidth;

		} while (m_vecSelect[m_curSelect.y][m_curSelect.x] == nullptr);
	}
	if (pKey->GetTrigger(DIK_UP) || pPad->GetTrigger(CInputGamepad::BUTTON_UP, m_nPadIdx))
	{
		do { // 選択先がない場合さらに動かす

			// 上に選択をずらす
			int nMaxHeight = (int)m_vecSelect.size();	// 行の最大数
			m_curSelect.y = (m_curSelect.y + (nMaxHeight - 1)) % nMaxHeight;

			// 列が増減した場合に列インデックスを補正
			int nPrevSelectY = (m_curSelect.y + 1) % nMaxHeight;
			if (m_vecSelect[m_curSelect.y].size() < m_vecSelect[nPrevSelectY].size())
			{ // 列が減少した場合

				// 列インデックスを小さい方の最大値で補正
				m_curSelect.x /= m_vecSelect[m_oldSelect.y].size() / XSELECT_MAX;
				UtilFunc::Transformation::ValueNormalize(m_curSelect.x, (int)(m_vecSelect[m_curSelect.y].size() - 1), 0);
			}
			else if (m_vecSelect[m_curSelect.y].size() > m_vecSelect[nPrevSelectY].size())
			{ // 列が増加した場合

				// 列インデックスを大きい方の最大値で補正
				m_curSelect.x *= m_vecSelect[m_curSelect.y].size() / XSELECT_MAX;
				UtilFunc::Transformation::ValueNormalize(m_curSelect.x, (int)(m_vecSelect[m_curSelect.y].size() - 1), 0);
			}

		} while (m_vecSelect[m_curSelect.y][m_curSelect.x] == nullptr);
	}
	if (pKey->GetTrigger(DIK_DOWN) || pPad->GetTrigger(CInputGamepad::BUTTON_DOWN, m_nPadIdx))
	{
		do { // 選択先がない場合さらに動かす

			// 下に選択をずらす
			int nMaxHeight = (int)m_vecSelect.size();	// 行の最大数
			m_curSelect.y = (m_curSelect.y + 1) % nMaxHeight;

			// 列が増減した場合に列インデックスを補正
			int nPrevSelectY = (m_curSelect.y + (nMaxHeight - 1)) % nMaxHeight;	// 一つ上の行選択インデックス
			if (m_vecSelect[m_curSelect.y].size() < m_vecSelect[nPrevSelectY].size())
			{ // 列が減少した場合

				// 列インデックスを小さい方の最大値で補正
				m_curSelect.x /= m_vecSelect[m_oldSelect.y].size() / XSELECT_MAX;
				UtilFunc::Transformation::ValueNormalize(m_curSelect.x, (int)(m_vecSelect[m_curSelect.y].size() - 1), 0);
			}
			else if (m_vecSelect[m_curSelect.y].size() > m_vecSelect[nPrevSelectY].size())
			{ // 列が増加した場合

				// 列インデックスを大きい方の最大値で補正
				m_curSelect.x *= m_vecSelect[m_curSelect.y].size() / XSELECT_MAX;
				UtilFunc::Transformation::ValueNormalize(m_curSelect.x, (int)(m_vecSelect[m_curSelect.y].size() - 1), 0);
			}

		} while (m_vecSelect[m_curSelect.y][m_curSelect.x] == nullptr);
	}
}

//============================================================
//	選択の更新処理
//============================================================
void CInputKeyButton::UpdateSelect()
{
	// 前回の選択肢を保存
	m_oldSelect = m_curSelect;

	// 選択の操作
	ControlSelect();

	// 前回の選択要素の色を白色に設定
	assert(m_vecSelect[m_oldSelect.y][m_oldSelect.x] != nullptr);
	m_vecSelect[m_oldSelect.y][m_oldSelect.x]->SetColor(select::COL_DEFAULT);

	// 現在の選択要素の色を黄色に設定
	assert(m_vecSelect[m_curSelect.y][m_curSelect.x] != nullptr);
	m_vecSelect[m_curSelect.y][m_curSelect.x]->SetColor(select::COL_CHOICE);
}

//============================================================
//	決定の更新処理
//============================================================
void CInputKeyButton::UpdateDecide()
{
	CInputKeyboard*	pKey = GET_INPUTKEY;	// キーボード情報
	CInputGamepad*	pPad = GET_INPUTPAD;	// パッド情報
	if (pKey->GetTrigger(DIK_RETURN)
	||  pPad->GetTrigger(CInputGamepad::BUTTON_A, m_nPadIdx)
	||  pPad->GetTrigger(CInputGamepad::BUTTON_Y, m_nPadIdx))
	{
		// 選択肢に応じて操作を変更
		std::wstring wsName = m_pName->GetWideStr();	// 名前の文字列
		switch (m_curSelect.y)
		{ // 現在の行選択肢ごとの処理
		default:	// 文字選択

			if (m_pName->GetNumChar() < MAX_STR_NAME)
			{ // 文字数が最大ではない場合

				// 追加する文字列を取得
				std::wstring wsAddChar = m_vecSelect[m_curSelect.y][m_curSelect.x]->GetWideStr();

				// 最後尾に文字を追加
				wsName.push_back(wsAddChar.front());	// 文字列として管理してるので先頭文字を取得

				// 文字列を再設定
				m_pName->SetString(wsName);
			}
			break;

		case YSELECT_CHAR_CHANGE:	// 文字変更

			if ((ETypeChar)m_curSelect.x != m_typeChar)
			{ // 変更先が別の文字形式な場合

				// 選択中の文字に変更
				ChangeChar((ETypeChar)m_curSelect.x);
			}
			break;

		case YSELECT_CHAR_DECIDE:	// 文字決定

			switch (m_curSelect.x)
			{ // 現在の列選択肢ごとの処理
			case XSELECT_LEFT:		// 止める

				// 自身の終了
				Uninit();
				break;

			case XSELECT_CENTER:	// 削除

				if (!wsName.empty())
				{ // 文字がまだある場合

					// 最後尾を一文字削除
					wsName.erase(wsName.end() - 1);

					// 文字列を再設定
					m_pName->SetString(wsName);
				}
				break;

			case XSELECT_RIGHT:		// 確定

				if (!wsName.empty())
				{ // 文字が設定されている場合

					// 変更した文字列を保存
					m_pChange->SetString(m_pName->GetStr());

					// 自身の終了
					Uninit();
				}
				break;

			default:
				assert(false);
				break;
			}
			break;
		}
	}
}

//============================================================
//	キャンセルの更新処理
//============================================================
void CInputKeyButton::UpdateChancel()
{
	CInputKeyboard*	pKey = GET_INPUTKEY;	// キーボード情報
	CInputGamepad*	pPad = GET_INPUTPAD;	// パッド情報
	if (pKey->GetTrigger(DIK_SPACE)
	||  pPad->GetTrigger(CInputGamepad::BUTTON_B, m_nPadIdx)
	||  pPad->GetTrigger(CInputGamepad::BUTTON_X, m_nPadIdx))
	{
		// 選択肢に応じて操作を変更
		std::wstring wsName = m_pName->GetWideStr();	// 名前の文字列
		if (!wsName.empty())
		{ // 文字がまだある場合

			// 最後尾を一文字削除
			wsName.erase(wsName.end() - 1);

			// 文字列を再設定
			m_pName->SetString(wsName);
		}
	}
}

//============================================================
//	文字の変更処理
//============================================================
HRESULT CInputKeyButton::ChangeChar(const ETypeChar typeChar)
{
	// 文字種類が不明な値の場合抜ける
	if (typeChar <= -1 || typeChar >= TYPECHAR_MAX) { assert(false); return E_FAIL; }

	// 選択文字ポリゴンの終了
	for (int i = YSELECT_MAX; i < (int)m_vecSelect.size(); i++)
	{ // 固定選択肢を除く行の総数分繰り返す

		for (int j = 0; j < (int)m_vecSelect[i].size(); j++)
		{ // 列の総数分繰り返す

			// 選択文字の終了
			SAFE_UNINIT(m_vecSelect[i][j]);
		}
	}

	// 固定選択肢を除いた選択文字配列をクリア
	m_vecSelect.erase(m_vecSelect.begin() + YSELECT_MAX, m_vecSelect.end());

	// 配置の読込
	if (FAILED(LoadArray(typeChar)))
	{ // 読込に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// 設定した文字種類を保存
	m_typeChar = typeChar;

	return S_OK;
}

//============================================================
//	配置の読込処理
//============================================================
HRESULT CInputKeyButton::LoadArray(const ETypeChar typeChar)
{
	MyLib::Vector3 posOffset	= VEC3_ZERO;	// 文字生成位置
	MyLib::Vector3 posStart		= VEC3_ZERO;	// 文字開始位置
	MyLib::Vector2 charOffset	= VEC2_ZERO;	// 文字のオフセット
	float fSpaceOffset			= 0.0f;			// 空白のオフセット

	// ファイルを開く
	std::ifstream file(PATH_CHAR[typeChar]);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "文字セットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// ファイルを読込
	std::string str;	// 読込文字列
	while (std::getline(file, str))
	{ // ファイルの終端ではない場合ループ

		// カンマ区切りごとにデータを読込
		std::istringstream iss(str);	// 文字列ストリーム
		while (std::getline(iss, str, ','))
		{
			if (str == "START_POS")
			{
				// 開始位置を読込
				iss >> posStart.x >> posStart.y >> posStart.z;

				// 開始位置を生成位置に設定
				posOffset = posStart;
			}
			else if (str == "CHAR_OFFSET")
			{
				// 文字のオフセットを読込
				iss >> charOffset.x >> charOffset.y;
			}
			else if (str == "SAPCE_OFFSET")
			{
				// 空白のオフセットを読込
				iss >> fSpaceOffset;
			}
			else if (str == "SETCHAR")
			{
				while (std::getline(file, str))
				{ // ファイルの終端ではない場合ループ

					// 終端の場合文字生成を抜ける
					if (str == "END_SETCHAR") { break; }

					// 横一行分の配列を拡張
					m_vecSelect.emplace_back();

					// カンマ区切りごとにデータを読込
					std::istringstream issChar(str);	// 文字列ストリーム
					while (std::getline(issChar, str, ','))
					{
						if (str == "") { continue; }	// 空白は無視する
						else if (str == "/s")
						{ // 空白を読み込んだ場合

							// 横位置に空白分のオフセットを与える
							posOffset.x += fSpaceOffset;
							continue;	// 配列インデックスは進めない
						}
						else if (str == "/e")
						{ // 空文字を読み込んだ場合

							// 現在の行列の最後尾にnullptrを追加
							m_vecSelect.back().push_back(nullptr);

							// 横位置に文字分のオフセットを与える
							posOffset.x += charOffset.x;
						}
						else
						{ // 特殊操作ではない場合

							// 読み込んだ文字の生成
							CString2D* pChar = CString2D::Create
							( // 引数
								select::FONT,		// フォントパス
								select::ITALIC,		// イタリック
								str,				// 指定文字列
								posOffset,			// 原点位置
								select::HEIGHT,		// 文字縦幅
								select::ALIGN_X,	// 横配置
								select::ROT,		// 原点向き
								select::COL_DEFAULT	// 色
							);

							// 現在の行列の最後尾に生成した文字を追加
							m_vecSelect.back().push_back(pChar);

							// 横位置に文字分のオフセットを与える
							posOffset.x += charOffset.x;
						}
					}

					// 横位置を先頭に戻す
					posOffset.x = posStart.x;

					// 縦位置に文字分のオフセットを与える
					posOffset.y += charOffset.y;
				}
			}
		}
	}

	// ファイルを閉じる
	file.close();

	return S_OK;
}

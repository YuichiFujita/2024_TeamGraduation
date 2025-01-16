//============================================================
//
//	文字列入力ヘッダー [inputKeyButton.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _INPUT_KEY_BUTTON_H_
#define _INPUT_KEY_BUTTON_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス
class CString2D;	// 文字列2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// 文字列入力クラス
class CInputKeyButton : public CObject
{
public:
	// 文字種類列挙
	enum ETypeChar
	{
		TYPECHAR_HIRAGANA = 0,	// ひらがな
		TYPECHAR_KATAKANA,		// カタカナ
		TYPECHAR_ALPHABET,		// アルファベット
		TYPECHAR_MAX			// この列挙型の総数
	};

	// 列選択列挙
	enum ESelectX
	{
		XSELECT_LEFT = 0,	// 左
		XSELECT_CENTER,		// 中央
		XSELECT_RIGHT,		// 右
		XSELECT_MAX			// この列挙型の総数
	};

	// 行選択列挙
	enum ESelectY
	{
		YSELECT_CHAR_CHANGE,	// 文字変更
		YSELECT_CHAR_DECIDE,	// 文字決定
		YSELECT_MAX				// この列挙型の総数
	};

	// コンストラクタ
	CInputKeyButton(const int nPadIdx, const std::string& rOrigin, CString2D* pChange);

	// デストラクタ
	~CInputKeyButton() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw() override;		// 描画

	// 静的メンバ関数
	static CInputKeyButton* Create	// 生成
	( // 引数
		const int nPadIdx,			// 操作権インデックス
		const std::string& rOrigin,	// 変更前の文字列
		CString2D* pChange			// 変更する文字列
	);
	static CInputKeyButton* GetInstance();	// インスタンス取得

private:
	// メンバ関数
	void ControlSelect();	// 選択操作
	void UpdateSelect();	// 選択更新
	void UpdateDecide();	// 決定更新
	HRESULT ChangeChar(const ETypeChar typeChar);	// 文字変更
	HRESULT LoadArray(const ETypeChar typeChar);	// 配置読込

	// 静的メンバ変数
	static CInputKeyButton* m_pInstance;	// 自身のインスタンス

	// メンバ変数
	std::vector<std::vector<CString2D*>> m_vecSelect;	// 選択文字
	const int m_nPadIdx;			// 操作権インデックス
	const std::string m_sOrigin;	// 変更前の文字列
	CString2D* m_pChange;			// 変更する文字列
	CObject2D* m_pBG;				// 背景
	CString2D* m_pTitle;			// タイトル
	CString2D* m_pName;				// 名前
	ETypeChar m_typeChar;			// 文字種類
	MyLib::PosGrid2 m_curSelect;	// 現在の選択肢
	MyLib::PosGrid2 m_oldSelect;	// 前回の選択肢
};

#endif	// _INPUT_KEY_BUTTON_H_

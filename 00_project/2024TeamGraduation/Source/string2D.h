//============================================================
//
//	文字列2Dヘッダー [string2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _STRING2D_H_
#define _STRING2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CFontChar;	// フォント文字クラス
class CChar2D;		// 文字2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// 文字列2Dクラス
class CString2D : public CObject
{
public:
	// 横配置列挙
	enum EAlignX
	{
		XALIGN_LEFT = 0,	// 左揃え
		XALIGN_CENTER,		// 中央揃え
		XALIGN_RIGHT,		// 右揃え
		XALIGN_MAX,			// この列挙型の総数
	};

	// コンストラクタ
	CString2D();

	// デストラクタ
	~CString2D() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Release(void) override;	// 破棄
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw(void) override;						// 描画
	void SetType(const TYPE type) override;			// 種類設定
	void SetEnableDisp(const bool bDraw) override;	// 描画状況設定
	void SetPosition(const MyLib::Vector3& pos) override;	// 位置設定
	void SetRotation(const MyLib::Vector3& rot) override;	// 向き設定

	// 静的メンバ関数
	static CString2D *Create	// 生成
	( // 引数
		const std::string &rFilePass,	// フォントパス
		const bool bItalic,				// イタリック
		const std::wstring &rStr,		// 指定文字列
		const D3DXVECTOR3 &rPos,		// 原点位置
		const float fHeight = 100.0f,			// 文字縦幅
		const EAlignX alignX = XALIGN_CENTER,	// 横配置
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 原点向き
		const D3DXCOLOR& rCol = MyLib::color::White()	// 色
	);

	// 仮想関数
	virtual HRESULT SetString(const std::wstring& rStr);	// 文字列の設定

	// メンバ関数
	void SetFont	// フォントの設定
	( // 引数
		const std::string &rFilePass,	// フォントパス
		const bool bItalic = false		// イタリック
	);
	void SetColor(const D3DXCOLOR& rCol);			// 色設定
	void SetAlpha(const float fAlpha);				// 透明度設定
	void SetCharHeight(const float fHeight);		// 文字の縦幅設定
	void SetAlignX(const EAlignX align);			// 横配置設定
	float GetStrWidth(void) const;					// 文字列の横幅取得
	CChar2D *GetChar2D(const int nCharID) const;	// 文字の取得
	D3DXCOLOR GetColor(void) const	{ return m_col; }				// 色取得
	float GetAlpha(void) const		{ return m_col.a; }				// 透明度取得
	float GetCharHeight(void) const	{ return m_fCharHeight; }		// 文字の縦幅取得
	EAlignX GetAlignX(void) const	{ return m_alignX; }			// 横配置取得
	int GetNumChar(void) const		{ return (int)m_wsStr.size(); }	// 文字数取得
	std::wstring GetStr(void) const { return m_wsStr; }				// 文字列取得

private:
	// メンバ関数
	void SetPositionRelative(void);	// 相対位置設定

	// メンバ変数
	CChar2D **m_ppChar;		// 文字ポリゴンの情報
	CFontChar *m_pFontChar;	// フォント文字
	std::wstring m_wsStr;	// 指定文字列
	D3DXCOLOR m_col;		// 色
	EAlignX m_alignX;		// 横配置
	float m_fCharHeight;	// 文字の縦幅
};

#endif	// _STRING2D_H_

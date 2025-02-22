//============================================================
//
//	タイムUIヘッダー [timeUI.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TIME_UI_H_
#define _TIME_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "number.h"

//************************************************************
//	定数宣言
//************************************************************
namespace timeUI
{
	const int MAX_MIN	= 2;	// タイマーの分の桁数
	const int MAX_SEC	= 2;	// タイマーの秒の桁数
	const int MAX_MSEC	= 3;	// タイマーのミリ秒の桁数
	const int MAX_PART	= 2;	// 区切りの最大数
	const int MAX_DIGIT = (MAX_MIN + MAX_SEC + MAX_MSEC);	// タイマーの桁数 (分・秒・ミリ秒)
}

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// タイムUIクラス
class CTimeUI : public CObject
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

	// 縦配置列挙
	enum EAlignY
	{
		YALIGN_TOP = 0,	// 上揃え
		YALIGN_CENTER,	// 中央揃え
		YALIGN_BOTTOM,	// 下揃え
		YALIGN_MAX,		// この列挙型の総数
	};

	// コンストラクタ
	CTimeUI();

	// デストラクタ
	~CTimeUI() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Kill() override;			// 動的削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw(void) override;						// 描画
	void SetEnableDisp(const bool bDisp) override;	// 描画状況設定
	void SetPosition(const MyLib::Vector3& pos) override;	// 位置設定
	void SetRotation(const MyLib::Vector3& rot) override;	// 向き設定

	// 静的メンバ関数
	static CTimeUI *Create	// 生成
	( // 引数
		const float fTime,				// 表示時間
		const D3DXVECTOR3& rPos,		// 位置
		const float fHeightValue,		// 数字の大きさ
		const float fHeightPart,		// 区切りの大きさ
		const D3DXVECTOR2& rSpaceValue,	// 数字の空白
		const D3DXVECTOR2& rSpacePart,	// 区切りの空白
		const EAlignX alignX = XALIGN_CENTER,			// 横配置
		const EAlignY alignY = YALIGN_CENTER,			// 縦配置
		const D3DXVECTOR3& rRot = VEC3_ZERO,			// 向き
		const D3DXCOLOR& rCol = MyLib::color::White()	// 色
	);

	// 仮想関数
	virtual void SetTime(const float fTime);				// 表示時間設定
	virtual float GetTime(void) const	{ return m_fTime; }	// 表示時間取得
	virtual int GetMin(void) const		{ return (int)((DWORD)(m_fTime * 1000.0f) / 60000); }		// 分取得
	virtual int GetSec(void) const		{ return (int)((DWORD)(m_fTime * 1000.0f) / 1000 % 60); }	// 秒取得
	virtual int GetMSec(void) const		{ return (int)((DWORD)(m_fTime * 1000.0f) % 1000); }		// ミリ秒取得

	// メンバ関数
	void BindTextureValue(const int nTexIdx);		// 区切りのテクスチャインデックス設定
	void BindTexturePart(const int nTexIdx);		// 数字のテクスチャインデックス設定
	void SetSizeValue(const float fHeight);			// 区切りの大きさ設定
	void SetSizePart(const float fHeight);			// 数字の大きさ設定
	void SetSizeValue(const D3DXVECTOR2& rSize);	// 区切りの大きさ設定
	void SetSizePart(const D3DXVECTOR2& rSize);		// 数字の大きさ設定
	void SetSpaceValue(const D3DXVECTOR2& rSpace);	// 区切りの空白設定
	void SetSpacePart(const D3DXVECTOR2& rSpace);	// 数字の空白設定
	void SetColor(const D3DXCOLOR& rCol);			// 色の設定
	void SetAlpha(const float fAlpha);				// 透明度の設定
	void SetAlignX(const EAlignX align);			// 横配置設定
	void SetAlignY(const EAlignY align);			// 縦配置設定
	float GetTimeWidth(void) const;					// タイム全体の横幅取得
	float GetTimeHeight(void) const;				// タイム全体の縦幅取得
	D3DXVECTOR2 GetTimeSize(void) const;			// タイム全体の大きさ取得
	D3DXVECTOR2 GetSizingValue(void) const	{ return m_sizeValue; }		// 区切りの大きさ取得
	D3DXVECTOR2 GetSizingPart(void) const	{ return m_sizePart; }		// 数字の大きさ取得
	D3DXVECTOR2 GetSpaceValue(void) const	{ return m_spaceValue; }	// 区切りの空白取得
	D3DXVECTOR2 GetSpacePart(void) const	{ return m_spacePart; }		// 数字の空白取得
	D3DXCOLOR GetColor(void) const			{ return m_col;}			// 色の設定
	float GetAlpha(void) const				{ return m_col.a; }			// 透明度の設定
	EAlignX GetAlignX(void) const			{ return m_alignX; }		// 横配置取得
	EAlignY GetAlignY(void) const			{ return m_alignY; }		// 縦配置取得

private:
	// メンバ関数
	void SetPositionRelative(void);	// 相対位置設定
	void SetTexNum(void);			// 数字のテクスチャ座標設定

	// メンバ変数
	CNumber *m_apValue[timeUI::MAX_DIGIT];	// 数値の情報
	CObject2D *m_apPart[timeUI::MAX_PART];	// 区切りの情報
	D3DXVECTOR2 m_sizeValue;	// 数字の大きさ
	D3DXVECTOR2 m_sizePart;		// 区切りの大きさ
	D3DXVECTOR2 m_spaceValue;	// 数字の空白
	D3DXVECTOR2 m_spacePart;	// 区切りの空白
	D3DXCOLOR m_col;			// 色
	EAlignX m_alignX;			// 横配置
	EAlignY m_alignY;			// 縦配置
	float m_fTime;				// 表示時間
};

#endif	// _TIME_UI_H_

//=============================================================================
// 
//  マルチナンバーヘッダー [multinumber.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MULTINUMBER_H_
#define _MULTINUMBER_H_	// 二重インクルード防止


//==========================================================================
// 前方宣言
//==========================================================================
#include "object.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CNumber;

//==========================================================================
// クラス定義
//==========================================================================
// マルチナンバークラス
class CMultiNumber : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// オブジェクト種類
	enum EObjType
	{
		OBJTYPE_NONE = 0,	// なし
		OBJTYPE_2D,			// 2Dポリゴン
		OBJTYPE_3D,			// 3Dポリゴン
		OBJTYPE_BILLBOARD,	// ビルボード
		OBJTYPE_MAX			// この列挙型の総数
	};

	// 寄せ種類
	enum EAlignmentType
	{
		ALIGNMENT_LEFT = 0,	// 左寄せ
		ALIGNMENT_RIGHT,	// 右寄せ
		ALIGNMENT_CENTER,	// 中央寄せ
		ALIGNMENT_MAX
	};

	CMultiNumber(int nPriority = 8);
	~CMultiNumber();
	
	/**
	@brief	生成処理
	@param	nNum		[in]	数字
	@param	texIdx		[in]	テクスチャのインデックス
	@param	pos			[in]	位置
	@param	size		[in]	サイズ
	@param	nNum		[in]	桁数
	@param	objtype		[in]	オブジェクトの種類 [2D / 3D / ビルビード]
	@param	bDigitDraw	[in]	有ケタ分のみ描画するかの判定
	@param	nPriority	[in]	描画優先順位
	@return	CMultiNumber*
	*/
	static CMultiNumber* Create(const int nNum, const int texIdx, const MyLib::Vector3& pos, const MyLib::Vector2& size, int nMaxDigit, EObjType objtype, bool bDigitDraw = false, int nPriority = 8);

	// メンバ関数
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	void Kill() override;	// 削除処理

	//--------------------------
	// 位置
	//--------------------------
	virtual void SetPosition(const MyLib::Vector3& pos);		// 位置設定
	virtual void SetOldPosition(const MyLib::Vector3& pos);		// 過去の位置設定
	virtual void SetOriginPosition(const MyLib::Vector3& pos);	// 元の位置設定

	//--------------------------
	// 向き
	//--------------------------
	virtual void SetRotation(const MyLib::Vector3& rot);		// 向き設定
	virtual void SetOldRotation(const MyLib::Vector3& rot);		// 前回の向き設定
	virtual void SetOriginRotation(const MyLib::Vector3& rot);	// 元の向き設定

	//--------------------------
	// サイズ
	//--------------------------
	virtual void SetSize(const MyLib::Vector2& size);						// サイズ設定
	virtual MyLib::Vector2 GetSize() const { return m_size; }				// サイズ取得
	virtual void SetSizeOrigin(const MyLib::Vector2& size);					// 元のサイズ設定
	virtual MyLib::Vector2 GetSizeOrigin() const { return m_sizeOrigin; }	// 元のサイズ取得
	
	//--------------------------
	// 色
	//--------------------------
	virtual void SetColor(const D3DXCOLOR& col);						// 色設定
	virtual D3DXCOLOR GetColor() const { return m_col; }				// 色取得
	virtual void SetOriginColor(const D3DXCOLOR& col);					// 元の色設定
	virtual D3DXCOLOR GetOriginColor() const { return m_colOrigin; }	// 元の色取得
	virtual void SetAlpha(const float alpha);							// 不透明度設定
	virtual float GetAlpha() const { return m_col.a; }					// 不透明度取得

	//--------------------------
	// 値
	//--------------------------
	void SetNum(int nNum);			// 値設定
	int GetNum() {return m_nNum; }	// 値取得

	//--------------------------
	// その他
	//--------------------------
	virtual void BindTexture(int nIdxTexture);	// テクスチャ割り当て
	void SetEnableDigitDraw(bool bDraw) { m_bDigitDraw = bDraw; }			// 桁数描画判定設定
	void SetAlignmentType(EAlignmentType type) { m_AlignmentType = type; }	// 寄せ種類設定

private:

	//=============================
	// メンバ関数
	//=============================
	void AdjustPosition();	// 位置揃え
	void AdjustLeft();		// 左揃え
	void AdjustRight();		// 右揃え
	void AdjustCenter();	// 中央揃え

	//=============================
	// メンバ変数
	//=============================
	MyLib::Vector2 m_size;			// サイズ
	MyLib::Vector2 m_sizeOrigin;	// 元のサイズ
	D3DXCOLOR m_col;			// 色
	D3DXCOLOR m_colOrigin;		// 元の色
	int m_nNum;							// 数字
	int m_nMaxDigit;					// 桁数
	int m_nIdxTexture;					// テクスチャのインデックス
	bool m_bDigitDraw;					// 桁数描画フラグ
	EObjType m_objType;					// オブジェクトの種類
	EAlignmentType m_AlignmentType;		// 寄せ種類
	std::vector<CNumber*> m_vecNumber;	// 数字
};

#endif
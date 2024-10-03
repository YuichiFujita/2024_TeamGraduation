//=============================================================================
// 
//  数字ヘッダー [multinumber.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MULTINUMBER_H_
#define _MULTINUMBER_H_	// 二重インクルード防止

#include "object2D.h"
#include "number.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CNumber;

//==========================================================================
// クラス定義
//==========================================================================
// 背景クラス定義
class CMultiNumber : public CObject
{
public:

	// 寄せ種類
	enum AlignmentType
	{
		ALIGNMENT_LEFT = 0,	// 左寄せ
		ALIGNMENT_RIGHT,	// 右寄せ
		ALIGNMENT_MAX
	};

	CMultiNumber(int nPriority = 8);
	~CMultiNumber();

	/**
	@brief	生成処理
	@param	pos			[in]	位置
	@param	size		[in]	サイズ
	@param	nNum		[in]	桁数
	@param	objtype		[in]	オブジェクトの種類 [2D / 3D / ビルビード]
	@param	bDigitDraw	[in]	有ケタ分のみ描画するかの判定
	@param	nPriority	[in]	描画優先順位
	@return	生成したポインタ
	*/
	static CMultiNumber *Create(MyLib::Vector3 pos, D3DXVECTOR2 size, int nNum, CNumber::EObjectType objtype, bool bDigitDraw = false, int nPriority = 8);
	
	/**
	@brief	生成処理
	@param	pos			[in]	位置
	@param	size		[in]	サイズ
	@param	nNum		[in]	桁数
	@param	objtype		[in]	オブジェクトの種類 [2D / 3D / ビルビード]
	@param	file		[in]	テクスチャファイル名
	@param	bDigitDraw	[in]	有ケタ分のみ描画するかの判定
	@param	nPriority	[in]	描画優先順位
	@return	生成したポインタ
	*/
	static CMultiNumber *Create(MyLib::Vector3 pos, D3DXVECTOR2 size, int nNum, CNumber::EObjectType objtype, const std::string& file, bool bDigitDraw = true, int nPriority = 8);

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void Kill();	// 削除処理

	void AddNumber(int nValue);
	void SetValue();		// 値の設定処理
	void SetValue(int nValue);	// 値の設定処理(オーバーロード)
	int GetValue();			// 値の取得処理
	int GetDigit() { return m_nNumNumber; }	// 桁数取得

	void SetEnableAutoUVSetting(bool frag) { m_bAutoUVSetting = frag; }	// 自動UV座標設定
	bool IsAutoUVSetting() { return m_bAutoUVSetting; }			// 自動UV座標設定

	CNumber** GetNumber() { return m_ppMultiNumber; }
	CNumber* GetNumber(int idx) { return m_ppMultiNumber[idx]; }

	void SetPosition(const MyLib::Vector3& pos) override;		// 位置設定
	void SetRotation(const MyLib::Vector3& pos) override;		// 向き設定

	void SetColor(const D3DXCOLOR col);			// 色設定
	D3DXCOLOR GetColor() const;				// 色取得
	void SetSize(const D3DXVECTOR2 size);		// サイズ設定
	D3DXVECTOR2 GetSize() const;			// サイズ取得
	void SetSizeOrigin(const D3DXVECTOR2 size);		// サイズ設定
	D3DXVECTOR2 GetSizeOrigin() const;			// サイズ取得
	void SetKerning(float kerning);	// 文字間隔設定
	void SetAlignmentType(AlignmentType type) { m_Alignment = type; }

private:

	void SettingDisp();

	// メンバ変数
	D3DXCOLOR m_col;				// 色
	D3DXVECTOR2 m_size;				// 数字のサイズ
	D3DXVECTOR2 m_sizeOrigin;		// 数字のサイズ
	int m_nNum;						// 数字
	int m_nNumNumber;				// 数字の数
	int m_nTexIdx;					// テクスチャのインデックス番号
	int m_nPriority;				// 優先順位
	bool m_bDigitDraw;				// 桁数描画
	bool m_bAutoUVSetting;			// 自動UV座標設定
	float m_fKerning;				// 文字間隔
	CNumber **m_ppMultiNumber;		// 数字のオブジェクト
	CNumber::EObjectType m_objType;	// オブジェクトの種類
	AlignmentType m_Alignment;		// 揃え

};

#endif
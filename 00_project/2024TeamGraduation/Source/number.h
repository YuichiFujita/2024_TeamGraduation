//==========================================================================
// 
//  数字ヘッダー [number.h]
//  Author : 相馬 靜雅
// 
//==========================================================================

#ifndef _NUMBER_H_
#define _NUMBER_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "multinumber.h"
#include "object.h"

//==========================================================================
// クラス定義
//==========================================================================
// 数字クラス
class CNumber : public CObject
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CNumber(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	virtual ~CNumber();

	//=============================
	// オーバーライド関数
	//=============================
	//--------------------------
	// パイプライン
	//--------------------------
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill();	// 削除

	//--------------------------
	// 位置
	//--------------------------
	virtual void SetPosition(const MyLib::Vector3& pos) = 0;		// 位置設定
	virtual void SetOldPosition(const MyLib::Vector3& pos) = 0;		// 過去の位置設定
	virtual void SetOriginPosition(const MyLib::Vector3& pos) = 0;	// 元の位置設定

	//--------------------------
	// 向き
	//--------------------------
	virtual void SetRotation(const MyLib::Vector3& rot) = 0;		// 向き設定
	virtual void SetOldRotation(const MyLib::Vector3& rot) = 0;		// 前回の向き設定
	virtual void SetOriginRotation(const MyLib::Vector3& rot) = 0;	// 元の向き設定

	//--------------------------
	// サイズ
	//--------------------------
	virtual void SetSize(const MyLib::Vector2& size) = 0;			// サイズ設定
	virtual MyLib::Vector2 GetSize() const = 0;						// サイズ取得
	virtual void SetSizeOrigin(const MyLib::Vector2& size) = 0;		// 元のサイズ設定
	virtual MyLib::Vector2 GetSizeOrigin() const = 0;				// 元のサイズ取得

	//--------------------------
	// 色
	//--------------------------
	virtual void SetColor(const D3DXCOLOR& col) = 0;			// 色設定
	virtual D3DXCOLOR GetColor() const = 0;						// 色取得
	virtual void SetOriginColor(const D3DXCOLOR& col) = 0;		// 元の色設定
	virtual D3DXCOLOR GetOriginColor() const = 0;				// 元の色取得
	virtual void SetAlpha(const float alpha) = 0;				// 不透明度設定
	virtual float GetAlpha() const = 0;							// 不透明度取得

	//--------------------------
	// 値
	//--------------------------
	virtual void SetNum(int nNum) = 0;	// 値設定

	//--------------------------
	// その他
	//--------------------------
	virtual void BindTexture(int nIdxTexture) = 0;						// テクスチャ割り当て

	//=============================
	// 静的メンバ関数
	//=============================
	static CNumber* Create(CMultiNumber::EObjType type, int nPriority);	// 生成

private:

	
};

#endif

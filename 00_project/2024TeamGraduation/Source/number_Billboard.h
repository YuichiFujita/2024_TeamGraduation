//=============================================================================
// 
//  数字(Billboard)ヘッダー [number_Billboard.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _NUMBER_BILLBOARD_H_
#define _NUMBER_BILLBOARD_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "number.h"
#include "objectBillboard_Anim.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CObjectBillboardAnim;

//==========================================================================
// クラス定義
//==========================================================================
// 数字(Billboard)クラス定義
class CNumberBillboard : public CNumber
{
public:

	CNumberBillboard(int nPriority = 7, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CNumberBillboard();

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
	virtual void SetPosition(const MyLib::Vector3& pos) override		{ m_pObjBillboard->SetPosition(pos); }	// 位置設定
	virtual void SetOldPosition(const MyLib::Vector3& pos) override		{ m_pObjBillboard->SetPosition(pos); }	// 過去の位置設定
	virtual void SetOriginPosition(const MyLib::Vector3& pos) override	{ m_pObjBillboard->SetPosition(pos); }	// 元の位置設定

	//--------------------------
	// 向き
	//--------------------------
	virtual void SetRotation(const MyLib::Vector3& rot) override		{ m_pObjBillboard->SetRotation(rot); }			// 向き設定
	virtual void SetOldRotation(const MyLib::Vector3& rot) override		{ m_pObjBillboard->SetOldRotation(rot); }		// 前回の向き設定
	virtual void SetOriginRotation(const MyLib::Vector3& rot) override	{ m_pObjBillboard->SetOriginRotation(rot); }	// 元の向き設定

	//--------------------------
	// サイズ
	//--------------------------
	virtual void SetSize(const MyLib::Vector2& size) override		{ m_pObjBillboard->SetSize(size); }			// サイズ設定
	virtual MyLib::Vector2 GetSize() const override					{ return m_pObjBillboard->GetSize(); }			// サイズ取得
	virtual void SetSizeOrigin(const MyLib::Vector2& size) override	{ m_pObjBillboard->SetSizeOrigin(size); }		// 元のサイズ設定
	virtual MyLib::Vector2 GetSizeOrigin() const override			{ return m_pObjBillboard->GetSizeOrigin(); }	// 元のサイズ取得

	//--------------------------
	// 色
	//--------------------------
	virtual void SetColor(const D3DXCOLOR& col) override		{ m_pObjBillboard->SetColor(col); }			// 色設定
	virtual D3DXCOLOR GetColor() const override					{ return m_pObjBillboard->GetColor(); }		// 色取得
	virtual void SetOriginColor(const D3DXCOLOR& col) override	{ m_pObjBillboard->SetOriginColor(col); }		// 元の色設定
	virtual D3DXCOLOR GetOriginColor() const override			{ return m_pObjBillboard->GetOriginColor(); }	// 元の色取得
	virtual void SetAlpha(const float alpha) override			{ m_pObjBillboard->SetAlpha(alpha); }			// 不透明度設定
	virtual float GetAlpha() const override						{ return m_pObjBillboard->GetAlpha(); }		// 不透明度取得

	//--------------------------
	// 値
	//--------------------------
	virtual void SetNum(int nNum) override	{ m_pObjBillboard->SetPatternAnim(nNum); }	// 値設定

	//--------------------------
	// その他
	//--------------------------
	virtual void BindTexture(int nIdxTexture) override { m_pObjBillboard->BindTexture(nIdxTexture); }						// テクスチャ割り当て
	void SetVtx();	// 頂点情報設定処理
	virtual void SetType(const TYPE type) override;		// 種類設定

private:

	//=============================
	// メンバ変数
	//=============================
	CObjectBillboardAnim* m_pObjBillboard;	// オブジェクトBillboard
};

#endif
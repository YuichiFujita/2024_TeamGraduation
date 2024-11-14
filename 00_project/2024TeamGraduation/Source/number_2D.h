//=============================================================================
// 
//  数字(2D)ヘッダー [number_2D.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _NUMBER_2D_H_
#define _NUMBER_2D_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "number.h"
#include "object2D_Anim.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CObject2D_Anim;

//==========================================================================
// クラス定義
//==========================================================================
// 数字(2D)クラス定義
class CNumber2D : public CNumber
{
public:

	CNumber2D(int nPriority = 7, const LAYER layer = LAYER::LAYER_2D);
	~CNumber2D();

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
	virtual void SetPosition(const MyLib::Vector3& pos) override		{ m_pObj2D->SetPosition(pos); }	// 位置設定
	virtual void SetOldPosition(const MyLib::Vector3& pos) override		{ m_pObj2D->SetPosition(pos); }	// 過去の位置設定
	virtual void SetOriginPosition(const MyLib::Vector3& pos) override	{ m_pObj2D->SetPosition(pos); }	// 元の位置設定

	//--------------------------
	// 向き
	//--------------------------
	virtual void SetRotation(const MyLib::Vector3& rot) override		{ m_pObj2D->SetRotation(rot); }			// 向き設定
	virtual void SetOldRotation(const MyLib::Vector3& rot) override		{ m_pObj2D->SetOldRotation(rot); }		// 前回の向き設定
	virtual void SetOriginRotation(const MyLib::Vector3& rot) override	{ m_pObj2D->SetOriginRotation(rot); }	// 元の向き設定

	//--------------------------
	// サイズ
	//--------------------------
	virtual void SetSize(const MyLib::Vector2& size) override		{ m_pObj2D->SetSize(size); }			// サイズ設定
	virtual MyLib::Vector2 GetSize() const override					{ return m_pObj2D->GetSize(); }			// サイズ取得
	virtual void SetSizeOrigin(const MyLib::Vector2& size) override	{ m_pObj2D->SetSizeOrigin(size); }		// 元のサイズ設定
	virtual MyLib::Vector2 GetSizeOrigin() const override			{ return m_pObj2D->GetSizeOrigin(); }	// 元のサイズ取得

	//--------------------------
	// 色
	//--------------------------
	virtual void SetColor(const D3DXCOLOR& col) override		{ m_pObj2D->SetColor(col); }			// 色設定
	virtual D3DXCOLOR GetColor() const override					{ return m_pObj2D->GetColor(); }		// 色取得
	virtual void SetOriginColor(const D3DXCOLOR& col) override	{ m_pObj2D->SetOriginColor(col); }		// 元の色設定
	virtual D3DXCOLOR GetOriginColor() const override			{ return m_pObj2D->GetOriginColor(); }	// 元の色取得
	virtual void SetAlpha(const float alpha) override			{ m_pObj2D->SetAlpha(alpha); }			// 不透明度設定
	virtual float GetAlpha() const override						{ return m_pObj2D->GetAlpha(); }		// 不透明度取得

	//--------------------------
	// 値
	//--------------------------
	virtual void SetNum(int nNum) override	{ m_pObj2D->SetPatternAnim(nNum); }	// 値設定

	//--------------------------
	// その他
	//--------------------------
	virtual void BindTexture(int nIdxTexture) override { m_pObj2D->BindTexture(nIdxTexture); }						// テクスチャ割り当て
	void SetVtx();	// 頂点情報設定処理

private:

	//=============================
	// メンバ変数
	//=============================
	CObject2D_Anim* m_pObj2D;	// オブジェクト2D
};

#endif
//=============================================================================
// 
//  数字(3D)ヘッダー [number_3D.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _NUMBER_3D_H_
#define _NUMBER_3D_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "number.h"
#include "object3D_Anim.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CObject3DAnim;

//==========================================================================
// クラス定義
//==========================================================================
// 数字(3D)クラス定義
class CNumber3D : public CNumber
{
public:

	CNumber3D(int nPriority = 7, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CNumber3D();

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
	virtual void SetPosition(const MyLib::Vector3& pos) override		{ m_pObj3D->SetPosition(pos); }				// 位置設定
	virtual MyLib::Vector3 GetPosition() override						{ return m_pObj3D->GetPosition(); }			// 位置取得
	virtual void SetOldPosition(const MyLib::Vector3& pos) override		{ m_pObj3D->SetPosition(pos); }				// 過去の位置設定
	virtual MyLib::Vector3 GetOldPosition() override					{ return m_pObj3D->GetOldPosition(); }		// 過去の位置取得
	virtual void SetOriginPosition(const MyLib::Vector3& pos) override	{ m_pObj3D->SetPosition(pos); }				// 元の位置設定
	virtual MyLib::Vector3 GetOriginPosition() override					{ return m_pObj3D->GetOriginPosition(); }	// 元の位置取得

	//--------------------------
	// 向き
	//--------------------------
	virtual void SetRotation(const MyLib::Vector3& rot) override		{ m_pObj3D->SetRotation(rot); }			// 向き設定
	virtual void SetOldRotation(const MyLib::Vector3& rot) override		{ m_pObj3D->SetOldRotation(rot); }		// 前回の向き設定
	virtual void SetOriginRotation(const MyLib::Vector3& rot) override	{ m_pObj3D->SetOriginRotation(rot); }	// 元の向き設定

	//--------------------------
	// サイズ
	//--------------------------
	virtual void SetSize(const MyLib::Vector2& size) override		{ m_pObj3D->SetSize(size); }			// サイズ設定
	virtual MyLib::Vector2 GetSize() const override					{ return m_pObj3D->GetSize(); }			// サイズ取得
	virtual void SetSizeOrigin(const MyLib::Vector2& size) override	{ m_pObj3D->SetSizeOrigin(size); }		// 元のサイズ設定
	virtual MyLib::Vector2 GetSizeOrigin() const override			{ return m_pObj3D->GetSizeOrigin(); }	// 元のサイズ取得

	//--------------------------
	// 色
	//--------------------------
	virtual void SetColor(const D3DXCOLOR& col) override		{ m_pObj3D->SetColor(col); }			// 色設定
	virtual D3DXCOLOR GetColor() const override					{ return m_pObj3D->GetColor(); }		// 色取得
	virtual void SetOriginColor(const D3DXCOLOR& col) override	{ m_pObj3D->SetColorOrigin(col); }		// 元の色設定
	virtual D3DXCOLOR GetOriginColor() const override			{ return m_pObj3D->GetColorOrigin(); }	// 元の色取得
	virtual void SetAlpha(const float alpha) override			{ m_pObj3D->SetAlpha(alpha); }			// 不透明度設定
	virtual float GetAlpha() const override						{ return m_pObj3D->GetAlpha(); }		// 不透明度取得

	//--------------------------
	// 値
	//--------------------------
	virtual void SetNum(int nNum) override	{ m_pObj3D->SetCurPtrn(nNum); }	// 値設定

	//--------------------------
	// その他
	//--------------------------
	virtual void BindTexture(int nIdxTexture) override { m_pObj3D->BindTexture(nIdxTexture); }						// テクスチャ割り当て
	void SetVtx();	// 頂点情報設定処理
	virtual void SetType(const TYPE type) override;		// 種類設定

private:

	//=============================
	// メンバ変数
	//=============================
	CObject3DAnim* m_pObj3D;	// オブジェクト3D
};

#endif
//=============================================================================
// 
//  数字ヘッダー [number.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _NUMBER_H_
#define _NUMBER_H_	// 二重インクルード防止

#include "object.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CObject2D;
class CObjectBillboard;

//==========================================================================
// クラス定義
//==========================================================================
// 数字クラス
class CNumber : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum EObjectType
	{
		OBJECTTYPE_2D = 0,		// 2D
		OBJECTTYPE_3D,			// 3D
		OBJECTTYPE_BILLBOARD,	// ビルボード
		OBJECTTYPE_MAX
	};

	CNumber(int nPriority = 6, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CNumber();


	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;


	virtual void Kill();	// 削除

	virtual void SetPosition(const MyLib::Vector3& pos) = 0;	// 位置設定
	virtual MyLib::Vector3 GetPosition() const = 0;		// 位置取得
	virtual void SetMove(const MyLib::Vector3& move) = 0;		// 移動量設定
	virtual MyLib::Vector3 GetMove() const = 0;			// 移動量取得
	virtual void SetRotation(const MyLib::Vector3& rot) = 0;	// 向き設定
	virtual MyLib::Vector3 GetRotation() const = 0;		// 向き取得

	virtual void SetColor(const D3DXCOLOR col) = 0;		// 色設定
	virtual D3DXCOLOR GetColor() const = 0;				// 色取得
	virtual void SetSize(const D3DXVECTOR2 size) = 0;		// サイズの設定
	virtual D3DXVECTOR2 GetSize() const = 0;			// サイズの取得
	virtual void SetSizeOrigin(const D3DXVECTOR2 size) = 0;	// 元のサイズの設定
	virtual D3DXVECTOR2 GetSizeOrigin() const = 0;		// 元のサイズの取得
	virtual void SetTex(D3DXVECTOR2* tex) = 0;				// テクスチャ座標の設定
	virtual D3DXVECTOR2* GetTex() = 0;					// テクスチャ座標の取得

	virtual void BindTexture(int nIdx) = 0;	// テクスチャ割り当て
	virtual void SetType(const CObject::TYPE type);

	void SetNum(int num) { m_nNum = num; }	// 自分の数字設定
	int GetNum() { return m_nNum; }			// 自分の数字取得

	static CNumber* Create(EObjectType objtype, int nPriority);

private:


	//=============================
	// メンバ変数
	//=============================
	int m_nNum = 0;			// 自分の数字
	EObjectType m_objType;	// オブジェクトの種類
};

#endif
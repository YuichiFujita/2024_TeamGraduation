//==========================================================================
// 
//  観客ヘッダー [audience.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _AUDIENCE_H_
#define _AUDIENCE_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object.h"

//==========================================================================
// クラス定義
//==========================================================================
// 観客クラス
class CAudience : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum EObjType
	{
		OBJTYPE_ANIM = 0,	// 3Dポリゴン
		OBJTYPE_LOWPOLY,	// ローポリキャラクター
		OBJTYPE_HIGHPOLY,	// ハイポリキャラクター
		OBJTYPE_MAX			// この列挙型の総数
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CAudience(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CAudience();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill();	// 削除

	//=============================
	// 純粋仮想関数
	//=============================
	virtual void SetPosition(const MyLib::Vector3& pos) override = 0;	// 位置設定
	virtual MyLib::Vector3 GetPosition() const override = 0;			// 位置取得
	virtual void SetMove(const MyLib::Vector3& move) override = 0;		// 移動量設定
	virtual MyLib::Vector3 GetMove() const override = 0;				// 移動量取得
	virtual void SetRotation(const MyLib::Vector3& rot) override = 0;	// 向き設定
	virtual MyLib::Vector3 GetRotation() const override = 0;			// 向き取得

	//=============================
	// 仮想関数
	//=============================


	//=============================
	// メンバ関数
	//=============================


	//=============================
	// 静的メンバ関数
	//=============================
	static CAudience* Create(EObjType type);

private:

	//=============================
	// メンバ変数
	//=============================
	EObjType m_type;	// オブジェクト種類
};

#endif

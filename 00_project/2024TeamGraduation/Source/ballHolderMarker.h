//==========================================================================
// 
//  ボール所持マーカーヘッダー [ballHolderMarker.h]
//  Author : 相馬靜雅
// 
//==========================================================================

#ifndef _BALLHOLDERMARKER_H_
#define _BALLHOLDERMARKER_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayer;

//==========================================================================
// クラス定義
//==========================================================================
// ボール所持マーカークラス
class CBallHolderMarker : public CObject
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CBallHolderMarker(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CBallHolderMarker();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill();	// 削除

	//=============================
	// メンバ関数
	//=============================
	void BindPlyer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }	// プレイヤー割り当て

	//=============================
	// 静的メンバ関数
	//=============================
	static CBallHolderMarker* Create(CPlayer* pPlayer);	// 生成

private:

	//=============================
	// メンバ変数
	//=============================
	HRESULT CreateCircle();	// 円生成
	HRESULT CreateArrow();		// 矢印生成

	//=============================
	// メンバ変数
	//=============================
	CObject3D* m_pCircle;	// 円
	CObject3D* m_pArrow;	// 矢印
	CPlayer* m_pPlayer;		// ボールを所持してるプレイヤーのポインタ
	float m_fArrowTime;		// 矢印のタイマー
};

#endif

//==========================================================================
// 
//  プレイヤーマーカーヘッダー [playerMarker.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_MARKER_H_
#define _PLAYER_MARKER_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "objectBillboard_Anim.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayer;

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤーマーカークラス
class CPlayerMarker : public CObjectBillboardAnim
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerMarker(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CPlayerMarker();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;	// 削除

	//=============================
	// メンバ関数
	//=============================
	void BindPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }	// プレイヤー割当

	//=============================
	// 静的メンバ関数
	//=============================
	static CPlayerMarker* Create(CPlayer* pPlayer);	// 生成

private:

	//=============================
	// メンバ変数
	//=============================
	CPlayer* m_pPlayer;	// 親プレイヤー情報
};

#endif

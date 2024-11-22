//=============================================================================
// 
//  ドレスアップ用プレイヤーヘッダー [playerDressup.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYER_DRESSUP_H_
#define _PLAYER_DRESSUP_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "player.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤークラス定義
class CPlayerDressUP : public CPlayer
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerDressUP(const CGameManager::ETeamSide typeTeam, const EFieldArea typeArea, const CPlayerDressUP::EBaseType typeBase, int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CPlayerDressUP();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;	// 動的削除処理

	//=============================
	// 静的関数
	//=============================
	/*
		@brief	プレイヤーの生成処理
		@param	rPos	 [in]	初期位置 (内野のみ)
		@param	typeTeam [in]	左/右
		@param	typeArea [in]	内野/外野
		@param	typeBase [in]	User/AI
		@param	typeBody [in]	標準/デブ/ガリ
		@param	typeHand [in]	右利き/左利き
	*/
	static CPlayerDressUP* Create
	(
		const MyLib::Vector3&	rPos,					// 位置
		CGameManager::ETeamSide	typeTeam,				// チームサイド
		EFieldArea	typeArea = EFieldArea::FIELD_IN,	// ポジション
		EBaseType	typeBase = EBaseType::TYPE_USER,	// ベースタイプ
		EBody		typeBody = EBody::BODY_NORMAL,		// 体型
		EHandedness	typeHand = EHandedness::HAND_R		// 利き手
	);

};

#endif

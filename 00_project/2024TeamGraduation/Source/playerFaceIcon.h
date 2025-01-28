//==========================================================================
// 
//  顔アイコンプレイヤーヘッダー [playerFaceIcon.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_FACEICON_H_
#define _PLAYER_FACEICON_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "player.h"

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤークラス定義
class CPlayerFaceIcon : public CPlayer
{
public:

	//=============================
	// 定数
	//=============================
	static constexpr int ID_FACE = 2;	// 顔のインデックス番号
	static constexpr int ID_HAIR = 15;	// 髪のインデックス番号

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerFaceIcon(const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase, int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CPlayerFaceIcon();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Kill() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
};

#endif

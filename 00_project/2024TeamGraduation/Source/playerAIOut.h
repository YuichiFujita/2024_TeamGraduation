//==========================================================================
// 
//  AI外野プレイヤーヘッダー [playerAIOut.h]
//  Author : 藤田勇一
//  Adder  : 小笠原彪
// 
//==========================================================================

#ifndef _PLAYER_AI_OUT_
#define _PLAYER_AI_OUT_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerAI.h"

//==========================================================================
// クラス定義
//==========================================================================
// AI外野プレイヤークラス
class CPlayerAIOut : public CPlayerAI
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerAIOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea);
	virtual ~CPlayerAIOut() override;

	//=============================
	// オーバーライド関数
	//=============================
	virtual CPlayerAIOut* GetPlayerAIOut() override { return nullptr; }	// AI外野プレイヤー取得
	virtual bool IsCrab() override { return false; }					// カニ歩き判定
	virtual void InitPosition(const MyLib::Vector3& rPos) override;		// 位置初期化

	//=============================
	// メンバ関数
	//=============================
	void SetPosLeft(const MyLib::Vector3& rPosLeft)		{ m_posLeft = rPosLeft; }	// 移動可能な左位置の設定
	void SetPosRight(const MyLib::Vector3& rPosRight)	{ m_posRight = rPosRight; }	// 移動可能な右位置の設定
	MyLib::Vector3 GetPosLeft() { return m_posLeft; }	// 移動可能な左位置の取得
	MyLib::Vector3 GetPosRight() { return m_posRight; }	// 移動可能な右位置の取得

private:

	//=============================
	// メンバ変数
	//=============================
	MyLib::Vector3 m_posLeft;	// 移動可能左位置
	MyLib::Vector3 m_posRight;	// 移動可能右位置
};

#endif

//==========================================================================
// 
//  外野プレイヤーヘッダー [playerOut.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_OUT_
#define _PLAYER_OUT_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerBase.h"

//==========================================================================
// クラス定義
//==========================================================================
// 外野プレイヤークラス
class CPlayerOut : public CPlayerBase
{
public:
	
	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase);
	virtual ~CPlayerOut() override = 0;

	//=============================
	// オーバーライド関数
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	virtual CPlayer::SHitInfo Hit(CBall* pBall) override;				// ヒット
	virtual void InitPosition(const MyLib::Vector3& rPos) override;		// 位置初期化
	virtual void MotionCrab(int nStartKey) override { assert(false); }	// カニ歩き変化
	virtual bool IsCrab() override;					// カニ歩き判定
	virtual bool IsCrabMotion() override;	// カニ歩きモーション判定
	virtual CPlayerOut* GetPlayerOut() override { return this; }		// 外野プレイヤー取得

	//=============================
	// メンバ関数
	//=============================
	void SetPosLeft(const MyLib::Vector3& rPosLeft)		{ m_posLeft = rPosLeft; }	// 移動可能な左位置の設定
	void SetPosRight(const MyLib::Vector3& rPosRight)	{ m_posRight = rPosRight; }	// 移動可能な右位置の設定
	MyLib::Vector3 GetPosLeft() { return m_posLeft; }	// 移動可能な左位置の取得
	MyLib::Vector3 GetPosRight() { return m_posRight; }	// 移動可能な右位置の取得

	void SetIsClab(bool clab) { m_bClab = clab; }
	bool GetIsClab() { return m_bClab; }

private:

	//=============================
	// メンバ変数
	//=============================
	MyLib::Vector3 m_posLeft;	// 移動可能左位置
	MyLib::Vector3 m_posRight;	// 移動可能右位置
	bool m_bClab;	// カニ歩きフラグ
};

#endif

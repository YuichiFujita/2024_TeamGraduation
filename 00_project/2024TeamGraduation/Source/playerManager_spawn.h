//==========================================================================
// 
//  プレイヤーマネージャー(登場時)ヘッダー [playerManager_spawn.h]
//  Author : 相馬靜雅
// 
//==========================================================================

#ifndef _PLAYER_MANAGER_SPAWN_H_
#define _PLAYER_MANAGER_SPAWN_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerManager.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤーマネージャー(登場時)クラス
class CPlayerManager_Spawn : public CPlayerManager
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerManager_Spawn();
	~CPlayerManager_Spawn() override;

	//=============================
	// 仮想関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;

private:

	//=============================
	// メンバ関数
	//=============================
	// 生成
	virtual HRESULT CreatePlayer() override;	// プレイヤー生成
	virtual HRESULT CreateLeftPlayer(int i, const LoadInfo& info) override;		// 左のプレイヤー生成
	virtual HRESULT CreateRightPlayer(int i, const LoadInfo& info) override;	// 右のプレイヤー生成

};

#endif

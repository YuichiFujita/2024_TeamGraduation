//==========================================================================
// 
//  プレイヤーマネージャー(リザルト)ヘッダー [playerManager_result.h]
//  Author : 相馬靜雅
// 
//==========================================================================

#ifndef _PLAYER_MANAGER_RESULT_H_
#define _PLAYER_MANAGER_RESULT_H_	// 二重インクルード防止

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
// プレイヤーマネージャー(リザルト)クラス
class CPlayerManager_Result : public CPlayerManager
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerManager_Result();
	~CPlayerManager_Result() override;

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
	virtual HRESULT CreateOutPlayer(CGameManager::ETeamSide team, const LoadInfo& info) override;	// 外野プレイヤー生成
	virtual HRESULT CreateLeftPlayer(int i, const LoadInfo& info) override;		// 左のプレイヤー生成
	virtual HRESULT CreateRightPlayer(int i, const LoadInfo& info) override;	// 右のプレイヤー生成

	// その他
	void InitPlayer();	// プレイヤー初期化
};

#endif

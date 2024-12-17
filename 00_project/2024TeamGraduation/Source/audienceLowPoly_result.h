//==========================================================================
// 
//  観客_ローポリキャラリザルトヘッダー [audienceLowPoly_result.h]
//  Author : Kai Takada
// 
//==========================================================================

#ifndef _AUDIENCE_LOWPOLY_RESULT_H_
#define _AUDIENCE_LOWPOLY_RESULT_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "audienceLowPoly.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// 観客_ローポリキャラクラス定義
class CAudienceLowPolyResult : public CAudienceLowPoly
{
public:

	//=============================
	// 列挙型
	//=============================
	// 観戦サイド列挙
	enum EAreaSide
	{
		SIDE_NONE = 0,	// 無し
		SIDE_REVERSE,	// 逆
		SIDE_MAX		// この列挙型の総数
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CAudienceLowPolyResult(CAudienceLowPoly::EObjType type, CGameManager::ETeamSide team);
	~CAudienceLowPolyResult();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

protected:

private:

	//=============================
	// メンバ関数
	//=============================
	void CalcWatchPositionFar() override;	// 観戦位置計算 (奥)
	void CalcWatchPositionSide() override;	// 観戦位置計算 (横)
	void CalcWatchPositionNear() override;	// 観戦位置計算 (手前)
	
	void EndSettingSpawn() override;		// スポーン終了時の設定

	//=============================
	// メンバ変数
	//=============================
	EAreaSide m_side;		// 観戦サイド
};

#endif

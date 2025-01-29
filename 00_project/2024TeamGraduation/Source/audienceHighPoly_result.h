//==========================================================================
// 
//  観客_ハイポリキャラリザルトヘッダー [audienceHighPoly_result.h]
//  Author : Kai Takada
// 
//==========================================================================

#ifndef _AUDIENCE_HIGHPOLY_RESULT_H_
#define _AUDIENCE_HIGHPOLY_RESULT_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "audienceHighPoly.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CObjectX;

//==========================================================================
// クラス定義
//==========================================================================
// 観客_ハイポリキャラクラス定義
class CAudienceHighPolyResult : public CAudienceHighPoly
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
	CAudienceHighPolyResult(CAudienceHighPoly::EObjType type, CGameManager::ETeamSide team);
	~CAudienceHighPolyResult();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

	bool SetNTR(CGameManager::ETeamSide team) override;		// NTR設定

protected:

private:
	//=============================
	// メンバ関数
	//=============================
	void CalcWatchPositionFar() override;	// 観戦位置計算 (奥)
	void CalcWatchPositionSide() override;	// 観戦位置計算 (横)
	void CalcWatchPositionNear() override;	// 観戦位置計算 (手前)
	
	void EndSettingSpawn() override;			// スポーン終了時の設定

	//=============================
	// メンバ変数
	//=============================
	EAreaSide m_side;		// 観戦サイド
};

#endif

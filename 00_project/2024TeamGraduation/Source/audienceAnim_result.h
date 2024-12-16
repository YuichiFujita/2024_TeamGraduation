//==========================================================================
// 
//  観客_アニメーション3Dリザルトヘッダー [audienceAnim_result.h]
//  Author : Kai Takada
// 
//==========================================================================

#ifndef _AUDIENCE_ANIM_RESULT_H_
#define _AUDIENCE_ANIM_RESULT_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "audienceAnim.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// 観客_アニメーション3Dクラス定義
class CAudienceAnimResult : public CAudienceAnim
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CAudienceAnimResult(CAudienceAnim::EObjType type, CGameManager::ETeamSide team);
	~CAudienceAnimResult();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

protected:

private:

	void CalcWatchPositionFar() override;	// 観戦位置計算 (奥)
	void CalcWatchPositionUp() override;		// 観戦位置計算 (上)
	void CalcWatchPositionNear() override;	// 観戦位置計算 (手前)
};

#endif

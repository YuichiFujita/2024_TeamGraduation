//==========================================================================
// 
//  観客_アニメーション3Dリザルトヘッダー [audienceAnim_result.cpp]
//  Author : Kai Takada
// 
//==========================================================================
#include "audienceAnim_result.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	namespace Far
	{
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };	// チームサイドごとの左ライン
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)CAudience::MAX_RIGHT_LINE };	// チームサイドごとの右ライン
		const int NEAR_LINE		= 1360;	// 手前の生成位置上限
		const int FAR_LINE		= 1700;	// 奥の生成位置上限
	}

	namespace Up
	{
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };	// チームサイドごとの左ライン
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)CAudience::MAX_RIGHT_LINE };	// チームサイドごとの右ライン
		const int NEAR_LINE		= 1700;		// 手前の生成位置上限
		const int FAR_LINE		= 1900;		// 奥の生成位置上限
		const float LANDY		= 545.0f;	// 着地Y座標
	}

	namespace Down
	{
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };	// チームサイドごとの左ライン
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)CAudience::MAX_RIGHT_LINE };	// チームサイドごとの右ライン
		const int NEAR_LINE		= -1700;	// 手前の生成位置上限
		const int FAR_LINE		= -1360;	// 奥の生成位置上限
	}
}

//==========================================================================
// コンストラクタ
//==========================================================================
CAudienceAnimResult::CAudienceAnimResult(EObjType type, CGameManager::ETeamSide team) : CAudienceAnim(type, team)
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CAudienceAnimResult::~CAudienceAnimResult()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CAudienceAnimResult::Init()
{
	// 親クラスの初期化
	CAudienceAnim::Init();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CAudienceAnimResult::Uninit()
{
	// 親クラスの終了
	CAudienceAnim::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CAudienceAnimResult::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親クラスの更新
	CAudienceAnim::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CAudienceAnimResult::Draw()
{
	// 親クラスの描画
	CAudienceAnim::Draw();
}

//==========================================================================
// 観戦位置の計算処理 (奥)
//==========================================================================
void CAudienceAnimResult::CalcWatchPositionFar()
{
	// ランダムに観戦位置を設定
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Far::LEFT_LINE[nIdxTeam], Far::RIGHT_LINE[nIdxTeam]);
	posWatch.y = GetLandY();
	posWatch.z = (float)UtilFunc::Transformation::Random(Far::NEAR_LINE, Far::FAR_LINE);
	SetWatchPosition(posWatch);

	// モーションの設定
	SetIdolMotion(MOTION_IDOL_D);	// 下待機モーション
	SetJumpMotion(MOTION_JUMP_D);	// 下ジャンプモーション
}

//==========================================================================
// 観戦位置の計算処理 (上)
//==========================================================================
void CAudienceAnimResult::CalcWatchPositionUp()
{
	// 着地Y座標の設定
	SetLandY(Up::LANDY);

	// ランダムに観戦位置を設定
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Up::LEFT_LINE[nIdxTeam], Up::RIGHT_LINE[nIdxTeam]);
	posWatch.y = GetLandY();
	posWatch.z = (float)UtilFunc::Transformation::Random(Up::NEAR_LINE, Up::FAR_LINE);
	SetWatchPosition(posWatch);

	// モーションの設定
	SetIdolMotion(MOTION_IDOL_D);	// 下待機モーション
	SetJumpMotion(MOTION_JUMP_D);	// 下ジャンプモーション
}

//==========================================================================
// 観戦位置の計算処理 (手前)
//==========================================================================
void CAudienceAnimResult::CalcWatchPositionNear()
{
	// ランダムに観戦位置を設定
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Down::LEFT_LINE[nIdxTeam], Down::RIGHT_LINE[nIdxTeam]);
	posWatch.y = GetLandY();
	posWatch.z = (float)UtilFunc::Transformation::Random(Down::NEAR_LINE, Down::FAR_LINE);
	SetWatchPosition(posWatch);

	// モーションの設定
	SetIdolMotion(MOTION_IDOL_U);	// 上待機モーション
	SetJumpMotion(MOTION_JUMP_U);	// 上ジャンプモーション
}

//==========================================================================
// 
//  観客_ローポリキャラリザルトヘッダー [audienceLowPoly_result.cpp]
//  Author : Kai Takada
// 
//==========================================================================
#include "audienceLowPoly_result.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	namespace Side
	{// 横
		const int RANGE = 80;	// 左右のライン幅
		const int MID_LEFT_LINE[]	= { 0.0f, RANGE };													// チームサイドごとの中央(左)ライン
		const int MID_RIGHT_LINE[]	= { RANGE, 0.0f };													// チームサイドごとの中央(右)ライン
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CAudience::MAX_RIGHT_LINE - RANGE };	// チームサイドごとの左ライン
		const int RIGHT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE + RANGE, (int)CAudience::MAX_RIGHT_LINE };	// チームサイドごとの右ライン
		const int NEAR_LINE		= -1060;	// 手前の生成位置上限
		const int FAR_LINE		= 1060;		// 奥の生成位置上限
	}

	namespace Far
	{// 奥
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };	// チームサイドごとの左ライン
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)CAudience::MAX_RIGHT_LINE };	// チームサイドごとの右ライン
		const int NEAR_LINE		= 1100;	// 手前の生成位置上限
		const int FAR_LINE		= 1300;	// 奥の生成位置上限
	}

	namespace Down
	{// 手前
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };	// チームサイドごとの左ライン
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)CAudience::MAX_RIGHT_LINE };	// チームサイドごとの右ライン
		const int NEAR_LINE		= -1300;	// 手前の生成位置上限
		const int FAR_LINE		= -1100;	// 奥の生成位置上限
	}

	const MyLib::Vector3 POS_COURT[CGameManager::ETeamSide::SIDE_MAX] =	// コート中央位置
	{
		MyLib::Vector3(-900.0f, 0.0f, 0.0f),	// 左
		MyLib::Vector3(+900.0f, 0.0f, 0.0f),	// 右
	};
}

//==========================================================================
// コンストラクタ
//==========================================================================
CAudienceLowPolyResult::CAudienceLowPolyResult(EObjType type, CGameManager::ETeamSide team) : CAudienceLowPoly(type, team),
	m_side(EAreaSide::SIDE_NONE)					// 観戦サイド
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CAudienceLowPolyResult::~CAudienceLowPolyResult()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CAudienceLowPolyResult::Init()
{
	// 親クラスの初期化
	CAudienceLowPoly::Init();

	// 観戦満了
	SetTimeStateByTimeStateMax();
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CAudienceLowPolyResult::Uninit()
{
	// 親クラスの終了
	CAudienceLowPoly::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CAudienceLowPolyResult::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親クラスの更新
	CAudienceLowPoly::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CAudienceLowPolyResult::Draw()
{
	// 親クラスの描画
	CAudienceLowPoly::Draw();
}

//==========================================================================
// NTRの設定処理
//==========================================================================
bool CAudienceLowPolyResult::SetNTR(CGameManager::ETeamSide team)
{
	// もう設定していたらはじく
	//if (m_teamNTR != CGameManager::ETeamSide::SIDE_NONE) return false;

	// NTR

	// 親クラスの設定
	CAudienceLowPoly::SetNTR(team);

	return true;
}

//==========================================================================
// 観戦位置の計算処理 (奥)
//==========================================================================
void CAudienceLowPolyResult::CalcWatchPositionFar()
{
	// チーム取得
	int nIdxTeam = GetTeam();

	// ランダムに観戦位置を設定
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Far::LEFT_LINE[nIdxTeam], Far::RIGHT_LINE[nIdxTeam]);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random(Far::NEAR_LINE, Far::FAR_LINE);
	SetWatchPosition(posWatch);
}

//==========================================================================
// 観戦位置の計算処理 (横)
//==========================================================================
void CAudienceLowPolyResult::CalcWatchPositionSide()
{
	// ランダムで左右決定
	int nLR = UtilFunc::Transformation::Random(0, 1);

	// チーム取得
	int nIdxTeam = GetTeam();

	// ランダムに観戦位置を設定
	MyLib::Vector3 posWatch;
	if (nLR == 0)
	{// 端側
		posWatch.x = (float)UtilFunc::Transformation::Random(Side::LEFT_LINE[nIdxTeam], Side::RIGHT_LINE[nIdxTeam]);
	}
	else
	{// 中央側
		posWatch.x = (float)UtilFunc::Transformation::Random(Side::MID_LEFT_LINE[nIdxTeam], Side::MID_RIGHT_LINE[nIdxTeam]);
	
		m_side = EAreaSide::SIDE_REVERSE;
	}
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random(Side::NEAR_LINE, Side::FAR_LINE);
	SetWatchPosition(posWatch);
}

//==========================================================================
// 観戦位置の計算処理 (手前)
//==========================================================================
void CAudienceLowPolyResult::CalcWatchPositionNear()
{
	// チーム取得
	int nIdxTeam = GetTeam();

	// ランダムに観戦位置を設定
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Down::LEFT_LINE[nIdxTeam], Down::RIGHT_LINE[nIdxTeam]);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random(Down::NEAR_LINE, Down::FAR_LINE);
	SetWatchPosition(posWatch);
}

//==========================================================================
// スポーン終了時の設定
//==========================================================================
void CAudienceLowPolyResult::EndSettingSpawn()
{
	// 親クラス
	CAudienceLowPoly::EndSettingSpawn();

	// 向き設定
	MyLib::Vector3 rot;
	rot.y = GetPosition().AngleXZ(POS_COURT[GetTeam()]);
	rot.y += UtilFunc::Transformation::Random(-50, 50) * 0.001f;
	SetRotation(rot);
}

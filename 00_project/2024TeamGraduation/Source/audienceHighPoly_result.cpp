//==========================================================================
// 
//  観客_ハイポリキャラリザルトヘッダー [audienceHighPoly_result.cpp]
//  Author : Kai Takada
// 
//==========================================================================
#include "audienceHighPoly_result.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	namespace Side
	{
		const int RANGE = 325;	// 左右のライン幅
		const int MID_LEFT_LINE[] = { RANGE * 0.5f, 0.0f };		// チームサイドごとの中央(左)ライン
		const int MID_RIGHT_LINE[] = { 0.0f, RANGE * 0.5f };	// チームサイドごとの中央(右)ライン
		const int LEFT_LINE[]	= { -1675, 1350 };				// チームサイドごとの左ライン
		const int RIGHT_LINE[]	= { -1350, 1675 };				// チームサイドごとの右ライン
		const int NEAR_LINE		= -960;	// 手前の生成位置上限
		const int FAR_LINE		= 960;	// 奥の生成位置上限
	}

	namespace Far
	{
		const int LEFT_LINE[]	= { (int)Side::LEFT_LINE[CGameManager::ETeamSide::SIDE_LEFT], (int)CGameManager::CENTER_LINE };		// チームサイドごとの左ライン
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)Side::RIGHT_LINE[CGameManager::ETeamSide::SIDE_RIGHT] };	// チームサイドごとの右ライン
		const int NEAR_LINE		= 1000;	// 手前の生成位置上限
		const int FAR_LINE		= 1100;	// 奥の生成位置上限
	}

	namespace Down
	{
		const int LEFT_LINE[]	= { (int)Side::LEFT_LINE[CGameManager::ETeamSide::SIDE_LEFT], (int)CGameManager::CENTER_LINE };		// チームサイドごとの左ライン
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)Side::RIGHT_LINE[CGameManager::ETeamSide::SIDE_RIGHT] };	// チームサイドごとの右ライン
		const int NEAR_LINE		= -1100;	// 手前の生成位置上限
		const int FAR_LINE		= -1000;	// 奥の生成位置上限
	}
}

//==========================================================================
// コンストラクタ
//==========================================================================
CAudienceHighPolyResult::CAudienceHighPolyResult(CAudienceHighPoly::EObjType type, CGameManager::ETeamSide team) : CAudienceHighPoly(type, team),
	m_side(EAreaSide::SIDE_NONE),						// 観戦サイド
	m_teamNTR(CGameManager::ETeamSide::SIDE_NONE)		// NTR後のチーム
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CAudienceHighPolyResult::~CAudienceHighPolyResult()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CAudienceHighPolyResult::Init()
{
	// 親クラスの初期化
	CAudienceHighPoly::Init();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CAudienceHighPolyResult::Uninit()
{
	// 親クラスの終了
	CAudienceHighPoly::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CAudienceHighPolyResult::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親クラスの更新
	CAudienceHighPoly::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CAudienceHighPolyResult::Draw()
{
	// 親クラスの描画
	CAudienceHighPoly::Draw();
}

//==========================================================================
// NTRの設定処理
//==========================================================================
bool CAudienceHighPolyResult::SetNTR(CGameManager::ETeamSide team)
{
	// もう設定していたらはじく
	//if (m_teamNTR != CGameManager::ETeamSide::SIDE_NONE) return false;

	// NTR
	m_teamNTR = team;

	// 親クラスの設定
	CAudienceHighPoly::SetNTR(team);

	return true;
}

//==========================================================================
// 観戦位置の計算処理 (奥)
//==========================================================================
void CAudienceHighPolyResult::CalcWatchPositionFar()
{
	// チーム取得
	int nIdxTeam = GetTeam();

	if (m_teamNTR != CGameManager::ETeamSide::SIDE_NONE)
	{// 設定されていたら
		nIdxTeam = m_teamNTR;
	}

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
void CAudienceHighPolyResult::CalcWatchPositionSide()
{
	// ランダムで左右決定
	int nLR = UtilFunc::Transformation::Random(0, 1);

	// チーム取得
	int nIdxTeam = GetTeam();

	if (m_teamNTR != CGameManager::ETeamSide::SIDE_NONE)
	{// 設定されていたら
		nIdxTeam = m_teamNTR;
	}

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
void CAudienceHighPolyResult::CalcWatchPositionNear()
{
	// チーム取得
	int nIdxTeam = GetTeam();

	if (m_teamNTR != CGameManager::ETeamSide::SIDE_NONE)
	{// 設定されていたら
		nIdxTeam = m_teamNTR;
	}

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
void CAudienceHighPolyResult::EndSettingSpawn()
{
	// 親クラス
	CAudienceHighPoly::EndSettingSpawn();

	// TODO: エリア定数を整理し、その中心点を向く感じ

	// サイドが逆なら逆向きに
	if (GetArea() == EArea::AREA_SIDE &&
		m_side == EAreaSide::SIDE_REVERSE)
	{
		MyLib::Vector3 rot = GetRotation();
	
		rot.y += D3DX_PI;
		UtilFunc::Transformation::RotNormalize(rot);

		// 向きを横に向ける
		SetRotation(rot);
	}
}

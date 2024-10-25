//==========================================================================
// 
//  観客_アニメーション3D処理 [audienceAnim.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "audienceAnim.h"
#include "gameManager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int PRIORITY = mylib_const::PRIORITY_DEFAULT;		// 優先順位
	const char* SETUP_TXT = "data\\CHARACTER\\frisk.txt";	// プレイヤーセットアップテキスト

	const int LEFT_LINE[]  = { (int)CAudienceAnim::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };		// チームサイドごとの左ライン
	const int RIGHT_LINE[] = { (int)CGameManager::CENTER_LINE, (int)CAudienceAnim::MAX_RIGHT_LINE };	// チームサイドごとの右ライン
}

//==========================================================================
// コンストラクタ
//==========================================================================
CAudienceAnim::CAudienceAnim(EObjType type, CGameManager::TeamSide team) : CAudience(type, team, PRIORITY, CObject::LAYER_DEFAULT),
	m_pAnimChara	(nullptr)	// キャラクターアニメーション情報
{
}

//==========================================================================
// デストラクタ
//==========================================================================
CAudienceAnim::~CAudienceAnim()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CAudienceAnim::Init()
{
	// 親クラスの初期化
	if (FAILED(CAudience::Init()))
	{ // 初期化に失敗した場合

		return E_FAIL;
	}

	// ランダムに観戦位置を設定
	int nIdxTeam = GetTeam() - 1;
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(LEFT_LINE[nIdxTeam], RIGHT_LINE[nIdxTeam]);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random((int)NEAR_LINE, (int)FAR_LINE);
	SetWatchPosition(posWatch);

	// ランダムに生成位置を設定
	MyLib::Vector3 posSpawn = posWatch;
	float fTurn = ((bool)(rand() % 2)) ? 1.0f : -1.0f;
	posSpawn.x = CAudience::SPAWN_SIDE_LINE * fTurn;
	SetSpawnPosition(posSpawn);

	// オブジェクトキャラクターアニメーションの生成
	m_pAnimChara = CObjectCharaAnim::Create(posSpawn);
	if (m_pAnimChara == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// キャラクター情報の割当
	m_pAnimChara->BindCharaData(SETUP_TXT);

	// モーションを設定
	m_pAnimChara->SetMotion(1);

	// アニメーション3Dの自動更新/自動描画/自動破棄をしない種類にする
	m_pAnimChara->SetType(CObject::TYPE::TYPE_NONE);

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CAudienceAnim::Uninit()
{
	// 親クラスの終了
	CAudience::Uninit();

	// オブジェクトキャラクターアニメーションの終了
	SAFE_UNINIT(m_pAnimChara);
}

//==========================================================================
// 削除処理
//==========================================================================
void CAudienceAnim::Kill()
{
	// 親クラスの削除
	CAudience::Kill();

	// オブジェクトキャラクターアニメーションの終了
	SAFE_UNINIT(m_pAnimChara);
}

//==========================================================================
// 更新処理
//==========================================================================
void CAudienceAnim::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	int nCurMotion = m_pAnimChara->GetMotion();	// TODO

	if (m_pAnimChara != nullptr)
	{
		// 死んでいる場合抜ける
		if (IsDeath()) { return; }

		// モーションが設定されていない場合抜ける
		if (nCurMotion == -1) { return; }

		int nAnimMotion = m_pAnimChara->GetMotion();	// 現在再生中のモーション
		if (m_pAnimChara->IsLoop())
		{ // ループするモーションだった場合

			if (nAnimMotion != nCurMotion)
			{ // 現在のモーションが再生中のモーションと一致しない場合

				// 現在のモーションの設定
				m_pAnimChara->SetMotion(nCurMotion);
			}
		}

		// オブジェクトキャラクターアニメーションの更新
		m_pAnimChara->Update(fDeltaTime, fDeltaRate, fSlowRate);

#if 0
		switch (m_pAnimChara->GetMotion())
		{ // モーションの種類ごとの処理
		case MOTION_IDOL_U:	// 上待機モーション
		case MOTION_IDOL_D:	// 下待機モーション
		case MOTION_IDOL_L:	// 左待機モーション
		case MOTION_IDOL_R:	// 右待機モーション
		case MOTION_MOVE_U:	// 上移動モーション
		case MOTION_MOVE_D:	// 下移動モーション
		case MOTION_MOVE_L:	// 左移動モーション
		case MOTION_MOVE_R:	// 右移動モーション
			break;
		}
#endif
	}

	// 親クラスの更新
	CAudience::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CAudienceAnim::Draw()
{
	// オブジェクトキャラクターアニメーションの描画
	m_pAnimChara->Draw();

	// 親クラスの描画
	CAudience::Draw();
}

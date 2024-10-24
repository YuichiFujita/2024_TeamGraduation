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
	const int PRIORITY = mylib_const::PRIORITY_DEFAULT;	// 優先順位
}

//==========================================================================
// コンストラクタ
//==========================================================================
CAudienceAnim::CAudienceAnim(EObjType type) : CAudience(type, PRIORITY, CObject::LAYER_DEFAULT),
	m_pAnim3D	(nullptr)	// アニメーション3D情報
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
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random((int)MAX_LEFT_LINE, (int)MAX_RIGHT_LINE);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random((int)NEAR_LINE, (int)FAR_LINE);
	SetWatchPosition(posWatch);

	// ランダムに生成位置を設定
	MyLib::Vector3 posSpawn = posWatch;
	float fTurn = ((bool)(rand() % 2)) ? 1.0f : -1.0f;
	posSpawn.x = CAudience::SPAWN_SIDE_LINE * fTurn;
	SetSpawnPosition(posSpawn);

	// アニメーション3Dの生成
	m_pAnim3D = CObject3DAnim::Create(posSpawn, 1, 1, 0.0f, false);
	if (m_pAnim3D == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// アニメーション3Dの自動更新/自動描画/自動破棄をしない種類にする
	m_pAnim3D->SetType(CObject::TYPE::TYPE_NONE);

	// TODO：大きさ定数必要
	m_pAnim3D->SetSizeOrigin(MyLib::Vector3(40.0f, 60.0f, 0.0f));
	m_pAnim3D->SetSize(MyLib::Vector3(40.0f, 60.0f, 0.0f));

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

	// アニメーション3Dの終了
	SAFE_UNINIT(m_pAnim3D);
}

//==========================================================================
// 削除処理
//==========================================================================
void CAudienceAnim::Kill()
{
	// 親クラスの削除
	CAudience::Kill();

	// アニメーション3Dの終了
	SAFE_UNINIT(m_pAnim3D);
}

//==========================================================================
// 更新処理
//==========================================================================
void CAudienceAnim::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pAnim3D != nullptr)
	{
		// アニメーション3Dの更新
		m_pAnim3D->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 親クラスの更新
	CAudience::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CAudienceAnim::Draw()
{
	// アニメーション3Dの描画
	m_pAnim3D->Draw();

	// 親クラスの描画
	CAudience::Draw();
}

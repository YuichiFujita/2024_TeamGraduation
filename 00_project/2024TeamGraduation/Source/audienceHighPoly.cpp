//==========================================================================
// 
//  観客_ハイポリキャラ処理 [audienceHighPoly.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "audienceHighPoly.h"
#include "gameManager.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* SETUP_TXT = "data\\TEXT\\character\\audience\\setup_player.txt";	// プレイヤーセットアップテキスト
	const char* LIGHT_PATH = "data\\MODEL\\penlight.x";		// ペンライトのモデルパス
	const int PRIORITY = mylib_const::PRIORITY_DEFAULT;		// 優先順位

	namespace Side
	{
		const int LEFT_LINE[]	= { -1675, 1350 };	// チームサイドごとの左ライン
		const int RIGHT_LINE[]	= { -1350, 1675 };	// チームサイドごとの右ライン
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
// 関数ポインタ
//==========================================================================
// 観戦位置計算リスト
CAudienceHighPoly::WATCH_POS_FUNC CAudienceHighPoly::m_CalcWatchPositionFunc[] =
{
	&CAudienceHighPoly::CalcWatchPositionFar,	// 奥観戦位置
	&CAudienceHighPoly::CalcWatchPositionSide,	// 横観戦位置
	&CAudienceHighPoly::CalcWatchPositionNear,	// 手前観戦位置
};

//==========================================================================
// コンストラクタ
//==========================================================================
CAudienceHighPoly::CAudienceHighPoly(EObjType type, CGameManager::ETeamSide team) : CAudience(type, team, PRIORITY, CObject::LAYER_DEFAULT),
	m_pChara	(nullptr),	// キャラクター情報
	m_pLight	(nullptr)	// ペンライト情報
{
}

//==========================================================================
// デストラクタ
//==========================================================================
CAudienceHighPoly::~CAudienceHighPoly()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CAudienceHighPoly::Init()
{
	// 親クラスの初期化
	if (FAILED(CAudience::Init()))
	{ // 初期化に失敗した場合

		return E_FAIL;
	}

	// 観戦エリアの設定
	SetArea((int)(rand() % EArea::AREA_MAX));

	// 観戦位置を設定し、生成情報を取得
	(this->*(m_CalcWatchPositionFunc[GetArea()]))();

	// チームごとに生成方向を設定
	float fTurn = 0.0f;		// 方向係数
	if ((bool)GetTeam())	{ fTurn =  1.0f; }	// 右からの入場にする
	else					{ fTurn = -1.0f; }	// 左からの入場にする

	// 生成/退場位置の設定
	MyLib::Vector3 posSpawn = GetWatchPosition();
	posSpawn.x = CAudience::SPAWN_SIDE_LINE * fTurn;
	SetSpawnPosition(posSpawn);		// 生成位置
	SetDespawnPosition(posSpawn);	// 退場位置

	// オブジェクトキャラクターの生成
	if (FAILED(CreateCharacter(posSpawn, MyLib::Vector3(0.0f, HALF_PI * fTurn, 0.0f))))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// ペンライトの生成
	if (FAILED(CreatePenLight()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 影の生成
	if (FAILED(CreateShadow(this)))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CAudienceHighPoly::Uninit()
{
	// 親クラスの終了
	CAudience::Uninit();

	// オブジェクトキャラクターの終了
	SAFE_UNINIT(m_pChara);
}

//==========================================================================
// 削除処理
//==========================================================================
void CAudienceHighPoly::Kill()
{
	// 親クラスの削除
	CAudience::Kill();

	// オブジェクトキャラクターの終了
	if (m_pChara != nullptr)
	{
		m_pChara->Kill();
		m_pChara = nullptr;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CAudienceHighPoly::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pChara != nullptr)
	{
		// オブジェクトキャラクターの更新
		m_pChara->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 親クラスの更新
	CAudience::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CAudienceHighPoly::Draw()
{
	if (m_pChara != nullptr)
	{
		// オブジェクトキャラクターの描画
		m_pChara->Draw();
	}

	// 親クラスの描画
	CAudience::Draw();
}

//==========================================================================
// 入場状態の更新処理
//==========================================================================
int CAudienceHighPoly::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 入場状態の更新
	CAudience::UpdateSpawn(fDeltaTime, fDeltaRate, fSlowRate);

	// ライトの自動描画をオフにする
	m_pLight->SetEnableDisp(false);

	// 移動モーションを返す
	return EMotion::MOTION_SPAWN;
}

//==========================================================================
// 通常状態の更新処理
//==========================================================================
int CAudienceHighPoly::UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 通常状態の更新
	CAudience::UpdateNormal(fDeltaTime, fDeltaRate, fSlowRate);

	// ライトの自動描画をオフにする
	m_pLight->SetEnableDisp(false);

	// 待機モーションを返す
	return EMotion::MOTION_DEF;
}

//==========================================================================
// 盛り上がり状態の更新処理
//==========================================================================
int CAudienceHighPoly::UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 盛り上がり状態の更新
	CAudience::UpdateJump(fDeltaTime, fDeltaRate, fSlowRate);

	// ライトの自動描画をオフにする
	m_pLight->SetEnableDisp(false);

	// ジャンプモーションを返す
	return EMotion::MOTION_JUMP;
}

//==========================================================================
// スペシャル状態の更新処理
//==========================================================================
int CAudienceHighPoly::UpdateSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// スペシャル状態の更新
	CAudience::UpdateSpecial(fDeltaTime, fDeltaRate, fSlowRate);

	// ライトの自動描画をオンにする
	m_pLight->SetEnableDisp(true);

	// ライトの位置を頭の上にする
	m_pLight->SetPosition(GetPosition() + MyLib::Vector3(0.0f, 230.0f, 0.0f));	// TODO：後で振ったりさせる

	// ジャンプモーションを返す
	return EMotion::MOTION_JUMP;
}

//==========================================================================
// 退場状態の更新処理
//==========================================================================
int CAudienceHighPoly::UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 退場状態の更新
	CAudience::UpdateDespawn(fDeltaTime, fDeltaRate, fSlowRate);

	// ライトの自動描画をオフにする
	m_pLight->SetEnableDisp(false);

	// 移動モーションを返す
	return EMotion::MOTION_DESPAWN;
}

//==========================================================================
// 退場の設定処理
//==========================================================================
bool CAudienceHighPoly::SetDespawn(EObjType type)
{
	// 退場の設定
	bool bDespawn = CAudience::SetDespawn(type);
	if (bDespawn)
	{ // 退場した場合

		// 退場位置を作成
		MyLib::Vector3 posDespawn = GetSpawnPosition();	// 退場位置

		// 向きを退場方向へ
		SetRotation(MyLib::Vector3(0.0f, MyLib::Vector3().AngleXZ(posDespawn), 0.0f));
	}

	// 退場フラグを返す
	return bDespawn;
}

//==========================================================================
// モーションの設定処理
//==========================================================================
void CAudienceHighPoly::SetMotion(const int nMotion)
{
	// キャラクター情報がない場合抜ける
	if (m_pChara == nullptr) { return; }
	CMotion* pMotion = m_pChara->GetMotion();

	int nAnimMotion = pMotion->GetType();	// 現在再生中のモーション
	if (nAnimMotion != nMotion)
	{ // 現在のモーションが再生中のモーションと一致しない場合

		// 現在のモーションの設定
		pMotion->Set(nMotion);
	}
}

//==========================================================================
// スポーン終了時の設定
//==========================================================================
void CAudienceHighPoly::EndSettingSpawn()
{
	// キャラクター情報がない場合抜ける
	if (m_pChara == nullptr) { return; }

	const float fGapRotY = UtilFunc::Transformation::Random(-100, 100) * 0.001f;	// 向きのブレ
	switch (GetArea())
	{ // 観戦エリアごとの処理
	case EArea::AREA_FAR:
	{
		// 向きを正面に向ける
		m_pChara->SetRotation(MyLib::Vector3(0.0f, fGapRotY, 0.0f));
		break;
	}
	case EArea::AREA_SIDE:
	{
		float fTurn = 0.0f;		// 方向係数
		if ((bool)GetTeam())	{ fTurn =  1.0f; }	// 右からの入場にする
		else					{ fTurn = -1.0f; }	// 左からの入場にする

		// 向きを横に向ける
		m_pChara->SetRotation(MyLib::Vector3(0.0f, (HALF_PI * fTurn) - fGapRotY, 0.0f));
		break;
	}
	case EArea::AREA_NEAR:
	{
		// 向きを奥に向ける
		m_pChara->SetRotation(MyLib::Vector3(0.0f, D3DX_PI - fGapRotY, 0.0f));
		break;
	}
	default:
		assert(false);
		break;
	};
}

//==========================================================================
// キャラクター生成
//==========================================================================
HRESULT CAudienceHighPoly::CreateCharacter(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot)
{
	// オブジェクトキャラクターの生成
	m_pChara = CObjectChara::Create(SETUP_TXT);
	if (m_pChara == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 位置の設定
	m_pChara->SetPosition(rPos);

	// 向きの設定
	m_pChara->SetRotation(rRot);

	// モーションの設定
	m_pChara->GetMotion()->Set(EMotion::MOTION_SPAWN);

	// ハイポリキャラの自動更新/自動描画/自動破棄をしない種類にする
	m_pChara->SetType(CObject::TYPE::TYPE_NONE);

	return S_OK;
}

//==========================================================================
// ペンライト生成
//==========================================================================
HRESULT CAudienceHighPoly::CreatePenLight()
{
	// ペンライトの生成
	m_pLight = CObjectX::Create(LIGHT_PATH, VEC3_ZERO);
	if (m_pLight == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類を設定
	m_pLight->SetType(CObject::TYPE::TYPE_OBJECTX);

	// 自動描画フラグをオフにする
	m_pLight->SetEnableDisp(false);

	// TODO：スケール調整後で消す
#if 1
	m_pLight->SetScale(MyLib::Vector3(1.0f, 0.4f, 1.0f));
#endif

	return S_OK;
}

//==========================================================================
// 観戦位置の計算処理 (奥)
//==========================================================================
void CAudienceHighPoly::CalcWatchPositionFar()
{
	// ランダムに観戦位置を設定
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Far::LEFT_LINE[nIdxTeam], Far::RIGHT_LINE[nIdxTeam]);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random(Far::NEAR_LINE, Far::FAR_LINE);
	SetWatchPosition(posWatch);
}

//==========================================================================
// 観戦位置の計算処理 (横)
//==========================================================================
void CAudienceHighPoly::CalcWatchPositionSide()
{
	// ランダムに観戦位置を設定
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Side::LEFT_LINE[nIdxTeam], Side::RIGHT_LINE[nIdxTeam]);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random(Side::NEAR_LINE, Side::FAR_LINE);
	SetWatchPosition(posWatch);
}

//==========================================================================
// 観戦位置の計算処理 (手前)
//==========================================================================
void CAudienceHighPoly::CalcWatchPositionNear()
{
	// ランダムに観戦位置を設定
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Down::LEFT_LINE[nIdxTeam], Down::RIGHT_LINE[nIdxTeam]);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random(Down::NEAR_LINE, Down::FAR_LINE);
	SetWatchPosition(posWatch);
}

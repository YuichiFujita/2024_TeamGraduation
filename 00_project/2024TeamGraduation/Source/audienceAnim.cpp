//==========================================================================
// 
//  観客_アニメーション3D処理 [audienceAnim.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "audienceAnim.h"
#include "gameManager.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char*	SETUP_FRONT_TXT[] =	// 表面プレイヤーセットアップテキスト
	{
		"data\\CHARACTER\\suiren_front.txt",	// スイレンテキスト
		"data\\CHARACTER\\twintail_front.txt",	// ツインテテキスト
	};
	const char*	SETUP_BACK_TXT[] =	// 裏面プレイヤーセットアップテキスト
	{
		"data\\CHARACTER\\suiren_back.txt",		// スイレンテキスト
		"data\\CHARACTER\\twintail_back.txt",	// ツインテテキスト
	};

	const char*	LIGHT_PATH	= "data\\MODEL\\penlight.x";		// ペンライトのモデルパス
	const int	PRIORITY	= mylib_const::PRIORITY_DEFAULT;	// 優先順位

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
// 関数ポインタ
//==========================================================================
// 観戦位置計算リスト
CAudienceAnim::WATCH_POS_FUNC CAudienceAnim::m_CalcWatchPositionFunc[] =
{
	&CAudienceAnim::CalcWatchPositionFar,	// 奥観戦位置
	&CAudienceAnim::CalcWatchPositionUp,	// 上観戦位置
	&CAudienceAnim::CalcWatchPositionNear,	// 手前観戦位置
};

//==========================================================================
// コンストラクタ
//==========================================================================
CAudienceAnim::CAudienceAnim(EObjType type, CGameManager::ETeamSide team) : CAudience(type, team, PRIORITY, CObject::LAYER_DEFAULT),
	m_pFrontAnimChara	(nullptr),			// 表面キャラクター情報
	m_pBackAnimChara	(nullptr),			// 裏面キャラクター情報
	m_pLight			(nullptr),			// ペンライト情報
	m_pLightBlur		(nullptr),			// ペンライトのブラー
	m_idolMotion		(MOTION_IDOL_U),	// 待機モーション
	m_jumpMotion		(MOTION_IDOL_U),	// ジャンプモーション
	m_moveMotion		(MOTION_IDOL_U)		// 移動モーション
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

	// 観戦エリアの設定
	SetArea((int)(rand() % EArea::AREA_MAX));

	// 観戦位置の設定
	(this->*(m_CalcWatchPositionFunc[GetArea()]))();

	// ランダムに生成方向を設定
	float fTurn = 0.0f;	// 方向係数
	if ((bool)(rand() % 2))
	{
		// 右からの入場にする
		m_moveMotion = MOTION_MOVE_L;
		fTurn = 1.0f;
	}
	else
	{
		// 左からの入場にする
		m_moveMotion = MOTION_MOVE_R;
		fTurn = -1.0f;
	}

	// 生成位置の設定
	MyLib::Vector3 posSpawn = GetWatchPosition();
	posSpawn.x = CAudience::SPAWN_SIDE_LINE * fTurn;
	SetSpawnPosition(posSpawn);

	// 退場位置の設定
	MyLib::Vector3 posDespawn = posSpawn;
	posDespawn.x = -posSpawn.x;
	SetDespawnPosition(posDespawn);

	// アニメーションキャラクターの生成
	if (FAILED(CreateAnimCharacter(posSpawn)))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// ペンライトの生成
	if (FAILED(CreatePenLight()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// ペンライトのブラー生成
	if (FAILED(CreatePenLightBlur()))
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
void CAudienceAnim::Uninit()
{
	// 親クラスの終了
	CAudience::Uninit();

	// オブジェクトキャラクターアニメーションの終了
	SAFE_UNINIT(m_pFrontAnimChara);
	SAFE_UNINIT(m_pBackAnimChara);
}

//==========================================================================
// 削除処理
//==========================================================================
void CAudienceAnim::Kill()
{
	// 親クラスの削除
	CAudience::Kill();

	// オブジェクトキャラクターアニメーションの終了
	SAFE_UNINIT(m_pFrontAnimChara);
	SAFE_UNINIT(m_pBackAnimChara);
}

//==========================================================================
// 位置設定処理
//==========================================================================
void CAudienceAnim::SetPosition(const MyLib::Vector3& pos)
{
	// 親クラスの位置設定
	CObject::SetPosition(pos);

	// オブジェクトキャラクターアニメーションの位置設定
	m_pFrontAnimChara->SetPosition(pos);
	m_pBackAnimChara->SetPosition(pos);
}

//==========================================================================
// 移動量設定処理
//==========================================================================
void CAudienceAnim::SetMove(const MyLib::Vector3& move)
{
	// 親クラスの移動量設定
	CObject::SetMove(move);

	// オブジェクトキャラクターアニメーションの移動量設定
	m_pFrontAnimChara->SetMove(move);
	m_pBackAnimChara->SetMove(move);
}

//==========================================================================
// 向き設定処理
//==========================================================================
void CAudienceAnim::SetRotation(const MyLib::Vector3& rot)
{
	// 親クラスの向き設定
	CObject::SetRotation(rot);

	// オブジェクトキャラクターアニメーションの向き設定
	m_pFrontAnimChara->SetRotation(rot);
	m_pBackAnimChara->SetRotation(rot);
}

//==========================================================================
// 更新処理
//==========================================================================
void CAudienceAnim::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pFrontAnimChara != nullptr)
	{
		// 表面キャラクターの更新
		m_pFrontAnimChara->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	if (m_pBackAnimChara != nullptr)
	{
		// 裏面キャラクターの更新
		m_pBackAnimChara->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 親クラスの更新
	CAudience::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ペンライトの更新
//==========================================================================
void CAudienceAnim::UpdatePenlight(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pLight == nullptr) return;
	if (m_pFrontAnimChara == nullptr || m_pBackAnimChara == nullptr) return;

	// ライトの位置を頭の上にする
	m_pLight->SetPosition(GetPosition() + MyLib::Vector3(0.0f, 230.0f, 0.0f));	// TODO：後で振ったりさせる
	m_pLightBlur->SetPosition(m_pLight->GetPosition());
}

//==========================================================================
// 描画処理
//==========================================================================
void CAudienceAnim::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイス情報

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 180);

	if (m_pFrontAnimChara != nullptr)
	{
		// 表面キャラクターの描画
		m_pFrontAnimChara->Draw();
	}

	if (m_pBackAnimChara != nullptr)
	{
		// 裏面キャラクターの描画
		m_pBackAnimChara->Draw();
	}

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 親クラスの描画
	CAudience::Draw();
}

//==========================================================================
// 入場状態の更新処理
//==========================================================================
int CAudienceAnim::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 入場状態の更新
	CAudience::UpdateSpawn(fDeltaTime, fDeltaRate, fSlowRate);

	// ライトの自動描画をオフにする
	m_pLight->SetEnableDisp(false);
	m_pLightBlur->SetEnableDisp(false);

	// 移動モーションを返す
	return m_moveMotion;
}

//==========================================================================
// 通常状態の更新処理
//==========================================================================
int CAudienceAnim::UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 通常状態の更新
	CAudience::UpdateNormal(fDeltaTime, fDeltaRate, fSlowRate);

	// ライトの自動描画をオフにする
	m_pLight->SetEnableDisp(false);
	m_pLightBlur->SetEnableDisp(false);

	// 待機モーションを返す
	return m_idolMotion;
}

//==========================================================================
// 盛り上がり状態の更新処理
//==========================================================================
int CAudienceAnim::UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 盛り上がり状態の更新
	CAudience::UpdateJump(fDeltaTime, fDeltaRate, fSlowRate);

	// ライトの自動描画をオフにする
	m_pLight->SetEnableDisp(false);
	m_pLightBlur->SetEnableDisp(false);

	// ジャンプモーションを返す
	return m_jumpMotion;
}

//==========================================================================
// スペシャル状態の更新処理
//==========================================================================
int CAudienceAnim::UpdateSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// スペシャル状態の更新
	CAudience::UpdateSpecial(fDeltaTime, fDeltaRate, fSlowRate);

	// ライトの自動描画をオンにする
	m_pLight->SetEnableDisp(true);
	m_pLightBlur->SetEnableDisp(true);

	// ライトの位置を頭の上にする
	m_pLight->SetPosition(GetPosition() + MyLib::Vector3(0.0f, 230.0f, 0.0f));	// TODO：後で振ったりさせる
	m_pLightBlur->SetPosition(m_pLight->GetPosition());

	// ジャンプモーションを返す
	return m_jumpMotion;
}

//==========================================================================
// 退場状態の更新処理
//==========================================================================
int CAudienceAnim::UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 退場状態の更新
	CAudience::UpdateDespawn(fDeltaTime, fDeltaRate, fSlowRate);

	// ライトの自動描画をオフにする
	m_pLight->SetEnableDisp(false);
	m_pLightBlur->SetEnableDisp(false);

	// 移動モーションを返す
	return m_moveMotion;
}

//==========================================================================
// モーションの設定処理
//==========================================================================
void CAudienceAnim::SetMotion(const int nMotion)
{
	// キャラクターアニメーション情報がない場合抜ける
	if (m_pFrontAnimChara == nullptr) { return; }

	int nAnimMotion = m_pFrontAnimChara->GetMotion();	// 現在再生中のモーション
	if (m_pFrontAnimChara->IsLoop())
	{ // ループするモーションだった場合

		if (nAnimMotion != nMotion)
		{ // 現在のモーションが再生中のモーションと一致しない場合

			// 現在のモーションの設定
			m_pFrontAnimChara->SetMotion(nMotion);
			m_pBackAnimChara->SetMotion(nMotion);
		}
	}
}

//==========================================================================
// キャラクター生成
//==========================================================================
HRESULT CAudienceAnim::CreateAnimCharacter(const MyLib::Vector3& rPos)
{
	int nRand = UtilFunc::Transformation::Random(0, NUM_ARRAY(SETUP_FRONT_TXT) - 1);	// 使用パスインデックス

	// 表面キャラクターの生成
	m_pFrontAnimChara = CObjectCharaAnim::Create(rPos);
	if (m_pFrontAnimChara == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// キャラクター情報の割当
	m_pFrontAnimChara->BindCharaData(SETUP_FRONT_TXT[nRand]);

	// モーションの設定
	m_pFrontAnimChara->SetMotion(m_moveMotion);

	// アニメーション3Dの自動更新/自動描画/自動破棄をしない種類にする
	m_pFrontAnimChara->SetType(CObject::TYPE::TYPE_NONE);

	// 裏面キャラクターの生成
	m_pBackAnimChara = CObjectCharaAnim::Create(rPos);
	if (m_pBackAnimChara == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// キャラクター情報の割当
	m_pBackAnimChara->BindCharaData(SETUP_BACK_TXT[nRand]);

	// 向きを反転する
	m_pBackAnimChara->SetRotation(MyLib::Vector3(0.0f, D3DX_PI, 0.0f));

	// モーションの設定
	m_pBackAnimChara->SetMotion(m_moveMotion);

	// アニメーション3Dの自動更新/自動描画/自動破棄をしない種類にする
	m_pBackAnimChara->SetType(CObject::TYPE::TYPE_NONE);

	return S_OK;
}

//==========================================================================
// ペンライト生成
//==========================================================================
HRESULT CAudienceAnim::CreatePenLight()
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

	return S_OK;
}

//==========================================================================
// ペンライトのブラー生成
//==========================================================================
HRESULT CAudienceAnim::CreatePenLightBlur()
{
	// ペンライトの生成
	m_pLightBlur = CEffect3D::Create(
		MyLib::Vector3(0.0f, 0.0f, 0.0f),	// 位置
		MyLib::Vector3(0.0f, 0.0f, 0.0f),	// 移動量
		D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f),	// 色
		200.0f,								// 半径
		0,									// 寿命
		CEffect3D::MOVEEFFECT_NONE,			// 移動の種類
		CEffect3D::TYPE_POINT);				// エフェクトの種類
	if (m_pLightBlur == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 自動描画フラグをオフにする
	m_pLightBlur->SetEnableDisp(false);

	// 寿命削除OFF
	m_pLightBlur->SetEnableDeleteLife(false);
	return S_OK;
}

//==========================================================================
// NTRの設定処理
//==========================================================================
bool CAudienceAnim::SetNTR(CGameManager::ETeamSide team)
{
	// 入場状態にする
	CAudience::SetNTR(team);

	// 入場開始位置を保存
	SetSpawnPosition(GetPosition());	// 現在の位置

	// 観戦位置を設定し、生成情報を取得
	(this->*(m_CalcWatchPositionFunc[GetArea()]))();

	return true;
}

//==========================================================================
// 観戦位置の計算処理 (奥)
//==========================================================================
void CAudienceAnim::CalcWatchPositionFar()
{
	// ランダムに観戦位置を設定
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Far::LEFT_LINE[nIdxTeam], Far::RIGHT_LINE[nIdxTeam]);
	posWatch.y = GetLandY();
	posWatch.z = (float)UtilFunc::Transformation::Random(Far::NEAR_LINE, Far::FAR_LINE);
	SetWatchPosition(posWatch);

	// 目標の向き設定
	SetOriginRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));

	// モーションの設定
	m_idolMotion = MOTION_IDOL_D;	// 下待機モーション
	m_jumpMotion = MOTION_JUMP_D;	// 下ジャンプモーション
}

//==========================================================================
// 観戦位置の計算処理 (上)
//==========================================================================
void CAudienceAnim::CalcWatchPositionUp()
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

	// 目標の向き設定
	SetOriginRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));

	// モーションの設定
	m_idolMotion = MOTION_IDOL_D;	// 下待機モーション
	m_jumpMotion = MOTION_JUMP_D;	// 下ジャンプモーション
}

//==========================================================================
// 観戦位置の計算処理 (手前)
//==========================================================================
void CAudienceAnim::CalcWatchPositionNear()
{
	// ランダムに観戦位置を設定
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Down::LEFT_LINE[nIdxTeam], Down::RIGHT_LINE[nIdxTeam]);
	posWatch.y = GetLandY();
	posWatch.z = (float)UtilFunc::Transformation::Random(Down::NEAR_LINE, Down::FAR_LINE);
	SetWatchPosition(posWatch);

	// 目標の向き設定
	SetOriginRotation(MyLib::Vector3(0.0f, D3DX_PI, 0.0f));

	// モーションの設定
	m_idolMotion = MOTION_IDOL_U;	// 上待機モーション
	m_jumpMotion = MOTION_JUMP_U;	// 上ジャンプモーション
}

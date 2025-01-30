//=============================================================================
//
// モテマネージャ処理 [charmManager.cpp]
// Author : Kai Takada
//
//=============================================================================
#include "charmManager.h"
#include "manager.h"
#include "gameManager.h"
#include "player.h"

// デバッグ用
#include "collisionLine_Box.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float HYPE_TIME[CCharmValueManager::ETypeAdd::ADD_MAX] =	// 加算量
	{
		1.0f,	// ヒット
		2.0f,	// ジャストキャッチ
		3.0f,	// カバーキャッチ
		1.0f,	// ジャストキャッチ直後の投げ
		1.0f,	// ドロップキャッチ直後の投げ
		4.0f,	// 回避
		6.0f,	// スペシャル
		6.0f,	// スペシャルをキャッチした
	};

	const float TIME_HAVE_LONG = 10.0f;		// 持ち続けている(秒数)
	const float TIME_EDGE_ESCAPE = 3.0f;	// 端に逃げ続けている(秒数)
}

//==========================================================================
// スタティックアサート
//==========================================================================
static_assert(NUM_ARRAY(HYPE_TIME) == CCharmValueManager::ETypeAdd::ADD_MAX, "ERROR : Type Count Mismatch");

//==========================================================================
// 静的メンバ変数
//==========================================================================
CCharmManager* CCharmManager::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CCharmManager::CCharmManager()
{
#if _DEBUG
	for (int i = 0; i < 4; i++)
	{
		m_pCourtSizeBox[i] = nullptr;
	}
#endif // _DEBUG

	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		m_fHypeTime[i] = 0.0f;	// 盛り上がり時間
	}
}

//==========================================================================
// デストラクタ
//==========================================================================
CCharmManager::~CCharmManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCharmManager* CCharmManager::Create()
{
	if (m_pThisPtr != nullptr)
	{// 既にある場合
		return m_pThisPtr;
	}

	// メモリの確保
	m_pThisPtr = DEBUG_NEW CCharmManager;

	if (m_pThisPtr != nullptr)
	{
		// 初期化処理
		m_pThisPtr->Init();
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCharmManager::Init()
{

#if _DEBUG
	// コートサイズのボックス
	for (int i = 0; i < 4; i++)
	{
		SAFE_UNINIT(m_pCourtSizeBox[i]);
		m_pCourtSizeBox[i] = CCollisionLine_Box::Create(MyLib::AABB(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
#endif

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCharmManager::Uninit()
{
	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CCharmManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		if (m_fHypeTime[i] > 0.0f)
		{ // 盛り上がり時間が設定されている場合

			// 盛り上がり時間を減算
			m_fHypeTime[i] -= fDeltaTime * fSlowRate;
			if (m_fHypeTime[i] < 0.0f) { m_fHypeTime[i] = 0.0f; }	// 盛り上がり時間補正
		}
	}
}

//==========================================================================
// プリセット盛り上がり時間の取得処理
//==========================================================================
float CCharmManager::GetPrisetHypeTime(const CCharmValueManager::ETypeAdd preset)
{
	// 盛り上がり時間のプリセットを返す
	return HYPE_TIME[preset];
}

//==========================================================================
// 盛り上がり時間の設定処理 (プリセット)
//==========================================================================
void CCharmManager::SetHypeTime(const CGameManager::ETeamSide team, const CCharmValueManager::ETypeAdd preset)
{
	// チームが範囲外の場合エラー
	if (team <= CGameManager::ETeamSide::SIDE_NONE || team >= CGameManager::ETeamSide::SIDE_MAX) { assert(false); return; }

	// プリセットが範囲外の場合エラー
	if (preset <= -1 || preset >= CCharmValueManager::ETypeAdd::ADD_MAX) { assert(false); return; }

	// 盛り上がり時間をプリセットから設定
	m_fHypeTime[team] = HYPE_TIME[preset];
}

//==========================================================================
// 非モテまとめ
//==========================================================================
void CCharmManager::UnCharm(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 持ち続け
	LongHold(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);

	// 端に逃げまくる
	EdgeEscape(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 持ち続けている判定
//==========================================================================
void CCharmManager::LongHold(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	float fHaveTime = pPlayer->GetHaveTime();

	if (pPlayer->GetBall() == nullptr)
	{// リセット
		pPlayer->SetHaveTime(0.0f);
		return;
	}

	CGameManager* pGameMgr = CGameManager::GetInstance();

	if (pGameMgr->GetType() != CGameManager::ESceneType::SCENE_SPECIAL)
	{// スペシャル中以外なら

		// 加算
		fHaveTime += fDeltaTime * fSlowRate;
	}

	if (fHaveTime > TIME_HAVE_LONG)
	{// モテダウン

		// モテ減算
		pGameMgr->SubCharmValue(pPlayer->GetTeam(), CCharmValueManager::ETypeSub::SUB_LONG_HOLD);

		fHaveTime = 0.0;
	}
	
	pPlayer->SetHaveTime(fHaveTime);
}

//==========================================================================
// 端に逃げ続ける
//==========================================================================
void CCharmManager::EdgeEscape(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (pPlayer->GetAreaType() != CPlayer::EFieldArea::FIELD_IN) return;

	CGameManager* pGameMgr = CGameManager::GetInstance();
	float fEscapeTime = pPlayer->GetEscapeTime();

	if (CheckEdgeEscape(pPlayer))
	{// 端だったら

		// 加算
		fEscapeTime += fDeltaTime * fSlowRate;
	}
	else
	{// エリア外だったらリセット

		fEscapeTime = 0.0;
	}

	if (fEscapeTime >= TIME_EDGE_ESCAPE)
	{// モテダウン

		// モテ減算
		pGameMgr->SubCharmValue(pPlayer->GetTeam(), CCharmValueManager::ETypeSub::SUB_EDGE_ESCAPE);

		fEscapeTime = 0.0;
	}

	pPlayer->SetEscapeTime(fEscapeTime);
}

//==========================================================================
// 端に逃げ続ける
//==========================================================================
bool CCharmManager::CheckEdgeEscape(CPlayer* pPlayer)
{
	MyLib::Vector3 pos = pPlayer->GetPosition();
	CGameManager* pGmMgr = CGameManager::GetInstance();
	MyLib::Vector3 posCourt = MyLib::Vector3();											// 自陣中央
	MyLib::Vector3 sizeCourt = pGmMgr->GetCourtSize(pPlayer->GetTeam(), posCourt);		// 自陣サイズ

	// ボール情報
	CBall* pBall = pGmMgr->GetBall();
	CPlayer* pBallPlayer = pBall->GetPlayer();
	if (pBallPlayer == nullptr) return false;
	if (pBallPlayer->GetTeam() == pPlayer->GetTeam()) return false;

	MyLib::Vector3 posBP = pBallPlayer->GetPosition();

	// ボール所持プレイヤーと自陣中央のベクトル
	MyLib::Vector3 vecDiff = (posCourt - posBP).Normal();

	// 端エリアをベクトルから交差判定
	// 自陣4点(上下左右)
	MyLib::Vector3 posEdge[4] =
	{
		MyLib::Vector3(posCourt.x + sizeCourt.x, 0.0f, posCourt.z + sizeCourt.z),	// 右奥
		MyLib::Vector3(posCourt.x + sizeCourt.x, 0.0f, posCourt.z - sizeCourt.z),	// 右手前
		MyLib::Vector3(posCourt.x - sizeCourt.x, 0.0f, posCourt.z - sizeCourt.z),	// 左手前
		MyLib::Vector3(posCourt.x - sizeCourt.x, 0.0f, posCourt.z + sizeCourt.z),	// 左奥
	};

	MyLib::Vector3 AreaMid = MyLib::Vector3();	// 交点
	MyLib::Vector2 areaSize = MyLib::Vector2(sizeCourt.x, sizeCourt.z);		// エリアサイズ
	areaSize.x *= 0.8f;							//z長x短
	float fAngle = D3DX_PI * 0.5f;						//rot
	int nIdx = 0;

	// 判定
	for (int i = 0; i < 4; i++)
	{
		MyLib::Vector3 posEdgeNext = posEdge[0];
		if (i < 3)
		{// 通常
			posEdgeNext = posEdge[i + 1];
		}

		fAngle += D3DX_PI * 0.5f;
		UtilFunc::Transformation::RotNormalize(fAngle);

		// ボール持ちプレイヤーの対角にある自陣の辺を見つける
		// コート中央からボール持ちプレイヤーの矢印飛ばして当たるとこ
		if (UtilFunc::Collision::IsVecToLine(posEdge[i], posEdgeNext, posCourt, vecDiff, &AreaMid))
		{// ライン交差

			break;
		}
	}

	if (UtilFunc::Collision::CollisionSquare(AreaMid, areaSize, fAngle, pos))
	{// 端エリア内にいるか

		return true;
	}

	return false;
}

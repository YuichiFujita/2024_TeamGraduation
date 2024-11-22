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

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float TIME_HAVE_LONG = 10.0f;				// 持ち続けている(秒数)
	const float TIME_EDGE_ESCAPE = 1.0f;			// 端に逃げ続けている(秒数)
}

//==========================================================================
// 静的メンバ変数
//==========================================================================
CCharmManager* CCharmManager::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CCharmManager::CCharmManager()
{
	
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

	// 加算
	fHaveTime += fDeltaTime * fSlowRate;

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
	MyLib::Vector3 vecDiff = (posBP - posCourt).Normal() + D3DX_PI;
	UtilFunc::Transformation::RotNormalize(vecDiff);

	// 端エリアをベクトルから交差判定
	// 自陣4点(上下左右)
	MyLib::Vector3 posEdge[4] =
	{
		MyLib::Vector3(posCourt.x + sizeCourt.x, 0.0f, posCourt.z + sizeCourt.x),	// 右奥
		MyLib::Vector3(posCourt.x + sizeCourt.x, 0.0f, posCourt.z - sizeCourt.x),	// 右手前
		MyLib::Vector3(posCourt.x - sizeCourt.x, 0.0f, posCourt.z - sizeCourt.x),	// 左手前
		MyLib::Vector3(posCourt.x - sizeCourt.x, 0.0f, posCourt.z + sizeCourt.x),	// 左奥
	};

	MyLib::Vector3 lineMid = MyLib::Vector3();	// 交点
	MyLib::Vector2 areaSize = MyLib::Vector2(sizeCourt.x, sizeCourt.z);		// エリアサイズ
	areaSize.x *= 0.2f;							//z長x短

	// 判定
	for (int i = 0; i < 4; i++)
	{
		MyLib::Vector3 pos1 = posEdge[0];
		if (i < 3)
		{
			pos1 = posEdge[i + 1];
		}

		if (UtilFunc::Collision::IsVecToLine(posEdge[i], pos1, posCourt, vecDiff))
		{// ライン交差

			lineMid = UtilFunc::Calculation::GetCenterPosition3D(posEdge[i], pos1);

			areaSize += lineMid;
			// エリアサイズ設定
			if (lineMid.z == posCourt.z)
			{
				float f = areaSize.x;
				areaSize.x = areaSize.y;
				areaSize.y = f;
			}

			break;
		}

	}

	if (UtilFunc::Collision::CollisionSquare(lineMid, areaSize, 0.0f, pos))
	{// 端エリア内にいるか

		return true;
	}

	return false;
}

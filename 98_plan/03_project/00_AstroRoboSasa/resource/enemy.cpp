//============================================================
//
//	敵処理 [enemy.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "enemy.h"
#include "collision.h"
#include "player.h"
#include "bullet.h"
#include "gameManager.h"

//************************************************************
//	子クラス [CEnemy] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemy::CEnemy() : CObject3D(CObject::LABEL_ENEMY),
	m_move		(VEC3_ZERO),	// 移動量
	m_fCurTime	(0.0f)			// 経過時間
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemy::~CEnemy()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemy::Init(void)
{
	// メンバ変数を初期化
	m_move = VEC3_ZERO;	// 移動量
	m_fCurTime = 0.0f;	// 経過時間

	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	SetVec3Sizing(D3DXVECTOR3(30.0f, 30.0f, 0.0f));
	SetOrigin(ORIGIN_DOWN);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemy::Uninit(void)
{
	// オブジェクト3Dの終了
	CObject3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemy::Update(const float fDeltaTime)
{
	D3DXVECTOR3 posEnemy = GetVec3Position();	// 敵位置

	// 重力を与える
	m_move.y -= 0.45f;

	// 敵を移動させる
	posEnemy += m_move;

	// 移動に完成を与える
	m_move.x += (0.0f - m_move.x) * 0.08f;

	if (posEnemy.y < 50.0f)
	{ // 位置が地面より下の場合

		// 位置を補正
		posEnemy.y = 50.0f;

		// 移動量を初期化
		m_move.y = 0.0f;
	}

	// 画面外との当たり判定
	CGameManager::CollisionScreen(&posEnemy, &m_move);

	// 弾との当たり判定
	CollisionBullet(posEnemy);

	// プレイヤーとの当たり判定
	CollisionPlayer(posEnemy);

	// 位置の反映
	SetVec3Position(posEnemy);

	// オブジェクト3Dの更新
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CEnemy::Draw(CShader *pShader)
{
	// オブジェクト3Dの描画
	CObject3D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CEnemy *CEnemy::Create(const D3DXVECTOR3& rPos)
{
	// 敵の生成
	CEnemy *pEnemy = new CEnemy;
	if (pEnemy == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 敵の初期化
		if (FAILED(pEnemy->Init()))
		{ // 初期化に失敗した場合

			// 敵の破棄
			SAFE_DELETE(pEnemy);
			return nullptr;
		}

		// 位置を設定
		pEnemy->SetVec3Position(rPos);

		// 確保したアドレスを返す
		return pEnemy;
	}
}

//============================================================
//	弾との当たり判定
//============================================================
bool CEnemy::CollisionBullet(const D3DXVECTOR3& rPos)
{
	CListManager<CBullet> *pListManager = CBullet::GetList();	// リストマネージャー
	if (pListManager == nullptr) { return false; }				// リスト未使用の場合抜ける
	std::list<CBullet*> list = pListManager->GetList();			// 内部リスト
	for (auto& rList : list)
	{ // 要素の総数分繰り返す

		bool bHit = collision::Circle3D
		(
			rPos,
			rList->GetVec3Position(),
			15.0f,
			10.0f
		);
		if (bHit)
		{
			D3DXVECTOR3 vecKnock = rList->GetMove();
			D3DXVec3Normalize(&vecKnock, &vecKnock);

			m_move = vecKnock * 5.0f;
			rList->Uninit();
			return true;
		}
	}

	return false;
}

//============================================================
//	プレイヤーとの当たり判定
//============================================================
bool CEnemy::CollisionPlayer(const D3DXVECTOR3& rPos)
{
	CListManager<CPlayer> *pListManager = CPlayer::GetList();	// リストマネージャー
	if (pListManager == nullptr) { return false; }				// リスト未使用の場合抜ける
	std::list<CPlayer*> list = pListManager->GetList();			// 内部リスト
	for (auto& rList : list)
	{ // 要素の総数分繰り返す

		bool bHit = collision::Circle3D
		(
			rPos,
			rList->GetVec3Position(),
			15.0f,
			15.0f
		);
		if (bHit)
		{
			D3DXVECTOR3 vecKnock = rList->GetMove();
			D3DXVec3Normalize(&vecKnock, &vecKnock);

			m_move = vecKnock * 8.0f;
			return true;
		}
	}

	return false;
}

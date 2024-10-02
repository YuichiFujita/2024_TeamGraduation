//============================================================
//
//	プレイヤー処理 [player.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "player.h"
#include "manager.h"
#include "camera.h"
#include "gun.h"
#include "gameManager.h"
#include "effect2D.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CPlayer> *CPlayer::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	子クラス [CPlayer] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayer::CPlayer() : CObject3D(CObject::LABEL_PLAYER),
	m_pGun	 (nullptr),		// 銃の情報
	m_rotGun (VEC3_ZERO),	// 銃の向き
	m_move	 (VEC3_ZERO)	// 移動量
{

}

//============================================================
//	デストラクタ
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayer::Init(void)
{
	// メンバ変数を初期化
	m_pGun	 = nullptr;		// 銃の情報
	m_rotGun = VEC3_ZERO;	// 銃の向き
	m_move	 = VEC3_ZERO;	// 移動量

	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	SetVec3Position(D3DXVECTOR3(0.0f, 500.0f, 0.0f));
	SetVec3Sizing(D3DXVECTOR3(30.0f, 30.0f, 0.0f));
	SetOrigin(ORIGIN_DOWN);

	// 銃の生成
	m_pGun = CGun::Create(this);
	if (m_pGun == nullptr) { return E_FAIL; }

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CPlayer>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayer::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクト3Dの終了
	CObject3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayer::Update(const float fDeltaTime)
{
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置

	// 重力を与える
	m_move.y -= 0.45f;

	// プレイヤーを移動させる
	posPlayer += m_move;

	// 移動に完成を与える
	m_move.x += (0.0f - m_move.x) * 0.08f;

	if (posPlayer.y < 50.0f)
	{ // 位置が地面より下の場合

		// 位置を補正
		posPlayer.y = 50.0f;

		// 移動量を初期化
		m_move.y = 0.0f;
	}

	// 画面外との当たり判定
	CGameManager::CollisionScreen(&posPlayer, &m_move);

	POINT cursor;
	if (GetCursorPos(&cursor))
	{
		const char* CLASS_NAME = "WindowClass";	// ウインドウクラスの名前
		const char* WINDOW_NAME = "LastBattle";	// ウインドウの名前 (キャプションに表示)

		ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &cursor);
		D3DXVECTOR3 posCursor = D3DXVECTOR3((float)cursor.x, (float)cursor.y, 0.0f);

		D3DXVECTOR3 posScreenPlayer;
		GET_MANAGER->GetCamera()->OnScreen(posPlayer, posScreenPlayer);

		CEffect2D::Create(posCursor, 20.0f);
		GET_MANAGER->GetDebugProc()->Print(CDebugProc::POINT_CENTER, "%f %f", posCursor.x, posCursor.y);

		CEffect2D::Create(posScreenPlayer, 20.0f);
		GET_MANAGER->GetDebugProc()->Print(CDebugProc::POINT_CENTER, "%f %f", posScreenPlayer.x, posScreenPlayer.y);

		m_rotGun.z = atan2f(posCursor.x - posScreenPlayer.x, posCursor.y - posScreenPlayer.y) - HALF_PI;
	}

	// 位置の反映
	SetVec3Position(posPlayer);

	// オブジェクト3Dの更新
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CPlayer::Draw(CShader *pShader)
{
	// オブジェクト3Dの描画
	CObject3D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CPlayer *CPlayer::Create(void)
{
	// プレイヤーの生成
	CPlayer *pPlayer = new CPlayer;
	if (pPlayer == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// プレイヤーの初期化
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// プレイヤーの破棄
			SAFE_DELETE(pPlayer);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pPlayer;
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CPlayer> *CPlayer::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
//	反動の移動処理
//============================================================
void CPlayer::MoveGunRecoil(const float fRecScalar)
{
	// 反動スカラーをベクトルに変更
	D3DXVECTOR3 moveRec = D3DXVECTOR3(sinf(m_rotGun.z - HALF_PI), -cosf(m_rotGun.z - HALF_PI), 0.0f) * fRecScalar;

	// 移動量に反映
	m_move.y = 0.0f;
	m_move += moveRec;
}

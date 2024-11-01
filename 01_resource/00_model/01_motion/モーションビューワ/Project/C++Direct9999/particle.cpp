//=============================================================================
// 
//  パーティクル処理 [particle.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "particle.h"
#include "calculation.h"
#include "3D_effect.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
int CParticle::m_nNumAll = 0;	// 総数

//==========================================================================
// コンストラクタ
//==========================================================================
CParticle::CParticle(int nPriority)
{
	// 値のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_fRadius = 0.0f;							// 半径
	m_fAngle = 0.0f;							// 向き
	m_fMove = 0.0f;								// 移動量
	m_nLife = 0;								// 寿命
	m_nType = TYPE_NONE;						// 種類

	// 総数加算
	m_nNumAll++;
}

//==========================================================================
// デストラクタ
//==========================================================================
CParticle::~CParticle()
{
	// 総数減算
	m_nNumAll--;
}

//==========================================================================
// パーティクルの初期化処理
//==========================================================================
HRESULT CParticle::Init(void)
{

	// パーティクルの情報の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fRadius = 0.0f;
	m_nLife = 0;
	m_nType = TYPE_NONE;

	return S_OK;
}

//==================================================================================
// パーティクルの終了処理
//==================================================================================
void CParticle::Uninit(void)
{
	
}

//==================================================================================
// パーティクルの更新処理
//==================================================================================
void CParticle::Update(void)
{

	
}

//==================================================================================
// パーティクルの描画処理
//==================================================================================
void CParticle::Draw(void)
{
	
}

//==================================================================================
// パーティクルの設定処理
//==================================================================================
void CParticle::Create(D3DXVECTOR3 pos, TYPE nType)
{
	//return;

	// 位置
	m_pos = pos;

	// 種類
	m_nType = nType;

	switch (m_nType)
	{
	case TYPE_OFFSETTING:
		m_nLife = 30;
		OffSetting();
		break;

	case TYPE_SUPERATTACK:
		m_nLife = 30;
		SuperAttack();
		break;

	case TYPE_CHECK:
		m_nLife = 10;
		Check();
		break;

	case TYPE_ICE_SPAWN:
		m_nLife = 30;
		IceSpawn();
		break;

	case TYPE_CANCELABLE:
		m_nLife = 2;
		Cancelable();
		break;

	}
}

//==================================================================================
// キャンセル可能
//==================================================================================
void CParticle::Cancelable(void)
{
	for (int nCntUse = 0; nCntUse < 1; nCntUse++)
	{

		float fMove = 0.0f;		// 移動量
		float fAngle = 0.0f;	// 角度

		// 移動量の設定
		m_move.x = 0.0f;
		m_move.y = 40.0f;
		m_move.z = 0.0f;

		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		m_fRadius = 40.0f;

		// エフェクトの設定
		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_CANCEL);
	}
}

//==================================================================================
// チェック用
//==================================================================================
void CParticle::Check(void)
{
	for (int nCntUse = 0; nCntUse < 1; nCntUse++)
	{

		float fMove = 0.0f;		// 移動量
		float fAngle = 0.0f;	// 角度

		// 移動量の設定
		m_move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		m_move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		m_move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		m_fRadius = 40.0f;

		// エフェクトの設定
		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_SUB, CEffect3D::TYPE_NORMAL);
	}
}

//==================================================================================
//打ち消しのパーティクル
//==================================================================================
void CParticle::OffSetting(void)
{
	float fAngle, fMove;

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{

		fMove = (float)(rand() % 10) / 1 + 3.0f;		// 移動量
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	// 角度

		// 移動量の設定
		m_move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		m_move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		m_move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		m_fRadius = 40.0f;

		// エフェクトの設定
		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_SUB, CEffect3D::TYPE_NORMAL);
	}
}

//==================================================================================
// 強力攻撃
//==================================================================================
void CParticle::SuperAttack(void)
{
	for (int nCntUse = 0; nCntUse < 40; nCntUse++)
	{
		float fMove = (float)Random(0, 30) * 0.1f;	// 移動量
		float fMoveY = (float)Random(0, 50) * 0.1f;	// 移動量

		// 移動量の設定
		m_move.x = sinf((float)Random(-314, 314) / 100.0f) * fMove;
		m_move.y = cosf((float)Random(-20, 20) / 100.0f) * fMoveY;
		m_move.z = cosf((float)Random(-314, 314) / 100.0f) * fMove;

		m_col = D3DXCOLOR(1.0f, 0.6f, 0.3f, 1.0f);

		m_pos.x += m_move.x * 0.1f;
		m_pos.z += m_move.z * 0.1f;

		m_fRadius = 60.0f + (float)Random(-50, 100) * 0.1f;

		// エフェクトの設定
		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_SUB, CEffect3D::TYPE_SMOKE);
	}

	for (int nCntUse = 0; nCntUse < 20; nCntUse++)
	{
		float fMove = (float)Random(0, 25) * 0.1f;	// 移動量
		float fMoveY = (float)Random(0, 40) * 0.1f;	// 移動量

		// 移動量の設定
		m_move.x = sinf((float)Random(-314, 314) / 100.0f) * fMove;
		m_move.y = cosf((float)Random(-20, 20) / 100.0f) * fMoveY;
		m_move.z = cosf((float)Random(-314, 314) / 100.0f) * fMove;

		m_col = D3DXCOLOR(0.4f, 0.1f, 0.0f, 0.6f);

		m_pos.x += m_move.x * 0.5f;
		m_pos.z += m_move.z * 0.5f;

		m_fRadius = 40.0f + (float)Random(-50, 100) * 0.1f;

		// エフェクトの設定
		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_SUB, CEffect3D::TYPE_SMOKEBLACK);
	}
}

//==========================================================================
// 氷出現
//==========================================================================
void CParticle::IceSpawn(void)
{
	for (int nCntUse = 0; nCntUse < 2; nCntUse++)
	{
		float fMove = 0.0f;		// 移動量

		// 移動量の設定
		m_move.x = sinf((float)Random(-314, 314) / 100.0f) * fMove;
		m_move.y = cosf((float)Random(-314, 314) / 100.0f) * fMove;
		m_move.z = cosf((float)Random(-314, 314) / 100.0f) * fMove;

		// 色設定
		m_col = D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f);

		// 半径設定
		m_fRadius = 120.0f;

		// エフェクトの設定
		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_SUB, CEffect3D::TYPE_JUJI);
	}
}

//==========================================================================
// 総数取得
//==========================================================================
int CParticle::GetNumAll(void)
{
	return m_nNumAll;
}

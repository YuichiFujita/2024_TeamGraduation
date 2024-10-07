//=============================================================================
// 
//  パーティクル処理 [my_particle.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "particle.h"
#include "calculation.h"
#include "2D_effect.h"
#include "3D_effect.h"
using namespace my_particle;	// 名前空間を指定

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
MyLib::Vector3 m_pos;		// 位置
MyLib::Vector3 m_move;		// 移動量
D3DXCOLOR m_col;		// 色
float m_fRadius;		// 半径
float m_fAngle;			// 向き
float m_fMove;			// 移動量
int m_nLife;			// 寿命
TYPE m_nType;			// 種類

HRESULT Init();
void Smoke();
void SmokeRed();
void SmokeYel();
void EnemyFade();
void OffSetting();
void OffSetting_2D();
void Fire_WallTorch();
void Fire_StandTorch();
void BattleStart();
void FloweringCharge();
void Pollen_Drop();
void Pollen_Lost();

//==========================================================================
// パーティクルの初期化処理
//==========================================================================
HRESULT Init()
{

	// パーティクルの情報の初期化
	m_pos = MyLib::Vector3(0.0f, 0.0f, 0.0f);		// 位置
	m_move = MyLib::Vector3(0.0f, 0.0f, 0.0f);		// 移動量
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fRadius = 0.0f;
	m_nLife = 0;
	m_nType = TYPE_NONE;

	return S_OK;
}

//==========================================================================
// パーティクルの設定処理
//==========================================================================
void my_particle::Create(const MyLib::Vector3& pos, TYPE nType)
{
	Init();

	// 位置
	m_pos = pos;

	// 種類
	m_nType = nType;

	switch (m_nType)
	{
	case TYPE_EXPLOSION:
		m_nLife = 30;
		Smoke();
		SmokeRed();
		SmokeYel();
		break;

	case TYPE_SMOKE:

		m_nLife = 30;
		Smoke();
		break;

	case TYPE_SMOKE_RED:

		m_nLife = 30;
		SmokeRed();
		break;


	case TYPE_SMOKE_YEL:

		m_nLife = 30;
		SmokeYel();
		break;

	case TYPE_OFFSETTING:
		m_nLife = 30;
		OffSetting();
		break;

	case TYPE_OFFSETTING_2D:
		m_nLife = 30;
		OffSetting_2D();
		break;

	case TYPE_ENEMY_FADE:
		m_nLife = 60;
		EnemyFade();
		break;

	case TYPE_FIRE_WALLTORCH:
		Fire_WallTorch();
		break;

	case TYPE_FIRE_STANDTORCH:
		Fire_StandTorch();
		break;

	case TYPE_BATTLESTART:
		m_nLife = 30;
		BattleStart();
		break;

	case TYPE_FLOWERINGCHARGE:
		m_nLife = 30;
		FloweringCharge();
		break;
	case TYPE_POLLENDROP:
		m_nLife = 30;
		Pollen_Drop();
		break;
	case TYPE_POLLENLOST:
		m_nLife = 30;
		Pollen_Lost();
		break;
	}
}

//==========================================================================
// チェック用
//==========================================================================
void Check()
{
	for (int nCntUse = 0; nCntUse < 1; nCntUse++)
	{
		// 移動量の設定
		m_move = mylib_const::DEFAULT_VECTOR3;

		m_col = D3DXCOLOR(1.0f, 0.6f, 0.2f, 1.0f);

		m_fRadius = 10.0f;

		// エフェクトの設定
		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}
}

//==========================================================================
// チェック用
//==========================================================================
void Check2D()
{
	for (int nCntUse = 0; nCntUse < 5; nCntUse++)
	{

		float fMove = 0.0f;		// 移動量
		float fAngle = 0.0f;	// 角度

		// 移動量の設定
		m_move.x = 0.0f;
		m_move.y = 0.0f;
		m_move.z = 0.0f;

		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		m_fRadius = 20.0f;

		// エフェクトの設定
		CEffect2D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect2D::MOVEEFFECT_NONE, CEffect2D::TYPE_NORMAL);
	}
}

//==========================================================================
// 煙のパーティクル
//==========================================================================
void Smoke()
{
	
	for (int nCntUse = 0; nCntUse < 3; nCntUse++)
	{
		m_fMove = (float)(UtilFunc::Transformation::Random(0, 10)) / 10 + 1.0f;		// 移動量
		m_fAngle = (float)(UtilFunc::Transformation::Random(-50, 50)) / 100.0f;	// 角度

		// 移動量の設定
		m_move.x = sinf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * m_fMove;
		m_move.y = cosf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * m_fMove;
		m_move.z = cosf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * m_fMove;

		m_pos.x += m_move.x * 0.3f;
		m_pos.y += m_move.y * 0.3f;

		m_col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.5f);

		m_fRadius = 60.0f;

		if (rand() % 3 == 0)
		{
			m_fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			m_fRadius *= 0.9f;
		}
		else if(rand() % 3 == 2)
		{
			m_fRadius *= 0.8f;
		}

		// エフェクトの設定
		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_ADD, CEffect3D::TYPE_SMOKEBLACK);
	}
}

//==========================================================================
// 煙のパーティクル(赤)
//==========================================================================
void SmokeRed()
{

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		m_fMove = (float)(UtilFunc::Transformation::Random(0, 10)) / 10 + 2.0f;		// 移動量
		m_fAngle = (float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f;	// 角度

		// 移動量の設定
		m_move.x = sinf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * m_fMove;
		m_move.y = cosf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * m_fMove;
		m_move.z = cosf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * m_fMove;

		m_pos.x += m_move.x * 0.3f;
		m_pos.y += m_move.y * 0.3f;

		m_col = D3DXCOLOR(1.0f, 0.4f, 0.3f, 1.0f);

		m_fRadius = 30.0f;

		if (rand() % 3 == 0)
		{
			m_fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			m_fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			m_fRadius *= 0.8f;
		}

		// エフェクトの設定
		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_ADD, CEffect3D::TYPE_SMOKEBLACK);
	}
}

//==========================================================================
// 煙のパーティクル(黄)
//==========================================================================
void SmokeYel()
{

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		m_fMove = (float)(UtilFunc::Transformation::Random(0, 10)) / 10 + 2.0f;		// 移動量
		m_fAngle = (float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f;	// 角度

		// 移動量の設定
		m_move.x = sinf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * m_fMove;
		m_move.y = cosf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * m_fMove;
		m_move.z = cosf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * m_fMove;

		m_pos.x += m_move.x * 0.3f;
		m_pos.y += m_move.y * 0.3f;

		m_col = D3DXCOLOR(0.8f, 0.5f, 0.0f, 1.0f);

		m_fRadius = 30.0f;

		if (rand() % 3 == 0)
		{
			m_fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			m_fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			m_fRadius *= 0.8f;
		}

		// エフェクトの設定
		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_ADD, CEffect3D::TYPE_SMOKEBLACK);
	}
}

//==========================================================================
//打ち消しのパーティクル
//==========================================================================
void OffSetting()
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
		if (CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_SUB, CEffect3D::TYPE_NORMAL) == nullptr)
		{
			return;
		}
	}
}

//==========================================================================
// 打ち消しのパーティクル(2D)
//==========================================================================
void OffSetting_2D()
{
	float fAngle, fMove;

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{

		fMove = (float)(rand() % 10) / 1 + 3.0f;		// 移動量
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	// 角度

		// 移動量の設定
		m_move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		m_move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		m_move.z = 0.0f;

		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		m_fRadius = 40.0f;

		// エフェクトの設定
		CEffect2D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect2D::MOVEEFFECT_SUB, CEffect2D::TYPE_NORMAL);
	}
}

//==========================================================================
// 敵フェード
//==========================================================================
void EnemyFade()
{
	for (int nCntUse = 0; nCntUse < 15; nCntUse++)
	{
		float fMoveXZ = (float)UtilFunc::Transformation::Random(0, 100) * 0.01f;		// 移動量
		float fMove = (float)UtilFunc::Transformation::Random(0, 60) * 0.1f + 1.0f;	// 移動量

		// 移動量の設定
		m_move.x = sinf(UtilFunc::Transformation::GetRandomPi()) * fMoveXZ;
		m_move.y = sinf((float)UtilFunc::Transformation::Random(0, 314) / 100.0f) * fMove;
		m_move.z = cosf(UtilFunc::Transformation::GetRandomPi()) * fMoveXZ;

		// 出現地点のブレ
		m_pos.y += UtilFunc::Transformation::Random(-100, 100) * 0.1f;
		m_pos.x += UtilFunc::Transformation::Random(-100, 100) * 0.1f;
		m_pos.z += UtilFunc::Transformation::Random(-100, 100) * 0.1f;

		m_fRadius = 20.0f + UtilFunc::Transformation::Random(-50, 50) * 0.1f;

		// 色決め
		int nRand = 0;
		//int nRand = UtilFunc::Transformation::Random(0, 1);
		switch (nRand)
		{
		case 0:
			m_col = D3DXCOLOR(
				0.2f + UtilFunc::Transformation::Random(-100, 100) * 0.001f,
				0.8f + UtilFunc::Transformation::Random(-200, 200) * 0.001f,
				0.2f + UtilFunc::Transformation::Random(-100, 100) * 0.001f + UtilFunc::Transformation::Random(0, 20) * 0.01f, 1.0f);
			break;

		case 1:
			m_col = D3DXCOLOR(
				0.8f + UtilFunc::Transformation::Random(-50, 50) * 0.001f,
				0.2f + UtilFunc::Transformation::Random(-50, 50) * 0.001f,
				0.8f + UtilFunc::Transformation::Random(-50, 50) * 0.001f, 1.0f);
			break;

		default:
			break;
		}

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

//==========================================================================
// 置き松明
//==========================================================================
void Fire_StandTorch()
{
	for (int nCntUse = 0; nCntUse < 2; nCntUse++)
	{
		// 炎
		float fMoveY = 3.5f + UtilFunc::Transformation::Random(-10, 10) * 0.1f;
		float fMove = 0.5f + UtilFunc::Transformation::Random(-20, 20) * 0.01f;
		float fRot = UtilFunc::Transformation::Random(-31, 31) * 0.1f;

		// 移動量の設定
		m_move.x = sinf(fRot) * fMove;
		m_move.y = fMoveY;
		m_move.z = cosf(fRot) * fMove;

		m_fRadius = 70.0f + UtilFunc::Transformation::Random(-50, 50) * 0.1f;

		m_col = D3DXCOLOR(0.9f + UtilFunc::Transformation::Random(-10, 0) * 0.01f, 0.0f, 0.0f, 0.6f);

		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			25,
			CEffect3D::MOVEEFFECT_ADD,
			CEffect3D::TYPE_SMOKE);

		// 炎
		m_fRadius = 50.0f + UtilFunc::Transformation::Random(-50, 50) * 0.1f;

		m_col = D3DXCOLOR(
			0.8f + UtilFunc::Transformation::Random(-10, 0) * 0.01f,
			0.5f + UtilFunc::Transformation::Random(-10, 0) * 0.01f,
			0.0f, 0.5f);

		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			25,
			CEffect3D::MOVEEFFECT_ADD,
			CEffect3D::TYPE_SMOKE);
	}
}

//==========================================================================
// 壁松明
//==========================================================================
void Fire_WallTorch()
{
	for (int nCntUse = 0; nCntUse < 2; nCntUse++)
	{
		// 炎
		float fMoveY = 3.5f + UtilFunc::Transformation::Random(-10, 10) * 0.1f;
		float fMove = 0.5f + UtilFunc::Transformation::Random(-20, 20) * 0.01f;
		float fRot = UtilFunc::Transformation::Random(-31, 31) * 0.1f;

		// 移動量の設定
		m_move.x = sinf(fRot) * fMove;
		m_move.y = fMoveY;
		m_move.z = cosf(fRot) * fMove;

		m_fRadius = 40.0f + UtilFunc::Transformation::Random(-50, 50) * 0.1f;

		m_col = D3DXCOLOR(0.9f + UtilFunc::Transformation::Random(-10, 0) * 0.01f, 0.0f, 0.0f, 0.6f);

		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			25,
			CEffect3D::MOVEEFFECT_ADD,
			CEffect3D::TYPE_SMOKE);

		// 炎
		m_fRadius = 30.0f + UtilFunc::Transformation::Random(-50, 50) * 0.1f;

		m_col = D3DXCOLOR(
			0.8f + UtilFunc::Transformation::Random(-10, 0) * 0.01f,
			0.5f + UtilFunc::Transformation::Random(-10, 0) * 0.01f,
			0.0f, 0.6f);

		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			25,
			CEffect3D::MOVEEFFECT_ADD,
			CEffect3D::TYPE_SMOKE);
	}
}

//==========================================================================
// 戦闘開始
//==========================================================================
void BattleStart()
{
	float m_fMove;
	m_fMove = (float)(rand() % 30) / 10 + 2.0f;		// 移動量

	for (int nCntCircle = 0; nCntCircle < 30; nCntCircle++)
	{
		float fRot = ((D3DX_PI * 2.0f) / (float)(15 % (nCntCircle + 1))) * nCntCircle;
		fRot -= D3DX_PI;

		for (int nCntUse = 0; nCntUse < 1; nCntUse++)
		{
			float fRandMove = (float)(UtilFunc::Transformation::Random(-15, 15) / 10.0f);		// バラバラ移動量

			float fRotRand = (float)(UtilFunc::Transformation::Random(-10, 10) / 100.0f);		// バラバラ向き

			// 移動量の設定
			m_move.x = sinf(fRot + fRotRand) * (m_fMove + fRandMove);
			m_move.z = 0.0f;
			m_move.y = cosf(fRot + fRotRand) * (m_fMove + fRandMove);

			m_col = D3DXCOLOR(0.9f, 0.5f, 0.1f, 1.0f);

			m_fRadius = 80.0f;

			if (rand() % 3 == 0)
			{
				m_fRadius *= 0.95f;
			}
			else if (rand() % 3 == 1)
			{
				m_fRadius *= 0.98f;
			}
			else if (rand() % 3 == 2)
			{
				m_fRadius *= 0.92f;
			}

			// エフェクトの設定
			CEffect2D::Create(m_pos,
				m_move,
				m_col,
				m_fRadius,
				50,
				CEffect2D::MOVEEFFECT_ADD, CEffect2D::TYPE::TYPE_JUJI);
		}
	}








	float fMove;
	fMove = (float)UtilFunc::Transformation::Random(20, 30) * 0.1f + 5.0f;		//移動量

	float fff = (D3DX_PI * 2.0f) / 40;

	for (int nCntCircle = 0; nCntCircle < 40; nCntCircle++)
	{
		float fRot = fff * nCntCircle;

		for (int nCntUse = 0; nCntUse < 10; nCntUse++)
		{
			float fRandMove = (float)UtilFunc::Transformation::Random(-15, 15) / 10;		//バラバラ移動量

			float fRotRand = (float)UtilFunc::Transformation::Random(-10, 10) / 100.0f;		//バラバラ向き

			//移動量の設定
			m_move.x = sinf(fRot + fRotRand) * (fMove + fRandMove);
			m_move.z = 0.0f;
			m_move.y = cosf(fRot + fRotRand) * (fMove + fRandMove);

			m_col = D3DXCOLOR(0.9f, 0.5f, 0.1f, 1.0f);

			m_fRadius = 15.0f;

			if (rand() % 3 == 0)
			{
				m_fRadius *= 0.95f;
			}
			else if (rand() % 3 == 1)
			{
				m_fRadius *= 0.98f;
			}
			else if (rand() % 3 == 2)
			{
				m_fRadius *= 0.92f;
			}

			//エフェクトの設定
			CEffect2D::Create(m_pos,
				m_move,
				m_col,
				m_fRadius,
				m_nLife,
				CEffect2D::MOVEEFFECT_SUB, CEffect2D::TYPE::TYPE_JUJI);
		}
	}


	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		fMove = (float)(rand() % 50) * 0.1f;		//移動量

		//移動量の設定
		m_move.x = sinf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * fMove;
		m_move.y = sinf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * fMove;
		m_move.z = 0.0f;

		//出現地点のブレ
		m_pos.x += m_move.x * 0.1f;
		m_pos.y += m_move.z * 0.1f;
		m_pos.z = 0;

		m_col = D3DXCOLOR(0.9f, 0.6f, 0.3f, 0.8f);

		m_fRadius = 200.0f;

		if (rand() % 3 == 0)
		{
			m_fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			m_fRadius *= 0.98f;
		}
		else if (rand() % 3 == 2)
		{
			m_fRadius *= 0.92f;
		}

		//エフェクトの設定
		CEffect2D::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect2D::MOVEEFFECT_ADD, CEffect2D::TYPE::TYPE_JUJI);
	}
}


//==========================================================================
// 開花チャージ
//==========================================================================
void FloweringCharge(void)
{
	MyLib::Vector3 pos;

	for (int nCntUse = 0; nCntUse < 2; nCntUse++)
	{
		float fBuff = (float)UtilFunc::Transformation::Random(80, 100) * 0.01f;
		float fDistance = 400.0f * fBuff;
		m_nLife = (int)(20.0f * fBuff);

		// 出現位置
		pos = UtilFunc::Transformation::GetRandomPositionSphere(m_pos, fDistance);

		m_col = D3DXCOLOR(
			0.1f + UtilFunc::Transformation::Random(-100, 100) * 0.001f,
			0.9f + UtilFunc::Transformation::Random(-100, 100) * 0.001f,
			0.1f + UtilFunc::Transformation::Random(-100, 100) * 0.001f,
			1.0f);

		// 半径設定
		m_fRadius = 120.0f * fBuff;

		// エフェクトの設定
		CEffect3D* pEffect = CEffect3D::Create(
			pos,
			mylib_const::DEFAULT_VECTOR3,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_SUB,
			CEffect3D::TYPE::TYPE_JUJI2,
			0.0f);

		// 目標の位置設定
		pEffect->SetPositionDest(m_pos);
		pEffect->SetRotation(MyLib::Vector3(0.0f, 0.0f, UtilFunc::Transformation::GetRandomPi()));
	}
}

//==========================================================================
// 花粉落とす
//==========================================================================
void Pollen_Drop()
{
	for (int nCntUse = 0; nCntUse < 1; nCntUse++)
	{
		m_fMove = (float)(UtilFunc::Transformation::Random(0, 10)) / 10 + 1.0f;		// 移動量
		m_fAngle = (float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f;	// 角度

		// 移動量の設定
		m_move.x = sinf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * m_fMove;
		m_move.y = cosf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * m_fMove;
		m_move.z = cosf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * m_fMove;

		m_pos.x += m_move.x * 0.3f;
		m_pos.y += m_move.y * 0.3f;

		m_col = D3DXCOLOR(1.0f, 1.0f, 0.3f, 0.6f);

		m_fRadius = 70.0f;

		if (rand() % 3 == 0)
		{
			m_fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			m_fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			m_fRadius *= 0.8f;
		}

		// エフェクトの設定
		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_ADD, CEffect3D::TYPE_SMOKEBLACK);
	}
}

//==========================================================================
// ぶつかって花粉落とす
//==========================================================================
void Pollen_Lost()
{

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		m_fMove = (float)(UtilFunc::Transformation::Random(80, 100)) / 10 + 2.0f;		// 移動量
		m_fAngle = (float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f;	// 角度

		// 移動量の設定
		m_move.x = sinf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * m_fMove;
		m_move.y = cosf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * m_fMove;
		m_move.z = cosf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * m_fMove;

		m_pos.x += m_move.x * 0.3f;
		m_pos.y += m_move.y * 0.3f;

		m_col = D3DXCOLOR(1.0f, 1.0f, 0.3f, 1.0f);

		m_fRadius = 200.0f;

		if (rand() % 3 == 0)
		{
			m_fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			m_fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			m_fRadius *= 0.8f;
		}

		// エフェクトの設定
		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_ADD, CEffect3D::TYPE_SMOKEBLACK);
	}
}
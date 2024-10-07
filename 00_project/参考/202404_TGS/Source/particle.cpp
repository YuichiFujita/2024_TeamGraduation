//=============================================================================
// 
//  �p�[�e�B�N������ [my_particle.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "particle.h"
#include "calculation.h"
#include "2D_effect.h"
#include "3D_effect.h"
using namespace my_particle;	// ���O��Ԃ��w��

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
MyLib::Vector3 m_pos;		// �ʒu
MyLib::Vector3 m_move;		// �ړ���
D3DXCOLOR m_col;		// �F
float m_fRadius;		// ���a
float m_fAngle;			// ����
float m_fMove;			// �ړ���
int m_nLife;			// ����
TYPE m_nType;			// ���

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
// �p�[�e�B�N���̏���������
//==========================================================================
HRESULT Init()
{

	// �p�[�e�B�N���̏��̏�����
	m_pos = MyLib::Vector3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_move = MyLib::Vector3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fRadius = 0.0f;
	m_nLife = 0;
	m_nType = TYPE_NONE;

	return S_OK;
}

//==========================================================================
// �p�[�e�B�N���̐ݒ菈��
//==========================================================================
void my_particle::Create(const MyLib::Vector3& pos, TYPE nType)
{
	Init();

	// �ʒu
	m_pos = pos;

	// ���
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
// �`�F�b�N�p
//==========================================================================
void Check()
{
	for (int nCntUse = 0; nCntUse < 1; nCntUse++)
	{
		// �ړ��ʂ̐ݒ�
		m_move = mylib_const::DEFAULT_VECTOR3;

		m_col = D3DXCOLOR(1.0f, 0.6f, 0.2f, 1.0f);

		m_fRadius = 10.0f;

		// �G�t�F�N�g�̐ݒ�
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
// �`�F�b�N�p
//==========================================================================
void Check2D()
{
	for (int nCntUse = 0; nCntUse < 5; nCntUse++)
	{

		float fMove = 0.0f;		// �ړ���
		float fAngle = 0.0f;	// �p�x

		// �ړ��ʂ̐ݒ�
		m_move.x = 0.0f;
		m_move.y = 0.0f;
		m_move.z = 0.0f;

		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		m_fRadius = 20.0f;

		// �G�t�F�N�g�̐ݒ�
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
// ���̃p�[�e�B�N��
//==========================================================================
void Smoke()
{
	
	for (int nCntUse = 0; nCntUse < 3; nCntUse++)
	{
		m_fMove = (float)(UtilFunc::Transformation::Random(0, 10)) / 10 + 1.0f;		// �ړ���
		m_fAngle = (float)(UtilFunc::Transformation::Random(-50, 50)) / 100.0f;	// �p�x

		// �ړ��ʂ̐ݒ�
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

		// �G�t�F�N�g�̐ݒ�
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
// ���̃p�[�e�B�N��(��)
//==========================================================================
void SmokeRed()
{

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		m_fMove = (float)(UtilFunc::Transformation::Random(0, 10)) / 10 + 2.0f;		// �ړ���
		m_fAngle = (float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f;	// �p�x

		// �ړ��ʂ̐ݒ�
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

		// �G�t�F�N�g�̐ݒ�
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
// ���̃p�[�e�B�N��(��)
//==========================================================================
void SmokeYel()
{

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		m_fMove = (float)(UtilFunc::Transformation::Random(0, 10)) / 10 + 2.0f;		// �ړ���
		m_fAngle = (float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f;	// �p�x

		// �ړ��ʂ̐ݒ�
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

		// �G�t�F�N�g�̐ݒ�
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
//�ł������̃p�[�e�B�N��
//==========================================================================
void OffSetting()
{
	float fAngle, fMove;

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{

		fMove = (float)(rand() % 10) / 1 + 3.0f;		// �ړ���
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	// �p�x

		// �ړ��ʂ̐ݒ�
		m_move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		m_move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		m_move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		m_fRadius = 40.0f;

		// �G�t�F�N�g�̐ݒ�
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
// �ł������̃p�[�e�B�N��(2D)
//==========================================================================
void OffSetting_2D()
{
	float fAngle, fMove;

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{

		fMove = (float)(rand() % 10) / 1 + 3.0f;		// �ړ���
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	// �p�x

		// �ړ��ʂ̐ݒ�
		m_move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		m_move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		m_move.z = 0.0f;

		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		m_fRadius = 40.0f;

		// �G�t�F�N�g�̐ݒ�
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
// �G�t�F�[�h
//==========================================================================
void EnemyFade()
{
	for (int nCntUse = 0; nCntUse < 15; nCntUse++)
	{
		float fMoveXZ = (float)UtilFunc::Transformation::Random(0, 100) * 0.01f;		// �ړ���
		float fMove = (float)UtilFunc::Transformation::Random(0, 60) * 0.1f + 1.0f;	// �ړ���

		// �ړ��ʂ̐ݒ�
		m_move.x = sinf(UtilFunc::Transformation::GetRandomPi()) * fMoveXZ;
		m_move.y = sinf((float)UtilFunc::Transformation::Random(0, 314) / 100.0f) * fMove;
		m_move.z = cosf(UtilFunc::Transformation::GetRandomPi()) * fMoveXZ;

		// �o���n�_�̃u��
		m_pos.y += UtilFunc::Transformation::Random(-100, 100) * 0.1f;
		m_pos.x += UtilFunc::Transformation::Random(-100, 100) * 0.1f;
		m_pos.z += UtilFunc::Transformation::Random(-100, 100) * 0.1f;

		m_fRadius = 20.0f + UtilFunc::Transformation::Random(-50, 50) * 0.1f;

		// �F����
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

		// �G�t�F�N�g�̐ݒ�
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
// �u������
//==========================================================================
void Fire_StandTorch()
{
	for (int nCntUse = 0; nCntUse < 2; nCntUse++)
	{
		// ��
		float fMoveY = 3.5f + UtilFunc::Transformation::Random(-10, 10) * 0.1f;
		float fMove = 0.5f + UtilFunc::Transformation::Random(-20, 20) * 0.01f;
		float fRot = UtilFunc::Transformation::Random(-31, 31) * 0.1f;

		// �ړ��ʂ̐ݒ�
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

		// ��
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
// �Ǐ���
//==========================================================================
void Fire_WallTorch()
{
	for (int nCntUse = 0; nCntUse < 2; nCntUse++)
	{
		// ��
		float fMoveY = 3.5f + UtilFunc::Transformation::Random(-10, 10) * 0.1f;
		float fMove = 0.5f + UtilFunc::Transformation::Random(-20, 20) * 0.01f;
		float fRot = UtilFunc::Transformation::Random(-31, 31) * 0.1f;

		// �ړ��ʂ̐ݒ�
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

		// ��
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
// �퓬�J�n
//==========================================================================
void BattleStart()
{
	float m_fMove;
	m_fMove = (float)(rand() % 30) / 10 + 2.0f;		// �ړ���

	for (int nCntCircle = 0; nCntCircle < 30; nCntCircle++)
	{
		float fRot = ((D3DX_PI * 2.0f) / (float)(15 % (nCntCircle + 1))) * nCntCircle;
		fRot -= D3DX_PI;

		for (int nCntUse = 0; nCntUse < 1; nCntUse++)
		{
			float fRandMove = (float)(UtilFunc::Transformation::Random(-15, 15) / 10.0f);		// �o���o���ړ���

			float fRotRand = (float)(UtilFunc::Transformation::Random(-10, 10) / 100.0f);		// �o���o������

			// �ړ��ʂ̐ݒ�
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

			// �G�t�F�N�g�̐ݒ�
			CEffect2D::Create(m_pos,
				m_move,
				m_col,
				m_fRadius,
				50,
				CEffect2D::MOVEEFFECT_ADD, CEffect2D::TYPE::TYPE_JUJI);
		}
	}








	float fMove;
	fMove = (float)UtilFunc::Transformation::Random(20, 30) * 0.1f + 5.0f;		//�ړ���

	float fff = (D3DX_PI * 2.0f) / 40;

	for (int nCntCircle = 0; nCntCircle < 40; nCntCircle++)
	{
		float fRot = fff * nCntCircle;

		for (int nCntUse = 0; nCntUse < 10; nCntUse++)
		{
			float fRandMove = (float)UtilFunc::Transformation::Random(-15, 15) / 10;		//�o���o���ړ���

			float fRotRand = (float)UtilFunc::Transformation::Random(-10, 10) / 100.0f;		//�o���o������

			//�ړ��ʂ̐ݒ�
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

			//�G�t�F�N�g�̐ݒ�
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
		fMove = (float)(rand() % 50) * 0.1f;		//�ړ���

		//�ړ��ʂ̐ݒ�
		m_move.x = sinf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * fMove;
		m_move.y = sinf((float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f) * fMove;
		m_move.z = 0.0f;

		//�o���n�_�̃u��
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

		//�G�t�F�N�g�̐ݒ�
		CEffect2D::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect2D::MOVEEFFECT_ADD, CEffect2D::TYPE::TYPE_JUJI);
	}
}


//==========================================================================
// �J�ԃ`���[�W
//==========================================================================
void FloweringCharge(void)
{
	MyLib::Vector3 pos;

	for (int nCntUse = 0; nCntUse < 2; nCntUse++)
	{
		float fBuff = (float)UtilFunc::Transformation::Random(80, 100) * 0.01f;
		float fDistance = 400.0f * fBuff;
		m_nLife = (int)(20.0f * fBuff);

		// �o���ʒu
		pos = UtilFunc::Transformation::GetRandomPositionSphere(m_pos, fDistance);

		m_col = D3DXCOLOR(
			0.1f + UtilFunc::Transformation::Random(-100, 100) * 0.001f,
			0.9f + UtilFunc::Transformation::Random(-100, 100) * 0.001f,
			0.1f + UtilFunc::Transformation::Random(-100, 100) * 0.001f,
			1.0f);

		// ���a�ݒ�
		m_fRadius = 120.0f * fBuff;

		// �G�t�F�N�g�̐ݒ�
		CEffect3D* pEffect = CEffect3D::Create(
			pos,
			mylib_const::DEFAULT_VECTOR3,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_SUB,
			CEffect3D::TYPE::TYPE_JUJI2,
			0.0f);

		// �ڕW�̈ʒu�ݒ�
		pEffect->SetPositionDest(m_pos);
		pEffect->SetRotation(MyLib::Vector3(0.0f, 0.0f, UtilFunc::Transformation::GetRandomPi()));
	}
}

//==========================================================================
// �ԕ����Ƃ�
//==========================================================================
void Pollen_Drop()
{
	for (int nCntUse = 0; nCntUse < 1; nCntUse++)
	{
		m_fMove = (float)(UtilFunc::Transformation::Random(0, 10)) / 10 + 1.0f;		// �ړ���
		m_fAngle = (float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f;	// �p�x

		// �ړ��ʂ̐ݒ�
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

		// �G�t�F�N�g�̐ݒ�
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
// �Ԃ����ĉԕ����Ƃ�
//==========================================================================
void Pollen_Lost()
{

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		m_fMove = (float)(UtilFunc::Transformation::Random(80, 100)) / 10 + 2.0f;		// �ړ���
		m_fAngle = (float)(UtilFunc::Transformation::Random(-314, 314)) / 100.0f;	// �p�x

		// �ړ��ʂ̐ݒ�
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

		// �G�t�F�N�g�̐ݒ�
		CEffect3D::Create(
			m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			CEffect3D::MOVEEFFECT_ADD, CEffect3D::TYPE_SMOKEBLACK);
	}
}
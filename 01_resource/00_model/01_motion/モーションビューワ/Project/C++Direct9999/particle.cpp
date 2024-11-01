//=============================================================================
// 
//  �p�[�e�B�N������ [particle.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "particle.h"
#include "calculation.h"
#include "3D_effect.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CParticle::m_nNumAll = 0;	// ����

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CParticle::CParticle(int nPriority)
{
	// �l�̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	m_fRadius = 0.0f;							// ���a
	m_fAngle = 0.0f;							// ����
	m_fMove = 0.0f;								// �ړ���
	m_nLife = 0;								// ����
	m_nType = TYPE_NONE;						// ���

	// �������Z
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CParticle::~CParticle()
{
	// �������Z
	m_nNumAll--;
}

//==========================================================================
// �p�[�e�B�N���̏���������
//==========================================================================
HRESULT CParticle::Init(void)
{

	// �p�[�e�B�N���̏��̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fRadius = 0.0f;
	m_nLife = 0;
	m_nType = TYPE_NONE;

	return S_OK;
}

//==================================================================================
// �p�[�e�B�N���̏I������
//==================================================================================
void CParticle::Uninit(void)
{
	
}

//==================================================================================
// �p�[�e�B�N���̍X�V����
//==================================================================================
void CParticle::Update(void)
{

	
}

//==================================================================================
// �p�[�e�B�N���̕`�揈��
//==================================================================================
void CParticle::Draw(void)
{
	
}

//==================================================================================
// �p�[�e�B�N���̐ݒ菈��
//==================================================================================
void CParticle::Create(D3DXVECTOR3 pos, TYPE nType)
{
	//return;

	// �ʒu
	m_pos = pos;

	// ���
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
// �L�����Z���\
//==================================================================================
void CParticle::Cancelable(void)
{
	for (int nCntUse = 0; nCntUse < 1; nCntUse++)
	{

		float fMove = 0.0f;		// �ړ���
		float fAngle = 0.0f;	// �p�x

		// �ړ��ʂ̐ݒ�
		m_move.x = 0.0f;
		m_move.y = 40.0f;
		m_move.z = 0.0f;

		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		m_fRadius = 40.0f;

		// �G�t�F�N�g�̐ݒ�
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
// �`�F�b�N�p
//==================================================================================
void CParticle::Check(void)
{
	for (int nCntUse = 0; nCntUse < 1; nCntUse++)
	{

		float fMove = 0.0f;		// �ړ���
		float fAngle = 0.0f;	// �p�x

		// �ړ��ʂ̐ݒ�
		m_move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		m_move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		m_move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		m_fRadius = 40.0f;

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

//==================================================================================
//�ł������̃p�[�e�B�N��
//==================================================================================
void CParticle::OffSetting(void)
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
// ���͍U��
//==================================================================================
void CParticle::SuperAttack(void)
{
	for (int nCntUse = 0; nCntUse < 40; nCntUse++)
	{
		float fMove = (float)Random(0, 30) * 0.1f;	// �ړ���
		float fMoveY = (float)Random(0, 50) * 0.1f;	// �ړ���

		// �ړ��ʂ̐ݒ�
		m_move.x = sinf((float)Random(-314, 314) / 100.0f) * fMove;
		m_move.y = cosf((float)Random(-20, 20) / 100.0f) * fMoveY;
		m_move.z = cosf((float)Random(-314, 314) / 100.0f) * fMove;

		m_col = D3DXCOLOR(1.0f, 0.6f, 0.3f, 1.0f);

		m_pos.x += m_move.x * 0.1f;
		m_pos.z += m_move.z * 0.1f;

		m_fRadius = 60.0f + (float)Random(-50, 100) * 0.1f;

		// �G�t�F�N�g�̐ݒ�
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
		float fMove = (float)Random(0, 25) * 0.1f;	// �ړ���
		float fMoveY = (float)Random(0, 40) * 0.1f;	// �ړ���

		// �ړ��ʂ̐ݒ�
		m_move.x = sinf((float)Random(-314, 314) / 100.0f) * fMove;
		m_move.y = cosf((float)Random(-20, 20) / 100.0f) * fMoveY;
		m_move.z = cosf((float)Random(-314, 314) / 100.0f) * fMove;

		m_col = D3DXCOLOR(0.4f, 0.1f, 0.0f, 0.6f);

		m_pos.x += m_move.x * 0.5f;
		m_pos.z += m_move.z * 0.5f;

		m_fRadius = 40.0f + (float)Random(-50, 100) * 0.1f;

		// �G�t�F�N�g�̐ݒ�
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
// �X�o��
//==========================================================================
void CParticle::IceSpawn(void)
{
	for (int nCntUse = 0; nCntUse < 2; nCntUse++)
	{
		float fMove = 0.0f;		// �ړ���

		// �ړ��ʂ̐ݒ�
		m_move.x = sinf((float)Random(-314, 314) / 100.0f) * fMove;
		m_move.y = cosf((float)Random(-314, 314) / 100.0f) * fMove;
		m_move.z = cosf((float)Random(-314, 314) / 100.0f) * fMove;

		// �F�ݒ�
		m_col = D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f);

		// ���a�ݒ�
		m_fRadius = 120.0f;

		// �G�t�F�N�g�̐ݒ�
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
// �����擾
//==========================================================================
int CParticle::GetNumAll(void)
{
	return m_nNumAll;
}

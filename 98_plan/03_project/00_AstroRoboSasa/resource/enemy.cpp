//============================================================
//
//	�G���� [enemy.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemy.h"
#include "collision.h"
#include "player.h"
#include "bullet.h"
#include "gameManager.h"

//************************************************************
//	�q�N���X [CEnemy] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemy::CEnemy() : CObject3D(CObject::LABEL_ENEMY),
	m_move		(VEC3_ZERO),	// �ړ���
	m_fCurTime	(0.0f)			// �o�ߎ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemy::~CEnemy()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemy::Init(void)
{
	// �����o�ϐ���������
	m_move = VEC3_ZERO;	// �ړ���
	m_fCurTime = 0.0f;	// �o�ߎ���

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	SetVec3Sizing(D3DXVECTOR3(30.0f, 30.0f, 0.0f));
	SetOrigin(ORIGIN_DOWN);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemy::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemy::Update(const float fDeltaTime)
{
	D3DXVECTOR3 posEnemy = GetVec3Position();	// �G�ʒu

	// �d�͂�^����
	m_move.y -= 0.45f;

	// �G���ړ�������
	posEnemy += m_move;

	// �ړ��Ɋ�����^����
	m_move.x += (0.0f - m_move.x) * 0.08f;

	if (posEnemy.y < 50.0f)
	{ // �ʒu���n�ʂ�艺�̏ꍇ

		// �ʒu��␳
		posEnemy.y = 50.0f;

		// �ړ��ʂ�������
		m_move.y = 0.0f;
	}

	// ��ʊO�Ƃ̓����蔻��
	CGameManager::CollisionScreen(&posEnemy, &m_move);

	// �e�Ƃ̓����蔻��
	CollisionBullet(posEnemy);

	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer(posEnemy);

	// �ʒu�̔��f
	SetVec3Position(posEnemy);

	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CEnemy::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CEnemy *CEnemy::Create(const D3DXVECTOR3& rPos)
{
	// �G�̐���
	CEnemy *pEnemy = new CEnemy;
	if (pEnemy == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�̏�����
		if (FAILED(pEnemy->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�̔j��
			SAFE_DELETE(pEnemy);
			return nullptr;
		}

		// �ʒu��ݒ�
		pEnemy->SetVec3Position(rPos);

		// �m�ۂ����A�h���X��Ԃ�
		return pEnemy;
	}
}

//============================================================
//	�e�Ƃ̓����蔻��
//============================================================
bool CEnemy::CollisionBullet(const D3DXVECTOR3& rPos)
{
	CListManager<CBullet> *pListManager = CBullet::GetList();	// ���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return false; }				// ���X�g���g�p�̏ꍇ������
	std::list<CBullet*> list = pListManager->GetList();			// �������X�g
	for (auto& rList : list)
	{ // �v�f�̑������J��Ԃ�

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
//	�v���C���[�Ƃ̓����蔻��
//============================================================
bool CEnemy::CollisionPlayer(const D3DXVECTOR3& rPos)
{
	CListManager<CPlayer> *pListManager = CPlayer::GetList();	// ���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return false; }				// ���X�g���g�p�̏ꍇ������
	std::list<CPlayer*> list = pListManager->GetList();			// �������X�g
	for (auto& rList : list)
	{ // �v�f�̑������J��Ԃ�

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

//============================================================
//
//	�e���� [bullet.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "bullet.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CBullet> *CBullet::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CBullet] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CBullet::CBullet() : CObject3D(CObject::LABEL_BULLET),
	m_move		(VEC3_ZERO),	// �ړ���
	m_fCurTime	(0.0f)			// �o�ߎ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CBullet::~CBullet()
{

}

//============================================================
//	����������
//============================================================
HRESULT CBullet::Init(void)
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

	SetVec3Sizing(D3DXVECTOR3(20.0f, 20.0f, 0.0f));

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CBullet>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CBullet::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CBullet::Update(const float fDeltaTime)
{
	// �d�͂�^����
	m_move.y -= 0.35f;

	// �ʒu���ړ�������
	SetVec3Position(GetVec3Position() + m_move);

	// ���Ԍo��
	m_fCurTime += fDeltaTime;
	if (m_fCurTime > 4.0f)
	{ // �����������ꍇ

		// ���g�̏I��
		Uninit();
		return;
	}

	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CBullet::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CBullet *CBullet::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rMove)
{
	// �e�̐���
	CBullet *pBullet = new CBullet;
	if (pBullet == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �e�̏�����
		if (FAILED(pBullet->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �e�̔j��
			SAFE_DELETE(pBullet);
			return nullptr;
		}

		// �ʒu��ݒ�
		pBullet->SetVec3Position(rPos);

		// �ړ��ʂ�ݒ�
		pBullet->m_move = rMove;

		// �m�ۂ����A�h���X��Ԃ�
		return pBullet;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CBullet> *CBullet::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

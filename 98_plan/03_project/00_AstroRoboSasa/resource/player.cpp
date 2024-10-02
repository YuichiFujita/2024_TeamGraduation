//============================================================
//
//	�v���C���[���� [player.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "player.h"
#include "manager.h"
#include "camera.h"
#include "gun.h"
#include "gameManager.h"
#include "effect2D.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CPlayer> *CPlayer::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CPlayer] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayer::CPlayer() : CObject3D(CObject::LABEL_PLAYER),
	m_pGun	 (nullptr),		// �e�̏��
	m_rotGun (VEC3_ZERO),	// �e�̌���
	m_move	 (VEC3_ZERO)	// �ړ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayer::Init(void)
{
	// �����o�ϐ���������
	m_pGun	 = nullptr;		// �e�̏��
	m_rotGun = VEC3_ZERO;	// �e�̌���
	m_move	 = VEC3_ZERO;	// �ړ���

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	SetVec3Position(D3DXVECTOR3(0.0f, 500.0f, 0.0f));
	SetVec3Sizing(D3DXVECTOR3(30.0f, 30.0f, 0.0f));
	SetOrigin(ORIGIN_DOWN);

	// �e�̐���
	m_pGun = CGun::Create(this);
	if (m_pGun == nullptr) { return E_FAIL; }

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CPlayer>::Create();
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
void CPlayer::Uninit(void)
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
void CPlayer::Update(const float fDeltaTime)
{
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu

	// �d�͂�^����
	m_move.y -= 0.45f;

	// �v���C���[���ړ�������
	posPlayer += m_move;

	// �ړ��Ɋ�����^����
	m_move.x += (0.0f - m_move.x) * 0.08f;

	if (posPlayer.y < 50.0f)
	{ // �ʒu���n�ʂ�艺�̏ꍇ

		// �ʒu��␳
		posPlayer.y = 50.0f;

		// �ړ��ʂ�������
		m_move.y = 0.0f;
	}

	// ��ʊO�Ƃ̓����蔻��
	CGameManager::CollisionScreen(&posPlayer, &m_move);

	POINT cursor;
	if (GetCursorPos(&cursor))
	{
		const char* CLASS_NAME = "WindowClass";	// �E�C���h�E�N���X�̖��O
		const char* WINDOW_NAME = "LastBattle";	// �E�C���h�E�̖��O (�L���v�V�����ɕ\��)

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

	// �ʒu�̔��f
	SetVec3Position(posPlayer);

	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CPlayer::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CPlayer *CPlayer::Create(void)
{
	// �v���C���[�̐���
	CPlayer *pPlayer = new CPlayer;
	if (pPlayer == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �v���C���[�̏�����
		if (FAILED(pPlayer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �v���C���[�̔j��
			SAFE_DELETE(pPlayer);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pPlayer;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CPlayer> *CPlayer::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	�����̈ړ�����
//============================================================
void CPlayer::MoveGunRecoil(const float fRecScalar)
{
	// �����X�J���[���x�N�g���ɕύX
	D3DXVECTOR3 moveRec = D3DXVECTOR3(sinf(m_rotGun.z - HALF_PI), -cosf(m_rotGun.z - HALF_PI), 0.0f) * fRecScalar;

	// �ړ��ʂɔ��f
	m_move.y = 0.0f;
	m_move += moveRec;
}

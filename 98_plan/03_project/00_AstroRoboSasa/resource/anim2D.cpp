//============================================================
//
//	�A�j���[�V����2D���� [anim2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "anim2D.h"

//************************************************************
//	�q�N���X [CAnim2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CAnim2D::CAnim2D(const CObject::ELabel label, const EDim dimension, const int nPriority) : CObject2D(label, dimension, nPriority),
	m_nCounter		(0),	// �A�j���[�V�����J�E���^�[
	m_nCntChange	(0),	// �p�^�[���ύX�J�E���g
	m_nPattern		(0),	// �A�j���[�V�����p�^�[��
	m_nMaxPtrn		(0),	// �p�^�[���̑���
	m_nWidthPtrn	(0),	// �e�N�X�`���̉��̕�����
	m_nHeightPtrn	(0),	// �e�N�X�`���̏c�̕�����
	m_nNumLoop		(0),	// �p�^�[���J��Ԃ���
	m_bStop		(false),	// ��~��
	m_bPlayBack	(false)		// �t�Đ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CAnim2D::~CAnim2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CAnim2D::Init(void)
{
	// �����o�ϐ���������
	m_nCounter		= 0;	// �A�j���[�V�����J�E���^�[
	m_nCntChange	= 0;	// �p�^�[���ύX�J�E���g
	m_nPattern		= 0;	// �A�j���[�V�����p�^�[��
	m_nMaxPtrn		= 0;	// �p�^�[���̑���
	m_nWidthPtrn	= 1;	// �e�N�X�`���̉��̕�����
	m_nHeightPtrn	= 1;	// �e�N�X�`���̏c�̕�����
	m_nNumLoop		= 0;	// �p�^�[���J��Ԃ���
	m_bStop		= false;	// ��~��
	m_bPlayBack	= false;	// �t�Đ���

	// �I�u�W�F�N�g2D�̏�����
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �A�j���[�V�����̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CAnim2D::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CAnim2D::Update(const float fDeltaTime)
{
	// ��~���̏ꍇ������
	if (m_bStop) { return; }

	if (m_nCntChange > 0)
	{ // �ύX�J�E���g�� 0���傫���ꍇ

		// �J�E���^�[�����Z
		m_nCounter++;

		if (m_nCounter % m_nCntChange == 0)
		{ // �J�E���^�[���ύX�J�E���g�ɂȂ����ꍇ

			// �J�E���^�[��������
			m_nCounter = 0;

			if (!m_bPlayBack)
			{ // �ʏ�Đ��̏ꍇ

				// �p�^�[�������Z
				m_nPattern = (m_nPattern + 1) % m_nMaxPtrn;
			}
			else
			{ // �t�Đ��̏ꍇ

				// �p�^�[�������Z
				m_nPattern = (m_nPattern + (m_nMaxPtrn - 1)) % m_nMaxPtrn;
			}

			if (m_nPattern == 0)
			{ // �p�^�[�������ꖇ�ڂ̏ꍇ

				// �J��Ԃ��������Z
				m_nNumLoop++;
			}
		}
	}

	// �I�u�W�F�N�g2D�̍X�V
	CObject2D::Update(fDeltaTime);

	// �A�j���[�V�����̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	�`�揈��
//============================================================
void CAnim2D::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CAnim2D *CAnim2D::Create(const int nWidthPtrn, const int nHeightPtrn, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const D3DXVECTOR3& rRot, const D3DXCOLOR& rCol)
{
	// �A�j���[�V����2D�̐���
	CAnim2D *pAnim2D = new CAnim2D;
	if (pAnim2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �A�j���[�V����2D�̏�����
		if (FAILED(pAnim2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �A�j���[�V����2D�̔j��
			SAFE_DELETE(pAnim2D);
			return nullptr;
		}

		// �e�N�X�`������������ݒ�
		pAnim2D->SetWidthPattern(nWidthPtrn);

		// �e�N�X�`���c��������ݒ�
		pAnim2D->SetHeightPattern(nHeightPtrn);

		// �ʒu��ݒ�
		pAnim2D->SetVec3Position(rPos);

		// ������ݒ�
		pAnim2D->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pAnim2D->SetVec3Sizing(rSize);

		// �F��ݒ�
		pAnim2D->SetColor(rCol);

		// �m�ۂ����A�h���X��Ԃ�
		return pAnim2D;
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CAnim2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �ʒu�̐ݒ�
	CObject2D::SetVec3Position(rPos);

	// �A�j���[�V�����̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CAnim2D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̐ݒ�
	CObject2D::SetVec3Rotation(rRot);

	// �A�j���[�V�����̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CAnim2D::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// �傫���̐ݒ�
	CObject2D::SetVec3Sizing(rSize);

	// �A�j���[�V�����̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CAnim2D::SetColor(const D3DXCOLOR& rCol)
{
	// �F�̐ݒ�
	CObject2D::SetColor(rCol);

	// �A�j���[�V�����̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	�p�^�[���̐ݒ菈��
//============================================================
void CAnim2D::SetPattern(const int nPattern)
{
	// �����̃p�^�[��������
	m_nPattern = nPattern;

	// �A�j���[�V�����̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	�p�^�[���̑����̐ݒ菈��
//============================================================
void CAnim2D::SetMaxPattern(const int nMaxPtrn)
{
	// �����̃p�^�[���̑�������
	m_nMaxPtrn = nMaxPtrn;

	// �A�j���[�V�����̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	�e�N�X�`���̉��������̐ݒ菈��
//============================================================
void CAnim2D::SetWidthPattern(const int nWidthPtrn)
{
	// �����̃e�N�X�`������������ݒ�
	m_nWidthPtrn = nWidthPtrn;

	// �p�^�[���̑�����ݒ�
	m_nMaxPtrn = m_nWidthPtrn * m_nHeightPtrn;

	// �A�j���[�V�����̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	�e�N�X�`���̏c�������̐ݒ菈��
//============================================================
void CAnim2D::SetHeightPattern(const int nHeightPtrn)
{
	// �����̃e�N�X�`���c��������ݒ�
	m_nHeightPtrn = nHeightPtrn;

	// �p�^�[���̑�����ݒ�
	m_nMaxPtrn = m_nWidthPtrn * m_nHeightPtrn;

	// �A�j���[�V�����̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	�J�E���^�[�̐ݒ菈��
//============================================================
void CAnim2D::SetCounter(const int nCntChange)
{
	// �����̃p�^�[���ύX�J�E���g����
	m_nCntChange = nCntChange;
}

//============================================================
//	��~�󋵂̐ݒ菈��
//============================================================
void CAnim2D::SetEnableStop(const bool bStop)
{
	// �����̒�~�󋵂���
	m_bStop = bStop;

	if (m_bStop)
	{ // ��~���ꂽ�ꍇ

		// �J��Ԃ�����������
		m_nNumLoop = 0;
	}
}

//============================================================
//	�t�Đ��󋵂̐ݒ菈��
//============================================================
void CAnim2D::SetEnablePlayBack(const bool bPlayBack)
{
	// �����̋t�Đ��󋵂���
	m_bPlayBack = bPlayBack;
}
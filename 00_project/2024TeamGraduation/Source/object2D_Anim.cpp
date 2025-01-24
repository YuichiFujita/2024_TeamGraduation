//=============================================================================
// 
//  �I�u�W�F�N�g2D�A�j������ [object2D_Anim.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "object2D_Anim.h"
#include "calculation.h"
#include "debugproc.h"
#include "manager.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObject2D_Anim::CObject2D_Anim(int nPriority) : CObject2D(nPriority)
{
	m_dwNumLoop = 0;		// �J��Ԃ���
	m_fTimerAnim = 0.0f;	// �A�j���[�V�����̃J�E���^�[
	m_fIntervalAnim = 0.0f;	// �A�j���[�V�����̃C���^�[�o��
	m_nPatternAnim = 0;		// �A�j���[�V�����̃p�^�[��
	m_nDivisionU = 0;		// U�̕�����
	m_nDivisionV = 0;		// V�̕�����
	m_fSplitValueU = 0.0f;	// U�̃X�v���C�g��
	m_fSplitValueV = 0.0f;	// V�̃X�v���C�g��
	m_bAutoDeath = false;	// �����폜�̃t���O
	m_bAutoPlay = true;		// �����Đ��̃t���O
	m_bFinish = false;		// �A�j���[�V�������I���������
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObject2D_Anim::~CObject2D_Anim()
{

}

//==========================================================================
// ��������
//==========================================================================
CObject2D_Anim* CObject2D_Anim::Create(const MyLib::Vector3& pos, const int nDivisionU, const int nDivisionV, const float fInterval, bool bAutoDeath, int nPriority)
{
	// �������̊m��
	CObject2D_Anim* pObject2D = DEBUG_NEW CObject2D_Anim(nPriority);

	if (pObject2D != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �������
		pObject2D->SetPosition(pos);
		pObject2D->SetOriginPosition(pos);
		pObject2D->m_nDivisionU = nDivisionU;
		pObject2D->m_nDivisionV = nDivisionV;
		pObject2D->m_fIntervalAnim = fInterval;
		pObject2D->m_bAutoDeath = bAutoDeath;

		// ����������
		pObject2D->Init();
	}

	return pObject2D;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObject2D_Anim::Init()
{
	// ����������
	HRESULT hr = CObject2D::Init();
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// �X�v���C�g��
	m_fSplitValueU = 1.0f / static_cast<float>(m_nDivisionU);
	m_fSplitValueV = 1.0f / static_cast<float>(m_nDivisionV);

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObject2D_Anim::Init(const int nDivisionU, const int nDivisionV, const float fInterval, bool bAutoDeath)
{
	// �������
	m_nDivisionU = nDivisionU;
	m_nDivisionV = nDivisionV;
	m_fIntervalAnim = fInterval;

	// ����������
	HRESULT hr = CObject2D::Init();
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// �X�v���C�g��
	m_fSplitValueU = 1.0f / static_cast<float>(m_nDivisionU);
	m_fSplitValueV = 1.0f / static_cast<float>(m_nDivisionV);

	// �����폜
	m_bAutoDeath = bAutoDeath;

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// �X�V����
//==========================================================================
void CObject2D_Anim::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_bAutoPlay)
	{
		// �J�E���g���X�V
		m_fTimerAnim += fDeltaTime * fSlowRate;

		// �p�^�[���X�V
		if (m_fTimerAnim >= m_fIntervalAnim)
		{
			// �p�^�[��No.���X�V
			m_nPatternAnim = (m_nPatternAnim + 1) % (m_nDivisionU * m_nDivisionV);
			if (m_nPatternAnim == 0)
			{ // �p�^�[�������������

				// �J��Ԃ��������Z
				m_dwNumLoop++;

				// �I�����
				m_bFinish = true;

				if (m_bAutoDeath)
				{
					// �I�u�W�F�N�g�j��
					Uninit();
					return;
				}
			}

			// �C���^�[�o�������Z
			m_fTimerAnim -= m_fIntervalAnim;
		}
	}

	// �X�V����
	CObject2D::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObject2D_Anim::SetVtx()
{
	// �X�V����
	CObject2D::SetVtx();

	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * m_fSplitValueU,			(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV);
	pVtx[1].tex = D3DXVECTOR2((m_nPatternAnim + 1) * m_fSplitValueU,	(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV);
	pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * m_fSplitValueU,			(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV + m_fSplitValueV);
	pVtx[3].tex = D3DXVECTOR2((m_nPatternAnim + 1) * m_fSplitValueU,	(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV + m_fSplitValueV);

	// ���_�o�b�t�@���A�����b�N���b�N
	GetVtxBuff()->Unlock();
}

//==========================================================================
// U�������ݒ�
//==========================================================================
void CObject2D_Anim::SetDivisionU(int div)
{
	// �������ݒ�
	m_nDivisionU = div;

	// �X�v���C�g��
	m_fSplitValueU = 1.0f / static_cast<float>(m_nDivisionU);
}

//==========================================================================
// V�������ݒ�
//==========================================================================
void CObject2D_Anim::SetDivisionV(int div)
{
	// �������ݒ�
	m_nDivisionV = div;

	// �X�v���C�g��
	m_fSplitValueV = 1.0f / static_cast<float>(m_nDivisionV);
}

//==========================================================================
// UV�������ݒ�
//==========================================================================
void CObject2D_Anim::SetDivisionUV(int divU, int divV)
{
	SetDivisionU(divU);
	SetDivisionV(divV);
}

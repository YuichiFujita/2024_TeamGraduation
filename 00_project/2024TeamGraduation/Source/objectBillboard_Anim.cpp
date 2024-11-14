//=============================================================================
// 
//  �I�u�W�F�N�g�r���{�[�h�A�j������ [objectBillboard_Anim.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "objectBillboard_Anim.h"
#include "calculation.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObjectBillboardAnim::CObjectBillboardAnim(int nPriority) : CObjectBillboard(nPriority)
{
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
CObjectBillboardAnim::~CObjectBillboardAnim()
{

}

//==========================================================================
// ��������
//==========================================================================
CObjectBillboardAnim* CObjectBillboardAnim::Create(const MyLib::Vector3& pos, const int nDivisionU, const int nDivisionV, const float fInterval, bool bAutoDeath, int nPriority)
{
	// �������̊m��
	CObjectBillboardAnim* pObject3D = DEBUG_NEW CObjectBillboardAnim(nPriority);

	if (pObject3D != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �������
		pObject3D->SetPosition(pos);
		pObject3D->SetOriginPosition(pos);
		pObject3D->m_nDivisionU = nDivisionU;
		pObject3D->m_nDivisionV = nDivisionV;
		pObject3D->m_fIntervalAnim = fInterval;
		pObject3D->m_bAutoDeath = bAutoDeath;

		// ����������
		pObject3D->Init();
	}

	return pObject3D;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectBillboardAnim::Init()
{
	// ����������
	HRESULT hr = CObjectBillboard::Init();
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
HRESULT CObjectBillboardAnim::Init(const int nDivisionU, const int nDivisionV, const float fInterval, bool bAutoDeath)
{
	// �������
	m_nDivisionU = nDivisionU;
	m_nDivisionV = nDivisionV;
	m_fIntervalAnim = fInterval;
	m_bAutoDeath = bAutoDeath;

	// ����������
	HRESULT hr = CObjectBillboard::Init();
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
// �X�V����
//==========================================================================
void CObjectBillboardAnim::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

	// �J�E���g���X�V
	if (m_bAutoPlay)
	{
		m_fTimerAnim += fDeltaTime * fDeltaRate * fSlowRate;
	}

	// �p�^�[���X�V
	if (m_fTimerAnim >= m_fIntervalAnim)
	{
		// �p�^�[��No.���X�V
		m_nPatternAnim = (m_nPatternAnim + 1) % (m_nDivisionU * m_nDivisionV);
		if (m_nPatternAnim == 0)
		{ // �p�^�[�������������

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

	// �X�V����
	CObjectBillboard::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObjectBillboardAnim::SetVtx()
{
	// �e�N�X�`�����W�擾
	std::vector<D3DXVECTOR2> vecUV = GetUV();

	// �e�N�X�`�����W�̐ݒ�
	vecUV[0] = D3DXVECTOR2(m_nPatternAnim * m_fSplitValueU,			(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV);
	vecUV[1] = D3DXVECTOR2((m_nPatternAnim + 1) * m_fSplitValueU,	(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV);
	vecUV[2] = D3DXVECTOR2(m_nPatternAnim * m_fSplitValueU,			(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV + m_fSplitValueV);
	vecUV[3] = D3DXVECTOR2((m_nPatternAnim + 1) * m_fSplitValueU,	(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV + m_fSplitValueV);

	// �e�N�X�`�����W�ݒ�
	SetUV(vecUV);

	// ���_���ݒ菈��
	CObjectBillboard::SetVtx();
}

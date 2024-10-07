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
	m_nCntAnim = 0;			// �A�j���[�V�����̃J�E���^�[
	m_nPatternAnim = 0;		// �A�j���[�V�����̃p�^�[��
	m_nDivisionU = 0;		// U�̕�����
	m_nDivisionV = 0;		// V�̕�����
	m_nIntervalAnim = 0;	// �A�j���[�V�����̃C���^�[�o��
	m_fSplitValueU = 0.0f;	// U�̃X�v���C�g��
	m_fSplitValueV = 0.0f;	// V�̃X�v���C�g��
	m_bAutoDeath = false;	// �����폜�̃t���O
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
CObjectBillboardAnim *CObjectBillboardAnim::Create(MyLib::Vector3 pos, const int nDivisionU, const int nDivisionV, const int nInterval, bool bAutoDeath)
{
	// �����p�̃I�u�W�F�N�g
	CObjectBillboardAnim *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{// nullptr��������

		// �������̊m��
		pObject3D = DEBUG_NEW CObjectBillboardAnim;

		if (pObject3D != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// �������
			pObject3D->SetPosition(pos);
			pObject3D->SetOriginPosition(pos);
			pObject3D->m_nDivisionU = nDivisionU;
			pObject3D->m_nDivisionV = nDivisionV;
			pObject3D->m_nIntervalAnim = nInterval;
			pObject3D->m_bAutoDeath = bAutoDeath;

			// ����������
			pObject3D->Init();
		}

		return pObject3D;
	}

	return nullptr;
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
HRESULT CObjectBillboardAnim::Init(const int nDivisionU, const int nDivisionV, const int nInterval, bool bAutoDeath)
{
	// �������
	m_nDivisionU = nDivisionU;
	m_nDivisionV = nDivisionV;
	m_nIntervalAnim = nInterval;
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
void CObjectBillboardAnim::Update()
{

	// �J�E���g���X�V
	m_nCntAnim = (m_nCntAnim + 1) % m_nIntervalAnim;

	// �p�^�[���X�V
	if (m_nCntAnim == 0)
	{
		// �p�^�[��No.���X�V
		m_nPatternAnim = (m_nPatternAnim + 1) % (m_nDivisionU * m_nDivisionV);

		if (m_nPatternAnim == 0)
		{// �p�^�[�������������

			// �I�����
			m_bFinish = true;

			if (m_bAutoDeath)
			{
				// �I�u�W�F�N�g�j��
				Uninit();
				return;
			}
		}
	}

	// �X�V����
	CObjectBillboard::Update();
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObjectBillboardAnim::SetVtx()
{
	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR2 size = GetSize();
	D3DXCOLOR col = GetColor();
	MyLib::Vector3 rot = GetRotation();

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = sinf(rot.z - D3DX_PI + m_fAngle) * m_fLength;
	pVtx[0].pos.y = cosf(rot.z - m_fAngle) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = sinf(rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.y = cosf(rot.z + m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = sinf(rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.y = cosf(rot.z - D3DX_PI + m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = sinf(rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.y = cosf(rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	
	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * m_fSplitValueU,			(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV);
	pVtx[1].tex = D3DXVECTOR2((m_nPatternAnim + 1) * m_fSplitValueU,	(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV);
	pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * m_fSplitValueU,			(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV + m_fSplitValueV);
	pVtx[3].tex = D3DXVECTOR2((m_nPatternAnim + 1) * m_fSplitValueU,	(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV + m_fSplitValueV);

	// ���_�o�b�t�@���A�����b�N���b�N
	m_pVtxBuff->Unlock();
}

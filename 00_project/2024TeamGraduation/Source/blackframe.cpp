//=============================================================================
// 
//  ���t���[������ [blackframe.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "blackframe.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"
#include "calculation.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define SIZE		(D3DXVECTOR2(640.0f, 50.0f))
#define START_UP	(MyLib::Vector3(640.0f, -SIZE.y, 0.0f))
#define START_DOWN	(MyLib::Vector3(640.0f, SCREEN_HEIGHT + SIZE.y, 0.0f))
#define MOVETIME	(50)
CBlackFrame* CBlackFrame::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const MyLib::Vector3 CBlackFrame::m_DestPosition[VTX_MAX] =	// �ڕW�̈ʒu
{
	MyLib::Vector3(640.0f, SIZE.y, 0.0f),
	MyLib::Vector3(640.0f, SCREEN_HEIGHT - SIZE.y, 0.0f),
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBlackFrame::CBlackFrame(int nPriority, const LAYER layer) : CObject(nPriority, layer)
{
	// �l�̃N���A
	memset(&m_pObj2D[0], 0, sizeof(m_pObj2D));	// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	m_state = STATE_NONE;	// ���
	m_nCntMove = 0;	// �ړ��J�E���g
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBlackFrame::~CBlackFrame()
{

}

//==========================================================================
// ��������
//==========================================================================
CBlackFrame *CBlackFrame::Create()
{
	if (m_pThisPtr == nullptr)
	{// nullptr��������

		// �������̊m��
		m_pThisPtr = DEBUG_NEW CBlackFrame;

		if (m_pThisPtr != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			m_pThisPtr->Init();
		}
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CBlackFrame::Init()
{
	
	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		// ��������
		m_pObj2D[nCntSelect] = CObject2D::Create(8);

		// ��ނ̐ݒ�
		m_pObj2D[nCntSelect]->SetType(CObject::TYPE::TYPE_NONE);

		// �e�N�X�`���̊��蓖��
		m_pObj2D[nCntSelect]->BindTexture(0);

		// �e��ϐ��̏�����
		switch (nCntSelect)
		{
		case VTX_UP:
			// �T�C�Y�擾
			m_pObj2D[nCntSelect]->SetSize(SIZE);	// �T�C�Y
			m_pObj2D[nCntSelect]->SetPosition(START_UP);	// �ʒu
			m_pObj2D[nCntSelect]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// �F
			break;

		case VTX_DOWN:
			// �T�C�Y�擾
			m_pObj2D[nCntSelect]->SetSize(SIZE);	// �T�C�Y
			m_pObj2D[nCntSelect]->SetPosition(START_DOWN);	// �ʒu
			m_pObj2D[nCntSelect]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// �F
			break;
		}
	}
	m_state = STATE_OUTCOMPLETION;	// ���

	// ��ސݒ�
	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);
	return S_OK;
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CBlackFrame::Reset()
{
	m_state = STATE_OUTCOMPLETION;
	m_nCntMove = 0;

	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_pObj2D[nCntVtx] != nullptr)
		{// nullptr����Ȃ�������

			MyLib::Vector3 pos = mylib_const::DEFAULT_VECTOR3;
			if (nCntVtx == VTX_UP)
			{
				pos = START_UP;
			}
			else if (nCntVtx == VTX_DOWN)
			{
				pos = START_DOWN;
			}

			// ���ݒ�
			m_pObj2D[nCntVtx]->SetPosition(pos);
		}
	}

}

//==========================================================================
// �I������
//==========================================================================
void CBlackFrame::Uninit()
{
	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		if (m_pObj2D[nCntSelect] != nullptr)
		{// nullptr����Ȃ�������

			// �I������
			m_pObj2D[nCntSelect]->Uninit();
			m_pObj2D[nCntSelect] = nullptr;
		}
	}

	// �J������
	Release();

	m_pThisPtr = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CBlackFrame::Update()
{
	CManager* pMgr = CManager::GetInstance();
	if (m_state == STATE::STATE_OUT ||
		m_state == STATE::STATE_OUTCOMPLETION)
	{
		pMgr->SerEnableDisp_UI(true);
	}
	else
	{
		pMgr->SerEnableDisp_UI(false);
	}

	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		if (m_pObj2D[nCntSelect] == nullptr)
		{// nullptr��������
			continue;
		}

		// ��ԍX�V����
		UpdateState(nCntSelect);

		// ���_���ݒ�
		m_pObj2D[nCntSelect]->SetVtx();
	}

	// �ړ��J�E���g���Z
	m_nCntMove++;
}

//==========================================================================
// ��ԍX�V����
//==========================================================================
void CBlackFrame::UpdateState(int nCntVtx)
{
	switch (m_state)
	{
	case CBlackFrame::STATE_NONE:
		break;

	case CBlackFrame::STATE_IN:
		StateIn(nCntVtx);
		break;

	case CBlackFrame::STATE_OUT:
		StateOut(nCntVtx);
		break;

	case CBlackFrame::STATE_INCOMPLETION:

		m_pObj2D[nCntVtx]->SetPosition(m_DestPosition[nCntVtx]);
		break;

	case CBlackFrame::STATE_OUTCOMPLETION:
		if (nCntVtx == VTX_UP)
		{
			m_pObj2D[nCntVtx]->SetPosition(START_UP);
		}
		else if (nCntVtx == VTX_DOWN)
		{
			m_pObj2D[nCntVtx]->SetPosition(START_DOWN);
		}
		break;
	}
}

//==========================================================================
// �N��
//==========================================================================
void CBlackFrame::StateIn(int nCntVtx)
{
	if (m_nCntMove > MOVETIME)
	{// �ړ����Ԃ��K��l��������
		m_nCntMove = MOVETIME;
		m_state = STATE_INCOMPLETION;
		return;
	}

	// ���擾
	MyLib::Vector3 pos = m_pObj2D[nCntVtx]->GetPosition();

	// �����ʒu
	MyLib::Vector3 start = mylib_const::DEFAULT_VECTOR3;
	if (nCntVtx == VTX_UP)
	{
		start = START_UP;
	}
	else if (nCntVtx == VTX_DOWN)
	{
		start = START_DOWN;
	}

	// �^�C������
	float fTime = (float)m_nCntMove / (float)MOVETIME;

	// �ʒu�X�V
	pos.x = UtilFunc::Correction::EasingLinear(start.x, m_DestPosition[nCntVtx].x, fTime);
	pos.y = UtilFunc::Correction::EasingLinear(start.y, m_DestPosition[nCntVtx].y, fTime);

	// ���ݒ�
	m_pObj2D[nCntVtx]->SetPosition(pos);
}

//==========================================================================
// �J��
//==========================================================================
void CBlackFrame::StateOut(int nCntVtx)
{
	if (m_nCntMove > MOVETIME)
	{// �ړ����Ԃ��K��l��������
		m_nCntMove = MOVETIME;
		m_state = STATE_OUTCOMPLETION;
		return;
	}

	// ���擾
	MyLib::Vector3 pos = m_pObj2D[nCntVtx]->GetPosition();

	// �����ʒu
	MyLib::Vector3 start = mylib_const::DEFAULT_VECTOR3;
	if (nCntVtx == VTX_UP)
	{
		start = START_UP;
	}
	else if (nCntVtx == VTX_DOWN)
	{
		start = START_DOWN;
	}

	// �^�C������
	float fTime = (float)m_nCntMove / (float)MOVETIME;

	// �ʒu�X�V
	pos.x = UtilFunc::Correction::EasingLinear(m_DestPosition[nCntVtx].x, start.x, fTime);
	pos.y = UtilFunc::Correction::EasingLinear(m_DestPosition[nCntVtx].y, start.y, fTime);

	// ���ݒ�
	m_pObj2D[nCntVtx]->SetPosition(pos);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CBlackFrame::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntSelect = 0; nCntSelect < VTX_MAX; nCntSelect++)
	{
		if (m_pObj2D[nCntSelect] == nullptr)
		{
			continue;
		}

		// �`�揈��
		m_pObj2D[nCntSelect]->Draw();
	}

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CBlackFrame::SetState(STATE state)
{
	if (m_state == STATE_INCOMPLETION ||
		m_state == STATE_OUTCOMPLETION)
	{
		m_state = state;
		m_nCntMove = 0;
	}
}

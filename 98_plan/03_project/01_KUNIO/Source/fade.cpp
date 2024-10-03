//=============================================================================
// 
//  �t�F�[�h���� [fade.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define ALPHAMOVE	(0.025f)

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CFade::CFade()
{
	// �l�̃N���A
	m_aObject2D = nullptr;					// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	m_ModeNext = CScene::MODE_TITLE;	// ���̃��[�h
	m_state = STATE_NONE;				// ���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CFade::~CFade()
{

}

//==========================================================================
// ��������
//==========================================================================
CFade *CFade::Create()
{
	// �����p�̃I�u�W�F�N�g
	CFade *pFade = nullptr;

	if (pFade == nullptr)
	{// nullptr��������

		// �������̊m��
		pFade = DEBUG_NEW CFade;

		if (pFade != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			if (FAILED(pFade->Init()))
			{// ���s���Ă�����
				return nullptr;
			}
		}
		else
		{
			delete pFade;
			pFade = nullptr;
		}

		return pFade;
	}

	return nullptr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CFade::Init()
{
	// ��������
	m_aObject2D = CObject2D::Create(8);

	if (m_aObject2D == nullptr)
	{// ���s���Ă�����
		return E_FAIL;
	}
	m_aObject2D->SetType(CObject::TYPE::TYPE_NONE);

	m_state = STATE_FADEIN;			// ���
#if _DEBUG
	m_ModeNext = CScene::MODE_TITLE;	// ���̃��[�h
	m_ModeNext = CScene::MODE_GAME;	// ���̃��[�h
#else
	m_ModeNext = CScene::MODE_TITLE;	// ���̃��[�h
#endif
	
	m_aObject2D->SetSize(D3DXVECTOR2(640.0f, 360.0f));	// �T�C�Y
	m_aObject2D->SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));	// �ʒu
	m_aObject2D->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// �F�ݒ�

	// �ŏ��������̉�ʂ�
	//CManager::GetInstance()->SetMode(m_ModeNext);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CFade::Uninit()
{
	if (m_aObject2D != nullptr)
	{// nullptr����Ȃ�������

		// �I������
		m_aObject2D->Uninit();
		delete m_aObject2D;
		m_aObject2D = nullptr;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CFade::Update()
{
	// �F�擾
	D3DXCOLOR col = m_aObject2D->GetColor();

	switch (m_state)
	{
	case STATE_NONE:
		break;

	case STATE_FADEOUT:

		// �s�����x����
		col.a += ALPHAMOVE;

		if (col.a >= 1.0f)
		{// �ڕW�܂ōs������
			col.a = 1.0f;
			m_state = STATE_FADECOMPLETION;
		}
		break;

	case STATE_FADEIN:

		// �s�����x����
		col.a -= ALPHAMOVE;

		if (col.a <= 0.0f)
		{// �����ɂȂ�����
			col.a = 0.0f;
			m_state = STATE_NONE;
		}
		break;

	case STATE_FADECOMPLETION:
		m_state = STATE_FADEIN;
		CManager::GetInstance()->SetMode(m_ModeNext);
		break;
	}


	// �F�ݒ�
	m_aObject2D->SetColor(col);

	// �X�V����
	m_aObject2D->Update();

	m_aObject2D->SetVtx();

}

//==========================================================================
// �`�揈��
//==========================================================================
void CFade::Draw()
{
	// �`�揈��
	m_aObject2D->Draw();
}

//==========================================================================
// ���̃��[�h�ݒ�
//==========================================================================
void CFade::SetFade(CScene::MODE mode)
{
	if (m_state != STATE_FADEOUT &&
		m_state != STATE_FADECOMPLETION)
	{// �������Ă��Ȃ��Ƃ�

		// ���̃��[�h�ݒ�
		m_ModeNext = mode;
		m_state = STATE_FADEOUT;	// �t�F�[�h�A�E�g��Ԃɐݒ�

		// �F�ݒ�
		m_aObject2D->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	}
}

//==========================================================================
// ��Ԏ擾
//==========================================================================
CFade::STATE CFade::GetState()
{
	return m_state;
}

//==========================================================================
// �I�u�W�F�N�g2D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject2D *CFade::GetMyObject()
{
	return m_aObject2D;
}
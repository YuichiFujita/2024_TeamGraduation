//=============================================================================
// 
// ����ҋ@���� [title_controllwait.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "title_controllwait.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "titleLogo.h"
#include "titlestudent.h"
#include "fade.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int PLAYER_MIN = 10;	// �v���C���[�ŏ��l��
	const int PLAYER_MAX = 14;	// �v���C���[�ő�l��
}

namespace StateTime
{
	const float WAIT = 15.0f;	// �ҋ@
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CTitle_ControllWait::STATE_FUNC CTitle_ControllWait::m_StateFunc[] =	// ��Ԋ֐�
{
	&CTitle_ControllWait::StateWait,		// �ҋ@
	&CTitle_ControllWait::StateControll,	// ����
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitle_ControllWait::CTitle_ControllWait() : CTitleScene()
{
	// ���
	m_state = EState::STATE_WAIT;		// ���
	m_fStateTime = 0.0f;				// ��Ԏ���

	// ���̑�
	m_pLogo = nullptr;			// ���S
	m_vecTitleStudent.clear();	// �^�C�g���̐��k
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTitle_ControllWait::~CTitle_ControllWait()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTitle_ControllWait::Init()
{
	// ����������
	if (FAILED(CTitleScene::Init())) return E_FAIL;

	// ���S�̐���
	m_pLogo = CTitleLogo::Create();
	if (m_pLogo == nullptr) return E_FAIL;

	// �v���C���[����
	int num = UtilFunc::Transformation::Random(PLAYER_MIN, PLAYER_MAX);
	for (int i = 0; i < num; i++)
	{
		// �^�C�g���̐��k����
		m_vecTitleStudent.push_back(CTitleStudent::Create());
		if (m_vecTitleStudent.back() == nullptr) return E_FAIL;
	}

	// BGM�Đ�
	CSound::GetInstance()->PlaySound(CSound::ELabel::LABEL_BGM_TITLE);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTitle_ControllWait::Uninit()
{
	delete this;
}

//==========================================================================
// �폜����
//==========================================================================
void CTitle_ControllWait::Kill()
{
	// ���S
	SAFE_KILL(m_pLogo);

	// ���k
	for (int i = 0; i < static_cast<int>(m_vecTitleStudent.size()); i++)
	{
		SAFE_KILL(m_vecTitleStudent[i]);
	}

	// �I������
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTitle_ControllWait::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (GET_MANAGER->GetInstantFade()->GetState() != CInstantFade::EState::STATE_NONE)
	{// �t�F�[�h���͔�����
		return;
	}

	// �e�̍X�V
	CTitleScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pKey->GetTrigger(DIK_RETURN))
	{
		// �J��
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_ENTRY);
	}
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CTitle_ControllWait::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += fDeltaTime * fSlowRate;

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �ҋ@
//==========================================================================
void CTitle_ControllWait::StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fStateTime >= StateTime::WAIT)
	{
		// �V�[���؂�ւ�
		//CTitle::GetInstance()->ChangeScene(CTitle::ESceneType::SCENETYPE_SUSURU);
	}
}

//==========================================================================
// ����
//==========================================================================
void CTitle_ControllWait::StateControll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_fStateTime = 0.0f;
}

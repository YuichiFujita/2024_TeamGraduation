//=============================================================================
// 
//  �^�C�g������ [title.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "sound.h"
#include "particle.h"
#include "MyEffekseer.h"

#include "titlescene.h"
#include "camera.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float TIME_FADELOGO = 0.6f;	// ���S�̃t�F�[�h�A�E�g����
	const char* TEXTURE = "data\\TEXTURE\\title\\title.png";
	
}

namespace STARTCAMERA
{
	const MyLib::Vector3 POSV = (321.91f, 233.25f, -654.0f);
	const MyLib::Vector3 POSR = (321.91f, 160.74f, 296.27f);
	const MyLib::Vector3 ROT = (0.0f, 0.0f, -0.2f);
	const float LENGTH = 365.0f;
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CTitle* CTitle::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �֐��|�C���^
//==========================================================================
CTitle::STATE_FUNC CTitle::m_StateFunc[] =
{
	&CTitle::StateNone,			// �Ȃɂ��Ȃ�
	&CTitle::StateChangeScene,	// �V�[���؂�ւ�
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitle::CTitle()
{
	// �l�̃N���A
	m_state = EState::STATE_NONE;				// ���
	m_SceneType = ESceneType::SCENETYPE_NONE;	// �V�[���̎��
	m_fSceneTime = 0.0f;						// �V�[���J�E���^�[
	m_pLogo = nullptr;		// ���S�̃|�C���^
	m_pTitleScene = nullptr;		// �^�C�g���V�[��
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTitle::~CTitle()
{

}

//==========================================================================
// ��������
//==========================================================================
CTitle* CTitle::Create()
{
	if (m_pThisPtr == nullptr)
	{// �܂��������Ă��Ȃ�������

		// �C���X�^���X����
		m_pThisPtr = DEBUG_NEW CTitle;
	}

	return m_pThisPtr;
}

//==========================================================================
// �C���X�^���X�擾
//==========================================================================
CTitle* CTitle::GetInstance()
{
	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTitle::Init()
{
	// BGM�Đ�
	CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_TITLE);

	// ����������
	if (FAILED(CScene::Init()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	// �V�[������
	m_pTitleScene = CTitleScene::Create(ESceneType::SCENETYPE_CONTROLLWAIT);

	// �V�[���ݒ�
	m_SceneType = ESceneType::SCENETYPE_CONTROLLWAIT;

	// ����
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTitle::Uninit()
{
	m_pThisPtr = nullptr;

	// �V�[���I��
	SAFE_UNINIT(m_pTitleScene);

	// �I������
	CScene::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTitle::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// �t�F�[�h���͔�����
		return;
	}

	// ��ԍX�V����
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ��ԍX�V����
//==========================================================================
void CTitle::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �V�[���J�E���^�[���Z
	m_fSceneTime += fDeltaTime * fSlowRate;

	// ��ԕʍX�V����
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �Ȃɂ��Ȃ�
//==========================================================================
void CTitle::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �V�[���̍X�V
	if (m_pTitleScene != nullptr)
	{
		m_pTitleScene->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �V�[���؂�ւ�
//==========================================================================
void CTitle::StateChangeScene(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �t�F�[�h����������V�[���؂�ւ�
	if (GET_MANAGER->GetInstantFade()->IsCompletion())
	{
		// �폜����
		SAFE_KILL(m_pTitleScene);

		// �V�[������
		m_pTitleScene = CTitleScene::Create(m_SceneType);

		// �J�������Z�b�g
		GET_MANAGER->GetCamera()->ResetByMode(CScene::MODE::MODE_TITLE);

		// NONE���
		m_state = EState::STATE_NONE;
	}
}

//==========================================================================
// �V�[���؂�ւ�
//==========================================================================
void CTitle::ChangeScene(ESceneType type)
{
	// �V�[���ݒ�
	m_SceneType = type;

	// �t�F�[�h�ݒ�
	GET_MANAGER->GetInstantFade()->SetFade();

	// �V�[���؂�ւ����
	m_state = EState::STATE_CHANGESCENE;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTitle::Draw()
{

}

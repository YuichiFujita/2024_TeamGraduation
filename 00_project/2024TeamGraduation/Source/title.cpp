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
#include "titleLogo.h"
#include "titlestudent.h"
#include "titlescene.h"
#include "camera.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float TIME_FADELOGO = 0.6f;	// ���S�̃t�F�[�h�A�E�g����
	const char* TEXTURE = "data\\TEXTURE\\title\\title.png";
	const int TITLEPLAYER_MIN = 7;	// �v���C���[�ŏ��l��
	const int TITLEPLAYER_MAX = 12;	// �v���C���[�ő�l��
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
CTitle::SCENE_FUNC CTitle::m_SceneFunc[] =
{
	&CTitle::SceneNone,			// �Ȃɂ��Ȃ�
	&CTitle::SceneFadeInLogo,	// ���S�t�F�[�h�C��
	&CTitle::SceneFadeOutLoGo,	// ���S�t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitle::CTitle()
{
	// �l�̃N���A
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

	// �V�[���̎��
	ChangeScene(ESceneType::SCENETYPE_SUSURU);

	// ���S�̐���
	CTitleLogo::Create();

	// �v���C���[����
	int num = UtilFunc::Transformation::Random(TITLEPLAYER_MIN, TITLEPLAYER_MAX);
	for (int i = 0; i < num; i++)
	{
		// �^�C�g���̐��k����
		CTitleStudent::Create();
	}

	// ����
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTitle::Uninit()
{
	m_pThisPtr = nullptr;

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

	// �V�[���̍X�V
	if (m_pTitleScene != nullptr)
	{
		m_pTitleScene->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pKey->GetTrigger(DIK_RETURN))
	{
		// �J��
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_ENTRY);
	}


#if _DEBUG

	// ����
	if (ImGui::TreeNode("Create"))
	{
		if (ImGui::Button("TitleStudent"))
		{
			// �^�C�g���̐��k����
			CTitleStudent::Create();
		}

		// �c���[�I�[
		ImGui::TreePop();
	}
#endif
}

//==========================================================================
// �V�[���̍X�V����
//==========================================================================
void CTitle::UpdateScene(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �V�[���J�E���^�[���Z
	m_fSceneTime += fDeltaTime * fSlowRate;

	// ��ԕʍX�V����
	(this->*(m_SceneFunc[m_SceneType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �Ȃɂ��Ȃ�
//==========================================================================
void CTitle::SceneNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
}

//==========================================================================
// ���S�t�F�[�h�C��
//==========================================================================
void CTitle::SceneFadeInLogo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ���S�t�F�[�h�A�E�g
//==========================================================================
void CTitle::SceneFadeOutLoGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
}

//==========================================================================
// �V�[���؂�ւ�
//==========================================================================
void CTitle::ChangeScene(ESceneType type)
{
	// �I������
	SAFE_UNINIT(m_pTitleScene);
	

	// ����
	m_pTitleScene = CTitleScene::Create(type);

	// �V�[���ݒ�
	m_SceneType = type;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTitle::Draw()
{

}

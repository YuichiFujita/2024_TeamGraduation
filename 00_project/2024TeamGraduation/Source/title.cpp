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
	m_SceneType = SCENETYPE::SCENETYPE_NONE;	// �V�[���̎��
	m_fSceneTime = 0.0f;						// �V�[���J�E���^�[
	m_pLogo = nullptr;		// ���S�̃|�C���^
	m_pPressEnter = nullptr;	// �v���X�G���^�[
	m_pConfigSetting = nullptr;
	m_pSpawn_Leaf = nullptr;		// �~��t����
	m_pPeopleManager = nullptr;	// �l�}�l�[�W��
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
	m_SceneType = SCENETYPE::SCENETYPE_NONE;

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
	// �V�[���J�E���^�[���Z
	m_fSceneTime -= fDeltaTime * fSlowRate;

	// �s�����x�X�V
	float alpha = m_fSceneTime / TIME_FADELOGO;

	if (m_fSceneTime <= 0.0f)
	{
		m_fSceneTime = 0.0f;
		m_SceneType = SCENETYPE_NONE;
		return;
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTitle::Draw()
{

}

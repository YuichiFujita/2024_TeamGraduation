//=============================================================================
// 
//  �G���g���[���� [entry.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "entry.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "sound.h"
#include "MyEffekseer.h"
#include "camera.h"

// �V�[��
#include "entryscene.h"
#include "entry_setupTeam.h"


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
CEntry* CEntry::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �֐��|�C���^
//==========================================================================
CEntry::SCENE_FUNC CEntry::m_SceneFunc[] =
{
	&CEntry::SceneNone,			// �Ȃɂ��Ȃ�
	&CEntry::SceneFadeInLogo,	// ���S�t�F�[�h�C��
	&CEntry::SceneFadeOutLoGo,	// ���S�t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEntry::CEntry()
{
	// �l�̃N���A
	m_SceneType = ESceneType::SCENETYPE_NONE;	// �V�[���̎��
	m_fSceneTime = 0.0f;						// �V�[���J�E���^�[
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEntry::~CEntry()
{

}

//==========================================================================
// ��������
//==========================================================================
CEntry* CEntry::Create()
{
	if (m_pThisPtr == nullptr)
	{// �܂��������Ă��Ȃ�������

		// �C���X�^���X����
		m_pThisPtr = DEBUG_NEW CEntry;
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEntry::Init()
{

	// BGM�Đ�
	CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_TITLE);

	// ����������
	if (FAILED(CScene::Init()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	// �V�[���ݒ�
	SetSceneType(ESceneType::SCENETYPE_SETUPTEAM);

	// �G���g���[�V�[������
	m_pEntryScene = DEBUG_NEW CEntry_SetUpTeam;

	// ����
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEntry::Uninit()
{
	m_pThisPtr = nullptr;

	// �I������
	CScene::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CEntry::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// �t�F�[�h���͔�����
		return;
	}

	m_pEntryScene->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ��ԕʍX�V����
	(this->*(m_SceneFunc[m_SceneType]))(fDeltaTime, fDeltaRate, fSlowRate);


	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pKey->GetTrigger(DIK_RETURN))
	{
		// �J��
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_GAME);
	}
}

//==========================================================================
// �Ȃɂ��Ȃ�
//==========================================================================
void CEntry::SceneNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
}

//==========================================================================
// ���S�t�F�[�h�C��
//==========================================================================
void CEntry::SceneFadeInLogo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ���S�t�F�[�h�A�E�g
//==========================================================================
void CEntry::SceneFadeOutLoGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CEntry::Draw()
{
}

//==========================================================================
// �V�[���ݒ�
//==========================================================================
void CEntry::SetSceneType(ESceneType type)
{
	m_SceneType = type;
	m_fSceneTime = 0.0f;
}

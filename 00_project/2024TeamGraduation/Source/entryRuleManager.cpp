//============================================================
//
//	�G���g���[���[���}�l�[�W���[���� [entryRuleManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "entryRuleManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"
#include "object2D_Anim.h"
#include "arrowUI.h"
#include "timeUI.h"
#include "fade.h"
#include "entry.h"
#include "entry_dressup.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE[] =	// �e�N�X�`���p�X
	{
		"data\\TEXTURE\\entry\\Choices.png",		// ���[���^�C�g���e�N�X�`��
		"data\\TEXTURE\\entry\\lifeRule.png",		// ���j�����e�N�X�`��
		"data\\TEXTURE\\entry\\Battle_Start.png",	// �J�n�{�^���e�N�X�`��
		"data\\TEXTURE\\entry\\B_Back.png",			// ����\���e�N�X�`��
		"data\\TEXTURE\\entry\\Arrow_Twin.png",		// ���e�N�X�`��
		"data\\TEXTURE\\entry\\flame_choice.png",	// �I���e�N�X�`��
	};

	const std::string SET_PATH = "data\\TEXT\\entry\\game_setting.txt";
	const int PRIORITY		= 6;	// �G���g���[�̗D�揇��
	const int MAX_SELECT	= CEntryRuleManager::RULE_MAX + 1;	// ���[���I���̑��� (����������܂�)
	const int SELECT_GAME	= CEntryRuleManager::RULE_MAX;		// �Q�[���J�ڑI��ԍ�

	// �t�F�[�h���
	namespace fade
	{
		const D3DXCOLOR INIT_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �������F
		const D3DXCOLOR SET_COL	 = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.84f);	// �ݒ�F
		const float	ADD_ALPHA = 0.025f;	// �����x�̉��Z��
		const float	SUB_ALPHA = 0.025f;	// �����x�̌��Z��
	}

	// ���[���^�C�g�����
	namespace rule
	{
		const MyLib::PosGrid2	PART	= MyLib::PosGrid2(1, CEntryRuleManager::RULE_MAX);	// �e�N�X�`��������
		const MyLib::Vector3	POS		= MyLib::Vector3(320.0f, 150.0f, 0.0f);		// �ʒu
		const MyLib::Vector2	SIZE	= MyLib::Vector2(588.0f, 130.0f) * 0.5f;	// �傫��
		const MyLib::Vector3	SPACE	= MyLib::Vector3(0.0f, 140.0f, 0.0f);		// ��
	}

	// �J�n�{�^�����
	namespace start
	{
		const MyLib::Vector3 POS	= MyLib::Vector3(VEC3_SCREEN_CENT.x, 595.0f, 0.0f);	// �ʒu
		const MyLib::Vector2 SIZE	= MyLib::Vector2(484.0f, 112.5f) * 0.5f;			// �傫��
	}

	// �^�C�����
	namespace time
	{
		const MyLib::Vector3 POS		= MyLib::Vector3(930.0f, 150.0f, 0.0f);	// �^�C�}�[�ʒu
		const MyLib::Vector2 VAL_SIZE	= MyLib::Vector2(40.0f, 40.0f);			// �^�C�}�[�����傫��
		const MyLib::Vector2 PART_SIZE	= MyLib::Vector2(30.0f, 40.0f);			// �^�C�}�[��؂�傫��
		const MyLib::Vector2 VAL_SPACE	= MyLib::Vector2(40.0f, 0.0f);			// �^�C�}�[������
		const MyLib::Vector2 PART_SPACE	= MyLib::Vector2(60.0f, 0.0f);			// �^�C�}�[��؂��
	}

	// �̗͏��
	namespace life
	{
		const MyLib::PosGrid2	PART	= MyLib::PosGrid2(1, CEntryRuleManager::LIFE_MAX);				// �e�N�X�`��������
		const MyLib::Vector3	POS		= MyLib::Vector3(930.0f, time::POS.y + rule::SPACE.y, 0.0f);	// �ʒu
		const MyLib::Vector2	SIZE	= MyLib::Vector2(444.0f, 96.0f) * 0.5f;							// �傫��
	}

	// �I�����
	namespace select
	{
		const float	ADD_SIZE = 17.5f;	// �傫�����Z��

		const MyLib::Vector3	POS	= MyLib::Vector3(930.0f, rule::POS.y, 0.0f);	// �ʒu
		const D3DXCOLOR			COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);			// �F
		const MyLib::Vector2	SIZE_RULE	= MyLib::Vector2(life::SIZE.x + ADD_SIZE, life::SIZE.y + ADD_SIZE);		// ���[���I�����̑傫��
		const MyLib::Vector2	SIZE_START	= MyLib::Vector2(start::SIZE.x + ADD_SIZE, start::SIZE.y + ADD_SIZE);	// �J�n�I�����̑傫��

		const D3DXCOLOR CHOICE_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �I�𒆃J���[
		const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);	// ��I�𒆃J���[
	}

	// �����
	namespace arrow
	{
		const float	ADD_ALPHA		= 0.02f;	// �����x�̉��Z��
		const float	ADD_SINROT		= 0.04f;	// �����x�ӂ�ӂ킳����ۂ̃T�C���J�[�u�������Z��
		const float	MAX_ADD_ALPHA	= 0.25f;	// �����x�̍ő���Z��
		const float	BASIC_ALPHA		= 0.95f;	// ��̓����x
		const float	SPACE_EDGE		= 40.0f;	// ���̋�

		const MyLib::PosGrid2	PART	= MyLib::PosGrid2(MAX_RULE_ARROW, 1);	// �e�N�X�`��������
		const float				SIZE	= 40.0f;								// �傫��
		const MyLib::Vector3	POS		= MyLib::Vector3(select::POS.x - select::SIZE_RULE.x - SPACE_EDGE, select::POS.y, 0.0f);	// �ʒu
		const MyLib::Vector3	SPACE	= MyLib::Vector3(select::SIZE_RULE.x * 2.0f + (SPACE_EDGE * 2.0f), 0.0f, 0.0f);	// ��
		const D3DXCOLOR			MIN_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, BASIC_ALPHA - MAX_ADD_ALPHA);	// �F
	}

	// ������
	namespace control
	{
		const float	ADD_ALPHA		= 0.0045f;	// �����x�̉��Z��
		const float	ADD_SINROT		= 0.04f;	// �����x�ӂ�ӂ킳����ۂ̃T�C���J�[�u�������Z��
		const float	MAX_ADD_ALPHA	= 0.45f;	// �����x�̍ő���Z��
		const float	BASIC_ALPHA		= 0.85f;	// ��̓����x

		const MyLib::Vector3	POS		= MyLib::Vector3(1080.0f, 660.0f, 0.0f);	// �ʒu
		const MyLib::Vector2	SIZE	= MyLib::Vector2(302.0f, 73.0f) * 0.5f;		// �傫��
		const D3DXCOLOR			MIN_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, BASIC_ALPHA - MAX_ADD_ALPHA);	// �F
	}
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE) == CEntryRuleManager::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�e�N���X [CEntryRuleManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEntryRuleManager::CEntryRuleManager(CEntry_Dressup* pParent) :
	m_pLife		 (nullptr),		// �̗͂̏��
	m_pTime		 (nullptr),		// ���Ԃ̏��
	m_pSelect	 (nullptr),		// �I���̏��
	m_pStart	 (nullptr),		// �J�n�{�^���̏��
	m_pControl	 (nullptr),		// ����\���̏��
	m_pFade		 (nullptr),		// �t�F�[�h�̏��
	m_pParent	 (pParent),		// �e�̏��
	m_state		 (STATE_INIT),	// ���
	m_nSelect	 (RULE_TIME),	// ���݂̑I��
	m_nOldSelect (RULE_TIME),	// �O��̑I��
	m_fSinControlAlpha	(0.0f),	// ����\���̓�������
	m_fSinArrowAlpha	(0.0f)	// ���\���̓�������
{
	// �����o�ϐ����N���A
	memset(&m_apRuleTitle[0], 0, sizeof(m_apRuleTitle));	// ���[���^�C�g���̏��
	memset(&m_apArrow[0], 0, sizeof(m_apArrow));			// ���̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEntryRuleManager::~CEntryRuleManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEntryRuleManager::Init()
{
	CTexture* pTexture = CTexture::GetInstance();	// �e�N�X�`�����

	// �����o�ϐ���������
	memset(&m_apRuleTitle[0], 0, sizeof(m_apRuleTitle));	// ���[���^�C�g���̏��
	memset(&m_apArrow[0], 0, sizeof(m_apArrow));			// ���̏��
	m_pLife		 = nullptr;			// �̗͂̏��
	m_pTime		 = nullptr;			// ���Ԃ̏��
	m_pSelect	 = nullptr;			// �I���̏��
	m_pStart	 = nullptr;			// �J�n�{�^���̏��
	m_pControl	 = nullptr;			// ����\���̏��
	m_pFade		 = nullptr;			// �t�F�[�h�̏��
	m_state		 = STATE_INIT;		// ���
	m_nSelect	 = RULE_TIME;		// ���݂̑I��
	m_nOldSelect = RULE_TIME;		// �O��̑I��
	m_fSinControlAlpha	= -HALF_PI;	// ����\���̓�������
	m_fSinArrowAlpha	= -HALF_PI;	// ���\���̓�������

	// �t�F�[�h�̐���
	{
		m_pFade = CObject2D::Create(PRIORITY);
		if (m_pFade == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// �ʒu��ݒ�
		m_pFade->SetPosition(VEC3_SCREEN_CENT);

		// �傫����ݒ�
		m_pFade->SetSize(VEC2_SCREEN_SIZE);

		// �F��ݒ�
		m_pFade->SetColor(fade::INIT_COL);
	}

	// ���[���^�C�g���̐���
	{
		for (int i = 0; i < RULE_MAX; i++)
		{ // ���[���̑������J��Ԃ�

			// ���[���^�C�g���̐���
			m_apRuleTitle[i] = CObject2D_Anim::Create
			( // ����
				rule::POS + ((float)i * rule::SPACE),	// �ʒu
				rule::PART.x,	// �e�N�X�`����������
				rule::PART.y,	// �e�N�X�`���c������
				0.0f,			// �ҋ@����
				false,			// �����j��
				PRIORITY		// �D�揇��
			);
			if (m_apRuleTitle[i] == nullptr)
			{ // �����Ɏ��s�����ꍇ

				assert(false);
				return E_FAIL;
			}

			// �e�N�X�`����o�^�E����
			m_apRuleTitle[i]->BindTexture(pTexture->Regist(TEXTURE[TEXTURE_RULE_TITLE]));

			// �傫����ݒ�
			m_apRuleTitle[i]->SetSize(rule::SIZE);

			// �p�^�[����ݒ�
			m_apRuleTitle[i]->SetPatternAnim(i);

			// �����Đ���OFF�ɂ���
			m_apRuleTitle[i]->SetEnableAutoPlay(false);
		}
	}

	// ���̐���
	{
		for (int i = 0; i < MAX_RULE_ARROW; i++)
		{ // ���̑������J��Ԃ�

			// ���̐���
			m_apArrow[i] = CArrowUI::Create
			( // ����
				(CArrowUI::EDirection)i,	// ����
				arrow::POS + ((float)i * arrow::SPACE),	// �ʒu
				arrow::SIZE,							// �T�C�Y
				MyLib::color::White(0.0f),				// �F
				PRIORITY		// �D�揇��
			);
			if (m_apArrow[i] == nullptr)
			{ // �����Ɏ��s�����ꍇ

				assert(false);
				return E_FAIL;
			}
		}
	}

	// �I���̐���
	{
		m_pSelect = CObject2D::Create(PRIORITY);
		if (m_pSelect == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_pSelect->BindTexture(pTexture->Regist(TEXTURE[TEXTURE_SELECT]));

		// �ʒu��ݒ�
		m_pSelect->SetPosition(select::POS);

		// �傫����ݒ�
		m_pSelect->SetSize(select::SIZE_RULE);

		// �F��ݒ�
		m_pSelect->SetColor(select::COL);
	}

	// �J�n�{�^���̐���
	{
		m_pStart = CObject2D::Create(PRIORITY);
		if (m_pStart == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_pStart->BindTexture(pTexture->Regist(TEXTURE[TEXTURE_START]));

		// �ʒu��ݒ�
		m_pStart->SetPosition(start::POS);

		// �傫����ݒ�
		m_pStart->SetSize(start::SIZE);

		// �F��ݒ�
		m_pStart->SetColor(select::DEFAULT_COL);
	}

	// ����\���̐���
	{
		m_pControl = CObject2D::Create(PRIORITY);
		if (m_pControl == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_pControl->BindTexture(pTexture->Regist(TEXTURE[TEXTURE_CONTROL]));

		// �ʒu��ݒ�
		m_pControl->SetPosition(control::POS);

		// �傫����ݒ�
		m_pControl->SetSize(control::SIZE);

		// �F��ݒ�
		m_pControl->SetColor(MyLib::color::White(0.0f));
	}

	// �̗͂̐���
	{
		m_pLife = CObject2D_Anim::Create
		( // ����
			life::POS,		// �ʒu
			life::PART.x,	// �e�N�X�`����������
			life::PART.y,	// �e�N�X�`���c������
			0.0f,			// �ҋ@����
			false,			// �����j��
			PRIORITY		// �D�揇��
		);
		if (m_pLife == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_pLife->BindTexture(pTexture->Regist(TEXTURE[TEXTURE_LIFE]));

		// �傫����ݒ�
		m_pLife->SetSize(life::SIZE);

		// �F��ݒ�
		m_pLife->SetColor(select::DEFAULT_COL);

		// �����Đ���OFF�ɂ���
		m_pLife->SetEnableAutoPlay(false);
	}

	// ���Ԃ̐���
	{
		m_pTime = CTimeUI::Create
		( // ����
			0.0f,				// �\������
			time::POS,			// �ʒu
			time::VAL_SIZE,		// �����̑傫��
			time::PART_SIZE,	// ��؂�̑傫��
			time::VAL_SPACE,	// �����̋�
			time::PART_SPACE	// ��؂�̋�
		);
		if (m_pTime == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}
	}

	// UI�̓����x�̐ݒ�
	SetAlphaUI(0.0f, false);

	// �Q�[���ݒ�̓Ǎ�
	SRule rule;	// ���[��
	if (FAILED(LoadSetting(&rule)))
	{ // �Ǎ��Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �p�^�[����ݒ�
	m_pLife->SetPatternAnim(rule.life);

	// ���Ԃ�ݒ�
	m_pTime->SetTime(rule.fTime);

	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CEntryRuleManager::Uninit()
{
	for (int i = 0; i < RULE_MAX; i++)
	{ // ���[���̑������J��Ԃ�

		// ���[���^�C�g���̏I��
		SAFE_UNINIT(m_apRuleTitle[i]);
	}

	for (int i = 0; i < MAX_RULE_ARROW; i++)
	{ // ���̑������J��Ԃ�

		// ���̏I��
		SAFE_UNINIT(m_apArrow[i]);
	}

	// �I���̏I��
	SAFE_UNINIT(m_pSelect);

	// �J�n�{�^���̏I��
	SAFE_UNINIT(m_pStart);

	// ����\���̏I��
	SAFE_UNINIT(m_pControl);

	// �̗͂̏I��
	SAFE_UNINIT(m_pLife);

	// ���Ԃ̏I��
	SAFE_UNINIT(m_pTime);

	// �t�F�[�h�̏I��
	SAFE_UNINIT(m_pFade);

	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CEntryRuleManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �t�F�[�h���̏ꍇ������
	if (GET_MANAGER->GetFade()->GetState() != CFade::STATE_NONE) { return; }

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_INIT:

		// UI�̎����`���ON�ɂ���
		SetEnableDispUI(true);

		// �t�F�[�h�C����Ԃɂ���
		m_state = STATE_FADEIN;

		// �����𔲂����Ƀt�F�[�h�C���Ɉڍs

	case STATE_FADEIN:

		// �t�F�[�h�C���̍X�V
		UpdateFadeIn();
		break;

	case STATE_RULESET:

		// �I�𑀍�
		Select();

		// ���葀��
		Decide();

		// ���[���ύX����
		ChangeRule();

		// ����X�V
		UpdateControl();

		// ���X�V
		UpdateArrow();
		break;

	case STATE_FADEOUT:

		// �t�F�[�h�A�E�g�̍X�V
		UpdateFadeOut();
		break;

	case STATE_ENTRYBACK:

		// UI�̎����`���OFF�ɂ���
		SetEnableDispUI(false);

		// �����ւ���Ԃɖ߂�
		m_pParent->SetState(CEntry_Dressup::EState::STATE_DRESSUP);
		return;	// �֐��𔲂���

	default:
		assert(false);
		break;
	}

	// TODO�F����
#if 0
	for (int i = 0; i < RULE_MAX; i++)
	{ // ���[���̑������J��Ԃ�

		// ���[���^�C�g���̍X�V
		m_apRuleTitle[i]->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	for (int i = 0; i < MAX_RULE_ARROW; i++)
	{ // ���̑������J��Ԃ�

		// ���̍X�V
		m_apArrow[i]->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// �I���̍X�V
	m_pSelect->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �J�n�{�^���̍X�V
	m_pStart->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ����\���̍X�V
	m_pControl->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ���Ԃ̍X�V
	m_pTime->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �̗͂̍X�V
	m_pLife->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �t�F�[�h�̍X�V
	m_pFade->Update(fDeltaTime, fDeltaRate, fSlowRate);
#endif
}

//============================================================
//	���[���擾����
//============================================================
CEntryRuleManager::SRule CEntryRuleManager::GetRule() const
{
	SRule rule;	// ���[��

	// �p�^�[�����擾
	rule.life = (ELife)m_pLife->GetPatternAnim();

	// ���Ԃ��擾
	rule.fTime = m_pTime->GetTime();

	return rule;
}

//============================================================
//	��������
//============================================================
CEntryRuleManager* CEntryRuleManager::Create(CEntry_Dressup* pParent)
{
	// �G���g���[���[���}�l�[�W���[�̐���
	CEntryRuleManager* pEntryRuleManager = DEBUG_NEW CEntryRuleManager(pParent);
	if (pEntryRuleManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G���g���[���[���}�l�[�W���[�̏�����
		if (FAILED(pEntryRuleManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G���g���[���[���}�l�[�W���[�̔j��
			SAFE_DELETE(pEntryRuleManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pEntryRuleManager;
	}
}

//============================================================
//	�j������
//============================================================
void CEntryRuleManager::Release(CEntryRuleManager*& prEntryRuleManager)
{
	// �G���g���[���[���}�l�[�W���[�̏I��
	assert(prEntryRuleManager != nullptr);
	prEntryRuleManager->Uninit();

	// �������J��
	SAFE_DELETE(prEntryRuleManager);
}

//============================================================
//	�Q�[���ݒ�̕ۑ�����
//============================================================
HRESULT CEntryRuleManager::SaveSetting(SRule* pRule)
{
	// �t�@�C�����J��
	std::ofstream file(SET_PATH);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�Q�[���ݒ�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// ���o���̏����o��
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	�Q�[���ݒ�e�L�X�g [game_setting.txt]" << std::endl;
	file << "#	Author : ���c �E��" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;

	// �Q�[���ݒ�̏����o��
	file << "#------------------------------------------------------------------------------" << std::endl;
	file << "#	�Q�[���ݒ���" << std::endl;
	file << "#------------------------------------------------------------------------------" << std::endl;
	file << "GAMESET" << std::endl;
	file << "	TIME = " << pRule->fTime << std::endl;
	file << "	LIFE = " << pRule->life << std::endl;
	file << "END_GAMESET" << std::endl;

	// �t�@�C�������
	file.close();

	return S_OK;
}

//============================================================
//	�Q�[���ݒ�̓Ǎ�����
//============================================================
HRESULT CEntryRuleManager::LoadSetting(SRule* pRule)
{
	// �t�@�C�����J��
	std::ifstream file(SET_PATH);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�Q�[���ݒ�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �t�@�C����Ǎ�
	std::string str;	// �Ǎ�������
	while (file >> str)
	{ // �t�@�C���̏I�[�ł͂Ȃ��ꍇ���[�v

		if (str.front() == '#') { std::getline(file, str); }	// �R�����g�A�E�g
		else if (str == "GAMESET")
		{
			do { // END_GAMESET��ǂݍ��ނ܂Ń��[�v

				// �������ǂݍ���
				file >> str;

				if (str.front() == '#') { std::getline(file, str); }	// �R�����g�A�E�g
				else if (str == "TIME")
				{
					file >> str;			// ����Ǎ�
					file >> pRule->fTime;	// ���Ԃ�Ǎ�
				}
				else if (str == "LIFE")
				{
					int nLife = 0;	// �̗�
					file >> str;	// ����Ǎ�
					file >> nLife;	// �̗͂�Ǎ�

					// �񋓂ɕϊ�
					pRule->life = (ELife)nLife;
				}
			} while (str != "END_GAMESET");	// END_GAMESET��ǂݍ��ނ܂Ń��[�v
		}
	}

	// �t�@�C�������
	file.close();

	return S_OK;
}

//============================================================
//	�t�F�[�h�C���̍X�V����
//============================================================
void CEntryRuleManager::UpdateFadeIn()
{
	float fAlphaFade = m_pFade->GetAlpha();	// �t�F�[�h�����x

	// �����x�����Z
	fAlphaFade += fade::ADD_ALPHA;
	if (fAlphaFade >= fade::SET_COL.a)
	{ // �����x���オ�肫�����ꍇ

		// �����x��␳
		fAlphaFade = fade::SET_COL.a;

		// ���[���ݒ��Ԃɂ���
		m_state = STATE_RULESET;
	}

	// UI�̓����x�̐ݒ�
	SetAlphaUI(fAlphaFade * 2.0f, false);

	// �����x�𔽉f
	m_pFade->SetAlpha(fAlphaFade);
}

//============================================================
//	�t�F�[�h�A�E�g�̍X�V����
//============================================================
void CEntryRuleManager::UpdateFadeOut()
{
	float fAlphaFade = m_pFade->GetAlpha();	// �t�F�[�h�����x

	// �����x�����Z
	fAlphaFade -= fade::SUB_ALPHA;
	if (fAlphaFade <= fade::INIT_COL.a)
	{ // �����x�������肫�����ꍇ

		// �����x��␳
		fAlphaFade = fade::INIT_COL.a;

		// �G���g���[�߂���Ԃɂ���
		m_state = STATE_ENTRYBACK;
	}

	// UI�̓����x�̐ݒ�
	SetAlphaUI(fAlphaFade * 2.0f, true);

	// �����x�𔽉f
	m_pFade->SetAlpha(fAlphaFade);
}

//============================================================
//	����̍X�V����
//============================================================
void CEntryRuleManager::UpdateControl()
{
	float fAlphaControl = m_pControl->GetAlpha();	// ����\�������x
	if (fAlphaControl < control::MIN_COL.a)
	{ // �����x���Œ�����Ⴂ�ꍇ

		// �����x�����Z
		fAlphaControl += control::ADD_ALPHA;
		if (fAlphaControl > control::MIN_COL.a)
		{ // �����x�����߂����ꍇ

			// �����x��␳
			fAlphaControl = control::MIN_COL.a;
		}

		// ����\�������x��ݒ�
		m_pControl->SetAlpha(fAlphaControl);
	}
	else
	{ // �����x���Œ���ȏ�̏ꍇ

		// �����x���グ��
		m_fSinControlAlpha += control::ADD_SINROT;
		UtilFunc::Transformation::RotNormalize(m_fSinControlAlpha);	// �������K��

		// �����x���Z�ʂ����߂�
		float fAddAlpha = (control::MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinControlAlpha) - 1.0f);

		// ����\�������x��ݒ�
		m_pControl->SetAlpha(control::BASIC_ALPHA + fAddAlpha);
	}
}

//============================================================
//	���̍X�V����
//============================================================
void CEntryRuleManager::UpdateArrow()
{
	for (int i = 0; i < MAX_RULE_ARROW; i++)
	{ // ���̑������J��Ԃ�

		float fAlphaArrow = m_apArrow[i]->GetAlpha();	// ��󓧖��x
		if (fAlphaArrow < arrow::MIN_COL.a)
		{ // �����x���Œ�����Ⴂ�ꍇ

			// �����x�����Z
			fAlphaArrow += arrow::ADD_ALPHA;
			if (fAlphaArrow > arrow::MIN_COL.a)
			{ // �����x�����߂����ꍇ

				// �����x��␳
				fAlphaArrow = arrow::MIN_COL.a;
			}

			// ��󓧖��x��ݒ�
			m_apArrow[i]->SetAlpha(fAlphaArrow);
		}
		else
		{ // �����x���Œ���ȏ�̏ꍇ

			// �����x���グ��
			m_fSinArrowAlpha += arrow::ADD_SINROT;
			UtilFunc::Transformation::RotNormalize(m_fSinArrowAlpha);	// �������K��

			// �����x���Z�ʂ����߂�
			float fAddAlpha = (arrow::MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinArrowAlpha) - 1.0f);

			// ��󓧖��x��ݒ�
			m_apArrow[i]->SetAlpha(arrow::BASIC_ALPHA + fAddAlpha);
		}
	}
}

//============================================================
//	�I�𑀍쏈��
//============================================================
void CEntryRuleManager::Select()
{
	CObject *apSelect[MAX_SELECT] =	// �I���I�u�W�F�N�g
	{
		m_pTime,	// ����
		m_pLife,	// �̗�
		m_pStart	// �J�n�{�^��
	};
	CInputKeyboard* pKey = GET_INPUTKEY;	// �L�[�{�[�h
	CInputGamepad* pPad = GET_INPUTPAD;		// �p�b�h
	MyLib::Vector3 posSelect = m_pSelect->GetPosition();	// �I���ʒu
	MyLib::Vector2 sizeSelect = m_pSelect->GetSize();		// �I��傫��

	if (pKey->GetTrigger(DIK_W)
	||  pKey->GetTrigger(DIK_UP)
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_UP))
	{ // ��ړ��̑��삪�s��ꂽ�ꍇ

		// ��ɑI�������炷
		m_nSelect = (m_nSelect + (MAX_SELECT - 1)) % MAX_SELECT;

		if (m_nSelect != SELECT_GAME)
		{ // �I�����Q�[���J�ڂł͂Ȃ��ꍇ

			// �ʒu��ύX
			posSelect.x = select::POS.x;
			posSelect.y = select::POS.y + rule::SPACE.y * (float)m_nSelect;

			// �傫����ύX
			sizeSelect = select::SIZE_RULE;
			for (int i = 0; i < MAX_RULE_ARROW; i++)
			{ // ���̑������J��Ԃ�

				// ���̈ʒu��ύX
				MyLib::Vector3 setpos = MyLib::Vector3(arrow::POS.x + (arrow::SPACE.x * (float)i), arrow::POS.y + rule::SPACE.y * (float)m_nSelect, 0.0f);
				m_apArrow[i]->SetPosition(setpos);
				m_apArrow[i]->SetOriginPosition(setpos);

				// �����`���ON�ɂ���
				m_apArrow[i]->SetEnableDisp(true);
			}
		}
		else
		{ // �I�����Q�[���J�ڂ̏ꍇ

			// �ʒu��ύX
			posSelect = start::POS;

			// �傫����ύX
			sizeSelect = select::SIZE_START;
			for (int i = 0; i < MAX_RULE_ARROW; i++)
			{ // ���̑������J��Ԃ�

				// �����`���OFF�ɂ���
				m_apArrow[i]->SetEnableDisp(false);
			}
		}

		// �T�E���h�̍Đ�
		PLAY_SOUND(CSound::LABEL_SE_GRIP01);
	}

	if (pKey->GetTrigger(DIK_S)
	||  pKey->GetTrigger(DIK_DOWN)
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_DOWN))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// ���ɑI�������炷
		m_nSelect = (m_nSelect + 1) % MAX_SELECT;

		if (m_nSelect != SELECT_GAME)
		{ // �I�����Q�[���J�ڂł͂Ȃ��ꍇ

			// �ʒu��ύX
			posSelect.x = select::POS.x;
			posSelect.y = select::POS.y + rule::SPACE.y * (float)m_nSelect;

			// �傫����ύX
			sizeSelect = select::SIZE_RULE;
			for (int i = 0; i < MAX_RULE_ARROW; i++)
			{ // ���̑������J��Ԃ�

				// ���̈ʒu��ύX
				MyLib::Vector3 setpos = MyLib::Vector3(arrow::POS.x + (arrow::SPACE.x * (float)i), arrow::POS.y + rule::SPACE.y * (float)m_nSelect, 0.0f);
				m_apArrow[i]->SetPosition(setpos);
				m_apArrow[i]->SetOriginPosition(setpos);


				// �����`���ON�ɂ���
				m_apArrow[i]->SetEnableDisp(true);
			}
		}
		else
		{ // �I�����Q�[���J�ڂ̏ꍇ

			// �ʒu��ύX
			posSelect = start::POS;

			// �傫����ύX
			sizeSelect = select::SIZE_START;
			for (int i = 0; i < MAX_RULE_ARROW; i++)
			{ // ���̑������J��Ԃ�

				// �����`���OFF�ɂ���
				m_apArrow[i]->SetEnableDisp(false);
			}
		}

		// �T�E���h�̍Đ�
		PLAY_SOUND(CSound::LABEL_SE_GRIP01);
	}

	// �O��̑I��v�f�̐F�����ɐݒ�
	apSelect[m_nOldSelect]->SetColor(select::DEFAULT_COL);

	// ���݂̑I��v�f�̐F�𔒂ɐݒ�
	apSelect[m_nSelect]->SetColor(select::CHOICE_COL);

	// ���݂̑I��v�f����
	m_nOldSelect = m_nSelect;

	// �I���ʒu�𔽉f
	m_pSelect->SetPosition(posSelect);

	// �I��傫���𔽉f
	m_pSelect->SetSize(sizeSelect);
}

//============================================================
//	���葀�쏈��
//============================================================
void CEntryRuleManager::Decide()
{
	CInputKeyboard* pKey = GET_INPUTKEY;	// �L�[�{�[�h
	CInputGamepad* pPad = GET_INPUTPAD;		// �p�b�h
	if (pKey->GetTrigger(DIK_RETURN)
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_A))
	{ // ����̑��삪�s��ꂽ�ꍇ

		switch (m_nSelect)
		{ // �I�����Ƃ̏���
		case RULE_TIME:
		case RULE_LIFE:
			break;

		case SELECT_GAME:

			// ���[���ύX���f
			SetRule();

			// �I����Ԃɖ߂�
			m_pParent->SetState(CEntry_Dressup::EState::STATE_END);

			// �T�E���h�̍Đ�
			PLAY_SOUND(CSound::LABEL_SE_PUSH);
			break;

		default:
			assert(false);
			break;
		}
	}

	if (pKey->GetTrigger(DIK_SPACE)
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_B))
	{ // �߂鑀�삪�s��ꂽ�ꍇ

		// �t�F�[�h�A�E�g��Ԃɂ���
		m_state = STATE_FADEOUT;

		// �T�E���h�̍Đ�
		PLAY_SOUND(CSound::LABEL_SE_GRIP01);
	}
}

//============================================================
//	���[���ύX���쏈��
//============================================================
void CEntryRuleManager::ChangeRule()
{
	CInputKeyboard* pKey = GET_INPUTKEY;	// �L�[�{�[�h
	CInputGamepad* pPad = GET_INPUTPAD;		// �p�b�h
	int nLife = m_pLife->GetPatternAnim();	// �̗�

	if (pKey->GetTrigger(DIK_A)
	||  pKey->GetTrigger(DIK_LEFT)
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_LEFT))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// �I�����ړ���Ԃ֑J��
		m_apArrow[CArrowUI::EDirection::DIRECTION_L]->SetState(CArrowUI::EState::STATE_SELECTMOVE);

		switch (m_nSelect)
		{ // �I�����Ƃ̏���
		case RULE_TIME:
		{
			float fCurTime = m_pTime->GetTime();	// ����

			// ���Ԃ����Z
			m_pTime->SetTime(fCurTime - 15.0f);

			// �T�E���h�̍Đ�
			PLAY_SOUND(CSound::LABEL_SE_GRIP01);
			break;
		}
		case RULE_LIFE:
		{
			// ���ɑI�������炷
			nLife = (nLife + (CEntryRuleManager::LIFE_MAX - 1)) % CEntryRuleManager::LIFE_MAX;

			// �T�E���h�̍Đ�
			PLAY_SOUND(CSound::LABEL_SE_GRIP01);
			break;
		}
		case SELECT_GAME:
		{
			break;
		}
		default:
			assert(false);
			break;
		}
	}
	if (pKey->GetTrigger(DIK_D)
	||  pKey->GetTrigger(DIK_RIGHT)
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_RIGHT))
	{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

		// �I�����ړ���Ԃ֑J��
		m_apArrow[CArrowUI::EDirection::DIRECTION_R]->SetState(CArrowUI::EState::STATE_SELECTMOVE);

		switch (m_nSelect)
		{ // �I�����Ƃ̏���
		case RULE_TIME:
		{
			float fCurTime = m_pTime->GetTime();	// ����

			// ���Ԃ����Z
			m_pTime->SetTime(fCurTime + 15.0f);

			// �T�E���h�̍Đ�
			PLAY_SOUND(CSound::LABEL_SE_GRIP01);
			break;
		}
		case RULE_LIFE:
		{
			// �E�ɑI�������炷
			nLife = (nLife + 1) % CEntryRuleManager::LIFE_MAX;

			// �T�E���h�̍Đ�
			PLAY_SOUND(CSound::LABEL_SE_GRIP01);
			break;
		}
		case SELECT_GAME:
		{
			break;
		}
		default:
			assert(false);
			break;
		}
	}

	// �̗͂𔽉f
	m_pLife->SetPatternAnim(nLife);
}

//============================================================
//	���[���ύX���f����
//============================================================
void CEntryRuleManager::SetRule()
{
	// TODO�F�O���t�@�C���ɏ����o��
#if 0
	// �|�C���^��錾
	CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// �f�[�^�ۑ����

	// �����|�C���g����ݒ�
	pRetention->SetWinPoint(m_pWinPoint->GetNum());

	// ���j������ݒ�
	pRetention->SetKillState((CRetentionManager::EKill)m_pKill->GetPattern());

	// ����������ݒ�
	pRetention->SetWinState((CRetentionManager::EWin)m_pWin->GetPattern());
#endif
}

//============================================================
//	UI�̕`��󋵂̐ݒ菈��
//============================================================
void CEntryRuleManager::SetEnableDispUI(const bool bDisp)
{
	for (int i = 0; i < RULE_MAX; i++)
	{ // ���[���̑������J��Ԃ�

		// ���[���^�C�g���̕`���ݒ�
		m_apRuleTitle[i]->SetEnableDisp(bDisp);
	}

	for (int i = 0; i < MAX_RULE_ARROW; i++)
	{ // ���̑������J��Ԃ�

		// ���̕`���ݒ�
		m_apArrow[i]->SetEnableDisp(bDisp);
	}

	// �I���̕`���ݒ�
	m_pSelect->SetEnableDisp(bDisp);

	// �J�n�{�^���̕`���ݒ�
	m_pStart->SetEnableDisp(bDisp);

	// ����\���̕`���ݒ�
	m_pControl->SetEnableDisp(bDisp);

	// �̗͂̕`���ݒ�
	m_pLife->SetEnableDisp(bDisp);

	// ���Ԃ̕`���ݒ�
	m_pTime->SetEnableDisp(bDisp);
}

//============================================================
//	UI�̓����x�̐ݒ菈��
//============================================================
void CEntryRuleManager::SetAlphaUI(const float fAlpha, const bool bFadeOut)
{
	for (int i = 0; i < RULE_MAX; i++)
	{ // ���[���̑������J��Ԃ�

		// ���[���^�C�g���̓����x��ݒ�
		m_apRuleTitle[i]->SetAlpha(fAlpha);
	}

	// �J�n�{�^���̓����x��ݒ�
	m_pStart->SetAlpha(fAlpha);

	// �̗͂̓����x��ݒ�
	m_pLife->SetAlpha(fAlpha);

	// ���Ԃ̓����x��ݒ�
	m_pTime->SetAlpha(fAlpha);

	if (bFadeOut)
	{ // �t�F�[�h�A�E�g���̏ꍇ

		for (int i = 0; i < MAX_RULE_ARROW; i++)
		{ // ���̑������J��Ԃ�

			// �����x����ݒ�
			float fMaxAlpha = m_apArrow[i]->GetAlpha();	// �ő哧���x
			float fSetAlpha = fAlpha;	// �ݒ蓧���x

			// �����x�𐧌�
			UtilFunc::Transformation::ValueNormalize(fSetAlpha, fMaxAlpha, 0.0f);

			// ���̓����x��ݒ�
			m_apArrow[i]->SetAlpha(fSetAlpha);
		}

		// �I���̓����x��ݒ�
		{
			// �����x����ݒ�
			float fMaxAlpha = m_pSelect->GetAlpha();	// �ő哧���x
			float fSetAlpha = fAlpha;	// �ݒ蓧���x

			// �����x�𐧌�
			UtilFunc::Transformation::ValueNormalize(fSetAlpha, fMaxAlpha, 0.0f);

			// �I���̓����x��ݒ�
			m_pSelect->SetAlpha(fSetAlpha);
		}

		// ����\���̓����x��ݒ�
		{
			// �����x����ݒ�
			float fMaxAlpha = m_pControl->GetAlpha();	// �ő哧���x
			float fSetAlpha = fAlpha;	// �ݒ蓧���x

			// �����x�𐧌�
			UtilFunc::Transformation::ValueNormalize(fSetAlpha, fMaxAlpha, 0.0f);

			// ����\���̓����x��ݒ�
			m_pControl->SetAlpha(fSetAlpha);
		}
	}
}

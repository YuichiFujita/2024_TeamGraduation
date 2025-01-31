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
#include "loadtext.h"

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
		"data\\TEXTURE\\entry\\infinity.png",		// ���ԁ��e�N�X�`��
	};

	const std::string SET_PATH = "data\\TEXT\\entry\\game_setting.txt";
	const int PRIORITY		= 6;	// �G���g���[�̗D�揇��
	const int MAX_SELECT	= CEntryRuleManager::RULE_MAX + 1;	// ���[���I���̑��� (����������܂�)
	const int SELECT_GAME	= CEntryRuleManager::RULE_MAX;		// �Q�[���J�ڑI��ԍ�

	// �t�F�[�h���
	namespace fade
	{
		const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// �������F
		const D3DXCOLOR SET_COL	 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);	// �ݒ�F
		const float	ADD_ALPHA = 0.025f;	// �����x�̉��Z��
		const float	SUB_ALPHA = 0.025f;	// �����x�̌��Z��
	}

	// ���[���^�C�g�����
	namespace rule
	{
		const MyLib::PosGrid2	PART	= MyLib::PosGrid2(1, CEntryRuleManager::RULE_MAX);	// �e�N�X�`��������
		const MyLib::Vector3	POS		= MyLib::Vector3(320.0f, 150.0f, 0.0f);		// �ʒu
		const MyLib::Vector2	SIZE	= MyLib::Vector2(588.0f, 75.0f);			// �傫��
		const MyLib::Vector3	SPACE	= MyLib::Vector3(0.0f, 200.0f, 0.0f);		// ��
	}

	// �J�n�{�^�����
	namespace start
	{
		const MyLib::Vector3 POS	= MyLib::Vector3(VEC3_SCREEN_CENT.x, 595.0f, 0.0f);	// �ʒu
		const MyLib::Vector2 SIZE	= MyLib::Vector2(484.0f, 90.0f);					// �傫��
	}

	// �^�C�����
	namespace time
	{
		const MyLib::Vector3 POS		= MyLib::Vector3(930.0f, 150.0f, 0.0f);	// �^�C�}�[�ʒu
		const MyLib::Vector2 VAL_SIZE	= MyLib::Vector2(30.0f, 28.0f);			// �^�C�}�[�����傫��
		const MyLib::Vector2 PART_SIZE	= MyLib::Vector2(30.0f, 18.0f);			// �^�C�}�[��؂�傫��
		const MyLib::Vector2 VAL_SPACE	= MyLib::Vector2(53.0f, 0.0f);			// �^�C�}�[������
		const MyLib::Vector2 PART_SPACE	= MyLib::Vector2(35.0f, 0.0f);			// �^�C�}�[��؂��
		const float INF_HEIGHT	= 55.0f;	// ��UI�̏c��
		const float MOVE_TIME	= 30.0f;	// ���ԕύX��
		const float MIN_TIME	= 60.0f;	// �ŏ�����
		const float MAX_TIME	= 180.0f;	// �ő厞��
	}

	// �̗͏��
	namespace life
	{
		const MyLib::PosGrid2	PART	= MyLib::PosGrid2(1, CEntryRuleManager::LIFE_MAX);				// �e�N�X�`��������
		const MyLib::Vector3	POS		= MyLib::Vector3(930.0f, time::POS.y + rule::SPACE.y, 0.0f);	// �ʒu
		const MyLib::Vector2	SIZE	= MyLib::Vector2(444.0f, 65.0f);								// �傫��
	}

	// �I�����
	namespace select
	{
		const float	ADD_SIZE = 2.5f;	// �傫�����Z��

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
		const MyLib::Vector3	SPACE	= MyLib::Vector3(select::SIZE_RULE.x * 0.5f + 50.0f, 0.0f, 0.0f);	// ��
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
	m_pTimeInf	 (nullptr),		// ���ԁ��̏��
	m_pSelect	 (nullptr),		// �I���̏��
	m_pStart	 (nullptr),		// �J�n�{�^���̏��
	m_pControl	 (nullptr),		// ����\���̏��
	m_pFade		 (nullptr),		// �t�F�[�h�̏��
	m_pBG		 (nullptr),		// �w�i�̏��
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
	m_pTimeInf	 = nullptr;			// ���ԁ��̏��
	m_pSelect	 = nullptr;			// �I���̏��
	m_pStart	 = nullptr;			// �J�n�{�^���̏��
	m_pControl	 = nullptr;			// ����\���̏��
	m_pFade		 = nullptr;			// �t�F�[�h�̏��
	m_state		 = STATE_INIT;		// ���
	m_nSelect	 = RULE_TIME;		// ���݂̑I��
	m_nOldSelect = RULE_TIME;		// �O��̑I��
	m_fSinControlAlpha	= -HALF_PI;	// ����\���̓�������
	m_fSinArrowAlpha	= -HALF_PI;	// ���\���̓�������
	
	// �w�i�̐���
	{
		m_pBG = CObject2D::Create(PRIORITY);
		if (m_pBG == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// �ʒu��ݒ�
		m_pBG->SetPosition(VEC3_SCREEN_CENT);

		// �傫����ݒ�
		m_pBG->SetSize(MyLib::Vector2(640.0f, 360.0f));

		// �F��ݒ�
		m_pBG->SetColor(fade::INIT_COL);

		// �e�N�X�`����o�^�E����
		int texID = pTexture->Regist("data\\TEXTURE\\entry\\bg.png");
		m_pBG->BindTexture(texID);
	}


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
		m_pFade->SetSize(MyLib::Vector2(640.0f, 360.0f));

		// �F��ݒ�
		m_pFade->SetColor(fade::INIT_COL);
		
		// �e�N�X�`����o�^�E����
		int texID = pTexture->Regist("data\\TEXTURE\\entry\\bg_scroll.png");
		m_pFade->BindTexture(texID);
	}

	// ���[���^�C�g���̐���
	{
		for (int i = 0; i < RULE_MAX; i++)
		{ // ���[���̑������J��Ԃ�

			MyAssert::TrueAssert(i < 0 || i >= sizeof(m_apRuleTitle), "�z��I�[�o�[");

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
			int texID = pTexture->Regist(TEXTURE[TEXTURE_RULE_TITLE]);
			m_apRuleTitle[i]->BindTexture(texID);

			// �摜�䗦����傫����ݒ�
			MyLib::Vector2 size = pTexture->GetImageSize(texID);
			size.y *= (1.0f / (float)rule::PART.y);

			size = UtilFunc::Transformation::AdjustSizeByHeight(size, rule::SIZE.y);	// �c�������ɃT�C�Y�v�Z
			m_apRuleTitle[i]->SetSize(size);
			m_apRuleTitle[i]->SetSizeOrigin(size);

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

			MyAssert::TrueAssert(i < 0 || i >= sizeof(m_apArrow), "�z��I�[�o�[");

			// ���̐���
			m_apArrow[i] = CArrowUI::Create
			( // ����
				(CArrowUI::EDirection)i,	// ����
				select::POS,				// �ʒu
				arrow::SIZE,				// �T�C�Y
				MyLib::color::White(0.0f),	// �F
				PRIORITY		// �D�揇��
			);
			if (m_apArrow[i] == nullptr)
			{ // �����Ɏ��s�����ꍇ

				assert(false);
				return E_FAIL;
			}

			// �I�t�Z�b�g�ݒ�
			m_apArrow[i]->SetOffset(arrow::SPACE);

			// �ʒu�Đݒ�
			m_apArrow[i]->SetPosition(select::POS);
			m_apArrow[i]->SetOriginPosition(select::POS);
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

		// �ʒu��ݒ�
		m_pSelect->SetPosition(select::POS);

		// �e�N�X�`����o�^�E����
		int texID = pTexture->Regist(TEXTURE[TEXTURE_SELECT]);
		m_pSelect->BindTexture(texID);

		// �摜�䗦����傫����ݒ�
		MyLib::Vector2 size = pTexture->GetImageSize(texID);
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, select::SIZE_RULE.y);	// �c�������ɃT�C�Y�v�Z
		m_pSelect->SetSize(size);
		m_pSelect->SetSizeOrigin(size);

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
		int texID = pTexture->Regist(TEXTURE[TEXTURE_START]);
		m_pStart->BindTexture(texID);

		// �摜�䗦����傫����ݒ�
		MyLib::Vector2 size = pTexture->GetImageSize(texID);
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, start::SIZE.y);	// �c�������ɃT�C�Y�v�Z
		m_pStart->SetSize(size);
		m_pStart->SetSizeOrigin(size);

		// �ʒu��ݒ�
		m_pStart->SetPosition(start::POS);

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
		int texID = pTexture->Regist(TEXTURE[TEXTURE_CONTROL]);
		m_pControl->BindTexture(texID);

		// �摜�䗦����傫����ݒ�
		MyLib::Vector2 size = pTexture->GetImageSize(texID);
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, control::SIZE.y);	// �c�������ɃT�C�Y�v�Z
		m_pControl->SetSize(size);
		m_pControl->SetSizeOrigin(size);

		// �ʒu��ݒ�
		m_pControl->SetPosition(control::POS);

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
		int texID = pTexture->Regist(TEXTURE[TEXTURE_LIFE]);
		m_pLife->BindTexture(texID);

		// �摜�䗦����傫����ݒ�
		MyLib::Vector2 size = pTexture->GetImageSize(texID); 
		size.y *= (1.0f / (float)life::PART.y);

		size = UtilFunc::Transformation::AdjustSizeByHeight(size, life::SIZE.y);	// �c�������ɃT�C�Y�v�Z
		m_pLife->SetSize(size);
		m_pLife->SetSizeOrigin(size);

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
			time::VAL_SIZE.y,	// �����̑傫��
			time::PART_SIZE.y,	// ��؂�̑傫��
			time::VAL_SPACE,	// �����̋�
			time::PART_SPACE	// ��؂�̋�
		);
		if (m_pTime == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}
	}

	// ���ԁ��̐���
	{
		m_pTimeInf = CObject2D::Create(PRIORITY);
		if (m_pTimeInf == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		int texID = pTexture->Regist(TEXTURE[TEXTURE_TIME_INF]);
		m_pTimeInf->BindTexture(texID);

		// �摜�䗦����傫����ݒ�
		MyLib::Vector2 size = pTexture->GetImageSize(texID);
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, time::INF_HEIGHT);	// �c�������ɃT�C�Y�v�Z
		m_pTimeInf->SetSize(size);
		m_pTimeInf->SetSizeOrigin(size);

		// �ʒu��ݒ�
		m_pTimeInf->SetPosition(time::POS);
	}

	// UI�̓����x�̐ݒ�
	SetAlphaUI(0.0f, false);

	// �Q�[���ݒ�̓Ǎ�
	SRule rule;	// ���[��
	if (FAILED(LoadRule(&rule)))
	{ // �Ǎ��Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �p�^�[����ݒ�
	m_pLife->SetPatternAnim(rule.life);

	// ���Ԃ�ݒ�
	m_pTime->SetTime(rule.fTime);

	// ���ԕ\���̍X�V
	UpdateTimeUI();

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

	// ���ԁ��̏I��
	SAFE_UNINIT(m_pTimeInf);

	// �t�F�[�h�̏I��
	SAFE_UNINIT(m_pFade);

	// �w�i�̏I��
	SAFE_UNINIT(m_pBG);

	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CEntryRuleManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �t�F�[�h�X�N���[��
	D3DXVECTOR2* pTex = m_pFade->GetTex();
	
	static float scrollX = 0.00038f;
	static float scrollY = 0.001f;

	ImGui::DragFloat("scrollX", &scrollX, 0.00001f, 0.0f, 0.0f, "%.8f");
	scrollY = scrollX * UtilFunc::Calculation::AspectRatio(D3DXVECTOR2(640.0f, 360.0f));

	for (int i = 0; i < 4; i++)
	{
		pTex[i].x += scrollX;
		pTex[i].y -= scrollY;
	}

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
//	�Q�[���ݒ菉�����̕ۑ�����
//============================================================
HRESULT CEntryRuleManager::SaveInit()
{
	SRule rule;	// ���[��
	rule.fTime = time::MAX_TIME;	// �ő厞��
	rule.life = ELife::LIFE_NORMAL;	// �ʏ�̗�

	std::string aTeamName[CGameManager::ETeamSide::SIDE_MAX];	// �`�[����
	for (int team = 0; team < CGameManager::ETeamSide::SIDE_MAX; team++)
	{ // �`�[�������J��Ԃ�

		// �����_���Ȗ��O��Ǎ�
		std::wstring wsName = loadtext::LoadText("data\\TEXT\\entry\\nameTeam.txt", UtilFunc::Transformation::Random(0, 9)).front();

		// �}���`�o�C�g�ϊ�
		aTeamName[team] = UtilFunc::Transformation::WideToMultiByte(wsName);
	}

	// �����l�̕ۑ�
	return SaveSetting(rule, aTeamName[CGameManager::ETeamSide::SIDE_LEFT], aTeamName[CGameManager::ETeamSide::SIDE_RIGHT]);
}

//============================================================
//	�Q�[���ݒ�̕ۑ�����
//============================================================
HRESULT CEntryRuleManager::SaveSetting(const SRule& rRule, const std::string& rNameLeft, const std::string& rNameRight)
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
	file << "	TIME = " << rRule.fTime << std::endl;
	file << "	LIFE = " << rRule.life << std::endl;
	file << "END_GAMESET\n" << std::endl;

	// �`�[�����̏����o��
	file << "#------------------------------------------------------------------------------" << std::endl;
	file << "#	�`�[�������" << std::endl;
	file << "#------------------------------------------------------------------------------" << std::endl;
	file << "TEAMNAME" << std::endl;
	file << "	LEFT  = " << rNameLeft << std::endl;
	file << "	RIGHT = " << rNameRight << std::endl;
	file << "END_TEAMNAME" << std::endl;

	// �t�@�C�������
	file.close();

	return S_OK;
}

//============================================================
//	�Q�[���ݒ�̓Ǎ�����
//============================================================
HRESULT CEntryRuleManager::LoadRule(SRule* pRule)
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
//	�`�[�����̓Ǎ�����
//============================================================
HRESULT CEntryRuleManager::LoadName(std::string* pName, CGameManager::ETeamSide team)
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
		else if (str == "TEAMNAME")
		{
			do { // END_TEAMNAME��ǂݍ��ނ܂Ń��[�v

				// �������ǂݍ���
				file >> str;

				if (str.front() == '#') { std::getline(file, str); }	// �R�����g�A�E�g
				else if (str == "LEFT" && team == CGameManager::ETeamSide::SIDE_LEFT)
				{
					file >> str;	// ����Ǎ�
					file >> *pName;	// ���`�[������Ǎ�
				}
				else if (str == "RIGHT" && team == CGameManager::ETeamSide::SIDE_RIGHT)
				{
					file >> str;	// ����Ǎ�
					file >> *pName;	// �E�`�[������Ǎ�
				}
			} while (str != "END_TEAMNAME");	// END_TEAMNAME��ǂݍ��ނ܂Ń��[�v
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
	m_pBG->SetAlpha(fAlphaFade);
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
	m_pBG->SetAlpha(fAlphaFade);
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

		MyAssert::TrueAssert(i < 0 || i >= sizeof(m_apArrow), "�z��I�[�o�[");
	
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
//	���ԕ\���̍X�V����
//============================================================
void CEntryRuleManager::UpdateTimeUI()
{
	float fCurTime = m_pTime->GetTime();	// ����
	if (UtilFunc::Calculation::IsNearlyTarget(fCurTime, 0.0f, 0.1f))
	{ // ���Ԃ������̏ꍇ

		// ���Ԃ̎����`���OFF�ɂ���
		m_pTime->SetEnableDisp(false);

		// ���ԁ��̎����`���ON�ɂ���
		m_pTimeInf->SetEnableDisp(true);
	}
	else
	{ // ���Ԃ��L���̏ꍇ

		// ���Ԃ̎����`���ON�ɂ���
		m_pTime->SetEnableDisp(true);

		// ���ԁ��̎����`���OFF�ɂ���
		m_pTimeInf->SetEnableDisp(false);
	}
}

//============================================================
//	�I�𑀍쏈��
//============================================================
void CEntryRuleManager::Select()
{
	CObject *apSelect[MAX_SELECT] =	// �I���I�u�W�F�N�g
	{
		GetTimeUI(),	// ����
		m_pLife,		// �̗�
		m_pStart		// �J�n�{�^��
	};
	CInputKeyboard* pKey = GET_INPUTKEY;	// �L�[�{�[�h
	CInputGamepad* pPad = GET_INPUTPAD;		// �p�b�h
	MyLib::Vector3 posSelect = m_pSelect->GetPosition();	// �I���ʒu
	MyLib::Vector2 sizeSelect = m_pSelect->GetSize();		// �I��傫��

	if (pKey->GetTrigger(DIK_W)
	||  pKey->GetTrigger(DIK_UP)
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_UP).bInput
	||  pPad->GetAllLStickTrigger(CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_UP))
	{ // ��ړ��̑��삪�s��ꂽ�ꍇ

		// ��ɑI�������炷
		m_nSelect = (m_nSelect + (MAX_SELECT - 1)) % MAX_SELECT;

		// �T�E���h�̍Đ�
		PLAY_SOUND(CSound::ELabel::LABEL_SE_CURSOR);

		if (m_nSelect != SELECT_GAME)
		{ // �I�����Q�[���J�ڂł͂Ȃ��ꍇ

			// �ʒu��ύX
			posSelect.x = select::POS.x;
			posSelect.y = select::POS.y + rule::SPACE.y * (float)m_nSelect;

			// �傫����ύX
			sizeSelect = select::SIZE_RULE;
			sizeSelect.x *= 0.5f;
			for (int i = 0; i < MAX_RULE_ARROW; i++)
			{ // ���̑������J��Ԃ�

				MyAssert::TrueAssert(i < 0 || i >= sizeof(m_apArrow), "�z��I�[�o�[");
				
				// ���̈ʒu��ύX
				MyLib::Vector3 setpos = posSelect;
				m_apArrow[i]->SetPosition(posSelect);
				m_apArrow[i]->SetOriginPosition(posSelect);

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
			sizeSelect.x *= 0.75f;
			for (int i = 0; i < MAX_RULE_ARROW; i++)
			{ // ���̑������J��Ԃ�

				MyAssert::TrueAssert(i < 0 || i >= sizeof(m_apArrow), "�z��I�[�o�[");
			
				// �����`���OFF�ɂ���
				m_apArrow[i]->SetEnableDisp(false);
			}
		}
	}

	if (pKey->GetTrigger(DIK_S)
	||  pKey->GetTrigger(DIK_DOWN)
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_DOWN).bInput
	||  pPad->GetAllLStickTrigger(CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_DOWN))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// �T�E���h�̍Đ�
		PLAY_SOUND(CSound::ELabel::LABEL_SE_CURSOR);

		// ���ɑI�������炷
		m_nSelect = (m_nSelect + 1) % MAX_SELECT;

		if (m_nSelect != SELECT_GAME)
		{ // �I�����Q�[���J�ڂł͂Ȃ��ꍇ

			// �ʒu��ύX
			posSelect.x = select::POS.x;
			posSelect.y = select::POS.y + rule::SPACE.y * (float)m_nSelect;

			// �傫����ύX
			sizeSelect = select::SIZE_RULE;
			sizeSelect.x *= 0.5f;
			for (int i = 0; i < MAX_RULE_ARROW; i++)
			{ // ���̑������J��Ԃ�

				MyAssert::TrueAssert(i < 0 || i >= sizeof(m_apArrow), "�z��I�[�o�[");
			
				// ���̈ʒu��ύX
				MyLib::Vector3 setpos = posSelect;
				m_apArrow[i]->SetPosition(posSelect);
				m_apArrow[i]->SetOriginPosition(posSelect);

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
			sizeSelect.x *= 0.75f;
			for (int i = 0; i < MAX_RULE_ARROW; i++)
			{ // ���̑������J��Ԃ�

				MyAssert::TrueAssert(i < 0 || i >= sizeof(m_apArrow), "�z��I�[�o�[");
			
				// �����`���OFF�ɂ���
				m_apArrow[i]->SetEnableDisp(false);
			}
		}
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
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_A).bInput)
	{ // ����̑��삪�s��ꂽ�ꍇ

		switch (m_nSelect)
		{ // �I�����Ƃ̏���
		case RULE_TIME:
		case RULE_LIFE:
			break;

		case SELECT_GAME:

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
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_B).bInput)
	{ // �߂鑀�삪�s��ꂽ�ꍇ

		// �t�F�[�h�A�E�g��Ԃɂ���
		m_state = STATE_FADEOUT;

		// �T�E���h�̍Đ�
		PLAY_SOUND(CSound::ELabel::LABEL_SE_CANCEL);
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
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_LEFT).bInput
	||  pPad->GetAllLStickTrigger(CInputGamepad::STICK_CROSS_LEFT))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		MyAssert::TrueAssert(m_apArrow[CArrowUI::EDirection::DIRECTION_L] == nullptr, "�z��I�[�o�[");

		// �I�����ړ���Ԃ֑J��
		m_apArrow[CArrowUI::EDirection::DIRECTION_L]->SetState(CArrowUI::EState::STATE_SELECTMOVE);

		switch (m_nSelect)
		{ // �I�����Ƃ̏���
		case RULE_TIME:
		{
			// ���Ԃ̌��Z
			AddTime(-time::MOVE_TIME);
			break;
		}
		case RULE_LIFE:
		{
			// ���ɑI�������炷
			nLife = (nLife + (CEntryRuleManager::LIFE_MAX - 1)) % CEntryRuleManager::LIFE_MAX;

			// �T�E���h�̍Đ�
			PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW);
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
	||  pPad->GetAllTrigger(CInputGamepad::BUTTON_RIGHT).bInput
	||  pPad->GetAllLStickTrigger(CInputGamepad::STICK_CROSS_RIGHT))
	{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

		MyAssert::TrueAssert(m_apArrow[CArrowUI::EDirection::DIRECTION_R] == nullptr, "�z��I�[�o�[");

		// �I�����ړ���Ԃ֑J��
		m_apArrow[CArrowUI::EDirection::DIRECTION_R]->SetState(CArrowUI::EState::STATE_SELECTMOVE);

		switch (m_nSelect)
		{ // �I�����Ƃ̏���
		case RULE_TIME:
		{
			// ���Ԃ̉��Z
			AddTime(time::MOVE_TIME);
			break;
		}
		case RULE_LIFE:
		{
			// �E�ɑI�������炷
			nLife = (nLife + 1) % CEntryRuleManager::LIFE_MAX;

			// �T�E���h�̍Đ�
			PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW);
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
//	���ԕ\��UI�̎擾����
//============================================================
CObject* CEntryRuleManager::GetTimeUI()
{
	float fCurTime = m_pTime->GetTime();	// ����
	if (UtilFunc::Calculation::IsNearlyTarget(fCurTime, 0.0f, 0.1f))
	{ // ���Ԃ������̏ꍇ

		return m_pTimeInf;
	}
	else
	{ // ���Ԃ��L���̏ꍇ

		return m_pTime;
	}
}

//============================================================
//	���ԉ��Z����
//============================================================
void CEntryRuleManager::AddTime(const float fAddTime)
{
	float fCurTime = m_pTime->GetTime();	// ����
	if (UtilFunc::Calculation::IsNearlyTarget(fCurTime, 0.0f, 0.1f))
	{ // ���Ԃ������̏ꍇ

		if (fAddTime < 0.0f)
		{ // ���Ԃ����炷�ꍇ

			// �ő厞�Ԃɕ␳
			fCurTime = time::MAX_TIME;

			// �T�E���h�̍Đ�
			PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW);
		}
		else
		{ // ���Ԃ𑝂₷�ꍇ

			// �������s�T�E���h�̍Đ�
			PLAY_SOUND(CSound::LABEL_SE_BOUND_HIGH);
		}
	}
	else
	{ // ���Ԃ��L���̏ꍇ

		// ���Ԃ����Z
		fCurTime += fAddTime;

		// ���Ԃ�␳
		if (fCurTime > time::MAX_TIME)
		{ // �ő厞�Ԃ��������ꍇ

			// �������Ԃɂ���
			fCurTime = 0.0f;

			// ���������T�E���h�̍Đ�
			PLAY_SOUND(CSound::LABEL_SE_ARROW);
		}
		else if (fCurTime < time::MIN_TIME)
		{ // �ŏ����Ԃ���������ꍇ

			// �ŏ����Ԃɕ␳
			fCurTime = time::MIN_TIME;

			// �������s�T�E���h�̍Đ�
			PLAY_SOUND(CSound::LABEL_SE_BOUND_HIGH);
		}
		else
		{ // �␳���Ȃ��ꍇ

			// �T�E���h�̍Đ�
			PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW);
		}
	}

	// ���Ԃ����Z
	m_pTime->SetTime(fCurTime);

	// ���ԕ\���̍X�V
	UpdateTimeUI();
}

//============================================================
//	UI�̕`��󋵂̐ݒ菈��
//============================================================
void CEntryRuleManager::SetEnableDispUI(const bool bDisp)
{
	for (int i = 0; i < RULE_MAX; i++)
	{ // ���[���̑������J��Ԃ�

		MyAssert::TrueAssert(i < 0 || i >= sizeof(m_apRuleTitle), "�z��I�[�o�[");

		// ���[���^�C�g���̕`���ݒ�
		m_apRuleTitle[i]->SetEnableDisp(bDisp);
	}

	for (int i = 0; i < MAX_RULE_ARROW; i++)
	{ // ���̑������J��Ԃ�

		MyAssert::TrueAssert(i < 0 || i >= sizeof(m_apArrow), "�z��I�[�o�[");
	
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
	GetTimeUI()->SetEnableDisp(bDisp);
}

//============================================================
//	UI�̓����x�̐ݒ菈��
//============================================================
void CEntryRuleManager::SetAlphaUI(const float fAlpha, const bool bFadeOut)
{
	for (int i = 0; i < RULE_MAX; i++)
	{ // ���[���̑������J��Ԃ�

		MyAssert::TrueAssert(i < 0 || i >= sizeof(m_apRuleTitle), "�z��I�[�o�[");

		// ���[���^�C�g���̓����x��ݒ�
		m_apRuleTitle[i]->SetAlpha(fAlpha);
	}

	// �J�n�{�^���̓����x��ݒ�
	m_pStart->SetAlpha(fAlpha);

	// �̗͂̓����x��ݒ�
	m_pLife->SetAlpha(fAlpha);

	// ���Ԃ̓����x��ݒ�
	m_pTime->SetAlpha(fAlpha);
	m_pTimeInf->SetAlpha(fAlpha);

	if (bFadeOut)
	{ // �t�F�[�h�A�E�g���̏ꍇ

		for (int i = 0; i < MAX_RULE_ARROW; i++)
		{ // ���̑������J��Ԃ�

			MyAssert::TrueAssert(i < 0 || i >= sizeof(m_apArrow), "�z��I�[�o�[");
	
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

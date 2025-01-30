//============================================================
//
//	�J��UI���� [transUI.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "transUI.h"
#include "manager.h"
#include "object2D.h"
#include "entry.h"
#include "entryscene.h"
#include "entry_setupTeam.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY	= 6;		// UI�̗D�揇��
	const float	MOVE_TIME	= 2.0f;		// �ړ�����
	const float	MIN_ALPHA	= 0.35f;	// �ŏ������x

	namespace gameset
	{
		const char* TEXTURE = "data\\TEXTURE\\entry\\PressStart_BG.png";	// ����\�L�̔w�i
		const MyLib::Vector3 INIT_POS = MyLib::Vector3(-VEC3_SCREEN_SIZE.x, 360.0f, 0.0f);		// �����ʒu
		const MyLib::Vector3 DEST_POS = MyLib::Vector3(VEC3_SCREEN_CENT.x, INIT_POS.y, 0.0f);	// �ڕW�ʒu
		const float MOVE_TIME = 0.35f;	// �ړ�����
		const float WIDTH = 450.0f;		// ����
	}

	namespace string
	{
		const char* TEXTURE	= "data\\TEXTURE\\entry\\PressStart.png";	// ����\�L�e�N�X�`��
		const float HEIGHT	= 150.0f;	// �c��
		const float INIT_ALPHA	= 0.0f;	// ���������x
		const float DEST_ALPHA	= 1.0f;	// �ڕW�����x
		const float MOVE_TIME	= 0.4f;	// �ړ�����
		const float WAIT_TIME	= 0.4f;	// �ҋ@����
		const MyLib::Vector2 INIT_SIZE = MyLib::Vector2(0.0f, 0.0f);	// �����傫��
		const MyLib::Vector3 POS = MyLib::Vector3(610.0f, 450.0f, 0.0f);	// �ʒu
	}
}

//************************************************************
//	�֐��|�C���^
//************************************************************
// ��Ԋ֐�
CTransUI::STATE_FUNC CTransUI::m_StateFuncList[] =
{
	&CTransUI::UpdateDispOFF,		// �\��OFF�̍X�V
	&CTransUI::UpdateSpawnBG,		// �w�i�����̍X�V
	&CTransUI::UpdateSpawnString,	// ���������̍X�V
	&CTransUI::UpdateDispON,		// �\��ON�̍X�V
};

//************************************************************
//	�q�N���X [CTransUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTransUI::CTransUI() : CObject(PRIORITY, LAYER::LAYER_2D),
	m_pBG		(nullptr),			// �w�i���
	m_pString	(nullptr),			// �������
	m_state		(STATE_DISP_OFF),	// ���
	m_fCurTime	(0.0f)				// �o�ߎ���
{
	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_StateFuncList) == CTransUI::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTransUI::~CTransUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTransUI::Init()
{
	// UI�̐���
	if (FAILED(CreateUI()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ��I�u�W�F�N�g2D�ɂ���
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTransUI::Uninit()
{
	// ���ɔj���ς݂̏ꍇ������
	if (IsDeath()) { return; }

	// �w�i�̏I��
	SAFE_UNINIT(m_pBG);

	// �����̏I��
	SAFE_UNINIT(m_pString);

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�폜����
//============================================================
void CTransUI::Kill()
{
	// ���g�̏I��
	CTransUI::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CTransUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_StateFuncList[m_state] != nullptr)
	{ // ��ԍX�V�֐�������ꍇ

		// ��ԕʏ���
		(this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//============================================================
//	�`�揈��
//============================================================
void CTransUI::Draw()
{

}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CTransUI::SetEnableDisp(const bool bDisp)
{
	// ���g�̕`��󋵂�ݒ�
	CObject::SetEnableDisp(bDisp);

	// �w�i�̕`��󋵂�ݒ�
	m_pBG->SetEnableDisp(bDisp);

	// �����̕`��󋵂�ݒ�
	m_pString->SetEnableDisp(bDisp);
}

//============================================================
//	��������
//============================================================
CTransUI *CTransUI::Create()
{
	// �C���X�^���X�̐���
	CTransUI* pTrans = DEBUG_NEW CTransUI;
	if (pTrans != nullptr)
	{ // �����ɐ��������ꍇ

		// �N���X�̏�����
		if (FAILED(pTrans->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pTrans);
			return nullptr;
		}
	}

	// �C���X�^���X��Ԃ�
	return pTrans;
}

//============================================================
//	���葀��̍X�V����
//============================================================
bool CTransUI::UpdateDecide(const bool bAutoUninit)
{
	CInputKeyboard*	pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CInputGamepad*	pPad = GET_INPUTPAD;	// �p�b�h���
	bool bInput = false;	// ���̓t���O

	// �Z�b�g�A�b�v�V�[���̎擾
	CEntry* pEntry = CEntry::GetInstance();							// �G���g���[���[�h���
	if (pEntry == nullptr) { assert(false); return false; }			// �G���g���[���[�h���Ȃ��ꍇ������
	CEntryScene* pEntryScene = pEntry->GetEntryScene();				// �G���g���[�V�[�����
	if (pEntryScene == nullptr) { assert(false); return false; }	// �G���g���[�V�[�����Ȃ��ꍇ������
	CEntry_SetUpTeam* pSetupTeam = pEntryScene->GetSetupTeam();		// �Z�b�g�A�b�v�V�[�����
	if (pSetupTeam == nullptr) { assert(false); return false; }		// �Z�b�g�A�b�v�V�[�����Ȃ��ꍇ������

	// �G���g���[���v���C���[�C���f�b�N�X�擾
	std::vector<int> vecIdx = pSetupTeam->GetEntryPlayerIdx();
	for (const auto& rIdx : vecIdx)
	{
		// �p�b�h���͌��ʂ�ۑ�
		bInput = (bInput || pPad->GetTrigger(CInputGamepad::BUTTON_A, rIdx));
	}
	
#ifdef _DEBUG
	// �L�[���͌��ʂ�ۑ�
	bInput = (bInput || pKey->GetTrigger(DIK_RETURN) || pKey->GetTrigger(DIK_SPACE));
#endif

	// �L�[���͂��Ȃ��ꍇ������
	if (!bInput) { return false; }

	// �\������Ă��Ȃ��ꍇ������
	if (m_state == STATE_DISP_OFF) { return false; }

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // �p�b�h�����J��Ԃ�

		// �g���K�[���͏�����
		pPad->InitTrigger(i);
	}

	if (bAutoUninit)
	{ // �����j�����L���̏ꍇ

		// ���g�̔j��
		Uninit();
	}

	return true;
}

//============================================================
//	UI�\���󋵂̐ݒ菈��
//============================================================
void CTransUI::SetEnableDispUI(const bool bDisp)
{
	if (bDisp)
	{ // �\�����J�n����ꍇ

		if (!IsDispTransState()
		&&  m_state != STATE_DISP_ON)
		{ // �\����Ԃ���Ȃ��ꍇ

			// UI�\���󋵂̐ݒ�
			SetDisp(false);

			// �o�ߎ��Ԃ�������
			m_fCurTime = 0.0f;

			// �\���J�n��Ԃɂ���
			m_state = STATE_SPAWN_BG;

			PLAY_SOUND(CSound::ELabel::LABEL_SE_NEXT);
		}
	}
	else
	{ // �\�����I������ꍇ

		// UI�\���󋵂̐ݒ�
		SetDisp(false);

		// �o�ߎ��Ԃ�������
		m_fCurTime = 0.0f;

		// �\���I����Ԃɂ���
		m_state = STATE_DISP_OFF;
	}
}

//============================================================
//	�\��OFF�̍X�V����
//============================================================
void CTransUI::UpdateDispOFF(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//============================================================
//	�w�i�����̍X�V����
//============================================================
void CTransUI::UpdateSpawnBG(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �o�ߎ��Ԃ����Z
	m_fCurTime += fDeltaTime * fSlowRate;

	// �w�i�̈ʒu���ړ�
	MyLib::Vector3 posBG = UtilFunc::Correction::EaseOutBack(gameset::INIT_POS, gameset::DEST_POS, 0.0f, gameset::MOVE_TIME, m_fCurTime);
	m_pBG->SetPosition(posBG);

	if (m_fCurTime >= gameset::MOVE_TIME)
	{ // �o�߂��������ꍇ

		// �o�ߎ��Ԃ�������
		m_fCurTime = 0.0f;

		// ����������Ԃɂ���
		m_state = STATE_SPAWN_STRING;
	}
}

//============================================================
//	���������̍X�V����
//============================================================
void CTransUI::UpdateSpawnString(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �o�ߎ��Ԃ����Z
	m_fCurTime += fDeltaTime * fSlowRate;

	// �c�������ɖڕW�傫�����v�Z
	CTexture* pTexture = CTexture::GetInstance();	// �e�N�X�`�����
	MyLib::Vector2 sizeDest = pTexture->GetImageSize(pTexture->Regist(string::TEXTURE));	// �摜�T�C�Y���擾
	sizeDest = UtilFunc::Transformation::AdjustSizeByHeight(sizeDest, string::HEIGHT);		// �摜�T�C�Y���c���w��Œ���

	// �������g��
	MyLib::Vector2 sizeStr = UtilFunc::Correction::EaseOutBack(string::INIT_SIZE, sizeDest, 0.0f, string::MOVE_TIME, m_fCurTime);
	m_pString->SetSize(sizeStr);

	// ������s�����ɂ���
	float fAlphaStr = UtilFunc::Correction::EasingCubicIn(string::INIT_ALPHA, string::DEST_ALPHA, 0.0f, string::MOVE_TIME, m_fCurTime);
	m_pString->SetAlpha(fAlphaStr);

	if (m_fCurTime >= string::MOVE_TIME + string::WAIT_TIME)
	{ // �o�߂��������ꍇ

		// �o�ߎ��Ԃ�������
		m_fCurTime = 0.0f;

		// �\��ON��Ԃɂ���
		m_state = STATE_DISP_ON;
	}
}

//============================================================
//	�\��ON�̍X�V����
//============================================================
void CTransUI::UpdateDispON(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	float fAlphaStr = 0.0f;	// ���������x

	// �o�ߎ��Ԃ����Z
	m_fCurTime += fDeltaTime * fSlowRate;
	if (m_fCurTime < MOVE_TIME * 0.5f)
	{ // �o�ߎ��Ԃ����������̏ꍇ

		// �����𓧖��x���v�Z
		fAlphaStr = UtilFunc::Correction::EasingEaseIn(string::DEST_ALPHA, MIN_ALPHA, 0.0f, MOVE_TIME * 0.5f, m_fCurTime);
	}
	else
	{ // �o�ߎ��Ԃ������ȏ�̏ꍇ

		// �����𓧖��x���v�Z
		fAlphaStr = UtilFunc::Correction::EasingEaseOut(MIN_ALPHA, string::DEST_ALPHA, MOVE_TIME * 0.5f, MOVE_TIME, m_fCurTime);
	}

	if (m_fCurTime > MOVE_TIME)
	{ // �o�߂��������ꍇ

		// �o�ߎ��ԕ����Z
		m_fCurTime -= MOVE_TIME;
	}

	// �����̓����x��ݒ�
	m_pString->SetAlpha(fAlphaStr);
}

//============================================================
//	�\���󋵂̐ݒ菈��
//============================================================
void CTransUI::SetDisp(const bool bDisp)
{
	if (bDisp)
	{ // �\������ꍇ

		// �c�������ɕ����̖ڕW�傫�����v�Z
		CTexture* pTexture = CTexture::GetInstance();	// �e�N�X�`�����
		MyLib::Vector2 sizeDest = pTexture->GetImageSize(pTexture->Regist(string::TEXTURE));	// �摜�T�C�Y���擾
		sizeDest = UtilFunc::Transformation::AdjustSizeByHeight(sizeDest, string::HEIGHT);		// �摜�T�C�Y���c���w��Œ���

		// �w�i�̈ʒu�̐ݒ�
		m_pBG->SetPosition(gameset::DEST_POS);

		// �����̑傫���̐ݒ�
		m_pString->SetSize(sizeDest);

		// �����̓����x�̐ݒ�
		m_pString->SetAlpha(string::DEST_ALPHA);
	}
	else
	{ // �\�����Ȃ��ꍇ

		// �w�i�̈ʒu�̐ݒ�
		m_pBG->SetPosition(gameset::INIT_POS);

		// �����̑傫���̐ݒ�
		m_pString->SetSize(string::INIT_SIZE);

		// �����̓����x�̐ݒ�
		m_pString->SetAlpha(string::INIT_ALPHA);
	}
}

//============================================================
//	�\���J�ڏ�Ԃ��̊m�F����
//============================================================
bool CTransUI::IsDispTransState()
{
	// �\����Ԃ��̊m�F
	return (m_state == STATE_SPAWN_BG || m_state == STATE_SPAWN_STRING);
}

//============================================================
//	UI�̐�������
//============================================================
HRESULT CTransUI::CreateUI()
{
	CTexture* pTexture = CTexture::GetInstance();	// �e�N�X�`�����

	// �w�i�̐���
	m_pBG = CObject2D::Create(PRIORITY);
	if (m_pBG == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �e�N�X�`���̊���
	int texID = pTexture->Regist(gameset::TEXTURE);
	m_pBG->BindTexture(texID);

	// ���������ɃT�C�Y�v�Z
	MyLib::Vector2 size = pTexture->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, gameset::WIDTH);
	m_pBG->SetSize(size);

	// �����̐���
	m_pString = CObject2D::Create(PRIORITY);
	if (m_pString == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �e�N�X�`���̊���
	m_pString->BindTexture(pTexture->Regist(string::TEXTURE));

	// �ʒu�̐ݒ�
	m_pString->SetPosition(string::POS);

	// UI�\����������
	SetDisp(false);

	return S_OK;
}

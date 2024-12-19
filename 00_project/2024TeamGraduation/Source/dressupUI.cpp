//============================================================
//
//	�h���X�A�b�vUI���� [dressupUI.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "dressupUI.h"
#include "dressup.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "entry_setupTeam.h"
#include "object2D.h"
#include "object2D_Anim.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// �h���X�A�b�vUI�̗D�揇��
	const D3DXCOLOR COL_CHOICE	= MyLib::color::White();				// �I�𒆐F
	const D3DXCOLOR COL_DEFAULT	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// ��I�𒆐F

	namespace change
	{
		const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(CDressupUI::EChangeType::TYPE_MAX, 1);	// �e�N�X�`��������
		const std::string TEXTURE = "data\\TEXTURE\\entry\\ChangeType000.png";	// �ύX��ރA�C�R���e�N�X�`��
		const float WIDTH = 60.0f;	// ����
	}

	namespace frame
	{
		namespace player
		{
			const std::string TEXTURE = "data\\TEXTURE\\entry\\PlayerFrame000.png";	// �ύX��ރA�C�R���e�N�X�`��
			const float WIDTH = 100.0f;	// ����
		}
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
int CDressupUI::m_nNumAI = 0;	// AI����

//************************************************************
//	�q�N���X [CDressupUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CDressupUI::CDressupUI(const int nPlayerIdx) : CObject(PRIORITY, LAYER::LAYER_2D),
	m_pChangeIcon	(nullptr),		// �ύX��ރA�C�R�����
	m_nPlayerIdx	(nPlayerIdx),	// �v���C���[�C���f�b�N�X
	m_nOrdinalAI	(-1),			// ���g���������ꂽ���� (AI�̂�)
	m_pPlayer		(nullptr),		// �v���C���[
	m_pHair			(nullptr),		// �������ւ�
	m_pAccessory	(nullptr),		// �A�N�Z�����ւ�
	m_pFace			(nullptr),		// �璅���ւ�
	m_typeEdit		(EEditType::EDIT_PROCESS),	// �G�f�B�b�g������
	m_typeChange	(EChangeType::TYPE_HAIR)	// �ύX������
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CDressupUI::~CDressupUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CDressupUI::Init()
{
	// �Z�b�g�A�b�v�̐���
	if (FAILED(CreateSetup()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// UI�̐���
	if (FAILED(CreateUI()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CDressupUI::Uninit()
{
	// �`�[���Z�b�g�A�b�v���̎擾
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	assert(pSetupTeam != nullptr);

	// �j�������v���C���[��AI�̏ꍇ��AI���������Z
	if (pSetupTeam->GetEntryIdx(m_nPlayerIdx) <= -1) { m_nNumAI--; }

	// �������ւ��̏I��
	SAFE_UNINIT(m_pHair);

	// �A�N�Z�؂�ւ��̏I��
	SAFE_UNINIT(m_pAccessory);

	// ��؂�ւ��̏I��
	SAFE_UNINIT(m_pFace);

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�폜����
//============================================================
void CDressupUI::Kill()
{
	// ���g�̏I��
	CDressupUI::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CDressupUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �`�[���Z�b�g�A�b�v���̎擾
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) { return; }

	// �G���g���[�C���f�b�N�X���擾
	const int nEntryIdx = pSetupTeam->GetEntryIdx(m_nPlayerIdx);
	if (nEntryIdx < 0) { return; }
	if (nEntryIdx >= mylib_const::MAX_PLAYER) { return; }

	// �G�f�B�b�g�����ޕύX
	ChangeEditType(nEntryIdx);

	switch (m_typeEdit)
	{
	case EEditType::EDIT_PROCESS:

		switch (m_typeChange)
		{
		case EChangeType::TYPE_HAIR:

			// ���X�V
			m_pHair->Update(fDeltaTime, fDeltaRate, fSlowRate);
			break;

		case EChangeType::TYPE_ACCESSORY:

			// �A�N�Z�X�V
			m_pAccessory->Update(fDeltaTime, fDeltaRate, fSlowRate);
			break;

		case EChangeType::TYPE_FACE:

			// ��X�V
			m_pFace->Update(fDeltaTime, fDeltaRate, fSlowRate);
			break;

		case EChangeType::TYPE_BODY:

			// �̌^�ύX
			ChangeBodyType(nEntryIdx);
			break;

		case EChangeType::TYPE_HANDEDNESS:

			// ������ύX
			ChangeHandedness(nEntryIdx);
			break;

		default:
			assert(false);
			break;
		}
		break;

	case EEditType::EDIT_CHANGETYPE:

		// �ύX����ӏ��̎�ޕύX
		ChangeChangeType(nEntryIdx);
		break;

	default:
		assert(false);
		break;
	}

	// UI�̍X�V
	UpdateUI();

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�`�揈��
//============================================================
void CDressupUI::Draw()
{

}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CDressupUI::SetType(const TYPE type)
{
	// ���g�̎�ނ�ݒ�
	CObject::SetType(type);
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CDressupUI::SetEnableDisp(const bool bDraw)
{
	// ���g�̕`��󋵂�ݒ�
	CObject::SetEnableDisp(bDraw);
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CDressupUI::SetPosition(const MyLib::Vector3& pos)
{
	// �ݒ肷��ʒu��ۑ�
	CObject::SetPosition(pos);

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	��������
//============================================================
CDressupUI *CDressupUI::Create
(
	const int nPlayerIdx,		// �v���C���[�C���f�b�N�X
	const MyLib::Vector3 &rPos	// ���_�ʒu
)
{
	// �h���X�A�b�vUI�̐���
	CDressupUI* pDressupUI = DEBUG_NEW CDressupUI(nPlayerIdx);
	if (pDressupUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �h���X�A�b�vUI�̏�����
		if (FAILED(pDressupUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �h���X�A�b�vUI�̔j��
			SAFE_DELETE(pDressupUI);
			return nullptr;
		}

		// ���_�ʒu��ݒ�
		pDressupUI->SetPosition(rPos);

		// �m�ۂ����A�h���X��Ԃ�
		return pDressupUI;
	}
}

//============================================================
// ���쌠�C���f�b�N�X�̎擾����
//============================================================
int CDressupUI::GetMyPlayerIdx() const
{
	// ���쌠�C���f�b�N�X��Ԃ�
	return m_pPlayer->GetMyPlayerIdx();
}

//============================================================
// �`�[���T�C�h�̎擾����
//============================================================
CGameManager::ETeamSide CDressupUI::GetTeam() const
{
	// �`�[���T�C�h��Ԃ�
	return m_pPlayer->GetTeam();
}

//============================================================
// ���̃C���f�b�N�X�ԍ��̎擾����
//============================================================
int CDressupUI::GetHairNowIdx() const
{
	// ���̃C���f�b�N�X�ԍ���Ԃ�
	return m_pHair->GetNowIdx();
}

//============================================================
// �A�N�Z�̃C���f�b�N�X�ԍ��̎擾����
//============================================================
int CDressupUI::GetAccessoryNowIdx() const
{
	// �A�N�Z�̃C���f�b�N�X�ԍ���Ԃ�
	return m_pAccessory->GetNowIdx();
}

//============================================================
// ��̃C���f�b�N�X�ԍ��̎擾����
//============================================================
int CDressupUI::GetFaceNowIdx() const
{
	// ��̃C���f�b�N�X�ԍ���Ԃ�
	return m_pFace->GetNowIdx();
}

//============================================================
// �̌^�̎擾����
//============================================================
CPlayer::EBody CDressupUI::GetBodyType() const
{
	// �̌^��Ԃ�
	return m_pPlayer->GetBodyType();
}

//============================================================
// ������̎擾����
//============================================================
CPlayer::EHandedness CDressupUI::GetHandedness() const
{
	// �������Ԃ�
	return m_pPlayer->GetHandedness();
}

//============================================================
// UI�̐�������
//============================================================
HRESULT CDressupUI::CreateUI()
{
	// �ύX��ރA�C�R���̐���
	if (FAILED(CreateChangeIcon()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �v���C���[�t���[���̐���
	if (FAILED(CreatePlayerFrame()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();

	return S_OK;
}

//============================================================
// �ύX��ރA�C�R���̐�������
//============================================================
HRESULT CDressupUI::CreateChangeIcon()
{
	// �ύX��ރA�C�R���̐���
	m_pChangeIcon = CObject2D_Anim::Create
	( // ����
		VEC3_ZERO,		// �ʒu
		change::PTRN.x,	// �e�N�X�`����������
		change::PTRN.y,	// �e�N�X�`���c������
		0.0f,			// �Đ�����
		false,			// �����j��
		PRIORITY		// �D�揇��
	);
	if (m_pChangeIcon == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �����Đ���OFF�ɂ���
	m_pChangeIcon->SetEnableAutoPlay(false);

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(change::TEXTURE);
	m_pChangeIcon->BindTexture(nTexID);

	// ���������ɃT�C�Y��ݒ�
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, change::WIDTH * (float)change::PTRN.x);
	size.x /= (float)change::PTRN.x;
	m_pChangeIcon->SetSize(size);
	m_pChangeIcon->SetSizeOrigin(m_pChangeIcon->GetSize());

	return S_OK;
}

//============================================================
// �v���C���[�t���[���̐�������
//============================================================
HRESULT CDressupUI::CreatePlayerFrame()
{
	// �v���C���[�t���[���̐���
	m_pPlayerFrame = CObject2D::Create(PRIORITY);
	if (m_pPlayerFrame == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(frame::player::TEXTURE);
	m_pPlayerFrame->BindTexture(nTexID);

	// ���������ɃT�C�Y��ݒ�
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, frame::player::WIDTH);
	m_pPlayerFrame->SetSize(size);
	m_pPlayerFrame->SetSizeOrigin(m_pPlayerFrame->GetSize());

	return S_OK;
}

//============================================================
// �Z�b�g�A�b�v�̐�������
//============================================================
HRESULT CDressupUI::CreateSetup()
{
	// �`�[���Z�b�g�A�b�v���̎擾
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) { return E_FAIL; }

	// �G���g���[�C���f�b�N�X���擾
	const int nEntryIdx = pSetupTeam->GetEntryIdx(m_nPlayerIdx);
	CGameManager::ETeamSide side;
	if (nEntryIdx > -1)
	{ // ���[�U�[�̏ꍇ

		// �G���g���[�C���f�b�N�X����`�[���T�C�h���擾
		side = pSetupTeam->GetTeamSideUser(nEntryIdx);
	}
	else
	{ // AI�̏ꍇ

		// ���g��AI��������ۑ�
		m_nOrdinalAI = m_nNumAI;

		// AI����������`�[���T�C�h���擾
		side = pSetupTeam->GetTeamSideAI(m_nOrdinalAI);

		// AI�����������Z
		m_nNumAI++;
	}

	// �v���C���[����
	MyLib::Vector3 pos = MyLib::Vector3(-400.0f, 0.0f, 0.0f);
	MyLib::Vector3 offset = MyLib::Vector3(200.0f * m_nPlayerIdx, 0.0f, 0.0f);
	m_pPlayer = CPlayer::Create
	( // ����
		pos + offset, 					// �ʒu
		side,							// �`�[���T�C�h
		CPlayer::EHuman::HUMAN_ENTRY,	// �|�W�V����
		CPlayer::EBody::BODY_NORMAL,	// �̌n
		CPlayer::EHandedness::HAND_R	// ������
	);
	if (m_pPlayer == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ���̈ʒu�ݒ�
	m_pPlayer->CObject::SetOriginPosition(pos + offset);

	// �C���f�b�N�X�̏㏑��
	m_pPlayer->SetMyPlayerIdx(nEntryIdx);

	// �������ւ�����
	m_pHair = CDressup::Create
	( // ����
		CDressup::EType::TYPE_HAIR,	// �����ւ��̎��
		m_pPlayer,					// �ύX����v���C���[
		CPlayer::ID_HAIR			// �ύX�ӏ��̃C���f�b�N�X
	);
	if (m_pHair == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}
	
	// �A�N�Z�؂�ւ�����
	m_pAccessory = CDressup::Create
	( // ����
		CDressup::EType::TYPE_ACCESSORY,	// �����ւ��̎��
		m_pPlayer,							// �ύX����v���C���[
		CPlayer::ID_ACCESSORY				// �ύX�ӏ��̃C���f�b�N�X
	);
	if (m_pAccessory == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��؂�ւ�����
	m_pFace = CDressup::Create
	( // ����
		CDressup::EType::TYPE_FACE,	// �����ւ��̎��
		m_pPlayer,					// �ύX����v���C���[
		CPlayer::ID_FACE			// �ύX�ӏ��̃C���f�b�N�X
	);
	if (m_pFace == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ���삷��C���f�b�N�X�ݒ�
	m_pHair->SetControllIdx(nEntryIdx);
	m_pAccessory->SetControllIdx(nEntryIdx);
	m_pFace->SetControllIdx(nEntryIdx);

	return S_OK;
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CDressupUI::SetPositionRelative()
{
	// ���g�̈ʒu���擾
	MyLib::Vector3 posThis = GetPosition();

	// �ύX��ރA�C�R���̈ʒu�ݒ�
	m_pChangeIcon->SetPosition(posThis + MyLib::Vector3(0.0f, -220.0f, 0.0f));

	// �v���C���[�t���[���̈ʒu�ݒ�
	m_pPlayerFrame->SetPosition(posThis);
}

//============================================================
// UI�̍X�V����
//============================================================
void CDressupUI::UpdateUI()
{
	// �e�N�X�`�����̎擾
	CTexture* pTexture = CTexture::GetInstance();
	assert(pTexture != nullptr);

	// �ύX��ނ��A�C�R���ɔ��f
	m_pChangeIcon->SetPatternAnim(m_typeChange);

	switch (m_typeEdit)
	{ // �G�f�B�b�g��ނ��Ƃ̏���
	case EEditType::EDIT_PROCESS:
	{
		// �ύX��ރA�C�R���̐F���O���[�ɂ���
		m_pChangeIcon->SetColor(COL_DEFAULT);

		// �v���C���[�t���[���̐F�𔒂ɂ���
		m_pPlayerFrame->SetColor(COL_CHOICE);
		break;
	}
	case EEditType::EDIT_CHANGETYPE:
	{
		// �ύX��ރA�C�R���̐F�𔒂ɂ���
		m_pChangeIcon->SetColor(COL_CHOICE);

		// �v���C���[�t���[���̐F���O���[�ɂ���
		m_pPlayerFrame->SetColor(COL_DEFAULT);
		break;
	}
	default:
		assert(false);
		break;
	}
}

//============================================================
// �G�f�B�b�g�����ޕύX
//============================================================
void CDressupUI::ChangeEditType(int nPadIdx)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, nPadIdx))
	{// �ύX����ӏ��̎�ޕύX�֕ύX

		m_typeEdit = EEditType::EDIT_PROCESS;
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, nPadIdx))
	{// ���ۂ̕ύX��ʂ�

		m_typeEdit = EEditType::EDIT_CHANGETYPE;
	}
}

//============================================================
// �ύX����ӏ��̎�ޕύX
//============================================================
void CDressupUI::ChangeChangeType(int nPadIdx)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nPadIdx))
	{// ���[�v

		// ���֕ύX
		int changeType = (m_typeChange + 1) % EChangeType::TYPE_MAX;
		m_typeChange = static_cast<EChangeType>(changeType);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nPadIdx))
	{// �t���[�v

		// �O�֕ύX
		int changeType = (m_typeChange + (EChangeType::TYPE_MAX - 1)) % EChangeType::TYPE_MAX;
		m_typeChange = static_cast<EChangeType>(changeType);
	}
}

//============================================================
// �̌^�ύX
//============================================================
void CDressupUI::ChangeBodyType(int nPadIdx)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �̌^�擾
	CPlayer::EBody body = m_pPlayer->GetBodyType();
	CPlayer::EBody oldBody = body;

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nPadIdx))
	{// ���[�v

		// ���֕ύX
		int afterBody = (body + 1) % CPlayer::EBody::BODY_MAX;
		body = static_cast<CPlayer::EBody>(afterBody);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nPadIdx))
	{// �t���[�v

		// �O�֕ύX
		int deforeBody = (body + (CPlayer::EBody::BODY_MAX - 1)) % CPlayer::EBody::BODY_MAX;
		body = static_cast<CPlayer::EBody>(deforeBody);
	}

	if (body != oldBody)
	{// �ύX����Ă�����

		// �v���C���[�Đ���
		ReCreatePlayer(m_pPlayer->GetHandedness(), body);
	}
}

//============================================================
// ������ύX
//============================================================
void CDressupUI::ChangeHandedness(int nPadIdx)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// ������擾
	CPlayer::EHandedness handedness = m_pPlayer->GetHandedness();
	CPlayer::EHandedness oldHandedness = handedness;

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, nPadIdx))
	{// ���[�v

		// ���֕ύX
		int afterHandedness = (handedness + 1) % CPlayer::EHandedness::HAND_MAX;
		handedness = static_cast<CPlayer::EHandedness>(afterHandedness);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, nPadIdx))
	{// �t���[�v

		// �O�֕ύX
		int deforeHandedness = (handedness + (CPlayer::EHandedness::HAND_MAX - 1)) % CPlayer::EHandedness::HAND_MAX;
		handedness = static_cast<CPlayer::EHandedness>(deforeHandedness);
	}

	if (handedness != oldHandedness)
	{// �ύX����Ă�����

		// �v���C���[�Đ���
		ReCreatePlayer(handedness, m_pPlayer->GetBodyType());
	}
}

//============================================================
// �v���C���[�Đ���
//============================================================
HRESULT CDressupUI::ReCreatePlayer(CPlayer::EHandedness handedness, CPlayer::EBody body)
{
	// �`�[���Z�b�g�A�b�v���̎擾
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) { return E_FAIL; }

	// �v���C���[�ʒu�̕ۑ�
	MyLib::Vector3 posPlayer = m_pPlayer->CObject::GetOriginPosition();

	// �v���C���[�̍폜
	SAFE_KILL(m_pPlayer);

	// �G���g���[�C���f�b�N�X���擾
	int nEntryIdx = pSetupTeam->GetEntryIdx(m_nPlayerIdx);
	CGameManager::ETeamSide side;
	if (nEntryIdx > -1)
	{ // ���[�U�[�̏ꍇ

		// �G���g���[�C���f�b�N�X����`�[���T�C�h���擾
		side = pSetupTeam->GetTeamSideUser(nEntryIdx);
	}
	else
	{ // AI�̏ꍇ

		// AI����������`�[���T�C�h���擾
		side = pSetupTeam->GetTeamSideAI(m_nOrdinalAI);
	}

	// �Đ���
	m_pPlayer = CPlayer::Create
	( // ����
		posPlayer, 						// �ʒu
		side,							// �`�[���T�C�h
		CPlayer::EHuman::HUMAN_ENTRY,	// �|�W�V����
		body,							// �̌n
		handedness						// ������
	);
	if (m_pPlayer == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �C���f�b�N�X�̏㏑��
	m_pPlayer->SetMyPlayerIdx(nEntryIdx);

	// ���̈ʒu�ݒ�
	m_pPlayer->CObject::SetPosition(posPlayer);
	m_pPlayer->CObject::SetOriginPosition(posPlayer);

	// �h���X�A�b�v�ɍĊ���
	m_pHair->BindObjectCharacter(m_pPlayer);
	m_pAccessory->BindObjectCharacter(m_pPlayer);
	m_pFace->BindObjectCharacter(m_pPlayer);
	m_pHair->ReRegist();
	m_pAccessory->ReRegist();
	m_pFace->ReRegist();

	return S_OK;
}

//============================================================
//
//	�����ւ�UI���� [dressupUI.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "dressupUI.h"
#include "dressup.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "game.h"
#include "renderTexture.h"
#include "entry_setupTeam.h"
#include "entry_dressup.h"
#include "object2D.h"
#include "object2D_Anim.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 5;	// �����ւ�UI�̗D�揇��
	const D3DXCOLOR COL_CHOICE	= MyLib::color::White();				// �I�𒆐F
	const D3DXCOLOR COL_DEFAULT	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// ��I�𒆐F

	namespace player
	{
		namespace frame
		{
			const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(mylib_const::MAX_PLAYER + 1, 1);	// �e�N�X�`��������
			const std::string TEXTURE = "data\\TEXTURE\\entry\\PlayerFrame001.png";			// �v���C���[�t���[���A�C�R���e�N�X�`��
			const float WIDTH = 100.0f;	// ����
		}
		namespace ui
		{
			namespace body
			{
				D3DXVECTOR2 TEX_POS[] =		// �e�N�X�`�����W
				{
					D3DXVECTOR2(0.0f, 0.0f),	// ����
					D3DXVECTOR2(1.0f, 0.0f),	// �E��
					D3DXVECTOR2(0.0f, 1.0f),	// ����
					D3DXVECTOR2(1.0f, 1.0f),	// �E��
				};
				const float WIDTH = 350.0f;	// ����
			}
			namespace face
			{
				D3DXVECTOR2 TEX_POS[] =		// �e�N�X�`�����W
				{
					D3DXVECTOR2(0.3f, 0.0f),	// ����
					D3DXVECTOR2(0.7f, 0.0f),	// �E��
					D3DXVECTOR2(0.3f, 0.42f),	// ����
					D3DXVECTOR2(0.7f, 0.42f),	// �E��
				};
				const float WIDTH = 620.0f;	// ����
			}
		}
	}

	namespace change
	{
		const MyLib::PosGrid2	PTRN	= MyLib::PosGrid2(CDressupUI::EChangeType::TYPE_MAX, 1);	// �e�N�X�`��������
		const std::string		TEXTURE	= "data\\TEXTURE\\entry\\ChangeType000.png";	// �ύX��ރA�C�R���e�N�X�`��
		const MyLib::Vector3	OFFSET	= MyLib::Vector3(0.0f, 175.0f, 0.0f);			// �I�t�Z�b�g
		const float WIDTH = 100.0f;	// ����
	}

	namespace ready
	{
		const MyLib::PosGrid2	PTRN	= MyLib::PosGrid2(1, 2);						// �e�N�X�`��������
		const std::string		TEXTURE	= "data\\TEXTURE\\entry\\ReadyCheck000.png";	// ���������`�F�b�N�e�N�X�`��
		const MyLib::Vector3	OFFSET	= MyLib::Vector3(0.0f, 230.0f, 0.0f);			// �I�t�Z�b�g
		const float WIDTH = 100.0f;	// ����
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
CDressupUI::CDressupUI(CEntry_Dressup* pParent, const CPlayer::EFieldArea typeArea, const int nPlayerIdx) : CObject(PRIORITY, LAYER::LAYER_2D),
	m_pParent		(pParent),		// �e�N���X���
	m_pRenderScene	(nullptr),		// �V�[�������_�[�e�N�X�`��
	m_pChangeIcon	(nullptr),		// �ύX��ރA�C�R�����
	m_pReadyCheck	(nullptr),		// ���������`�F�b�N���
	m_pPlayerFrame	(nullptr),		// �v���C���[�t���[�����
	m_pPlayerUI		(nullptr),		// �v���C���[UI���
	m_nOrdinalAI	(-1),			// ���g���������ꂽ���� (AI�̂�)
	m_nPadIdx		(-1),			// ���쌠�C���f�b�N�X
	m_bReady		(false),		// ���������t���O
	m_pPlayer		(nullptr),		// �v���C���[
	m_pHair			(nullptr),		// �������ւ�
	m_pAccessory	(nullptr),		// �A�N�Z�����ւ�
	m_pFace			(nullptr),		// �璅���ւ�
	m_typeEdit		(EEditType::EDIT_PROCESS),	// �G�f�B�b�g������
	m_typeChange	(EChangeType::TYPE_BODY),	// �ύX������
	m_nPlayerIdx	(nPlayerIdx),				// �v���C���[�C���f�b�N�X
	m_typeArea		(typeArea)					// �v���C���[�|�W�V����
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

	// �����_�[�e�N�X�`���̐���
	if (FAILED(CreateRenderTexture()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

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
void CDressupUI::Uninit()
{
	// ���ɔj���ς݂̏ꍇ������
	if (IsDeath()) { return; }

	// �`�[���Z�b�g�A�b�v���̎擾
	assert(m_pParent != nullptr);
	CEntry_SetUpTeam* pSetupTeam = m_pParent->GetSetupTeam();
	if (pSetupTeam == nullptr)
	{ // �`�[���Z�b�g�A�b�v��񂪔j������Ă���ꍇ

		// AI�̑�����������
		m_nNumAI = 0;
	}
	else
	{ // �`�[���Z�b�g�A�b�v��񂪔j������Ă��Ȃ��ꍇ

		// �j�������v���C���[�����슎��AI�̏ꍇ��AI���������Z
		if (m_typeArea == CPlayer::EFieldArea::FIELD_IN
		&&  pSetupTeam->PlayerIdxToPadIdx(m_nPlayerIdx) <= -1) { m_nNumAI--; }
	}

	// �����_�[�e�N�X�`���̔j��
	SAFE_REF_RELEASE(m_pRenderScene);

	// �v���C���[�̏I��
	SAFE_UNINIT(m_pPlayer);

	// �ύX��ރA�C�R���̏I��
	SAFE_UNINIT(m_pChangeIcon);

	// ���������`�F�b�N�̏I��
	SAFE_UNINIT(m_pReadyCheck);

	// �v���C���[�t���[���̏I��
	SAFE_UNINIT(m_pPlayerFrame);

	// �v���C���[UI�̏I��
	SAFE_UNINIT(m_pPlayerUI);

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
	// �v���C���[�̍폜
	SAFE_KILL(m_pPlayer);

	// ���g�̏I��
	CDressupUI::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CDressupUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ����̍X�V
	UpdateControl(fDeltaTime, fDeltaRate, fSlowRate);

	// UI�̍X�V
	UpdateUI();

	// ���Έʒu�̐ݒ�
	SetPositionRelative();

	// �ύX��ރA�C�R���̍X�V
	m_pChangeIcon->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ���������`�F�b�N�̍X�V
	m_pReadyCheck->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �v���C���[�t���[���̍X�V
	m_pPlayerFrame->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �v���C���[UI�̍X�V
	m_pPlayerUI->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �v���C���[�̍X�V
	m_pPlayer->Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//============================================================
//	�`�揈��
//============================================================
void CDressupUI::Draw()
{
	// �ύX��ރA�C�R���̕`��
	m_pChangeIcon->Draw();

	// ���������`�F�b�N�̕`��
	m_pReadyCheck->Draw();

	// �v���C���[�t���[���̕`��
	m_pPlayerFrame->Draw();

	// �v���C���[UI�̕`��
	m_pPlayerUI->Draw();
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
	CEntry_Dressup* pParent,			// �e�N���X���
	const CPlayer::EFieldArea typeArea,	// �v���C���[�|�W�V����
	const int nPlayerIdx,				// �v���C���[�C���f�b�N�X
	const MyLib::Vector3& rPos			// ���_�ʒu
)
{
	// �����ւ�UI�̐���
	CDressupUI* pDressupUI = DEBUG_NEW CDressupUI(pParent, typeArea, nPlayerIdx);
	if (pDressupUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �����ւ�UI�̏�����
		if (FAILED(pDressupUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �����ւ�UI�̔j��
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
// ���������t���O�̐ݒ菈��
//============================================================
void CDressupUI::SetReady(const bool bReady)
{
	// ���������t���O�̐ݒ�
	m_bReady = bReady;

	// �e�N�X�`���p�^�[���̍X�V
	m_pReadyCheck->SetPatternAnim((int)m_bReady);
}

//============================================================
// ���쌠�C���f�b�N�X�̐ݒ菈��
//============================================================
void CDressupUI::SetPadIdx(const int nPadIdx)
{
	// ���쌠�C���f�b�N�X�̐ݒ�
	m_nPadIdx = nPadIdx;

	// �����ւ����̑��쌠�C���f�b�N�X�̐ݒ�
	m_pHair->SetControllIdx(m_nPadIdx);
	m_pAccessory->SetControllIdx(m_nPadIdx);
	m_pFace->SetControllIdx(m_nPadIdx);
}

//============================================================
// �v���C���[�C���f�b�N�X�̎擾����
//============================================================
int CDressupUI::GetMyPlayerIdx() const
{
	// �v���C���[�C���f�b�N�X��Ԃ�
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

	// ���������`�F�b�N�̐���
	if (FAILED(CreateReadyCheck()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �v���C���[�t���[���̐���
	if (FAILED(CreatePlayerFrame()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �v���C���[UI�̐���
	if (FAILED(CreatePlayerUI()))
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

	// ��ނ������j��/�X�V/�`�悵�Ȃ����̂ɂ���
	m_pChangeIcon->SetType(CObject::TYPE::TYPE_NONE);

	// �����Đ���OFF�ɂ���
	m_pChangeIcon->SetEnableAutoPlay(false);

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(change::TEXTURE);
	m_pChangeIcon->BindTexture(nTexID);

	// ���������ɃT�C�Y��ݒ�
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, change::WIDTH);
	size.y *= (float)change::PTRN.x;
	m_pChangeIcon->SetSize(size);
	m_pChangeIcon->SetSizeOrigin(m_pChangeIcon->GetSize());

	return S_OK;
}

//============================================================
// ���������`�F�b�N�̐�������
//============================================================
HRESULT CDressupUI::CreateReadyCheck()
{
	// ���������`�F�b�N�̐���
	m_pReadyCheck = CObject2D_Anim::Create
	( // ����
		VEC3_ZERO,		// �ʒu
		ready::PTRN.x,	// �e�N�X�`����������
		ready::PTRN.y,	// �e�N�X�`���c������
		0.0f,			// �Đ�����
		false,			// �����j��
		PRIORITY		// �D�揇��
	);
	if (m_pReadyCheck == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ������j��/�X�V/�`�悵�Ȃ����̂ɂ���
	m_pReadyCheck->SetType(CObject::TYPE::TYPE_NONE);

	// �����Đ���OFF�ɂ���
	m_pReadyCheck->SetEnableAutoPlay(false);

	// �e�N�X�`���p�^�[���̏�����
	m_pReadyCheck->SetPatternAnim((int)m_bReady);

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(ready::TEXTURE);
	m_pReadyCheck->BindTexture(nTexID);

	// ���������ɃT�C�Y��ݒ�
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, ready::WIDTH);
	size.y /= (float)ready::PTRN.y;
	m_pReadyCheck->SetSize(size);
	m_pReadyCheck->SetSizeOrigin(m_pReadyCheck->GetSize());

	return S_OK;
}

//============================================================
// �v���C���[�t���[���̐�������
//============================================================
HRESULT CDressupUI::CreatePlayerFrame()
{
	// �`�[���Z�b�g�A�b�v���̎擾
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) { return E_FAIL; }

	// �v���C���[�t���[���̐���
	m_pPlayerFrame = CObject2D_Anim::Create
	( // ����
		VEC3_ZERO,				// �ʒu
		player::frame::PTRN.x,	// �e�N�X�`����������
		player::frame::PTRN.y,	// �e�N�X�`���c������
		0.0f,					// �Đ�����
		false,					// �����j��
		PRIORITY				// �D�揇��
	);
	if (m_pPlayerFrame == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ������j��/�X�V/�`�悵�Ȃ����̂ɂ���
	m_pPlayerFrame->SetType(CObject::TYPE::TYPE_NONE);

	// �����Đ���OFF�ɂ���
	m_pPlayerFrame->SetEnableAutoPlay(false);

	switch (m_typeArea)
	{ // �|�W�V�������Ƃ̏���
	case CPlayer::FIELD_IN:
	{
		// �e�N�X�`���p�^�[���̏�����
		m_pPlayerFrame->SetPatternAnim(pSetupTeam->PlayerIdxToPadIdx(m_nPlayerIdx));
		break;
	}
	case CPlayer::FIELD_OUT:
	{
		// �e�N�X�`���p�^�[���̏�����
		m_pPlayerFrame->SetPatternAnim(-1);
		break;
	}
	default:
		assert(false);
		break;
	}

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(player::frame::TEXTURE);
	m_pPlayerFrame->BindTexture(nTexID);

	// ���������ɃT�C�Y��ݒ�
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, player::frame::WIDTH);
	size.y *= (float)player::frame::PTRN.x;
	m_pPlayerFrame->SetSize(size);
	m_pPlayerFrame->SetSizeOrigin(m_pPlayerFrame->GetSize());

	return S_OK;
}

//============================================================
// �v���C���[UI�̐�������
//============================================================
HRESULT CDressupUI::CreatePlayerUI()
{
	// �v���C���[UI�̐���
	m_pPlayerUI = CObject2D::Create(PRIORITY);
	if (m_pPlayerUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ������j��/�X�V/�`�悵�Ȃ����̂ɂ���
	m_pPlayerUI->SetType(CObject::TYPE::TYPE_NONE);

	// �e�N�X�`���̊���
	m_pPlayerUI->BindTexture(m_pRenderScene->GetTextureIndex());

	// �v���C���[UI�̍X�V
	UpdatePlayerUI();

	// ���̑傫����ݒ�
	m_pPlayerUI->SetSizeOrigin(m_pPlayerUI->GetSize());

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

	CGameManager::ETeamSide side;	// �`�[��
	switch (m_typeArea)
	{ // �|�W�V�������Ƃ̏���
	case CPlayer::FIELD_IN:
	{
		// �G���g���[�C���f�b�N�X���擾
		const int nPadIdx = pSetupTeam->PlayerIdxToPadIdx(m_nPlayerIdx);
		if (nPadIdx > -1)
		{ // ���[�U�[�̏ꍇ

			// �G���g���[�C���f�b�N�X����`�[���T�C�h���擾
			side = pSetupTeam->GetTeamSide(nPadIdx);
		}
		else
		{ // AI�̏ꍇ

			// ���g��AI��������ۑ�
			m_nOrdinalAI = m_nNumAI;

			// AI����������`�[���T�C�h���擾
			side = pSetupTeam->GetTeamSideAI(m_nOrdinalAI);

			// ���������ς݂ɂ���
			m_bReady = true;

			// AI�����������Z
			m_nNumAI++;
		}
		break;
	}
	case CPlayer::FIELD_OUT:
	{
		// �O��̃`�[�����w��
		side = (CGameManager::ETeamSide)(m_nPlayerIdx / (CPlayerManager::OUT_MAX / 2));

		// ���������ς݂ɂ���
		m_bReady = true;
		break;
	}
	default:
		assert(false);
		break;
	}

	// �v���C���[����
	m_pPlayer = CPlayer::Create
	( // ����
		VEC3_ZERO,	// �ʒu
		side,		// �`�[���T�C�h
		CPlayer::EHuman::HUMAN_ENTRY,	// �|�W�V����
		CPlayer::EBody::BODY_NORMAL,	// �̌n
		CPlayer::EHandedness::HAND_R	// ������
	);
	if (m_pPlayer == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ������j��/�X�V/�`�悵�Ȃ����̂ɂ���
	m_pPlayer->SetType(CObject::TYPE::TYPE_NONE);

	// ���̈ʒu�ݒ�
	m_pPlayer->CObject::SetOriginPosition(VEC3_ZERO);

	switch (m_typeArea)
	{ // �|�W�V�������Ƃ̏���
	case CPlayer::FIELD_IN:
	{
		// �C���f�b�N�X�̏㏑��
		m_pPlayer->SetMyPlayerIdx(pSetupTeam->PlayerIdxToPadIdx(m_nPlayerIdx));
		break;
	}
	case CPlayer::FIELD_OUT:
	{
		// �C���f�b�N�X�̏㏑��
		m_pPlayer->SetMyPlayerIdx(-1);
		break;
	}
	default:
		assert(false);
		break;
	}

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

	return S_OK;
}

//============================================================
// �����_�[�e�N�X�`����������
//============================================================
HRESULT CDressupUI::CreateRenderTexture()
{
	// �V�[�������_�[�e�N�X�`���̐���
	m_pRenderScene = CRenderTexture::Create
	( // ����
		CRenderTextureManager::LAYER_PLAYER,			// �`�揇���C���[
		std::bind(&CDressupUI::CreateTexture, this),	// �e�N�X�`���쐬�֐��|�C���^
		std::bind(&CCamera::SetCameraDressup, GET_MANAGER->GetCamera())	// �J�����ݒ�֐��|�C���^
	);
	if (m_pRenderScene == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
// �e�N�X�`���쐬����
//============================================================
void CDressupUI::CreateTexture()
{
	// �v���C���[�̕`��
	assert(m_pPlayer != nullptr);
	m_pPlayer->Draw();
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CDressupUI::SetPositionRelative()
{
	// ���g�̈ʒu���擾
	MyLib::Vector3 posThis = GetPosition();

	// �ύX��ރA�C�R���̈ʒu�ݒ�
	m_pChangeIcon->SetPosition(posThis + change::OFFSET);

	// ���������`�F�b�N�̈ʒu�ݒ�
	m_pReadyCheck->SetPosition(posThis + ready::OFFSET);

	// �v���C���[�t���[���̈ʒu�ݒ�
	m_pPlayerFrame->SetPosition(posThis);

	// �v���C���[UI�̈ʒu�ݒ�
	m_pPlayerUI->SetPosition(posThis);
}

//============================================================
// ����̍X�V����
//============================================================
void CDressupUI::UpdateControl(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����ւ���Ԃł͂Ȃ��ꍇ������
	if (m_pParent->GetState() != CEntry_Dressup::EState::STATE_DRESSUP) { return; }

	//--------------------------
	// �f�o�b�O��������
	//--------------------------
#if _DEBUG
	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_RETURN))
	{
		// ����������Ԃɂ���
		m_bReady = true;

		// �G�f�B�b�g��ނ����ۂ̕ύX�ɂ���
		m_typeEdit = EEditType::EDIT_PROCESS;

		// �ύX��ނ�̌^�̕ύX�ɂ���
		m_typeChange = EChangeType::TYPE_BODY;

		// �v���C���[UI�̍X�V
		UpdatePlayerUI();

		// �e�N�X�`���p�^�[���̍X�V
		m_pReadyCheck->SetPatternAnim((int)m_bReady);
	}
#endif // _DEBUG

	// ���쌠�C���f�b�N�X���͈͊O�̏ꍇ������
	if (m_nPadIdx <= -1 || m_nPadIdx >= mylib_const::MAX_PLAYER) { return; }

	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, m_nPadIdx))
	{
		// �����ւ��V�[���̎擾
		CEntry* pEntry = CEntry::GetInstance();						// �G���g���[���[�h���
		if (pEntry == nullptr) { assert(false); return; }			// �G���g���[���[�h���Ȃ��ꍇ������
		CEntryScene* pEntryScene = pEntry->GetEntryScene();			// �G���g���[�V�[�����
		if (pEntryScene == nullptr) { assert(false); return; }		// �G���g���[�V�[�����Ȃ��ꍇ������
		CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// �����ւ��V�[�����
		if (pDressup == nullptr) { assert(false); return; }			// �����ւ��V�[�����Ȃ��ꍇ������

		// ����������Ԃɂ���
		SetReady(true);

		// �G�f�B�b�g��ނ����ۂ̕ύX�ɂ���
		m_typeEdit = EEditType::EDIT_PROCESS;

		// �ύX��ނ�̌^�̕ύX�ɂ���
		m_typeChange = EChangeType::TYPE_BODY;

		// �v���C���[UI�̍X�V
		UpdatePlayerUI();

		// �I��UI�𑀍�\�ɂ���
		pDressup->SetSelectUISelect(m_nPadIdx, true);

		// ���쌠�̏�����
		SetPadIdx(-1);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, m_nPadIdx))
	{
		// ������������Ԃɂ���
		SetReady(false);
	}

	// �����������Ă���ꍇ������
	if (m_bReady) { return; }

	// �G�f�B�b�g��ނ̕ύX
	ChangeEditType(m_nPadIdx);

	switch (m_typeEdit)
	{ // �G�f�B�b�g��ނ��Ƃ̏���
	case EEditType::EDIT_PROCESS:

		switch (m_typeChange)
		{ // �ύX��ނ��Ƃ̏���
		case EChangeType::TYPE_HAIR:

			// ���̍X�V
			m_pHair->Update(fDeltaTime, fDeltaRate, fSlowRate);
			break;

		case EChangeType::TYPE_ACCESSORY:

			// �A�N�Z�̍X�V
			m_pAccessory->Update(fDeltaTime, fDeltaRate, fSlowRate);
			break;

		case EChangeType::TYPE_FACE:

			// ��̍X�V
			m_pFace->Update(fDeltaTime, fDeltaRate, fSlowRate);
			break;

		case EChangeType::TYPE_BODY:

			// �̌^�̕ύX
			ChangeBodyType(m_nPadIdx);
			break;

		case EChangeType::TYPE_HANDEDNESS:

			// ������̕ύX
			ChangeHandedness(m_nPadIdx);
			break;

		default:
			assert(false);
			break;
		}
		break;

	case EEditType::EDIT_CHANGETYPE:

		// �ύX����ӏ��̎�ޕύX
		ChangeChangeType(m_nPadIdx);
		break;

	default:
		assert(false);
		break;
	}
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

		// �v���C���[UI�̐F�𔒂ɂ���
		m_pPlayerUI->SetColor(COL_CHOICE);
		break;
	}
	case EEditType::EDIT_CHANGETYPE:
	{
		// �ύX��ރA�C�R���̐F�𔒂ɂ���
		m_pChangeIcon->SetColor(COL_CHOICE);

		// �v���C���[�t���[���̐F���O���[�ɂ���
		m_pPlayerFrame->SetColor(COL_DEFAULT);

		// �v���C���[UI�̐F���O���[�ɂ���
		m_pPlayerUI->SetColor(COL_DEFAULT);
		break;
	}
	default:
		assert(false);
		break;
	}
}

//============================================================
// �v���C���[UI�̍X�V����
//============================================================
void CDressupUI::UpdatePlayerUI()
{
	CTexture* pTexture = CTexture::GetInstance();			// �e�N�X�`�����
	int nTexID = m_pPlayerUI->GetIdxTexture();				// �e�N�X�`���C���f�b�N�X
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);	// �e�N�X�`���T�C�Y
	switch (m_typeChange)
	{ // �ύX��ނ��Ƃ̏���
	case EChangeType::TYPE_HAIR:
	case EChangeType::TYPE_ACCESSORY:
	case EChangeType::TYPE_FACE:
	{ // �����̒����ւ��̏ꍇ

		// �e�N�X�`�����W�̍X�V
		std::vector<D3DXVECTOR2> vecTex = m_pPlayerUI->GetVecTexUV();
		vecTex[0] = player::ui::face::TEX_POS[0];
		vecTex[1] = player::ui::face::TEX_POS[1];
		vecTex[2] = player::ui::face::TEX_POS[2];
		vecTex[3] = player::ui::face::TEX_POS[3];
		m_pPlayerUI->SetTexUV(vecTex);

		// �e�N�X�`���傫������̍������v�Z
		D3DXVECTOR2 diff = D3DXVECTOR2(vecTex[1].x - vecTex[0].x, vecTex[2].y - vecTex[0].y);
		D3DXVECTOR2 rate = D3DXVECTOR2(diff.x / 1.0f, diff.y / 1.0f);

		// ���������ɑ傫����ݒ�
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, player::ui::face::WIDTH);
		m_pPlayerUI->SetSize(D3DXVECTOR2(size.x * rate.x, size.y * rate.y));
		break;
	}
	case EChangeType::TYPE_BODY:
	case EChangeType::TYPE_HANDEDNESS:
	{ // �̎���̒����ւ��̏ꍇ

		// �e�N�X�`�����W�̍X�V
		std::vector<D3DXVECTOR2> vecTex = m_pPlayerUI->GetVecTexUV();
		vecTex[0] = player::ui::body::TEX_POS[0];
		vecTex[1] = player::ui::body::TEX_POS[1];
		vecTex[2] = player::ui::body::TEX_POS[2];
		vecTex[3] = player::ui::body::TEX_POS[3];
		m_pPlayerUI->SetTexUV(vecTex);

		// ���������ɑ傫����ݒ�
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, player::ui::body::WIDTH);
		m_pPlayerUI->SetSize(size);
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

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, nPadIdx))
	{// �ύX����ӏ��̎�ޕύX�֕ύX

		m_typeEdit = EEditType::EDIT_PROCESS;
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, nPadIdx))
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

		// �v���C���[UI�̍X�V
		UpdatePlayerUI();
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nPadIdx))
	{// �t���[�v

		// �O�֕ύX
		int changeType = (m_typeChange + (EChangeType::TYPE_MAX - 1)) % EChangeType::TYPE_MAX;
		m_typeChange = static_cast<EChangeType>(changeType);

		// �v���C���[UI�̍X�V
		UpdatePlayerUI();
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

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nPadIdx))
	{// ���[�v

		// ���֕ύX
		int afterHandedness = (handedness + 1) % CPlayer::EHandedness::HAND_MAX;
		handedness = static_cast<CPlayer::EHandedness>(afterHandedness);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nPadIdx))
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

	// ���̃`�[����ۑ�
	CGameManager::ETeamSide side = m_pPlayer->GetTeam();

	// �v���C���[�̍폜
	SAFE_KILL(m_pPlayer);

	// �Đ���
	m_pPlayer = CPlayer::Create
	( // ����
		VEC3_ZERO, 						// �ʒu
		side,							// �`�[���T�C�h
		CPlayer::EHuman::HUMAN_ENTRY,	// �|�W�V����
		body,							// �̌n
		handedness						// ������
	);
	if (m_pPlayer == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ������j��/�X�V/�`�悵�Ȃ����̂ɂ���
	m_pPlayer->SetType(CObject::TYPE::TYPE_NONE);

	// �C���f�b�N�X�̏㏑��
	m_pPlayer->SetMyPlayerIdx(pSetupTeam->PlayerIdxToPadIdx(m_nPlayerIdx));

	// �h���X�A�b�v�ɍĊ���
	m_pHair->BindObjectCharacter(m_pPlayer);
	m_pAccessory->BindObjectCharacter(m_pPlayer);
	m_pFace->BindObjectCharacter(m_pPlayer);
	m_pHair->ReRegist();
	m_pAccessory->ReRegist();
	m_pFace->ReRegist();

	return S_OK;
}

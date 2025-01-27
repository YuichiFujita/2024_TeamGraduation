//=============================================================================
// 
//  �h���X�A�b�v�ݒ菈�� [entry_dressup.cpp]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
// 
//=============================================================================
#include "entry_dressup.h"
#include "manager.h"
#include "fade.h"
#include "entry_setupTeam.h"
#include "playerManager.h"
#include "inputKeyButton.h"
#include "string2D.h"
#include "dressupUI.h"
#include "selectUI.h"
#include "object2D.h"
#include "object2D_Anim.h"
#include "entryRuleManager.h"
#include "loadtext.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTFILE	= "data\\TEXT\\entry\\setupTeam.txt";
	const std::string TOP_LINE	= "#==============================================================================";	// �e�L�X�g�̃��C��
	const std::string TEXT_LINE	= "#------------------------------------------------------------------------------";	// �e�L�X�g�̃��C��
	const int PRIORITY = 6;	// �D�揇��

	namespace ui
	{
		namespace left
		{
			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, VEC3_SCREEN_CENT.y - 80.0f, 0.0f);	// �����S�ʒu
			const float OFFSET_X = 210.0f;	// X���W�I�t�Z�b�g
		}

		namespace right
		{
			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, VEC3_SCREEN_CENT.y - 80.0f, 0.0f);	// �E���S�ʒu
			const float OFFSET_X = 210.0f;	// X���W�I�t�Z�b�g
		}

		namespace area
		{
			const MyLib::Vector3 POS	= MyLib::Vector3(VEC3_SCREEN_CENT.x - 240.0f, 570.0f, 0.0f);	// ���ʒu
			const std::string TEXTURE	= "data\\TEXTURE\\entry\\inout.png";	// �ύX��ރA�C�R���e�N�X�`��
			const MyLib::Vector3 OFFSET	= MyLib::Vector3(480.0f, 0.0f, 0.0f);	// �I�t�Z�b�g
			const MyLib::PosGrid2 PTRN	= MyLib::PosGrid2(1, 2);	// �e�N�X�`��������
			const float WIDTH			= 40.0f;	// ����
		}

		namespace name
		{
			const MyLib::Vector3 POS[] = { MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, 60.0f, 0.0f), MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, 60.0f, 0.0f) };	// ���_�ʒu
			const char*	FONT		= "data\\FONT\\�`���[�NS.otf";	// �t�H���g�p�X
			const bool	ITALIC		= false;						// �C�^���b�N
			const float	HEIGHT		= 42.0f;						// �����c��
			const EAlignX ALIGN_X	= XALIGN_CENTER;				// ���z�u
			const D3DXCOLOR COL		= MyLib::color::Black();		// �F
		}

		namespace back
		{
			const std::string TEXTURE	= "data\\TEXTURE\\entry\\back.png";	// �e�N�X�`��
			const MyLib::Vector3 POS	= MyLib::Vector3(VEC3_SCREEN_CENT.x - 500.0f, 645.0f, 0.0f);	// �ʒu
			const float HEGHT = 55.0f;	// �傫��
		}

		namespace enter
		{
			const std::string TEXTURE = "data\\TEXTURE\\entry\\next.png";	// �e�N�X�`��
			const MyLib::Vector3 POS	= MyLib::Vector3(VEC3_SCREEN_CENT.x + 500.0f, 645.0f, 0.0f);	// �ʒu
			const float HEGHT = 55.0f;	// �傫��
		}
	}
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEntry_Dressup::CEntry_Dressup() : CEntryScene(),
	m_pRuleManager	(nullptr),		// ���[���}�l�[�W���[
	m_state			(STATE_DRESSUP)	// ���
{
	// �����o�ϐ����N���A
	memset(&m_apTeamName[0], 0, sizeof(m_apTeamName));		// �`�[�������
	memset(&m_apDressInfo[0], 0, sizeof(m_apDressInfo));	// �O�쒅���ւ����
	memset(&m_apAreaUI[0], 0, sizeof(m_apAreaUI));			// �|�W�V�����ύXUI���
	memset(&m_apTransUI[0], 0, sizeof(m_apTransUI));		// �J��UI���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEntry_Dressup::~CEntry_Dressup()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEntry_Dressup::Init()
{
	CTexture* pTexture = CTexture::GetInstance();

	// �O��̒����ւ��Ǎ�
	Load();

	// �`�[���Z�b�g�A�b�v���̎擾
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) { return E_FAIL; }

	for (int i = 0; i < CGameManager::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�

		// �|�W�V�����ύXUI�̐���
		m_apAreaUI[i] = CObject2D_Anim::Create
		( // ����
			ui::area::POS + (ui::area::OFFSET * (float)i),	// �ʒu
			ui::area::PTRN.x,	// �e�N�X�`����������
			ui::area::PTRN.y,	// �e�N�X�`���c������
			0.0f,				// �ҋ@����
			false,				// �����j��
			PRIORITY			// �D�揇��
		);
		if (m_apAreaUI[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// �����Đ���OFF�ɂ���
		m_apAreaUI[i]->SetEnableAutoPlay(false);

		// �e�N�X�`���̊���
		int nTexID = CTexture::GetInstance()->Regist(ui::area::TEXTURE);
		m_apAreaUI[i]->BindTexture(nTexID);

		// ���������ɃT�C�Y��ݒ�
		MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, ui::area::WIDTH);
		size.x *= ui::area::PTRN.y;
		m_apAreaUI[i]->SetSize(size);
		m_apAreaUI[i]->SetSizeOrigin(m_apAreaUI[i]->GetSize());
	}

	// �߂�{�^���̐���
	m_apTransUI[TRANS_BACK] = CObject2D::Create(PRIORITY);
	if (m_apTransUI[TRANS_BACK] == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`���̊���
	int nTexBackID = CTexture::GetInstance()->Regist(ui::back::TEXTURE);
	m_apTransUI[TRANS_BACK]->BindTexture(nTexBackID);

	// �ʒu��ݒ�
	m_apTransUI[TRANS_BACK]->SetPosition(ui::back::POS);

	// ���������ɃT�C�Y��ݒ�
	MyLib::Vector2 sizeBack = pTexture->GetImageSize(nTexBackID);
	sizeBack = UtilFunc::Transformation::AdjustSizeByHeight(sizeBack, ui::back::HEGHT);
	m_apTransUI[TRANS_BACK]->SetSize(sizeBack);
	m_apTransUI[TRANS_BACK]->SetSizeOrigin(m_apTransUI[TRANS_BACK]->GetSize());

	// �i�ރ{�^���̐���
	m_apTransUI[TRANS_NEXT] = CObject2D::Create(PRIORITY);
	if (m_apTransUI[TRANS_NEXT] == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`���̊���
	int nTexNextID = CTexture::GetInstance()->Regist(ui::enter::TEXTURE);
	m_apTransUI[TRANS_NEXT]->BindTexture(nTexNextID);

	// �ʒu��ݒ�
	m_apTransUI[TRANS_NEXT]->SetPosition(ui::enter::POS);

	// ���������ɃT�C�Y��ݒ�
	MyLib::Vector2 sizeNext = pTexture->GetImageSize(nTexNextID);
	sizeNext = UtilFunc::Transformation::AdjustSizeByHeight(sizeNext, ui::enter::HEGHT);
	m_apTransUI[TRANS_NEXT]->SetSize(sizeNext);
	m_apTransUI[TRANS_NEXT]->SetSizeOrigin(m_apTransUI[TRANS_NEXT]->GetSize());

	int nCurLeft = 0;	// ���݂̍��v���C���[��
	int nMaxLeft = pSetupTeam->GetPlayerNum(CGameManager::ETeamSide::SIDE_LEFT);	// ���v���C���[����
	for (int nPlayerIdx = 0; nPlayerIdx < CGameManager::MAX_PLAYER; nPlayerIdx++)
	{ // �v���C���[�l�����J��Ԃ�

		// �`�[���w�肪�Ȃ��ꍇ����
		CGameManager::ETeamSide team = pSetupTeam->GetTeamSide(nPlayerIdx);	// �`�[���T�C�h
		if (team != CGameManager::ETeamSide::SIDE_LEFT) { continue; }

		// UI�̈ʒu���v�Z
		MyLib::Vector3 posUI = ui::left::POS;	// UI�ʒu
		posUI.x = ui::left::POS.x - (ui::left::OFFSET_X * (float)(nMaxLeft - 1)) * 0.5f + (ui::left::OFFSET_X * (float)nCurLeft);

		// �`�[�������o�[�������Z
		nCurLeft++;

		// �����ւ�UI�̐���
		CDressupUI* pDressup = CDressupUI::Create(this, CPlayer::FIELD_IN, nPlayerIdx, posUI);
		if (pDressup == nullptr)
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}

		// �����ւ�UI�̒ǉ�
		m_vecDressInfo.push_back(pDressup);

		const int nPadIdx = pSetupTeam->PlayerIdxToPadIdx(nPlayerIdx);	// ���쌠�C���f�b�N�X
		if (nPadIdx > -1)
		{ // �v���C���[�̏ꍇ

			// �I��UI�̐���
			int nSelectX = nCurLeft - 1;	// X�I��
			CSelectUI* pSelect = CSelectUI::Create(team, nSelectX, nPadIdx, posUI);
			if (pSelect == nullptr)
			{ // �����Ɏ��s�����ꍇ

				return E_FAIL;
			}

			// �����ւ�UI���쌠�̐ݒ�
			SetDressUIControl(nPadIdx, nSelectX);

			// �I�𑀍���~
			pSelect->SetSelect(false);

			// �I��UI�̒ǉ�
			m_vecSelect.push_back(pSelect);
		}
	}

	int nCurRight = 0;	// ���݂̉E�v���C���[��
	int nMaxRight = pSetupTeam->GetPlayerNum(CGameManager::ETeamSide::SIDE_RIGHT);	// �E�v���C���[����
	for (int nPlayerIdx = 0; nPlayerIdx < CGameManager::MAX_PLAYER; nPlayerIdx++)
	{ // �v���C���[�l�����J��Ԃ�

		// �`�[���w�肪�Ȃ��ꍇ����
		CGameManager::ETeamSide team = pSetupTeam->GetTeamSide(nPlayerIdx);	// �`�[���T�C�h
		if (team != CGameManager::ETeamSide::SIDE_RIGHT) { continue; }

		// UI�̈ʒu���v�Z
		MyLib::Vector3 posUI = ui::right::POS;	// UI�ʒu
		posUI.x = ui::right::POS.x - (ui::right::OFFSET_X * (float)(nMaxRight - 1)) * 0.5f + (ui::right::OFFSET_X * (float)nCurRight);

		// �`�[�������o�[�������Z
		nCurRight++;

		// �����ւ�UI�̐���
		CDressupUI* pDressup = CDressupUI::Create(this, CPlayer::FIELD_IN, nPlayerIdx, posUI);
		if (pDressup == nullptr)
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}

		// �����ւ�UI�̒ǉ�
		m_vecDressInfo.push_back(pDressup);

		const int nPadIdx = pSetupTeam->PlayerIdxToPadIdx(nPlayerIdx);	// ���쌠�C���f�b�N�X
		if (nPadIdx > -1)
		{ // �v���C���[�̏ꍇ

			// �I��UI�̐���
			int nSelectX = nCurLeft + nCurRight - 1;	// X�I��
			CSelectUI* pSelect = CSelectUI::Create(team, nSelectX, nPadIdx, posUI);
			if (pSelect == nullptr)
			{ // �����Ɏ��s�����ꍇ

				return E_FAIL;
			}

			// �����ւ�UI���쌠�̐ݒ�
			SetDressUIControl(nPadIdx, nSelectX);

			// �I�𑀍���~
			pSelect->SetSelect(false);

			// �I��UI�̒ǉ�
			m_vecSelect.push_back(pSelect);
		}
	}

	for (int nPlayerIdx = 0; nPlayerIdx < CPlayerManager::OUT_MAX; nPlayerIdx++)
	{ // �O��l�����J��Ԃ�

		int nMaxTeam = CPlayerManager::OUT_MAX / 2;	// �`�[�����Ƃ̊O��l��
		MyLib::Vector3 posUI;	// UI�ʒu
		if (nPlayerIdx < nMaxTeam)
		{
			// UI�̈ʒu���v�Z
			posUI = ui::left::POS;
			posUI.x = ui::left::POS.x - (ui::left::OFFSET_X * (float)(nMaxTeam - 1)) * 0.5f + (ui::left::OFFSET_X * (float)nPlayerIdx);
		}
		else
		{
			// UI�̈ʒu���v�Z
			posUI = ui::right::POS;
			posUI.x = ui::right::POS.x - (ui::right::OFFSET_X * (float)(nMaxTeam - 1)) * 0.5f + (ui::right::OFFSET_X * (float)(nPlayerIdx - nMaxTeam));
		}

		// �����ւ�UI�̐���
		m_apDressInfo[nPlayerIdx] = CDressupUI::Create(this, CPlayer::FIELD_OUT, -1, posUI);
		if (m_apDressInfo[nPlayerIdx] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}

		// �����`���OFF�ɂ���
		m_apDressInfo[nPlayerIdx]->SetEnableDisp(false);
	}

	for (int i = 0; i < CGameManager::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�

		// �`�[�����̐���
		m_apTeamName[i] = CString2D::Create
		( // ����
			ui::name::FONT,		// �t�H���g�p�X
			ui::name::ITALIC,	// �C�^���b�N
			L"",				// �w�蕶����
			ui::name::POS[i],	// ���_�ʒu
			ui::name::HEIGHT,	// �����c��
			ui::name::ALIGN_X,	// ���z�u
			VEC3_ZERO,			// ���_����
			ui::name::COL		// �F
		);
		if (m_apTeamName[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// �e�L�X�g������
		loadtext::BindString(m_apTeamName[i], loadtext::LoadText("data\\TEXT\\entry\\nameTeam.txt", UtilFunc::Transformation::Random(0, 9)));
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEntry_Dressup::Uninit()
{
	// �G���g���[���[���̔j��
	SAFE_REF_RELEASE(m_pRuleManager);

	for (int nPlayerIdx = 0; nPlayerIdx < CPlayerManager::OUT_MAX; nPlayerIdx++)
	{ // �O��l�����J��Ԃ�

		// �����ւ�UI�̏I��
		SAFE_UNINIT(m_apDressInfo[nPlayerIdx]);
	}

	for (auto& rInfo : m_vecDressInfo)
	{ // �v�f�����J��Ԃ�

		// �����ւ�UI�̏I��
		SAFE_UNINIT(rInfo);
	}

	for (auto& rSelect : m_vecSelect)
	{ // �v�f�����J��Ԃ�

		// �I��UI�̏I��
		SAFE_UNINIT(rSelect);
	}

	for (int i = 0; i < CGameManager::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�

		// �`�[�����̏I��
		SAFE_UNINIT(m_apTeamName[i]);

		// �|�W�V�����ύXUI�̏I��
		SAFE_UNINIT(m_apAreaUI[i]);
	}

	for (int i = 0; i < TRANS_MAX; i++)
	{ // �J�ڃ{�^���̑������J��Ԃ�

		// �J��UI�̏I��
		SAFE_UNINIT(m_apTransUI[i]);
	}

	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CEntry_Dressup::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case EState::STATE_DRESSUP:
		break;

	case EState::STATE_SETTING:

		// ���[���}�l�[�W���[�̍X�V
		assert(m_pRuleManager != nullptr);
		m_pRuleManager->Update(fDeltaTime, fDeltaRate, fSlowRate);
		break;

	case EState::STATE_END:

		// �Z�[�u����
		Save();

		// �Q�[����ʂɑJ�ڂ���
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_GAME);
		break;

	default:
		assert(false);
		break;
	}

	// �e�̍X�V
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CEntry_Dressup::SetState(const EState state)
{
	// ��Ԃ�ݒ�
	m_state = state;
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_DRESSUP:

		// �G���g���[���[���̔j��
		SAFE_REF_RELEASE(m_pRuleManager);
		break;

	case STATE_SETTING:

		if (m_pRuleManager == nullptr)
		{
			// �G���g���[���[���̐���
			m_pRuleManager = CEntryRuleManager::Create(this);
		}
		break;

	case STATE_END:
		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// �����ւ�UI���������t���O�̐ݒ�
//==========================================================================
void CEntry_Dressup::SetDressUIReady(const int nPlayerIdx, const bool bReady)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ�G���[
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumDressUI()) { assert(false); return; }

	// ���������t���O��Ԃ�
	GetPtrDressUI(nPlayerIdx)->SetReady(bReady);
}

//==========================================================================
// �����ւ�UI�̏��������ς݂��̊m�F
//==========================================================================
bool CEntry_Dressup::IsDressUIReady(const int nPlayerIdx)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ�G���[
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumDressUI()) { assert(false); return false; }

	// ���������t���O��Ԃ�
	return GetPtrDressUI(nPlayerIdx)->IsReady();
}

//==========================================================================
// �����ւ�UI���쌠�̐ݒ�
//==========================================================================
void CEntry_Dressup::SetDressUIControl(const int nPadIdx, const int nPlayerIdx)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ�G���[
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumDressUI()) { assert(false); return; }

	// �����v���C���[�̒����ւ�UI�ʒu��Ԃ�
	GetPtrDressUI(nPlayerIdx)->SetPadIdx(nPadIdx);
}

//==========================================================================
// �����ւ�UI�I���\���̊m�F
//==========================================================================
bool CEntry_Dressup::IsDressSelectOK(const int nPadIdx, const int nPlayerIdx) const
{
	// �����ȊO�̃��[�U�[�̒����ւ�UI�̏ꍇ�I��s��
	const int nSelectPadIdx = GetPtrDressUI(nPlayerIdx)->GetMyPlayerIdx();	// �I��\���̑��쌠�C���f�b�N�X
	if (nSelectPadIdx > -1 && nSelectPadIdx != nPadIdx) { return false; }

	for (const auto& rSelect : m_vecSelect)
	{ // �v�f�����J��Ԃ�

		// ���Ɏ����ȊO�̃��[�U�[���I�𒆂̏ꍇ�I��s��
		if (rSelect->GetSelectIdx().y == CSelectUI::SELECT_DRESSUP
		&&  rSelect->GetSelectIdx().x == nPlayerIdx
		&&  rSelect->GetPadIdx()	  != nPadIdx) { return false; }
	}

	return true;
}

//==========================================================================
// �I��UI�I�𑀍�t���O�̐ݒ�
//==========================================================================
void CEntry_Dressup::SetSelectUISelect(const int nPadIdx, const bool bSelect)
{
	for (auto& rSelect : m_vecSelect)
	{ // �v�f�����J��Ԃ�

		// �I��UI�̑��쌠�C���f�b�N�X�������̑��쌠�C���f�b�N�X�ƈ�v���Ȃ��ꍇ����
		if (rSelect->GetPadIdx() != nPadIdx) { continue; }

		// �I�𑀍�\�t���O�̐ݒ�
		return rSelect->SetSelect(bSelect);
	}
}

//==========================================================================
// �����ւ�UI�ʒu�擾
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetDressUIPosition(const int nPlayerIdx) const
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ�G���[
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumDressUI()) { assert(false); return VEC3_ZERO; }

	// �����v���C���[�̒����ւ�UI�ʒu��Ԃ�
	return GetPtrDressUI(nPlayerIdx)->GetPosition();
}

//==========================================================================
// ���OUI�ʒu�擾
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetNameUIPosition(const CGameManager::ETeamSide team)
{
	// ������̈ʒu��Ԃ�
	return m_apTeamName[team]->GetPosition();
}

//==========================================================================
// ���OUI�傫���擾
//==========================================================================
MyLib::Vector2 CEntry_Dressup::GetNameUISize(const CGameManager::ETeamSide team)
{
	// ������̑傫����Ԃ�
	return MyLib::Vector2(m_apTeamName[team]->GetStrWidth(), ui::name::HEIGHT);
}

//==========================================================================
// �|�W�V�����ύXUI�ʒu�擾
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetAreaUIPosition(const CPlayer::EFieldArea area)
{
	// �|���S���̈ʒu��Ԃ�
	return m_apAreaUI[area]->GetPosition();
}

//==========================================================================
// �|�W�V�����ύXUI�傫���擾
//==========================================================================
MyLib::Vector2 CEntry_Dressup::GetAreaUISize(const CPlayer::EFieldArea area)
{
	// �|���S���̑傫����Ԃ�
	return m_apAreaUI[area]->GetSize();
}

//==========================================================================
// �J��UI�ʒu�擾
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetTransUIPosition(const ETrans trans)
{
	// �|���S���̈ʒu��Ԃ�
	return m_apTransUI[trans]->GetPosition();
}

//==========================================================================
// �J��UI�傫���擾
//==========================================================================
MyLib::Vector2 CEntry_Dressup::GetTransUISize(const ETrans trans)
{
	// �|���S���̑傫����Ԃ�
	return m_apTransUI[trans]->GetSize();
}

//==========================================================================
// �\�������ւ�UI���擾 (�S�����ւ�UI)
//==========================================================================
int CEntry_Dressup::GetNumDressUI() const
{
	int nNumDressUI = 0;	// �����ւ�UI��
	for (int team = 0; team < CGameManager::SIDE_MAX; team++)
	{ // �`�[�������J��Ԃ�

		// �\�������ւ�UI���擾
		nNumDressUI += GetNumDressUI((CGameManager::ETeamSide)team);
	}
	return nNumDressUI;
}

//==========================================================================
// �\�������ւ�UI���擾 (�`�[���ʒ����ւ�UI)
//==========================================================================
int CEntry_Dressup::GetNumDressUI(const CGameManager::ETeamSide team) const
{
	int nNumDressUI = 0;	// �����ւ�UI��
	switch (m_apAreaUI[team]->GetPatternAnim())
	{ // ���݃p�^�[�����Ƃ̏���
	case CPlayer::FIELD_IN:
	{
		for (auto& rInfo : m_vecDressInfo)
		{ // �v�f�����J��Ԃ�

			// �Ⴄ�`�[���̏ꍇ����
			if (rInfo->GetTeam() != team) { continue; }

			// UI�������Z
			nNumDressUI++;
		}
		break;
	}
	case CPlayer::FIELD_OUT:
	{
		// �O��l����UI�������Z
		nNumDressUI += CPlayerManager::OUT_MAX / 2;
		break;
	}
	default:
		assert(false);
		break;
	}
	return nNumDressUI;
}

//==========================================================================
// �����ւ�UI�̃|�W�V�����ύX
//==========================================================================
void CEntry_Dressup::ChangeDressUIArea(const CGameManager::ETeamSide team)
{
	// �`�[���̏������������Ă��Ȃ��ꍇ�ύX�s��
	if (!IsAreaChangeOK(team)) { return; }

	// TODO�F�����Ō��݂̑I���C���f�b�N�X������DressUI�|�C���^��ۑ�
	std::map<CSelectUI*, CDressupUI*> mapDressUI;	// �����ւ�UI�ꎞ�ۑ��}�b�v
	for (auto& rSelect : m_vecSelect)
	{ // �v�f�����J��Ԃ�

		MyLib::PosGrid2 select = rSelect->GetSelectIdx();	// �I���C���f�b�N�X
		if (select.y == CSelectUI::SELECT_DRESSUP)
		{ // �����ւ��I�𒆂̏ꍇ

			// ���[�J���}�b�v�ɒ����ւ�UI���ꎞ�ۑ�
			mapDressUI.insert(std::make_pair(rSelect, GetPtrDressUI(select.x)));
		}
	}

	// �p�^�[����i�߂�
	m_apAreaUI[team]->AddPatternAnim();

	switch (m_apAreaUI[team]->GetPatternAnim())
	{ // ���݃p�^�[�����Ƃ̏���
	case CPlayer::FIELD_IN:
	{
		int nSideMax = CPlayerManager::OUT_MAX / 2;	// �`�[�����Ƃ̍ő�l��
		int nOffset = nSideMax * (int)team;			// �C���f�b�N�X�I�t�Z�b�g
		for (int nPlayerIdx = nOffset; nPlayerIdx < nSideMax + nOffset; nPlayerIdx++)
		{ // �O��l�����J��Ԃ�

			// �O�쒅���ւ�UI�̔�\��
			m_apDressInfo[nPlayerIdx]->SetEnableDisp(false);

			// ��������������
			m_apDressInfo[nPlayerIdx]->SetReady(true);
		}

		for (auto& rInfo : m_vecDressInfo)
		{ // �v�f�����J��Ԃ�

			// �Ⴄ�`�[���̏ꍇ����
			if (rInfo->GetTeam() != team) { continue; }

			// ���쒅���ւ�UI�̕\��
			rInfo->SetEnableDisp(true);
		}
		break;
	}
	case CPlayer::FIELD_OUT:
	{
		int nSideMax = CPlayerManager::OUT_MAX / 2;	// �`�[�����Ƃ̍ő�l��
		int nOffset = nSideMax * (int)team;			// �C���f�b�N�X�I�t�Z�b�g
		for (int nPlayerIdx = nOffset; nPlayerIdx < nSideMax + nOffset; nPlayerIdx++)
		{ // �O��l�����J��Ԃ�

			// �O�쒅���ւ�UI�̕\��
			m_apDressInfo[nPlayerIdx]->SetEnableDisp(true);
		}

		for (auto& rInfo : m_vecDressInfo)
		{ // �v�f�����J��Ԃ�

			// �Ⴄ�`�[���̏ꍇ����
			if (rInfo->GetTeam() != team) { continue; }

			// ���쒅���ւ�UI�̔�\��
			rInfo->SetEnableDisp(false);

			// ��������������
			rInfo->SetReady(true);
		}
		break;
	}
	default:
		assert(false);
		break;
	}

	// TODO�F������DressUI�|�C���^���\������Ă���ꍇ�͑I���C���f�b�N�X���X�V
	// ELSE�F��\���̏ꍇ�͑S���I������NEXT�Ɉڍs
	for (auto& rSelect : m_vecSelect)
	{ // �v�f�����J��Ԃ�

		// ����I��UI��������Ȃ��ꍇ����
		auto itr = mapDressUI.find(rSelect);	// �����C�e���[�^�[
		if (itr == mapDressUI.end()) { continue; }

		CDressupUI* pDressUI = itr->second;	// �R�Â���ꂽ�����ւ�UI
		if (pDressUI->IsDisp())
		{ // �\�����̏ꍇ

			// TODO�F���̒����ւ�UI�|�C���^�̑I���C���f�b�N�X��select.x�ɐݒ�
			int nSelectX = GetIdxDressUI(pDressUI);
			assert(nSelectX != -1);

			// �I�𒆂����������ւ�UI�Ɉړ�
			rSelect->SetSelectIdxX(nSelectX);
		}
		else
		{ // ��\���̏ꍇ

			// �|�W�V�����ύX�{�^���Ɉړ�
			rSelect->SetSelectIdxX(team);
			rSelect->SetSelectIdxY(CSelectUI::SELECT_AREA);
		}
	}
}

//==========================================================================
// �J��UI�I�𐔎擾
//==========================================================================
int CEntry_Dressup::GetTransUINumSelect(const int nSelectX) const
{
	int nNumSelect = 0;	// �I��
	for (auto& rSelect : m_vecSelect)
	{ // �v�f�����J��Ԃ�

		MyLib::PosGrid2 select = rSelect->GetSelectIdx();	// �I���C���f�b�N�X
		if (select.y == CSelectUI::SELECT_TRANS && select.x == nSelectX)
		{ // �N�����J�ڂ�I�����Ă���ꍇ

			// �I�𐔂̉��Z
			nNumSelect++;
		}
	}

	return nNumSelect;
}

//==========================================================================
// �|�W�V�����ύXUI�I�𐔎擾
//==========================================================================
int CEntry_Dressup::GetAreaUINumSelect(const int nSelectX) const
{
	int nNumSelect = 0;	// �I��
	for (auto& rSelect : m_vecSelect)
	{ // �v�f�����J��Ԃ�

		MyLib::PosGrid2 select = rSelect->GetSelectIdx();	// �I���C���f�b�N�X
		if (select.y == CSelectUI::SELECT_AREA && select.x == nSelectX)
		{ // �N�����J�ڂ�I�����Ă���ꍇ

			// �I�𐔂̉��Z
			nNumSelect++;
		}
	}

	return nNumSelect;
}

//==========================================================================
// �`�[���̏����S�����m�F
//==========================================================================
bool CEntry_Dressup::IsTeamReady(const CPlayer::EFieldArea area, const CGameManager::ETeamSide team)
{
	switch (area)
	{ // �|�W�V�������Ƃ̏���
	case CPlayer::FIELD_IN:
	{
		for (auto& rInfo : m_vecDressInfo)
		{ // �v�f�����J��Ԃ�

			// �Ⴄ�`�[���̏ꍇ����
			if (rInfo->GetTeam() != team) { continue; }

			// �������I����Ă���ꍇ����
			if (rInfo->IsReady()) { continue; }

			// ������������Ԃ�
			return false;
		}

		// �����S������Ԃ�
		return true;
	}
	case CPlayer::FIELD_OUT:
	{
		int nSideMax = CPlayerManager::OUT_MAX / 2;	// �`�[�����Ƃ̍ő�l��
		int nOffset = nSideMax * (int)team;			// �C���f�b�N�X�I�t�Z�b�g
		for (int nPlayerIdx = nOffset; nPlayerIdx < nSideMax + nOffset; nPlayerIdx++)
		{ // �O��l�����J��Ԃ�

			// �������I����Ă���ꍇ����
			if (m_apDressInfo[nPlayerIdx]->IsReady()) { continue; }

			// ������������Ԃ�
			return false;
		}

		// �����S������Ԃ�
		return true;
	}
	default:
		assert(false);
		return false;
	}
}

//==========================================================================
// ���O�I���\���̊m�F
//==========================================================================
bool CEntry_Dressup::IsNameSelectOK(const CGameManager::ETeamSide team) const
{
	for (auto& rSelect : m_vecSelect)
	{ // �v�f�����J��Ԃ�

		MyLib::PosGrid2 select = rSelect->GetSelectIdx();	// �I���C���f�b�N�X
		if (select.y == CSelectUI::SELECT_NAME && select.x == team)
		{ // �N�������O��I�����Ă���ꍇ

			return false;
		}
	}

	return true;
}

//==========================================================================
// �|�W�V�����ύX�\���̊m�F
//==========================================================================
bool CEntry_Dressup::IsAreaChangeOK(const CGameManager::ETeamSide team) const
{
	const int nMaxDressUI = GetNumDressUI(team);		// �����ւ�UI����
	const int nSideMax = CPlayerManager::OUT_MAX / 2;	// �`�[�����Ƃ̍ő�l��
	const int nOffset = nSideMax * (int)team;			// �C���f�b�N�X�I�t�Z�b�g
	for (const auto& rSelect : m_vecSelect)
	{ // �v�f�����J��Ԃ�

		MyLib::PosGrid2 select = rSelect->GetSelectIdx();

		// ���Ɏ����ȊO�̃��[�U�[���I�𒆂̏ꍇ�I��s��
		if (select.y != CSelectUI::SELECT_DRESSUP) { continue; }

		// �I��͈͂������`�[������Ȃ��ꍇ����
		if (!(select.x >= nOffset && select.x < nMaxDressUI + nOffset)) { continue; }

		// ���삵�Ă��Ȃ��ꍇ����
		if (rSelect->IsSelect()) { continue; }

		return false;
	}

	return true;
}

//==========================================================================
// �Q�[���ݒ�J��
//==========================================================================
bool CEntry_Dressup::TransSetting()
{
	// ���������ł��Ă��Ȃ��ꍇ������
	if (!IsAllReady()) { return false; }

	// �Q�[���ݒ��ԂɑJ��
	SetState(EState::STATE_SETTING);

	return true;
}

//==========================================================================
// �����ւ�UI�̎擾����
//==========================================================================
CDressupUI* CEntry_Dressup::GetPtrDressUI(const int nIdx) const
{
	int nNumDressUI = 0;	// �����ւ�UI��
	for (int team = 0; team < CGameManager::SIDE_MAX; team++)
	{ // �`�[�������J��Ԃ�

		switch (m_apAreaUI[team]->GetPatternAnim())
		{ // ���݃p�^�[�����Ƃ̏���
		case CPlayer::FIELD_IN:
		{
			for (auto& rInfo : m_vecDressInfo)
			{ // �v�f�����J��Ԃ�

				// �Ⴄ�`�[���̏ꍇ����
				if (rInfo->GetTeam() != team) { continue; }

				// �����C���f�b�N�X�ƈ�v�����ꍇ���݂̒����ւ�UI��Ԃ�
				if (nNumDressUI == nIdx) { return rInfo; }

				// UI�������Z
				nNumDressUI++;
			}
			break;
		}
		case CPlayer::FIELD_OUT:
		{
			int nSideMax = CPlayerManager::OUT_MAX / 2;	// �`�[�����Ƃ̍ő�l��
			int nOffset = nSideMax * (int)team;			// �C���f�b�N�X�I�t�Z�b�g
			for (int nPlayerIdx = nOffset; nPlayerIdx < nSideMax + nOffset; nPlayerIdx++)
			{ // �O��l�����J��Ԃ�

				// �����C���f�b�N�X�ƈ�v�����ꍇ���݂̒����ւ�UI��Ԃ�
				if (nNumDressUI == nIdx) { return m_apDressInfo[nPlayerIdx]; }

				// UI�������Z
				nNumDressUI++;
			}
			break;
		}
		default:
			assert(false);
			break;
		}
	}

	return nullptr;
}

//==========================================================================
// X�I���C���f�b�N�X�̎擾����
//==========================================================================
int CEntry_Dressup::GetIdxDressUI(const CDressupUI* pUI) const
{
	int nNumDressUI = GetNumDressUI();	// �����ւ�UI��
	for (int i = 0; i < nNumDressUI; i++)
	{ // �����ւ�UI�����J��Ԃ�

		// �����|�C���^�����������ꍇ�C���f�b�N�X��Ԃ�
		if (GetPtrDressUI(i) == pUI) { return i; }
	}

	return -1;
}

//==========================================================================
// �����S�����t���O�̎擾����
//==========================================================================
bool CEntry_Dressup::IsAllReady()
{
	const int nNumDressUI = GetNumDressUI(); // �����ւ�UI��
	for (int i = 0; i < nNumDressUI; i++)
	{ // �����ւ�UI�����J��Ԃ�

		// �������I����Ă���ꍇ����
		if (GetPtrDressUI(i)->IsReady()) { continue; }

		// ������������Ԃ�
		return false;
	}

	// �����S������Ԃ�
	return true;
}

//==========================================================================
// �Z�[�u����
//==========================================================================
void CEntry_Dressup::Save()
{
	// �v���C���[�}�l�[�W���擾
	CPlayerManager* pPlayerMgr = CPlayerManager::GetInstance();

	// �ǂݍ��ݏ��
	std::vector<CPlayerManager::LoadInfo> vecInSaveInfo[CGameManager::ETeamSide::SIDE_MAX];
	std::vector<CPlayerManager::LoadInfo> vecOutSaveInfo[CGameManager::ETeamSide::SIDE_MAX];

	// ����̃T�C�Y
	int size = static_cast<int>(m_vecDressInfo.size());
	for (int i = 0; i < size; i++)
	{
		// ���g�̑��삷��C���f�b�N�X�ԍ��擾
		int nPadIdx = m_vecDressInfo[i]->GetMyPlayerIdx();
		CGameManager::ETeamSide side = m_vecDressInfo[i]->GetTeam();

		// �ǂݍ��ݏ��
		vecInSaveInfo[side].emplace_back();
		CPlayerManager::LoadInfo* pLoadInfo = &vecInSaveInfo[side].back();
		pLoadInfo->nControllIdx = nPadIdx;									// ���g�̑��삷��C���f�b�N�X�ԍ��擾
		pLoadInfo->nHair = m_vecDressInfo[i]->GetHairNowIdx();				// ���̃C���f�b�N�X�ԍ�
		pLoadInfo->nAccessory = m_vecDressInfo[i]->GetAccessoryNowIdx();	// �A�N�Z�̃C���f�b�N�X�ԍ�
		pLoadInfo->nFace = m_vecDressInfo[i]->GetFaceNowIdx();				// ��̃C���f�b�N�X�ԍ�
		pLoadInfo->eBody = m_vecDressInfo[i]->GetBodyType();				// �̌^
		pLoadInfo->eHanded = m_vecDressInfo[i]->GetHandedness();			// ������
	}

	for (int i = 0; i < CPlayerManager::OUT_MAX; i++)
	{ // �O��l�����J��Ԃ�

		// ���g�̑��삷��C���f�b�N�X�ԍ��擾
		int nPadIdx = m_apDressInfo[i]->GetMyPlayerIdx();
		CGameManager::ETeamSide side = m_apDressInfo[i]->GetTeam();

		// �ǂݍ��ݏ��
		vecOutSaveInfo[side].emplace_back();
		CPlayerManager::LoadInfo* pLoadInfo = &vecOutSaveInfo[side].back();
		pLoadInfo->nControllIdx = nPadIdx;								// ���g�̑��삷��C���f�b�N�X�ԍ��擾
		pLoadInfo->nHair = m_apDressInfo[i]->GetHairNowIdx();			// ���̃C���f�b�N�X�ԍ�
		pLoadInfo->nAccessory = m_apDressInfo[i]->GetAccessoryNowIdx();	// �A�N�Z�̃C���f�b�N�X�ԍ�
		pLoadInfo->nFace = m_apDressInfo[i]->GetFaceNowIdx();			// ��̃C���f�b�N�X�ԍ�
		pLoadInfo->eBody = m_apDressInfo[i]->GetBodyType();				// �̌^
		pLoadInfo->eHanded = m_apDressInfo[i]->GetHandedness();			// ������
	}

	// �Z�[�u����
	pPlayerMgr->Save
	(
		vecInSaveInfo[CGameManager::ETeamSide::SIDE_LEFT],
		vecInSaveInfo[CGameManager::ETeamSide::SIDE_RIGHT],
		vecOutSaveInfo[CGameManager::ETeamSide::SIDE_LEFT],
		vecOutSaveInfo[CGameManager::ETeamSide::SIDE_RIGHT]
	);

	// �Q�[���ݒ�̕ۑ�
	assert(m_pRuleManager != nullptr);
	CEntryRuleManager::SRule rule = m_pRuleManager->GetRule();	// ���[��
	CEntryRuleManager::SaveSetting(rule, m_apTeamName[CGameManager::ETeamSide::SIDE_LEFT]->GetStr(), m_apTeamName[CGameManager::ETeamSide::SIDE_RIGHT]->GetStr());
}

//==========================================================================
// ���[�h
//==========================================================================
void CEntry_Dressup::Load()
{
	// �t�@�C�����J��
	std::ifstream File(TEXTFILE);
	if (!File.is_open()) {
		return;
	}

	// �R�����g�p
	std::string hoge;

	// �f�[�^�ǂݍ���
	std::string line;
	while (std::getline(File, line))
	{
		// �R�����g�̓X�L�b�v
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("PLAYERNUM") != std::string::npos)
		{// PLAYERNUM�Ń`�[�����̃v���C���[���ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);
			
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// �t�@�C�������
	File.close();
}

//==========================================================================
// �f�o�b�O����
//==========================================================================
void CEntry_Dressup::Debug()
{

	//--------------------------
	// �V�[���J��
	//--------------------------
	if (IsAllReady() &&
		CInputKeyboard::GetInstance()->GetTrigger(DIK_X))
	{
		// �Q�[���ݒ��ԂɑJ��
		SetState(EState::STATE_SETTING);
	}

	if (ImGui::TreeNode("SetUpTeam"))
	{
		//=============================
		// �Z�[�u
		//=============================s
		if (ImGui::Button("Save"))
		{
			Save();
		}

		// ����̃T�C�Y
		int size = static_cast<int>(m_vecDressInfo.size());
		for (int i = 0; i < size; i++)
		{
			std::string treename = "Info : " + std::to_string(i);	// �c���[��
			if (ImGui::TreeNode(treename.c_str()))
			{
				const auto& info = m_vecDressInfo[i];

				ImGui::Text("editType[%s]", magic_enum::enum_name(info->GetTypeEdit()));
				ImGui::Text("changeType[%s]", magic_enum::enum_name(info->GetTypeChange()));
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}

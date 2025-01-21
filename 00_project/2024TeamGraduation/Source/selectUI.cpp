//============================================================
//
//	�I��UI���� [selectUI.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "selectUI.h"
#include "object2D.h"
#include "object2D_Anim.h"
#include "entry.h"
#include "entryscene.h"
#include "entry_dressup.h"
#include "inputKeyButton.h"
#include "string2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIO_BG = 4;		// �w�i�̗D�揇��
	const int PRIO_UI = 6;		// UI�̗D�揇��
	const int NUM_SELECTX[] =	// X�I������
	{
		CGameManager::SIDE_MAX,		// ���O
		-1,							// �����ւ�
		CGameManager::SIDE_MAX,		// �|�W�V����
		CEntry_Dressup::TRANS_MAX,	// �J��
	};

	namespace pad
	{
		const std::string TEXTURE	= "data\\TEXTURE\\entry\\playerMarker001.png";	// �R���g���[���[UI�e�N�X�`��
		const MyLib::PosGrid2 PTRN	= MyLib::PosGrid2(4, 1);	// �e�N�X�`��������
		const float NAME_WIDTH	= 22.0f;	// ���O�I�����̉���
		const float DRESS_WIDTH	= 55.0f;	// �����ւ����̉���
		const float OTHER_WIDTH	= 18.0f;	// ���I�����̉���
		const MyLib::Vector3 NAME_OFFSET	= MyLib::Vector3(0.0f, -60.0f, 0.0f);	// ���O�I�t�Z�b�g
		const MyLib::Vector3 DRESS_OFFSET	= MyLib::Vector3(0.0f, -140.0f, 0.0f);	// �����ւ��I�t�Z�b�g
	}

	namespace frame
	{
		namespace name
		{
			const float PLUS_EDGE = 10.0f;	// �t���[�������g��
		}
		namespace dress
		{
			const MyLib::Vector3 OFFSET	= MyLib::Vector3(0.0f, 55.0f, 0.0f);	// �I�t�Z�b�g
			const MyLib::Vector2 SIZE	= MyLib::Vector2(110.0f, 200.0f);			// �傫��
		}
		namespace area
		{
			const float PLUS_EDGE	= 10.0f;	// �t���[�������g��
			const float OFFSET_Y	= -58.0f;	// Y�I�t�Z�b�g
		}
		namespace trans
		{
			const float PLUS_EDGE = 10.0f;	// �t���[�������g��
			const float OFFSET_Y = -58.0f;	// Y�I�t�Z�b�g
		}
	}
}

//************************************************************
//	�֐��|�C���^
//************************************************************
// �I���֐�
CSelectUI::SELECT_FUNC CSelectUI::m_SelectFuncList[] =
{
	&CSelectUI::UpdateName,		// ���O�̍X�V
	&CSelectUI::UpdateDressup,	// �����ւ��̍X�V
	&CSelectUI::UpdateArea,		// �|�W�V�����̍X�V
	&CSelectUI::UpdateTrans,	// �J�ڂ̍X�V
};

//************************************************************
//	�q�N���X [CSelectUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSelectUI::CSelectUI(CGameManager::ETeamSide team, const int nPlayerIdx, const int nPadIdx) : CObject(PRIO_BG, LAYER::LAYER_2D),
	m_select	(MyLib::PosGrid2(nPlayerIdx, SELECT_DRESSUP)),	// �I���C���f�b�N�X
	m_pPadUI	(nullptr),			// �R���g���[���[UI���
	m_pFrame	(nullptr),			// �t���[�����
	m_nPadIdx	(nPadIdx),			// ���쌠�C���f�b�N�X
	m_bSelect	(true),				// �I�𑀍�t���O
	m_team		(team)				// �`�[��
{
	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_SelectFuncList) == CSelectUI::SELECT_MAX, "ERROR : Select Count Mismatch");
	static_assert(NUM_ARRAY(NUM_SELECTX)	  == CSelectUI::SELECT_MAX, "ERROR : Select Count Mismatch");
}

//============================================================
//	�f�X�g���N�^
//============================================================
CSelectUI::~CSelectUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSelectUI::Init()
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
void CSelectUI::Uninit()
{
	// ���ɔj���ς݂̏ꍇ������
	if (IsDeath()) { return; }

	// �R���g���[���[UI�̏I��
	SAFE_UNINIT(m_pPadUI);

	// �t���[���̏I��
	SAFE_UNINIT(m_pFrame);

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�폜����
//============================================================
void CSelectUI::Kill()
{
	// ���g�̏I��
	CSelectUI::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSelectUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �I���̍X�V
	UpdateSelect();

	// ���Έʒu�̐ݒ�
	SetPositionRelative();

	if (m_SelectFuncList[m_select.y] != nullptr)
	{ // �I���X�V�֐�������ꍇ

		// �I��ʏ���
		(this->*(m_SelectFuncList[m_select.y]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//============================================================
//	�`�揈��
//============================================================
void CSelectUI::Draw()
{

}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CSelectUI::SetEnableDisp(const bool bDraw)
{
	// ���g�̕`��󋵂�ݒ�
	CObject::SetEnableDisp(bDraw);

	// �R���g���[���[UI�̕`��󋵂�ݒ�
	m_pPadUI->SetEnableDisp(bDraw);

	// �t���[���̕`��󋵂�ݒ�
	m_pFrame->SetEnableDisp(bDraw);
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CSelectUI::SetPosition(const MyLib::Vector3& pos)
{
	// �ݒ肷��ʒu��ۑ�
	CObject::SetPosition(pos);

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	��������
//============================================================
CSelectUI *CSelectUI::Create(CGameManager::ETeamSide team, const int nPlayerIdx, const int nPadIdx, const MyLib::Vector3& pos)
{
	// �I��UI�̐���
	CSelectUI* pSelectUI = DEBUG_NEW CSelectUI(team, nPlayerIdx, nPadIdx);
	if (pSelectUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �I��UI�̏�����
		if (FAILED(pSelectUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �I��UI�̔j��
			SAFE_DELETE(pSelectUI);
			return nullptr;
		}

		// UI�ʒu�𔽉f
		pSelectUI->SetPosition(pos);

		// �m�ۂ����A�h���X��Ԃ�
		return pSelectUI;
	}
}

//============================================================
//	���O�̍X�V����
//============================================================
void CSelectUI::UpdateName(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����ւ��V�[���̎擾
	CEntry* pEntry = CEntry::GetInstance();						// �G���g���[���[�h���
	if (pEntry == nullptr) { assert(false); return; }			// �G���g���[���[�h���Ȃ��ꍇ������
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// �G���g���[�V�[�����
	if (pEntryScene == nullptr) { assert(false); return; }		// �G���g���[�V�[�����Ȃ��ꍇ������
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// �����ւ��V�[�����
	if (pDressup == nullptr) { assert(false); return; }			// �����ւ��V�[�����Ȃ��ꍇ������

	// X�I�������`�[���C���f�b�N�X�ɂ���
	m_select.x = m_team;

	// �I�𑀍삪�ł��Ȃ��ꍇ������
	if (!IsSelectOK()) { return; }

	// ���ɖ��O���͒��̏ꍇ������
	if (CInputKeyButton::GetInstance() != nullptr) { return; }

	// ����̍X�V
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���
	if (pPad->GetTrigger(CInputGamepad::BUTTON_A, m_nPadIdx))
	{
		// ���O�ύX�N���X�̐���
		CString2D* pName = pDressup->GetNameString2D(m_team);	// ���O������
		CInputKeyButton::Create(m_nPadIdx, pName->GetStr(), pName);
	}
}

//============================================================
//	�����ւ��̍X�V����
//============================================================
void CSelectUI::UpdateDressup(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsSelectOK())
	{ // �I�𑀍쒆�̏ꍇ

		// �I���v���C���[�̍X�V
		UpdateSelectPlayer();

		// ����̍X�V
		UpdateDecideDressup();
	}
	else
	{ // �����ւ����쒆�̏ꍇ

		// �L�����Z���̍X�V
		UpdateCancelDressup();
	}

	// �����ւ��V�[���̎擾
	CEntry* pEntry = CEntry::GetInstance();						// �G���g���[���[�h���
	if (pEntry == nullptr) { assert(false); return; }			// �G���g���[���[�h���Ȃ��ꍇ������
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// �G���g���[�V�[�����
	if (pEntryScene == nullptr) { assert(false); return; }		// �G���g���[�V�[�����Ȃ��ꍇ������
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// �����ւ��V�[�����
	if (pDressup == nullptr) { assert(false); return; }			// �����ւ��V�[�����Ȃ��ꍇ������

	// UI�ʒu�𔽉f
	SetPosition(pDressup->GetDressUIPosition(m_select.x));
}

//============================================================
//	�|�W�V�����̍X�V����
//============================================================
void CSelectUI::UpdateArea(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����ւ��V�[���̎擾
	CEntry* pEntry = CEntry::GetInstance();						// �G���g���[���[�h���
	if (pEntry == nullptr) { assert(false); return; }			// �G���g���[���[�h���Ȃ��ꍇ������
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// �G���g���[�V�[�����
	if (pEntryScene == nullptr) { assert(false); return; }		// �G���g���[�V�[�����Ȃ��ꍇ������
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// �����ւ��V�[�����
	if (pDressup == nullptr) { assert(false); return; }			// �����ւ��V�[�����Ȃ��ꍇ������

	// �I�𑀍삪�ł��Ȃ��ꍇ������
	if (!IsSelectOK()) { return; }

	// X�I���̍X�V
	UpdateSelectX(m_select.y);

	// ����̍X�V
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���
	if (pPad->GetTrigger(CInputGamepad::BUTTON_A, m_nPadIdx))
	{
		// �����ւ�UI�̃|�W�V�����ύX
		pDressup->ChangeDressUIArea((CGameManager::ETeamSide)m_select.x);
	}
}

//============================================================
//	�J�ڂ̍X�V����
//============================================================
void CSelectUI::UpdateTrans(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����ւ��V�[���̎擾
	CEntry* pEntry = CEntry::GetInstance();						// �G���g���[���[�h���
	if (pEntry == nullptr) { assert(false); return; }			// �G���g���[���[�h���Ȃ��ꍇ������
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// �G���g���[�V�[�����
	if (pEntryScene == nullptr) { assert(false); return; }		// �G���g���[�V�[�����Ȃ��ꍇ������
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// �����ւ��V�[�����
	if (pDressup == nullptr) { assert(false); return; }			// �����ւ��V�[�����Ȃ��ꍇ������

	// �I�𑀍삪�ł��Ȃ��ꍇ������
	if (!IsSelectOK()) { return; }

	// X�I���̍X�V
	UpdateSelectX(m_select.y);

	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���
	switch (m_select.x)
	{ // X�I�����Ƃ̏���
	case CEntry_Dressup::TRANS_BACK:	// �߂�I��
	{
		// ����̍X�V
		if (pPad->GetTrigger(CInputGamepad::BUTTON_A, m_nPadIdx))
		{
			// �`�[���ݒ�V�[���֑J��
			pEntry->ChangeEntryScene(CEntry::ESceneType::SCENETYPE_SETUPTEAM);
		}
		break;
	}
	case CEntry_Dressup::TRANS_NEXT:	// �i�ޑI��
	{
		// ����̍X�V
		if (pPad->GetTrigger(CInputGamepad::BUTTON_A, m_nPadIdx))
		{
			// �Q�[���ݒ�J��
			pDressup->TransSetting();
		}
		break;
	}
	default:
		assert(false);
		break;
	}
}

//============================================================
//	UI�̐�������
//============================================================
HRESULT CSelectUI::CreateUI()
{
	// �t���[���̐���
	m_pFrame = CObject2D::Create(PRIO_BG);
	if (m_pFrame == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �傫���̐ݒ�
	m_pFrame->SetSize(frame::dress::SIZE);

	// �R���g���[���[UI�̐���
	m_pPadUI = CObject2D_Anim::Create
	( // ����
		VEC3_ZERO,		// �ʒu
		pad::PTRN.x,	// �e�N�X�`����������
		pad::PTRN.y,	// �e�N�X�`���c������
		0.0f,			// �Đ�����
		false,			// �����j��
		PRIO_UI			// �D�揇��
	);
	if (m_pPadUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �����Đ���OFF�ɂ���
	m_pPadUI->SetEnableAutoPlay(false);

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist(pad::TEXTURE);
	m_pPadUI->BindTexture(nTexID);

	// �e�N�X�`���p�^�[���̐ݒ�
	m_pPadUI->SetPatternAnim(m_nPadIdx);

	// ���������ɃT�C�Y��ݒ�
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, pad::OTHER_WIDTH);
	size.y *= (float)pad::PTRN.x;
	m_pPadUI->SetSize(size);
	m_pPadUI->SetSizeOrigin(m_pPadUI->GetSize());

	return S_OK;
}

//============================================================
//	�I���̍X�V����
//============================================================
void CSelectUI::UpdateSelect()
{
	// �I�𑀍삪�ł��Ȃ��ꍇ������
	if (!IsSelectOK()) { return; }

	// �I���̍X�V
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���
	if (pPad->GetTrigger(CInputGamepad::BUTTON_UP, m_nPadIdx))
	{
		// �����ւ��V�[���̎擾
		CEntry* pEntry = CEntry::GetInstance();						// �G���g���[���[�h���
		if (pEntry == nullptr) { assert(false); return; }			// �G���g���[���[�h���Ȃ��ꍇ������
		CEntryScene* pEntryScene = pEntry->GetEntryScene();			// �G���g���[�V�[�����
		if (pEntryScene == nullptr) { assert(false); return; }		// �G���g���[�V�[�����Ȃ��ꍇ������
		CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// �����ւ��V�[�����
		if (pDressup == nullptr) { assert(false); return; }			// �����ւ��V�[�����Ȃ��ꍇ������

		// ����ȏ��ɍs���Ȃ��ꍇ������
		if (m_select.y <= 0) { return; }

		// Y�ړ�����X�␳�ʒu���擾�ł��Ȃ��ꍇ������
		int nSelectX = GetMoveYSelectX(m_select.y - 1);
		if (nSelectX == -1) { return; }

		// �N�������O�ɂ���ꍇ������
		if (m_select.y - 1 == SELECT_NAME && !pDressup->IsNameSelectOK(m_team)) { return; }

		// X�I���ʒu��␳
		m_select.x = nSelectX;

		// ��Ɉړ�
		m_select.y--;
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON_DOWN, m_nPadIdx))
	{
		// ����ȏ㉺�ɍs���Ȃ��ꍇ������
		if (m_select.y >= SELECT_MAX - 1) { return; }

		// Y�ړ�����X�␳�ʒu�̎擾
		int nSelectX = m_select.x = GetMoveYSelectX(m_select.y + 1);
		if (nSelectX == -1) { return; }

		// X�I���ʒu��␳
		m_select.x = nSelectX;

		// ���Ɉړ�
		m_select.y++;
	}
}

//============================================================
//	����̍X�V����
//============================================================
void CSelectUI::UpdateDecideDressup()
{
	// �����ւ��V�[���̎擾
	CEntry* pEntry = CEntry::GetInstance();						// �G���g���[���[�h���
	if (pEntry == nullptr) { assert(false); return; }			// �G���g���[���[�h���Ȃ��ꍇ������
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// �G���g���[�V�[�����
	if (pEntryScene == nullptr) { assert(false); return; }		// �G���g���[�V�[�����Ȃ��ꍇ������
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// �����ւ��V�[�����
	if (pDressup == nullptr) { assert(false); return; }			// �����ւ��V�[�����Ȃ��ꍇ������

	// ����̍X�V
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���
	if (pPad->GetTrigger(CInputGamepad::BUTTON_A, m_nPadIdx))
	{
		// ���͏��̏�����
		pPad->InitTrigger(m_nPadIdx);

		// �����ւ�UI���������t���O�̏�����
		pDressup->SetDressUIReady(m_select.x, false);

		// �����ւ�UI���쌠�̐ݒ�
		pDressup->SetDressUIControl(m_nPadIdx, m_select.x);

		// �I�𑀍���~
		m_bSelect = false;
	}
}

//============================================================
//	�L�����Z���̍X�V����
//============================================================
void CSelectUI::UpdateCancelDressup()
{
	// �����ւ��V�[���̎擾
	CEntry* pEntry = CEntry::GetInstance();						// �G���g���[���[�h���
	if (pEntry == nullptr) { assert(false); return; }			// �G���g���[���[�h���Ȃ��ꍇ������
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// �G���g���[�V�[�����
	if (pEntryScene == nullptr) { assert(false); return; }		// �G���g���[�V�[�����Ȃ��ꍇ������
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// �����ւ��V�[�����
	if (pDressup == nullptr) { assert(false); return; }			// �����ւ��V�[�����Ȃ��ꍇ������

	// ���������ς݂̏ꍇ����s��
	if (pDressup->IsDressUIReady(m_select.x)) { return; }

	// �L�����Z���̍X�V
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���
	if (pPad->GetTrigger(CInputGamepad::BUTTON_B, m_nPadIdx))
	{
		// �����ւ�UI���쌠�̏�����
		pDressup->SetDressUIControl(-1, m_select.x);

		// �I�𑀍���ĊJ
		m_bSelect = true;
	}
}

//============================================================
//	�I���v���C���[�̍X�V����
//============================================================
void CSelectUI::UpdateSelectPlayer()
{
	// �����ւ��V�[���̎擾
	CEntry* pEntry = CEntry::GetInstance();						// �G���g���[���[�h���
	if (pEntry == nullptr) { assert(false); return; }			// �G���g���[���[�h���Ȃ��ꍇ������
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// �G���g���[�V�[�����
	if (pEntryScene == nullptr) { assert(false); return; }		// �G���g���[�V�[�����Ȃ��ꍇ������
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// �����ւ��V�[�����
	if (pDressup == nullptr) { assert(false); return; }			// �����ւ��V�[�����Ȃ��ꍇ������

	// �I���̍X�V
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���
	if (pPad->GetTrigger(CInputGamepad::BUTTON_LEFT, m_nPadIdx))
	{
		const int nNumPlayer = pDressup->GetNumDressUI();	// �v���C���[��
		int nLoop = 0;	// ���[�v��
		do {
			// ���ɑI�������炷
			m_select.x = (m_select.x + (nNumPlayer - 1)) % nNumPlayer;

			// �I������������ꍇ������
			nLoop++;
			if (nLoop >= nNumPlayer) { break; }

		} while (!pDressup->IsDressSelectOK(m_nPadIdx, m_select.x));
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON_RIGHT, m_nPadIdx))
	{
		const int nNumPlayer = pDressup->GetNumDressUI();	// �v���C���[��
		int nLoop = 0;	// ���[�v��
		do {
			// �E�ɑI�������炷
			m_select.x = (m_select.x + 1) % nNumPlayer;

			// �I������������ꍇ������
			nLoop++;
			if (nLoop >= nNumPlayer) { break; }

		} while (!pDressup->IsDressSelectOK(m_nPadIdx, m_select.x));
	}
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CSelectUI::SetPositionRelative()
{
	// �����ւ��V�[���̎擾
	CEntry* pEntry = CEntry::GetInstance();						// �G���g���[���[�h���
	if (pEntry == nullptr) { assert(false); return; }			// �G���g���[���[�h���Ȃ��ꍇ������
	CEntryScene* pEntryScene = pEntry->GetEntryScene();			// �G���g���[�V�[�����
	if (pEntryScene == nullptr) { assert(false); return; }		// �G���g���[�V�[�����Ȃ��ꍇ������
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// �����ւ��V�[�����
	if (pDressup == nullptr) { return; }						// �����ւ��V�[�����Ȃ��ꍇ������

	switch (m_select.y)
	{ // �c�I�����Ƃ̏���
	case SELECT_NAME:
	{
		// �t���[�����̐ݒ�
		MyLib::Vector3 posName = pDressup->GetNameUIPosition(m_team);	// ���O�̈ʒu
		m_pFrame->SetPosition(posName);									// �ʒu�ݒ�
		m_pFrame->SetSize(pDressup->GetNameUISize(m_team) + frame::name::PLUS_EDGE);	// �傫���ݒ�

		// �R���g���[���[UI�̈ʒu�ݒ�
		m_pPadUI->SetPosition(posName + pad::NAME_OFFSET);

		// �R���g���[���[UI�̑傫���ݒ�
		SetWidthPadUI(pad::NAME_WIDTH);
		break;
	}
	case SELECT_DRESSUP:
	{
		MyLib::Vector3 posThis = GetPosition();	// ���g�̈ʒu

		// �t���[�����̐ݒ�
		m_pFrame->SetPosition(posThis + frame::dress::OFFSET);	// �ʒu�ݒ�
		m_pFrame->SetSize(frame::dress::SIZE);	// �傫���ݒ�

		// �R���g���[���[UI�̈ʒu�ݒ�
		m_pPadUI->SetPosition(posThis + pad::DRESS_OFFSET);

		// �R���g���[���[UI�̑傫���ݒ�
		SetWidthPadUI(pad::DRESS_WIDTH);
		break;
	}
	case SELECT_AREA:
	{
		// �t���[�����̐ݒ�
		const MyLib::Vector3 posArea = pDressup->GetAreaUIPosition((CPlayer::EFieldArea)m_select.x);	// �|�W�V����UI�ʒu
		const MyLib::Vector2 sizeArea = pDressup->GetAreaUISize((CPlayer::EFieldArea)m_select.x);		// �|�W�V����UI�傫��
		m_pFrame->SetPosition(posArea);							// �ʒu�ݒ�
		m_pFrame->SetSize(sizeArea + frame::area::PLUS_EDGE);	// �傫���ݒ�

		// �R���g���[���[UI�̈ʒu�ݒ�
		const float	fHalfWidthPad = m_pPadUI->GetSize().x;			// �p�b�h����
		const float	fMaxWidth = sizeArea.x * 2.0f - fHalfWidthPad;	// �g�p����
		const int	nPadIdx = std::abs(m_nPadIdx + (-(mylib_const::MAX_PLAYER - 1) * m_select.x));	// �v�Z�p�p�b�h�C���f�b�N�X
		const float	fOffset = (fMaxWidth / (float)mylib_const::MAX_PLAYER) * nPadIdx - (fMaxWidth * 0.5f) + fHalfWidthPad;	// �ʒu�I�t�Z�b�g
		m_pPadUI->SetPosition(posArea + MyLib::Vector3(fOffset, frame::area::OFFSET_Y, 0.0f));

		// �R���g���[���[UI�̑傫���ݒ�
		SetWidthPadUI(pad::OTHER_WIDTH);
		break;
	}
	case SELECT_TRANS:
	{
		// �t���[�����̐ݒ�
		const MyLib::Vector3 posTrans = pDressup->GetTransUIPosition((CEntry_Dressup::ETrans)m_select.x);	// �J��UI�ʒu
		const MyLib::Vector2 sizeTrans = pDressup->GetTransUISize((CEntry_Dressup::ETrans)m_select.x);		// �J��UI�傫��
		m_pFrame->SetPosition(posTrans);						// �ʒu�ݒ�
		m_pFrame->SetSize(sizeTrans + frame::trans::PLUS_EDGE);	// �傫���ݒ�

		// �R���g���[���[UI�̈ʒu�ݒ�
		const float fSide = (m_select.x == 0) ? 1.0f : -1.0f;	// �I�t�Z�b�g�J�n����
		const float fWidthPad = (m_pPadUI->GetSize().x * 2.0f);	// �p�b�h����
		const float fOffset = (fWidthPad * m_nPadIdx) - sizeTrans.x + (fWidthPad * 0.5f);	// �p�b�h�ʒu�I�t�Z�b�g
		m_pPadUI->SetPosition(posTrans + MyLib::Vector3(fOffset * fSide, frame::trans::OFFSET_Y, 0.0f));	// �p�b�h�ʒu���f

		// �R���g���[���[UI�̑傫���ݒ�
		SetWidthPadUI(pad::OTHER_WIDTH);
		break;
	}
	default:
		assert(false);
		break;
	}
}

//============================================================
//	�I�𑀍�\���̊m�F����
//============================================================
bool CSelectUI::IsSelectOK() const
{
	// �����ւ��V�[���̎擾
	CEntry* pEntry = CEntry::GetInstance();							// �G���g���[���[�h���
	if (pEntry == nullptr) { assert(false); return false; }			// �G���g���[���[�h���Ȃ��ꍇ������
	CEntryScene* pEntryScene = pEntry->GetEntryScene();				// �G���g���[�V�[�����
	if (pEntryScene == nullptr) { assert(false); return false; }	// �G���g���[�V�[�����Ȃ��ꍇ������
	CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();		// �����ւ��V�[�����
	if (pDressup == nullptr) { assert(false); return false; }		// �����ւ��V�[�����Ȃ��ꍇ������

	// �I�𑀍삪�\���A�����ւ���Ԃ̏ꍇ
	return (m_bSelect && pDressup->GetState() == CEntry_Dressup::STATE_DRESSUP);
}

//============================================================
//	�p�b�hUI�̑傫���ݒ菈��
//============================================================
void CSelectUI::SetWidthPadUI(const float fWidth)
{
	// �R���g���[���[UI�̑傫���v�Z
	CTexture* pTexture = CTexture::GetInstance();								// �e�N�X�`�����
	MyLib::Vector2 size = pTexture->GetImageSize(m_pPadUI->GetIdxTexture());	// �e�N�X�`���傫��
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, fWidth);			// ��������
	size.y *= (float)pad::PTRN.x;												// �c������

	// �R���g���[���[UI�̑傫�����f
	m_pPadUI->SetSize(size);
}

//============================================================
//	X�I�������̎擾����
//============================================================
int CSelectUI::GetNumSelectX(const int nSelectY) const
{
	switch (nSelectY)
	{ // Y�I�����Ƃ̏���
	case SELECT_DRESSUP:
	{
		// �����ւ��V�[���̎擾
		CEntry* pEntry = CEntry::GetInstance();						// �G���g���[���[�h���
		if (pEntry == nullptr) { assert(false); return -1; }		// �G���g���[���[�h���Ȃ��ꍇ������
		CEntryScene* pEntryScene = pEntry->GetEntryScene();			// �G���g���[�V�[�����
		if (pEntryScene == nullptr) { assert(false); return -1; }	// �G���g���[�V�[�����Ȃ��ꍇ������
		CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// �����ւ��V�[�����
		if (pDressup == nullptr) { assert(false); return -1; }		// �����ւ��V�[�����Ȃ��ꍇ������

		// �v���C���[����Ԃ�
		return pDressup->GetNumDressUI();
	}
	case SELECT_NAME:
	case SELECT_AREA:
	case SELECT_TRANS:
	{
		// �萔��Ԃ�
		return NUM_SELECTX[nSelectY];
	}
	default:
		assert(false);
		return -1;
	}
}

//============================================================
//	Y�ړ�����X�␳�ʒu�̎擾����
//============================================================
int CSelectUI::GetMoveYSelectX(const int nNextSelectY)
{
	const int nCurNumX = GetNumSelectX(m_select.y);		// ���݂�X�I��
	const int nNextNumX = GetNumSelectX(nNextSelectY);	// ����X�I��
	const float fCurRate = (float)m_select.x / (float)nCurNumX;	// �I������

	if (nNextSelectY == SELECT_DRESSUP)
	{ // ���̑I���������ւ��̏ꍇ

		// �����ւ��V�[���̎擾
		CEntry* pEntry = CEntry::GetInstance();						// �G���g���[���[�h���
		if (pEntry == nullptr) { assert(false); return -1; }		// �G���g���[���[�h���Ȃ��ꍇ������
		CEntryScene* pEntryScene = pEntry->GetEntryScene();			// �G���g���[�V�[�����
		if (pEntryScene == nullptr) { assert(false); return -1; }	// �G���g���[�V�[�����Ȃ��ꍇ������
		CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// �����ւ��V�[�����
		if (pDressup == nullptr) { assert(false); return -1; }		// �����ւ��V�[�����Ȃ��ꍇ������

		int nNumDressUI = pDressup->GetNumDressUI();
		switch ((int)((float)CGameManager::SIDE_MAX * fCurRate))
		{ // �`�[�����Ƃ̏���
		case CGameManager::SIDE_LEFT:

			for (int i = 0; i < nNumDressUI; i++)
			{ // �����ւ�UI����������J��Ԃ�

				// �I���\�Ȓ����ւ�UI���������ꍇ�Ԃ�
				if (pDressup->IsDressSelectOK(m_nPadIdx, i)) { return i; }
			}
			return -1;

		case CGameManager::SIDE_RIGHT:

			for (int i = nNumDressUI - 1; i >= 0; i--)
			{ // �����ւ�UI�����E����J��Ԃ�

				// �I���\�Ȓ����ւ�UI���������ꍇ�Ԃ�
				if (pDressup->IsDressSelectOK(m_nPadIdx, i)) { return i; }
			}
			return -1;

		default:
			assert(false);
			break;
		}
	}

	// ����X�I���ʒu��Ԃ�
	return (int)((float)nNextNumX * fCurRate);
}

//============================================================
//	X�I���̍X�V����
//============================================================
void CSelectUI::UpdateSelectX(const int nSelectY)
{
	// �I���̍X�V
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���
	if (pPad->GetTrigger(CInputGamepad::BUTTON_LEFT, m_nPadIdx))
	{
		// ���ɑI�������炷
		const int nCurNumX = GetNumSelectX(nSelectY);	// ���݂�X�I��
		m_select.x = (m_select.x + (nCurNumX - 1)) % nCurNumX;
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON_RIGHT, m_nPadIdx))
	{
		// �E�ɑI�������炷
		const int nCurNumX = GetNumSelectX(nSelectY);	// ���݂�X�I��
		m_select.x = (m_select.x + 1) % nCurNumX;
	}
}

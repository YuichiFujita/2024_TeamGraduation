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
	const int PRIO_BG = 4;	// �w�i�̗D�揇��
	const int PRIO_UI = 6;	// UI�̗D�揇��

	const int NUM_SELECTX[] =	// X�I������
	{
		2,	// ���O
		-1,	// �����ւ�
		2,	// �J��
	};

	namespace pad
	{
		const std::string TEXTURE = "data\\TEXTURE\\entry\\playerMarker001.png";	// �R���g���[���[UI�e�N�X�`��
		const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(4, 1);	// �e�N�X�`��������
		const float WIDTH = 55.0f;	// ����
	}

	namespace frame
	{
		namespace dressup
		{
			const MyLib::Vector2 SIZE = MyLib::Vector2(110.0f, 200.0f);	// �傫��
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

	// UI���𔽉f
	m_pFrame->SetPosition(pDressup->GetNameUIPosition(m_team));
	m_pFrame->SetSize(pDressup->GetNameUISize(m_team) + 10.0f);

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
	case 0:	// �߂�I��

		// UI���𔽉f
		m_pFrame->SetPosition(pDressup->GetBackUIPosition());
		m_pFrame->SetSize(pDressup->GetBackUISize() + 10.0f);

		// ����̍X�V
		if (pPad->GetTrigger(CInputGamepad::BUTTON_A, m_nPadIdx))
		{
			// �`�[���ݒ�V�[���֑J��
			pEntry->ChangeEntryScene(CEntry::ESceneType::SCENETYPE_SETUPTEAM);
		}
		break;

	case 1:	// �i�ޑI��

		// UI���𔽉f
		m_pFrame->SetPosition(pDressup->GetEnterUIPosition());
		m_pFrame->SetSize(pDressup->GetEnterUISize() + 10.0f);

		// ����̍X�V
		if (pPad->GetTrigger(CInputGamepad::BUTTON_A, m_nPadIdx))
		{
			// �Q�[���ݒ�J��
			pDressup->TransSetting();
		}
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
	m_pFrame->SetSize(frame::dressup::SIZE);

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
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, pad::WIDTH);
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
		// ����ȏ��ɍs���Ȃ��ꍇ������
		if (m_select.y <= 0) { return; }

		// Y�ړ�����X�␳�ʒu�̎擾
		m_select.x = GetMoveYSelectX(m_select.y - 1);

		// ��Ɉړ�
		m_select.y--;
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON_DOWN, m_nPadIdx))
	{
		// ����ȏ㉺�ɍs���Ȃ��ꍇ������
		if (m_select.y >= SELECT_MAX - 1) { return; }

		// Y�ړ�����X�␳�ʒu�̎擾
		m_select.x = GetMoveYSelectX(m_select.y + 1);

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
		const int nNumPlayer = pDressup->GetNumPlayer();	// �v���C���[��
		int nLoop = 0;	// ���[�v��
		do {
			// ���ɑI�������炷
			m_select.x = (m_select.x + (nNumPlayer - 1)) % nNumPlayer;

			// �I������������ꍇ������
			nLoop++;
			if (nLoop >= nNumPlayer) { break; }

		} while (!pDressup->IsSelectOK(m_nPadIdx, m_select.x));
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON_RIGHT, m_nPadIdx))
	{
		const int nNumPlayer = pDressup->GetNumPlayer();	// �v���C���[��
		int nLoop = 0;	// ���[�v��
		do {
			// �E�ɑI�������炷
			m_select.x = (m_select.x + 1) % nNumPlayer;

			// �I������������ꍇ������
			nLoop++;
			if (nLoop >= nNumPlayer) { break; }

		} while (!pDressup->IsSelectOK(m_nPadIdx, m_select.x));
	}
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CSelectUI::SetPositionRelative()
{
	// ���g�̈ʒu���擾
	MyLib::Vector3 posThis = GetPosition();

	if (m_select.y == SELECT_DRESSUP)
	{
		// �R���g���[���[UI�̎����`���ON�ɂ���
		m_pPadUI->SetEnableDisp(true);

		// �R���g���[���[UI�̈ʒu�ݒ�
		m_pPadUI->SetPosition(posThis + MyLib::Vector3(0.0f, -155.0f, 0.0f));

		// �t���[���̈ʒu�ݒ�
		m_pFrame->SetPosition(posThis + MyLib::Vector3(0.0f, 55.0f, 0.0f));

		// �t���[���̑傫���ݒ�
		m_pFrame->SetSize(frame::dressup::SIZE);
	}
	else
	{
		// �R���g���[���[UI�̎����`���OFF�ɂ���
		m_pPadUI->SetEnableDisp(false);
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
		return pDressup->GetNumPlayer();
	}
	case SELECT_NAME:
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

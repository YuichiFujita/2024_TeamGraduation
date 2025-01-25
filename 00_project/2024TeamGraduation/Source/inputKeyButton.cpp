//============================================================
//
//	��������͏��� [inputKeyButton.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "inputKeyButton.h"
#include "manager.h"
#include "object2D.h"
#include "string2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* PATH_CHAR[] =	// �����z�u���̑��΃p�X
	{
		"data\\CSV\\char_hiragana.csv",	// �Ђ炪�Ȕz�u���
		"data\\CSV\\char_katakana.csv",	// �J�^�J�i�z�u���
		"data\\CSV\\char_alphabet.csv",	// �A���t�@�x�b�g�z�u���
	};

	const char* STR_BUTTON[] =	// �{�^���̕�����
	{
		"�Ђ炪��",			// �Ђ炪��
		"�J�^�J�i",			// �J�^�J�i
		"�A���t�@�x�b�g",	// �A���t�@�x�b�g
		"��߂�",			// ��߂�
		"��������",			// ��������
		"�����Ă�",			// �����Ă�
	};

	const char* PATH		= "data\\TEXT\\start.txt";	// �e�L�X�g�p�X
	const int PRIORITY		= 7;	// �D�揇��
	const int MAX_STR_NAME	= 10;	// ���O�̍ő啶����

	namespace title
	{	
		const char*	FONT	= "data\\FONT\\�`���[�NS.otf";	// �t�H���g�p�X
		const bool	ITALIC	= false;	// �C�^���b�N
		const float	HEIGHT	= 28.0f;	// �����c��

		const MyLib::Vector3	POS = MyLib::Vector3(VEC3_SCREEN_CENT.x, 80.0f, 0.0f);	// �ʒu
		const MyLib::Vector3	ROT = VEC3_ZERO;		// ����
		const D3DXCOLOR	COL = MyLib::color::White();	// �F
		const EAlignX	ALIGN_X = XALIGN_CENTER;		// ���z�u
	}
	
	namespace name
	{
		const char*	FONT = "data\\FONT\\�`���[�NS.otf";	// �t�H���g�p�X
		const bool	ITALIC = false;	// �C�^���b�N
		const float	HEIGHT = 48.0f;	// �����c��

		const MyLib::Vector3	POS = MyLib::Vector3(VEC3_SCREEN_CENT.x, 150.0f, 0.0f);	// �ʒu
		const MyLib::Vector3	ROT = VEC3_ZERO;		// ����
		const D3DXCOLOR	COL = MyLib::color::White();	// �F
		const EAlignX	ALIGN_X = XALIGN_CENTER;		// ���z�u
	}

	namespace select
	{	
		const MyLib::Vector3 POS[CInputKeyButton::YSELECT_MAX][CInputKeyButton::XSELECT_MAX] =	// �ʒu�z��
		{
			{ MyLib::Vector3(180.0f + 145.0f, 560.0f, 0.0f), MyLib::Vector3(440.0f + 145.0f, 560.0f, 0.0f), MyLib::Vector3(770.0f + 145.0f, 560.0f, 0.0f) },
			{ MyLib::Vector3(220.0f + 145.0f, 630.0f, 0.0f), MyLib::Vector3(460.0f + 145.0f, 630.0f, 0.0f), MyLib::Vector3(720.0f + 145.0f, 630.0f, 0.0f) },
		};

		const CInputKeyButton::ETypeChar INIT_TYPE = CInputKeyButton::TYPECHAR_HIRAGANA;	// ���������Z�b�g
		const MyLib::PosGrid2 INIT_SELECT = MyLib::PosGrid2(0, 2);	// �����I���ʒu

		const char*	FONT		= "data\\FONT\\�`���[�NS.otf";	// �t�H���g�p�X
		const bool	ITALIC		= false;	// �C�^���b�N
		const float	HEIGHT		= 28.0f;	// �����c��
		const float	HEIGHT_BIG	= 34.0f;	// �����c��
		const float	NEXT_TIME	= 0.035f;	// �����U���̑ҋ@����
		const float	MOVE		= 1.0f;		// �U���ړ���
		const MyLib::Vector3 ROT	= VEC3_ZERO;				// ����
		const D3DXCOLOR COL_DEFAULT	= MyLib::color::White();	// �ʏ�F
		const D3DXCOLOR COL_CHOICE	= MyLib::color::Yellow();	// �I��F
		const EAlignX ALIGN_X		= XALIGN_CENTER;			// ���z�u
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CInputKeyButton* CInputKeyButton::m_pInstance = nullptr;	// ���g�̃C���X�^���X

//************************************************************
//	�q�N���X [CInputKeyButton] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CInputKeyButton::CInputKeyButton(const int nPadIdx, const std::string& rOrigin, CString2D* pChange) :
	m_nPadIdx	(nPadIdx),				// ���쌠�C���f�b�N�X
	m_sOrigin	(rOrigin),				// ���̕�����
	m_pChange	(pChange),				// �ύX���镶����
	m_pBG		(nullptr),				// �w�i
	m_pTitle	(nullptr),				// �^�C�g��
	m_pName		(nullptr),				// ���O
	m_typeChar	((ETypeChar)0),			// �������
	m_curSelect	(MyLib::PosGrid2()),	// ���݂̑I����
	m_oldSelect	(MyLib::PosGrid2())		// �O��̑I����
{
	// �I�𕶎��z����N���A
	m_vecSelect.clear();
}

//============================================================
//	�f�X�g���N�^
//============================================================
CInputKeyButton::~CInputKeyButton()
{

}

//============================================================
//	����������
//============================================================
HRESULT CInputKeyButton::Init()
{
	// �����o�ϐ���������
	m_pBG		= nullptr;				// �w�i
	m_pTitle	= nullptr;				// �^�C�g��
	m_pName		= nullptr;				// ���O
	m_typeChar	= (ETypeChar)0;			// �������
	m_curSelect	= select::INIT_SELECT;	// ���݂̑I����
	m_oldSelect	= select::INIT_SELECT;	// �O��̑I����

	// �I�𕶎��z���������
	m_vecSelect.clear();

	// �w�i�̐���
	m_pBG = CObject2D::Create(6);
	if (m_pBG == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// ���e�N�X�`��������
	CTexture* pTexture = CTexture::GetInstance();
	m_pBG->BindTexture(pTexture->Regist("data\\TEXTURE\\entry\\blackboard.png"));

	// �ʒu��ݒ�
	m_pBG->SetPosition(VEC3_SCREEN_CENT);

	// �傫����ݒ�
	m_pBG->SetSize(VEC2_SCREEN_SIZE * 0.48f);

	// �^�C�g���̐���
	m_pTitle = CString2D::Create
	( // ����
		title::FONT,		// �t�H���g�p�X
		title::ITALIC,		// �C�^���b�N
		L"�`�[����",		// �w�蕶����
		title::POS,			// ���_�ʒu
		title::HEIGHT,		// �����c��
		title::ALIGN_X,		// ���z�u
		title::ROT,			// ���_����
		title::COL,			// �F
		PRIORITY			// �D�揇��
	);
	if (m_pTitle == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// ���O�̐���
	m_pName = CString2D::Create
	( // ����
		name::FONT,		// �t�H���g�p�X
		name::ITALIC,	// �C�^���b�N
		m_sOrigin,		// �w�蕶����
		name::POS,		// ���_�ʒu
		name::HEIGHT,	// �����c��
		name::ALIGN_X,	// ���z�u
		name::ROT,		// ���_����
		name::COL,		// �F
		PRIORITY		// �D�揇��
	);
	if (m_pName == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	for (int i = 0; i < YSELECT_MAX; i++)
	{ // �s�̌Œ�I�����̑������J��Ԃ�

		// ����s���̔z����g��
		m_vecSelect.emplace_back();

		for (int j = 0; j < XSELECT_MAX; j++)
		{ // ��̌Œ�I�����̑������J��Ԃ�

			// �I�����̐���
			int nTextIdx = j + (i * XSELECT_MAX);	// �e�L�X�g�C���f�b�N�X
			CString2D* pSelect = CString2D::Create
			( // ����
				select::FONT,			// �t�H���g�p�X
				select::ITALIC,			// �C�^���b�N
				STR_BUTTON[nTextIdx],	// �w�蕶����
				select::POS[i][j],		// ���_�ʒu
				select::HEIGHT_BIG,		// �����c��
				select::ALIGN_X,		// ���z�u
				select::ROT,			// ���_����
				select::COL_DEFAULT,	// �F
				PRIORITY				// �D�揇��
			);
			if (pSelect == nullptr)
			{ // �����Ɏ��s�����ꍇ

				assert(false);
				return E_FAIL;
			}

			// ���݂̍s��̍Ō���ɐ�������������ǉ�
			m_vecSelect.back().push_back(pSelect);
		}
	}

	// �z�u�̓Ǎ�
	if (FAILED(ChangeChar(select::INIT_TYPE)))
	{ // �Ǎ��Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// ���E��~���ɓ�����悤�ɂ���
	SetEnablePosibleMove_WorldPause(true);

	// ���E�̎����~�߂�
	GET_MANAGER->SetEnableWorldPaused(true);

	// �}�l�[�W���[��ނɕύX
	SetType(CObject::TYPE::TYPE_MANAGER);

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CInputKeyButton::Uninit()
{
	// ���g�̕ۑ��C���X�^���X��������
	m_pInstance = nullptr;

	// ���E�̎��𓮂���
	GET_MANAGER->SetEnableWorldPaused(false);

	// �w�i�̏I��
	SAFE_UNINIT(m_pBG);

	// �^�C�g���̏I��
	SAFE_UNINIT(m_pTitle);

	// ���O�̏I��
	SAFE_UNINIT(m_pName);

	for (int i = 0; i < (int)m_vecSelect.size(); i++)
	{ // �s�̑������J��Ԃ�

		for (int j = 0; j < (int)m_vecSelect[i].size(); j++)
		{ // ��̑������J��Ԃ�

			// �I�𕶎��̏I��
			SAFE_UNINIT(m_vecSelect[i][j]);
		}
	}

	// �I�𕶎��z����N���A
	m_vecSelect.clear();

	// �I�u�W�F�N�g�̔j��
	Release();
}

//============================================================
//	�폜����
//============================================================
void CInputKeyButton::Kill()
{
	// ���g�̏I��
	CInputKeyButton::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CInputKeyButton::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �I���̍X�V
	UpdateSelect();

	// ����̍X�V
	UpdateDecide();

	// �L�����Z���̍X�V
	UpdateChancel();

	if (m_pBG != nullptr)
	{
		// �w�i�̍X�V
		m_pBG->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	if (m_pTitle != nullptr)
	{
		// �^�C�g���̍X�V
		m_pTitle->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	if (m_pName != nullptr)
	{
		// ���O�̍X�V
		m_pName->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	for (int i = 0; i < (int)m_vecSelect.size(); i++)
	{ // �s�̑������J��Ԃ�

		for (int j = 0; j < (int)m_vecSelect[i].size(); j++)
		{ // ��̑������J��Ԃ�

			if (m_vecSelect[i][j] != nullptr)
			{
				// �I�𕶎��̍X�V
				m_vecSelect[i][j]->Update(fDeltaTime, fDeltaRate, fSlowRate);
			}
		}
	}
}

//============================================================
//	�`�揈��
//============================================================
void CInputKeyButton::Draw()
{

}

//============================================================
//	��������
//============================================================
CInputKeyButton* CInputKeyButton::Create
(
	const int nPadIdx,			// ���쌠�C���f�b�N�X
	const std::string& rOrigin,	// �ύX�O�̕�����
	CString2D* pChange			// �ύX���镶����
)
{
	// �C���X�^���X�̐���
	assert(m_pInstance == nullptr);
	m_pInstance = DEBUG_NEW CInputKeyButton(nPadIdx, rOrigin, pChange);
	if (m_pInstance != nullptr)
	{ // �����ɐ��������ꍇ

		// �N���X�̏�����
		if (FAILED(m_pInstance->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(m_pInstance);
			return nullptr;
		}
	}

	// �C���X�^���X��Ԃ�
	return m_pInstance;
}

//============================================================
//	�C���X�^���X�擾����
//============================================================
CInputKeyButton* CInputKeyButton::GetInstance()
{
	// �C���X�^���X��Ԃ�
	return m_pInstance;
}

//============================================================
//	�I���̑��쏈��
//============================================================
void CInputKeyButton::ControlSelect()
{
	CInputKeyboard*	pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CInputGamepad*	pPad = GET_INPUTPAD;	// �p�b�h���
	if (pKey->GetTrigger(DIK_LEFT) || pPad->GetTrigger(CInputGamepad::BUTTON_LEFT, m_nPadIdx))
	{
		do { // �I��悪�Ȃ��ꍇ����ɓ�����

			// ���ɑI�������炷
			int nMaxWidth = (int)m_vecSelect[m_curSelect.y].size();	// ��̍ő吔
			m_curSelect.x = (m_curSelect.x + (nMaxWidth - 1)) % nMaxWidth;

		} while (m_vecSelect[m_curSelect.y][m_curSelect.x] == nullptr);
	}
	if (pKey->GetTrigger(DIK_RIGHT) || pPad->GetTrigger(CInputGamepad::BUTTON_RIGHT, m_nPadIdx))
	{
		do { // �I��悪�Ȃ��ꍇ����ɓ�����

			// �E�ɑI�������炷
			int nMaxWidth = (int)m_vecSelect[m_curSelect.y].size();	// ��̍ő吔
			m_curSelect.x = (m_curSelect.x + 1) % nMaxWidth;

		} while (m_vecSelect[m_curSelect.y][m_curSelect.x] == nullptr);
	}
	if (pKey->GetTrigger(DIK_UP) || pPad->GetTrigger(CInputGamepad::BUTTON_UP, m_nPadIdx))
	{
		do { // �I��悪�Ȃ��ꍇ����ɓ�����

			// ��ɑI�������炷
			int nMaxHeight = (int)m_vecSelect.size();	// �s�̍ő吔
			m_curSelect.y = (m_curSelect.y + (nMaxHeight - 1)) % nMaxHeight;

			// �񂪑��������ꍇ�ɗ�C���f�b�N�X��␳
			int nPrevSelectY = (m_curSelect.y + 1) % nMaxHeight;
			if (m_vecSelect[m_curSelect.y].size() < m_vecSelect[nPrevSelectY].size())
			{ // �񂪌��������ꍇ

				// ��C���f�b�N�X�����������̍ő�l�ŕ␳
				m_curSelect.x /= m_vecSelect[m_oldSelect.y].size() / XSELECT_MAX;
				UtilFunc::Transformation::ValueNormalize(m_curSelect.x, (int)(m_vecSelect[m_curSelect.y].size() - 1), 0);
			}
			else if (m_vecSelect[m_curSelect.y].size() > m_vecSelect[nPrevSelectY].size())
			{ // �񂪑��������ꍇ

				// ��C���f�b�N�X��傫�����̍ő�l�ŕ␳
				m_curSelect.x *= m_vecSelect[m_curSelect.y].size() / XSELECT_MAX;
				UtilFunc::Transformation::ValueNormalize(m_curSelect.x, (int)(m_vecSelect[m_curSelect.y].size() - 1), 0);
			}

		} while (m_vecSelect[m_curSelect.y][m_curSelect.x] == nullptr);
	}
	if (pKey->GetTrigger(DIK_DOWN) || pPad->GetTrigger(CInputGamepad::BUTTON_DOWN, m_nPadIdx))
	{
		do { // �I��悪�Ȃ��ꍇ����ɓ�����

			// ���ɑI�������炷
			int nMaxHeight = (int)m_vecSelect.size();	// �s�̍ő吔
			m_curSelect.y = (m_curSelect.y + 1) % nMaxHeight;

			// �񂪑��������ꍇ�ɗ�C���f�b�N�X��␳
			int nPrevSelectY = (m_curSelect.y + (nMaxHeight - 1)) % nMaxHeight;	// ���̍s�I���C���f�b�N�X
			if (m_vecSelect[m_curSelect.y].size() < m_vecSelect[nPrevSelectY].size())
			{ // �񂪌��������ꍇ

				// ��C���f�b�N�X�����������̍ő�l�ŕ␳
				m_curSelect.x /= m_vecSelect[m_oldSelect.y].size() / XSELECT_MAX;
				UtilFunc::Transformation::ValueNormalize(m_curSelect.x, (int)(m_vecSelect[m_curSelect.y].size() - 1), 0);
			}
			else if (m_vecSelect[m_curSelect.y].size() > m_vecSelect[nPrevSelectY].size())
			{ // �񂪑��������ꍇ

				// ��C���f�b�N�X��傫�����̍ő�l�ŕ␳
				m_curSelect.x *= m_vecSelect[m_curSelect.y].size() / XSELECT_MAX;
				UtilFunc::Transformation::ValueNormalize(m_curSelect.x, (int)(m_vecSelect[m_curSelect.y].size() - 1), 0);
			}

		} while (m_vecSelect[m_curSelect.y][m_curSelect.x] == nullptr);
	}
}

//============================================================
//	�I���̍X�V����
//============================================================
void CInputKeyButton::UpdateSelect()
{
	// �O��̑I������ۑ�
	m_oldSelect = m_curSelect;

	// �I���̑���
	ControlSelect();

	// �O��̑I��v�f�̐F�𔒐F�ɐݒ�
	assert(m_vecSelect[m_oldSelect.y][m_oldSelect.x] != nullptr);
	m_vecSelect[m_oldSelect.y][m_oldSelect.x]->SetColor(select::COL_DEFAULT);

	// ���݂̑I��v�f�̐F�����F�ɐݒ�
	assert(m_vecSelect[m_curSelect.y][m_curSelect.x] != nullptr);
	m_vecSelect[m_curSelect.y][m_curSelect.x]->SetColor(select::COL_CHOICE);
}

//============================================================
//	����̍X�V����
//============================================================
void CInputKeyButton::UpdateDecide()
{
	CInputKeyboard*	pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CInputGamepad*	pPad = GET_INPUTPAD;	// �p�b�h���
	if (pKey->GetTrigger(DIK_RETURN)
	||  pPad->GetTrigger(CInputGamepad::BUTTON_A, m_nPadIdx)
	||  pPad->GetTrigger(CInputGamepad::BUTTON_Y, m_nPadIdx))
	{
		// �I�����ɉ����đ����ύX
		std::wstring wsName = m_pName->GetWideStr();	// ���O�̕�����
		switch (m_curSelect.y)
		{ // ���݂̍s�I�������Ƃ̏���
		default:	// �����I��

			if (m_pName->GetNumChar() < MAX_STR_NAME)
			{ // ���������ő�ł͂Ȃ��ꍇ

				// �ǉ����镶������擾
				std::wstring wsAddChar = m_vecSelect[m_curSelect.y][m_curSelect.x]->GetWideStr();

				// �Ō���ɕ�����ǉ�
				wsName.push_back(wsAddChar.front());	// ������Ƃ��ĊǗ����Ă�̂Ő擪�������擾

				// ��������Đݒ�
				m_pName->SetString(wsName);
			}
			break;

		case YSELECT_CHAR_CHANGE:	// �����ύX

			if ((ETypeChar)m_curSelect.x != m_typeChar)
			{ // �ύX�悪�ʂ̕����`���ȏꍇ

				// �I�𒆂̕����ɕύX
				ChangeChar((ETypeChar)m_curSelect.x);
			}
			break;

		case YSELECT_CHAR_DECIDE:	// ��������

			switch (m_curSelect.x)
			{ // ���݂̗�I�������Ƃ̏���
			case XSELECT_LEFT:		// �~�߂�

				// ���g�̏I��
				Uninit();
				break;

			case XSELECT_CENTER:	// �폜

				if (!wsName.empty())
				{ // �������܂�����ꍇ

					// �Ō�����ꕶ���폜
					wsName.erase(wsName.end() - 1);

					// ��������Đݒ�
					m_pName->SetString(wsName);
				}
				break;

			case XSELECT_RIGHT:		// �m��

				if (!wsName.empty())
				{ // �������ݒ肳��Ă���ꍇ

					// �ύX�����������ۑ�
					m_pChange->SetString(m_pName->GetStr());

					// ���g�̏I��
					Uninit();
				}
				break;

			default:
				assert(false);
				break;
			}
			break;
		}
	}
}

//============================================================
//	�L�����Z���̍X�V����
//============================================================
void CInputKeyButton::UpdateChancel()
{
	CInputKeyboard*	pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CInputGamepad*	pPad = GET_INPUTPAD;	// �p�b�h���
	if (pKey->GetTrigger(DIK_SPACE)
	||  pPad->GetTrigger(CInputGamepad::BUTTON_B, m_nPadIdx)
	||  pPad->GetTrigger(CInputGamepad::BUTTON_X, m_nPadIdx))
	{
		// �I�����ɉ����đ����ύX
		std::wstring wsName = m_pName->GetWideStr();	// ���O�̕�����
		if (!wsName.empty())
		{ // �������܂�����ꍇ

			// �Ō�����ꕶ���폜
			wsName.erase(wsName.end() - 1);

			// ��������Đݒ�
			m_pName->SetString(wsName);
		}
	}
}

//============================================================
//	�����̕ύX����
//============================================================
HRESULT CInputKeyButton::ChangeChar(const ETypeChar typeChar)
{
	// ������ނ��s���Ȓl�̏ꍇ������
	if (typeChar <= -1 || typeChar >= TYPECHAR_MAX) { assert(false); return E_FAIL; }

	// �I�𕶎��|���S���̏I��
	for (int i = YSELECT_MAX; i < (int)m_vecSelect.size(); i++)
	{ // �Œ�I�����������s�̑������J��Ԃ�

		for (int j = 0; j < (int)m_vecSelect[i].size(); j++)
		{ // ��̑������J��Ԃ�

			// �I�𕶎��̏I��
			SAFE_UNINIT(m_vecSelect[i][j]);
		}
	}

	// �Œ�I�������������I�𕶎��z����N���A
	m_vecSelect.erase(m_vecSelect.begin() + YSELECT_MAX, m_vecSelect.end());

	// �z�u�̓Ǎ�
	if (FAILED(LoadArray(typeChar)))
	{ // �Ǎ��Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �ݒ肵��������ނ�ۑ�
	m_typeChar = typeChar;

	return S_OK;
}

//============================================================
//	�z�u�̓Ǎ�����
//============================================================
HRESULT CInputKeyButton::LoadArray(const ETypeChar typeChar)
{
	MyLib::Vector3 posOffset	= VEC3_ZERO;	// ���������ʒu
	MyLib::Vector3 posStart		= VEC3_ZERO;	// �����J�n�ʒu
	MyLib::Vector2 charOffset	= VEC2_ZERO;	// �����̃I�t�Z�b�g
	float fSpaceOffset			= 0.0f;			// �󔒂̃I�t�Z�b�g

	// �t�@�C�����J��
	std::ifstream file(PATH_CHAR[typeChar]);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�����Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �t�@�C����Ǎ�
	std::string str;	// �Ǎ�������
	while (std::getline(file, str))
	{ // �t�@�C���̏I�[�ł͂Ȃ��ꍇ���[�v

		// �J���}��؂育�ƂɃf�[�^��Ǎ�
		std::istringstream iss(str);	// ������X�g���[��
		while (std::getline(iss, str, ','))
		{
			if (str == "START_POS")
			{
				// �J�n�ʒu��Ǎ�
				iss >> posStart.x >> posStart.y >> posStart.z;

				// �J�n�ʒu�𐶐��ʒu�ɐݒ�
				posOffset = posStart;
			}
			else if (str == "CHAR_OFFSET")
			{
				// �����̃I�t�Z�b�g��Ǎ�
				iss >> charOffset.x >> charOffset.y;
			}
			else if (str == "SAPCE_OFFSET")
			{
				// �󔒂̃I�t�Z�b�g��Ǎ�
				iss >> fSpaceOffset;
			}
			else if (str == "SETCHAR")
			{
				while (std::getline(file, str))
				{ // �t�@�C���̏I�[�ł͂Ȃ��ꍇ���[�v

					// �I�[�̏ꍇ���������𔲂���
					if (str == "END_SETCHAR") { break; }

					// ����s���̔z����g��
					m_vecSelect.emplace_back();

					// �J���}��؂育�ƂɃf�[�^��Ǎ�
					std::istringstream issChar(str);	// ������X�g���[��
					while (std::getline(issChar, str, ','))
					{
						if (str == "") { continue; }	// �󔒂͖�������
						else if (str == "/s")
						{ // �󔒂�ǂݍ��񂾏ꍇ

							// ���ʒu�ɋ󔒕��̃I�t�Z�b�g��^����
							posOffset.x += fSpaceOffset;
							continue;	// �z��C���f�b�N�X�͐i�߂Ȃ�
						}
						else if (str == "/e")
						{ // �󕶎���ǂݍ��񂾏ꍇ

							// ���݂̍s��̍Ō����nullptr��ǉ�
							m_vecSelect.back().push_back(nullptr);

							// ���ʒu�ɕ������̃I�t�Z�b�g��^����
							posOffset.x += charOffset.x;
						}
						else
						{ // ���ꑀ��ł͂Ȃ��ꍇ

							// �ǂݍ��񂾕����̐���
							CString2D* pChar = CString2D::Create
							( // ����
								select::FONT,		// �t�H���g�p�X
								select::ITALIC,		// �C�^���b�N
								str,				// �w�蕶����
								posOffset,			// ���_�ʒu
								select::HEIGHT,		// �����c��
								select::ALIGN_X,	// ���z�u
								select::ROT,		// ���_����
								select::COL_DEFAULT	// �F
							);

							// ���݂̍s��̍Ō���ɐ�������������ǉ�
							m_vecSelect.back().push_back(pChar);

							// ���ʒu�ɕ������̃I�t�Z�b�g��^����
							posOffset.x += charOffset.x;
						}
					}

					// ���ʒu��擪�ɖ߂�
					posOffset.x = posStart.x;

					// �c�ʒu�ɕ������̃I�t�Z�b�g��^����
					posOffset.y += charOffset.y;
				}
			}
		}
	}

	// �t�@�C�������
	file.close();

	return S_OK;
}

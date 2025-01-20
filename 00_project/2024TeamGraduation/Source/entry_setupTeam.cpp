//=============================================================================
// 
//  �`�[�����ݒ菈�� [entry_setupTeam.cpp]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
// 
//=============================================================================
#include "entry_setupTeam.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"
#include "object2D_Anim.h"
#include "number.h"
#include "transUI.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTFILE	= "data\\TEXT\\entry\\setupTeam.txt";
	const std::string TOP_LINE	= "#==============================================================================";	// �e�L�X�g�̃��C��
	const std::string TEXT_LINE	= "#------------------------------------------------------------------------------";	// �e�L�X�g�̃��C��
	const int PRIORITY = 5;	// �D�揇��

	namespace pad
	{
		const std::string TEXTURE = "data\\TEXTURE\\entry\\playerMarker001.png";	// �R���g���[���[UI�e�N�X�`��
		const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(4, 1);	// �e�N�X�`��������
		const float WIDTH = 25.0f;	// ����

		namespace none
		{
			const MyLib::Vector3 POS = VEC3_SCREEN_CENT;	// ���S�ʒu
			const float OFFSET_Y = 60.0f;	// Y���W�I�t�Z�b�g
		}

		namespace left
		{
			namespace up
			{
				const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, 120.0f, 0.0f);	// ����ʒu
			}

			const MyLib::Vector3 POS = MyLib::Vector3(up::POS.x, VEC3_SCREEN_CENT.y, 0.0f);	// �����S�ʒu
			const float OFFSET_X = 60.0f;	// X���W�I�t�Z�b�g
		}

		namespace right
		{
			namespace up
			{
				const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, 120.0f, 0.0f);	// �E��ʒu
			}

			const MyLib::Vector3 POS = MyLib::Vector3(up::POS.x, VEC3_SCREEN_CENT.y, 0.0f);	// �E���S�ʒu
			const float OFFSET_X = 60.0f;	// X���W�I�t�Z�b�g
		}
	}

	namespace num
	{
		const std::string TEXTURE = "data\\TEXTURE\\number\\number000.png";	// �����e�N�X�`��
		const float WIDTH = 200.0f;	// ����
	}
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEntry_SetUpTeam::CEntry_SetUpTeam() : CEntryScene(),
	m_pTransUI	(nullptr)	// �J��UI���
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�

		m_vecAddIdx[i].clear();		// �ǉ����ꂽ�C���f�b�N�X
		m_apNumInTeam[i] = nullptr;	// �`�[���l�����
		m_nMaxChangeIdx[i] = -1;	// �ő吔�ύX����C���f�b�N�X
		m_nPlayerNum[i] = 1;		// �v���C���[�̐�
	}

	for (int i = 0; i < CGameManager::MAX_PLAYER; i++)
	{ // �`�[�����v�̃v���C���[�������J��Ԃ�

		m_TeamSide[i].team = CGameManager::ETeamSide::SIDE_NONE;	// �`�[���T�C�h
		m_TeamSide[i].nPadIdx = -1;	// ���쌠�C���f�b�N�X
		m_apPadUI[i] = nullptr;		// �R���g���[���[UI���
	}

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // �p�b�h�F���̍ő吔���J��Ԃ�

		m_TeamSide[i].nPadIdx = i;	// ���쌠�C���f�b�N�X
		m_nEntryIdx[i] = -1;		// �G���g���[�̃C���f�b�N�X
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEntry_SetUpTeam::~CEntry_SetUpTeam()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEntry_SetUpTeam::Init()
{
	// �O��̃Z�b�g�A�b�v�Ǎ� // TODO�F�������̏����o����CManager�j�����ɌĂяo���i���̂܂܂��ƑO��̒N���̐ݒ肪���̂܂܂ɂȂ�j
	Load();

	// �`�[���l���̐���
	if (FAILED(CreateNumInTeam()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �R���g���[���[UI�̐���
	if (FAILED(CreatePadUI()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �J��UI�̐���
	if (FAILED(CreateTransUI()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �ǉ����ꂽ�C���f�b�N�X���Z�b�g // TODO�F���񃊃Z�b�g�����Ⴄ����v����
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�

		m_vecAddIdx[i].clear();
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEntry_SetUpTeam::Uninit()
{
	// �ۑ����ꂽ���g�̃A�h���X��������
	CEntry::GetInstance()->ResetSetupTeam();

	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CEntry_SetUpTeam::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�̍X�V
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �`�[���I���̍X�V
	bool bAllReady = SelectTeam();

	// �v���C���[�ő吔�ύX�̍X�V
	ChangeMaxPlayer();

	// UI�̈ʒu�␳
	PosAdjUI(bAllReady, fDeltaTime, fDeltaRate, fSlowRate);

	// �����ւ��J��
	TransDressUp(bAllReady);
}

//==========================================================================
// �`�[���l���̐�������
//==========================================================================
HRESULT CEntry_SetUpTeam::CreateNumInTeam()
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�

		// �`�[���l���̐���
		m_apNumInTeam[i] = CNumber::Create(CMultiNumber::EObjType::OBJTYPE_2D, PRIORITY);
		if (m_apNumInTeam[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}

		// �ʒu�̐ݒ�
		m_apNumInTeam[i]->SetPosition(MyLib::Vector3(320.0f + (640.0f * (float)i), 120.0f, 0.0f));	// TODO�F���������ʒu�̒���

		// �e�N�X�`���̊���
		CTexture* pTexture = CTexture::GetInstance();
		int nTexID = pTexture->Regist(num::TEXTURE);
		m_apNumInTeam[i]->BindTexture(nTexID);

		// �����̐ݒ�
		m_apNumInTeam[i]->SetNum(m_nPlayerNum[i]);

		// ���������ɃT�C�Y��ݒ�
		MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, num::WIDTH);
		size.x /= 10.0f;
		m_apNumInTeam[i]->SetSize(size);
		m_apNumInTeam[i]->SetSizeOrigin(m_apNumInTeam[i]->GetSize());
	}

	return S_OK;
}

//==========================================================================
// �R���g���[���[UI�̐�������
//==========================================================================
HRESULT CEntry_SetUpTeam::CreatePadUI()
{
	for (int i = 0; i < CGameManager::MAX_PLAYER; i++)
	{ // �`�[�����v�̃v���C���[�������J��Ԃ�

		// �R���g���[���[UI�̐���
		m_apPadUI[i] = CObject2D_Anim::Create
		( // ����
			MyLib::Vector3(VEC3_SCREEN_CENT.x, 100.0f + (300.0f * (float)i), 0.0f),	// �ʒu	// TODO�F���������ʒu�̒���
			pad::PTRN.x,	// �e�N�X�`����������
			pad::PTRN.y,	// �e�N�X�`���c������
			0.0f,			// �Đ�����
			false,			// �����j��
			PRIORITY		// �D�揇��
		);
		if (m_apPadUI[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}

		// �����Đ���OFF�ɂ���
		m_apPadUI[i]->SetEnableAutoPlay(false);
		
		// �����`���OFF�ɂ���
		m_apPadUI[i]->SetEnableDisp(false);
		
		// �e�N�X�`���̊���
		CTexture* pTexture = CTexture::GetInstance();
		int nTexID = pTexture->Regist(pad::TEXTURE);
		m_apPadUI[i]->BindTexture(nTexID);
		
		// �e�N�X�`���p�^�[���̐ݒ�
		m_apPadUI[i]->SetPatternAnim(i);
		
		// ���������ɃT�C�Y��ݒ�
		MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, pad::WIDTH);
		size.y *= (float)pad::PTRN.x;
		m_apPadUI[i]->SetSize(size);
		m_apPadUI[i]->SetSizeOrigin(m_apPadUI[i]->GetSize());
	}

	return S_OK;
}

//==========================================================================
// �J��UI�̐�������
//==========================================================================
HRESULT CEntry_SetUpTeam::CreateTransUI()
{
	// �J��UI�̐���
	m_pTransUI = CTransUI::Create();
	if (m_pTransUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �󂫃����o�[��AI����
//==========================================================================
void CEntry_SetUpTeam::FillAI()
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�

		for (int j = 0; j < CGameManager::MAX_PLAYER; j++)
		{ // �`�[�����v�̃v���C���[�������J��Ԃ�

			if (m_TeamSide[j].team == CGameManager::ETeamSide::SIDE_NONE)
			{ // �`�[�������ݒ�̃v���C���[�������ꍇ

				const int nNumInTeam = (int)m_vecAddIdx[i].size();	// ����l��
				if (nNumInTeam <= j && m_nPlayerNum[i] > nNumInTeam)
				{ // �T�C�h���̃v���C���[�����傫�����A�K�v�`�[���l���ɓ��B���Ă��Ȃ��ꍇ

					// ���݂̃`�[����ۑ�
					m_TeamSide[j].team = (CGameManager::ETeamSide)i;

					// ���쌠�C���f�b�N�X��������
					m_TeamSide[j].nPadIdx = -1;

					// AI�C���f�b�N�X�̃����o�[��ǉ�
					m_vecAddIdx[i].push_back(-1);
				}
				else
				{ // ���Ƀ`�[���Q���v���C���[�������Ă���ꍇ

					// ���쌠�C���f�b�N�X��������
					m_TeamSide[j].nPadIdx = -1;
				}
			}
		}
	}
}

//==========================================================================
// UI�̍폜
//==========================================================================
void CEntry_SetUpTeam::KillUI()
{
	// �J��UI�̍폜
	SAFE_KILL(m_pTransUI);

	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�

		// �`�[���l���̍폜
		SAFE_KILL(m_apNumInTeam[i]);
	}

	for (int i = 0; i < CGameManager::MAX_PLAYER; i++)
	{ // �`�[�����v�̃v���C���[�������J��Ԃ�

		// �R���g���[���[UI�̍폜
		SAFE_KILL(m_apPadUI[i]);
	}
}

//==========================================================================
// UI�̈ʒu�␳
//==========================================================================
void CEntry_SetUpTeam::PosAdjUI(const bool bAllReady, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�

		// �`�[���l���̑����𔽉f
		m_apNumInTeam[i]->SetNum(m_nPlayerNum[i]);
	}

	// �R���g���[���[UI�ʒu�␳
	PosAdjPadUI();

	// �J�ڕ\����ON/OFF�؂�ւ�
	m_pTransUI->SetEnableDispUI(bAllReady);
}

//==========================================================================
// �R���g���[���[UI�ʒu�␳
//==========================================================================
void CEntry_SetUpTeam::PosAdjPadUI()
{
	std::vector<CObject2D_Anim*> vecNone;	// �`�[���w��Ȃ��z��
	std::vector<CObject2D_Anim*> vecLeft;	// ���`�[���z��
	std::vector<CObject2D_Anim*> vecRight;	// �E�`�[���z��
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // �p�b�h�F���̍ő吔���J��Ԃ�

		// �v���C���[�C���f�b�N�X���w�肳��Ă��Ȃ��ꍇ����
		if (m_nEntryIdx[i] <= -1) { continue; }

		// �`�[���w�育�Ƃ̔z��ɒǉ�
		const int nCurIdx = m_nEntryIdx[i];	// �G���g���[�C���f�b�N�X
		switch (m_TeamSide[nCurIdx].team)
		{ // �`�[�����Ƃ̏���
		case CGameManager::ETeamSide::SIDE_NONE:
			vecNone.push_back(m_apPadUI[nCurIdx]);
			break;

		case CGameManager::ETeamSide::SIDE_LEFT:
			if (m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_LEFT] != nCurIdx)
			{ vecLeft.push_back(m_apPadUI[nCurIdx]); }
			break;

		case CGameManager::ETeamSide::SIDE_RIGHT:
			if (m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_RIGHT] != nCurIdx)
			{ vecRight.push_back(m_apPadUI[nCurIdx]); }
			break;

		default:
			assert(false);
			break;
		}
	}

	// �z����v�f���\�[�g����
	std::sort(vecNone.begin(),  vecNone.end(),  [](CObject2D_Anim* p1, CObject2D_Anim* p2) { return p1->GetPatternAnim() < p2->GetPatternAnim(); });
	std::sort(vecLeft.begin(),  vecLeft.end(),  [](CObject2D_Anim* p1, CObject2D_Anim* p2) { return p1->GetPatternAnim() < p2->GetPatternAnim(); });
	std::sort(vecRight.begin(), vecRight.end(), [](CObject2D_Anim* p1, CObject2D_Anim* p2) { return p1->GetPatternAnim() < p2->GetPatternAnim(); });

	// �`�[���w��Ȃ��̈ʒu�␳
	const int nNumNone = (int)vecNone.size();	// �v�f��
	int nLoopNone = 0;	// ���[�v��
	for (auto& rNone : vecNone)
	{ // �w��Ȃ��I�𒆂̗v�f�����J��Ԃ�

		MyLib::Vector3 posNone = rNone->GetPosition();	// �ʒu

		// �I�t�Z�b�g��^�����ʒu���v�Z
		posNone.x = pad::none::POS.x;
		posNone.y = pad::none::POS.y - (pad::none::OFFSET_Y * (float)(nNumNone - 1)) * 0.5f + (pad::none::OFFSET_Y * (float)nLoopNone);

		// �ʒu�𔽉f
		rNone->SetPosition(posNone);

		// ���[�v�񐔂����Z
		nLoopNone++;
	}

	// ���`�[���̈ʒu�␳
	const int nNumLeft = (int)vecLeft.size();	// �v�f��
	int nLoopLeft = 0;	// ���[�v��
	for (auto& rLeft : vecLeft)
	{ // �w��Ȃ��I�𒆂̗v�f�����J��Ԃ�

		MyLib::Vector3 posLeft = rLeft->GetPosition();	// �ʒu

		// �I�t�Z�b�g��^�����ʒu���v�Z
		posLeft.x = pad::left::POS.x - (pad::left::OFFSET_X * (float)(nNumLeft - 1)) * 0.5f + (pad::left::OFFSET_X * (float)nLoopLeft);
		posLeft.y = pad::left::POS.y;

		// �ʒu�𔽉f
		rLeft->SetPosition(posLeft);

		// ���[�v�񐔂����Z
		nLoopLeft++;
	}

	// �E�`�[���̈ʒu�␳
	const int nNumRight = (int)vecRight.size();	// �v�f��
	int nLoopRight = 0;	// ���[�v��
	for (auto& rRight : vecRight)
	{ // �w��Ȃ��I�𒆂̗v�f�����J��Ԃ�

		MyLib::Vector3 posRight = rRight->GetPosition();	// �ʒu

		// �I�t�Z�b�g��^�����ʒu���v�Z
		posRight.x = pad::right::POS.x - (pad::right::OFFSET_X * (float)(nNumRight - 1)) * 0.5f + (pad::right::OFFSET_X * (float)nLoopRight);
		posRight.y = pad::right::POS.y;

		// �ʒu�𔽉f
		rRight->SetPosition(posRight);

		// ���[�v�񐔂����Z
		nLoopRight++;
	}

	// ���`�[���l�������̈ʒu�␳
	const int nLeftUpIdx = m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_LEFT];		// ���`�[���l�������p�b�h�C���f�b�N�X
	if (nLeftUpIdx > -1)
	{ // ���삷��v���C���[�������ꍇ

		// �ʒu�𔽉f
		m_apPadUI[nLeftUpIdx]->SetPosition(pad::left::up::POS);
	}

	// �E�`�[���l�������̈ʒu�␳
	const int nRightUpIdx = m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_RIGHT];	// �E�`�[���l�������p�b�h�C���f�b�N�X
	if (nRightUpIdx > -1)
	{ // ���삷��v���C���[�������ꍇ

		// �ʒu�𔽉f
		m_apPadUI[nRightUpIdx]->SetPosition(pad::right::up::POS);
	}
}

//==========================================================================
// �`�[���I��
//==========================================================================
bool CEntry_SetUpTeam::SelectTeam()
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���
	int nNumEntry = 0;	// �G���g���[�l��
	int nNumReady = 0;	// ���������l��

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // �p�b�h�F���̍ő吔���J��Ԃ�

		//--------------------------
		// �G���g���[����
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, i))
		{ // �G���g���[���삪�s��ꂽ�ꍇ

			for (int j = 0; j < mylib_const::MAX_PLAYER; j++)
			{ // �p�b�h�F���̍ő吔���J��Ԃ�

				// ���ɃG���g���[����Ă���ꍇ����
				if (m_nEntryIdx[j] >= 0 && m_nEntryIdx[j] != i) { continue; }

				// ����̃v���C���[�C���f�b�N�X��ǉ�
				m_nEntryIdx[j] = i;

				// �R���g���[���[UI�̎����`���ON�ɂ���
				m_apPadUI[i]->SetEnableDisp(true);
				break;
			}
		}

		// ���G���g���[�̏ꍇ����
		const int nUserIdx = m_nEntryIdx[i];	// ���[�U�[�C���f�b�N�X
		if (nUserIdx <= -1) { continue; }

		// �G���g���[�l�������Z
		nNumEntry++;

		//--------------------------
		// ���E����
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, nUserIdx))
		{ // ���E���삪�s��ꂽ�ꍇ

			std::vector<int>::iterator itr;	// ���������z��C�e���[�^�[
			if (IsUserReady(nUserIdx, &itr))
			{ // �����������Ă���ꍇ

				// ���������z�񂩂�C�e���[�^�[���폜
				m_vecAddIdx[m_TeamSide[nUserIdx].team].erase(itr);
				break;
			}
			else
			{ // �����������Ă��Ȃ��ꍇ

				// �G���g���[������
				DeleteEntry(&m_nEntryIdx[i]);
				break;
			}
		}

		// ���ɏ��������ς݂̏ꍇ����
		if (IsUserReady(nUserIdx)) { nNumReady++; continue; }	// ���������l�������Z

		//--------------------------
		// �`�[���ւ�
		//--------------------------
		if (!IsUserMaxChange(nUserIdx))
		{ // �ő吔�ύX�̑��쌠�������Ă��Ȃ��ꍇ

			if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nUserIdx)
			&&  m_TeamSide[nUserIdx].team != CGameManager::ETeamSide::SIDE_LEFT)
			{ // ���[����Ȃ����ɍ��ړ����삪�s��ꂽ�ꍇ

				// ���Ɉړ�
				m_TeamSide[nUserIdx].team = (m_TeamSide[nUserIdx].team == CGameManager::ETeamSide::SIDE_NONE) ? CGameManager::ETeamSide::SIDE_LEFT : CGameManager::ETeamSide::SIDE_NONE;
			}
			else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nUserIdx)
				 &&  m_TeamSide[nUserIdx].team != CGameManager::ETeamSide::SIDE_RIGHT)
			{ // �E�[����Ȃ����ɉE�ړ����삪�s��ꂽ�ꍇ

				// �E�Ɉړ�
				m_TeamSide[nUserIdx].team = (m_TeamSide[nUserIdx].team == CGameManager::ETeamSide::SIDE_NONE) ? CGameManager::ETeamSide::SIDE_RIGHT : CGameManager::ETeamSide::SIDE_NONE;
			}
		}

		// �I���`�[����ۑ�
		const int nSide = m_TeamSide[nUserIdx].team;

		//--------------------------
		// �`�[�����l���ς�
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, nUserIdx)
		&&  IsUserTeamSelect(nUserIdx)		// �`�[���I��
		&&  m_nMaxChangeIdx[nSide] <= -1)	// �ő吔�ύX�̑��쌠��N�������Ă��Ȃ�
		{ // ��ړ����삪�s��ꂽ�ꍇ

			// �����̃T�C�h��ύX����p�C���f�b�N�X�ێ�
			m_nMaxChangeIdx[nSide] = nUserIdx;
		}

		//--------------------------
		// ������������
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, nUserIdx)
		&&  IsUserTeamSelect(nUserIdx)	// �`�[���I��
		&&  !IsUserMaxChange(nUserIdx))	// �ő吔�ύX�����������Ă��Ȃ�
		{ // �����������삪�s��ꂽ�ꍇ

			const int nCurInTeam = static_cast<int>(m_vecAddIdx[nSide].size());	// ���݂̃`�[���l��
			if (!IsUserReady(nUserIdx) && m_nPlayerNum[nSide] > nCurInTeam)
			{ // �������������A�ő�`�[���l�������̏ꍇ

				// ���������z��ɒǉ�
				m_vecAddIdx[nSide].push_back(nUserIdx);
			}
		}
	}

	// �`�[���Z�b�g�A�b�v���������Ă��邩��Ԃ�
	return (nNumEntry == nNumReady && nNumEntry > 0);	// �S�������������ς݊��G���g���[�l������l�ł�����ꍇtrue
}

//==========================================================================
// �v���C���[�ő吔�ύX
//==========================================================================
void CEntry_SetUpTeam::ChangeMaxPlayer()
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���
	for (int nSide = 0; nSide < CGameManager::ETeamSide::SIDE_MAX; nSide++)
	{ // �`�[�������J��Ԃ�

		// ���쌠�������Ă��Ȃ��ꍇ����
		const int nUserIdx = m_nMaxChangeIdx[nSide];	// ���[�U�[�C���f�b�N�X
		if (nUserIdx <= -1) { continue; }

		//--------------------------
		// �ő吔�ύX����
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nUserIdx))
		{ // ���Z���삪�s��ꂽ�ꍇ

			// �`�[���l�������Z
			m_nPlayerNum[nSide]--;
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nUserIdx))
		{ // ���Z���삪�s��ꂽ�ꍇ

			// �`�[���l�������Z
			m_nPlayerNum[nSide]++;
		}

		// �`�[���l����␳
		m_nPlayerNum[nSide] = UtilFunc::Transformation::Clamp(m_nPlayerNum[nSide], 1, CGameManager::MAX_SIDEPLAYER);

		//--------------------------
		// �ő吔�ύX����
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, nUserIdx))
		{ // ���ړ����삪�s��ꂽ�ꍇ

			// �ύX���쌠��������
			m_nMaxChangeIdx[nSide] = -1;
		}
	}
}

//==========================================================================
// �����ւ��J��
//==========================================================================
void CEntry_SetUpTeam::TransDressUp(const bool bAllReady)
{
	if (m_pTransUI->UpdateDecide())
	{ // �S���̏������������Ă��銎�A���삪�s��ꂽ�ꍇ

		// �󂫃����o�[��AI����
		FillAI();

		// UI�̍폜
		KillUI();

		// �����ւ��V�[���֑J��
		CEntry::GetInstance()->ChangeEntryScene(CEntry::ESceneType::SCENETYPE_DRESSUP);
	}
}

//==========================================================================
// �G���g���[����
//==========================================================================
void CEntry_SetUpTeam::DeleteEntry(int* pEntryIdx)
{
	if (m_nMaxChangeIdx[m_TeamSide[*pEntryIdx].team] == *pEntryIdx)
	{ // �ő吔�ύX�̑��쌠�����g�̏ꍇ

		// �ő吔�ύX�̑��쌠��������
		m_nMaxChangeIdx[m_TeamSide[*pEntryIdx].team] = -1;
	}

	// �R���g���[���[UI�̎����`���OFF�ɂ���
	m_apPadUI[*pEntryIdx]->SetEnableDisp(false);

	// �`�[����������
	m_TeamSide[*pEntryIdx].team = CGameManager::ETeamSide::SIDE_NONE;

	// �G���g���[�C���f�b�N�X��������
	*pEntryIdx = -1;
}

//==========================================================================
// ���������t���O�擾
//==========================================================================
bool CEntry_SetUpTeam::IsUserReady(const int nUserIdx, std::vector<int>::iterator* pItr)
{
	// �`�[�������I���̏ꍇ������
	const int nSide = m_TeamSide[nUserIdx].team;	// �I���`�[��
	if (nSide == CGameManager::ETeamSide::SIDE_NONE) { return false; }

	// ���[�U�[�C���f�b�N�X������ (�����������Ă���ꍇ�͔z����ɃC���f�b�N�X���i�[����Ă���)
	std::vector<int>::iterator itr = std::find(m_vecAddIdx[nSide].begin(), m_vecAddIdx[nSide].end(), nUserIdx);

	// �ۑ��p�|�C���^���w�肳��Ă���ꍇ�C�e���[�^�[��ۑ�
	if (pItr != nullptr) { *pItr = itr; }

	// ���������󋵂�Ԃ�
	return (itr != m_vecAddIdx[nSide].end());
}

//==========================================================================
// �`�[���I�𒆃t���O�擾
//==========================================================================
bool CEntry_SetUpTeam::IsUserTeamSelect(const int nUserIdx)
{
	// �I�𒆃`�[���̎w�肪���邩��Ԃ�
	return (m_TeamSide[nUserIdx].team != CGameManager::ETeamSide::SIDE_NONE);
}

//==========================================================================
// �ő吔�ύX�t���O�擾
//==========================================================================
bool CEntry_SetUpTeam::IsUserMaxChange(const int nUserIdx)
{
	// �`�[�������I���̏ꍇ������
	const int nSide = m_TeamSide[nUserIdx].team;	// �I���`�[��
	if (nSide == CGameManager::ETeamSide::SIDE_NONE) { return false; }

	// �ő吔�ύX�̑��쌠�C���f�b�N�X���������[�U�[����Ԃ�
	return (m_nMaxChangeIdx[nSide] == nUserIdx);
}

//==========================================================================
// �Z�[�u����
//==========================================================================
void CEntry_SetUpTeam::Save()
{
	// �t�@�C�����J��
	std::ofstream File(TEXTFILE);
	if (!File.is_open()) {
		return;
	}

	// �e�L�X�g�t�@�C�����ڎ�
	File << TOP_LINE			<< std::endl;
	File << "# �`�[�����ݒ�"	<< std::endl;
	File << TOP_LINE			<< std::endl;


	// �T�C�h���̐l��
	File << TEXT_LINE			<< std::endl;
	File << "# �v���C���[��"	<< std::endl;
	File << TEXT_LINE			<< std::endl;
	File << "PLAYERNUM = "	<< 
		m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT] << 
		" " <<
		m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT] << std::endl;
	
	File << "" << std::endl;


	// �v���C���[�C���f�b�N�X
	File << TEXT_LINE << std::endl;
	File << "# �v���C���[�C���f�b�N�X" << std::endl;
	File << TEXT_LINE << std::endl;
	
	// ��
	File << "LEFT_ID = ";
	for (int i = 0; i < CGameManager::MAX_SIDEPLAYER; i++)
	{
		if (static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size()) - 1 >= i)
		{// �T�C�Y��
			File << m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][i];
		}
		else
		{
			File << -1;
		}
		File << " ";

	}
	File << std::endl;

	// �E
	File << "RIGHT_ID = ";
	for (int i = 0; i < CGameManager::MAX_SIDEPLAYER; i++)
	{
		if (static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size()) - 1 >= i)
		{// �T�C�Y��
			File << m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][i];
		}
		else
		{
			File << -1;
		}
		File << " ";
	}
	File << std::endl;


	// �t�@�C�������
	File.close();
}

//==========================================================================
// ���[�h
//==========================================================================
void CEntry_SetUpTeam::Load()
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

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT] >>
				m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT];	// �v���C���[��
		}

		if (line.find("LEFT_ID") != std::string::npos)
		{// LEFT_ID�ō����`�[���̃C���f�b�N�X

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// �j��
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].clear();

			// ���T�C�Y
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].resize(CGameManager::MAX_SIDEPLAYER);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][0] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][1] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][2];	// �v���C���[��
		}

		if (line.find("RIGHT_ID") != std::string::npos)
		{// RIGHT_ID�ŉE���`�[���̃C���f�b�N�X

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// �j��
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].clear();

			// ���T�C�Y
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].resize(CGameManager::MAX_SIDEPLAYER);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][0] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][1] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][2];	// �v���C���[��
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
void CEntry_SetUpTeam::Debug()
{

	//--------------------------
	// �G���g���[����
	//--------------------------
	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_RETURN))
	{
		// ����̃v���C���[�C���f�b�N�X��ǉ�
		m_nEntryIdx[0] = 0;

		// �R���g���[���[UI�̎����`���ON�ɂ���
		m_apPadUI[0]->SetEnableDisp(true);

		// ���Ɉړ�
		m_TeamSide[0].team = CGameManager::ETeamSide::SIDE_LEFT;

		// ���������z��ɒǉ�
		m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].push_back(0);
	}

	//--------------------------
	// �V�[���J��
	//--------------------------
	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_X))
	{
		// �����ւ��V�[���֑J��
		CEntry::GetInstance()->ChangeEntryScene(CEntry::ESceneType::SCENETYPE_DRESSUP);
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

		//=============================
		// �����ύX
		//=============================
		//--------------------------
		// ��
		//--------------------------
		{
			ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Entry Left:");
			ImGui::SameLine();
			if (ImGui::ArrowButton("##left", ImGuiDir_Left) &&
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size() > 1)
			{// �폜
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].pop_back();
				m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT]--;		// �v���C���[�̐�
			}
			ImGui::SameLine(0.0f);

			if (ImGui::ArrowButton("##right", ImGuiDir_Right) &&
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size() < CGameManager::MAX_SIDEPLAYER)
			{// �ǉ�
				int size = m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size();
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].push_back(size);
			}
			ImGui::SameLine();
			ImGui::PopID();

			// �T�C�Y
			int segmentSize = static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size());
			ImGui::Text("%d", segmentSize);
		}

		//--------------------------
		// �E
		//--------------------------
		{
			ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Entry Right:");
			ImGui::SameLine();
			if (ImGui::ArrowButton("##left", ImGuiDir_Left) &&
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size() > 1)
			{// �폜
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].pop_back();
			}
			ImGui::SameLine(0.0f);

			if (ImGui::ArrowButton("##right", ImGuiDir_Right) &&
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size() < CGameManager::MAX_SIDEPLAYER)
			{// �ǉ�
				int size = m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size();
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].push_back(size);
			}
			ImGui::SameLine();
			ImGui::PopID();

			// �T�C�Y
			int segmentSize = static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size());
			ImGui::Text("%d", segmentSize);
		}


		//=============================
		// �����ύX
		//=============================
		ImGui::Text("Side Player");
		ImGui::SliderInt("SIDE_LEFT", &m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT], 1, CGameManager::MAX_SIDEPLAYER);
		ImGui::SliderInt("SIDE_RIGHT", &m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT], 1, CGameManager::MAX_SIDEPLAYER);

		// �G���g���[�̃C���f�b�N�X
		for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
		{
			ImGui::Text("EntryIdx[%d] : %d", i, m_nEntryIdx[i]);
		}

		ImGui::TreePop();
	}
}

//==========================================================================
// �p�b�h�C���f�b�N�X�̃G���g���[�C���f�b�N�X�ϊ�
//==========================================================================
int CEntry_SetUpTeam::PadIdxToEntryIdx(int nPadIdx)
{
	if (nPadIdx >= mylib_const::MAX_PLAYER) return -1;
	return m_nEntryIdx[nPadIdx];
}

//==========================================================================
// �v���C���[�C���f�b�N�X�̃p�b�h�C���f�b�N�X�ϊ�
//==========================================================================
int CEntry_SetUpTeam::PlayerIdxToPadIdx(int nPlayerIdx)
{
	if (nPlayerIdx >= CGameManager::MAX_PLAYER) return -1;
	return m_TeamSide[nPlayerIdx].nPadIdx;
}

//==========================================================================
// �`�[���T�C�h�擾
//==========================================================================
CGameManager::ETeamSide CEntry_SetUpTeam::GetTeamSide(int nPlayerIdx)
{
	if (nPlayerIdx >= CGameManager::MAX_PLAYER) return CGameManager::ETeamSide::SIDE_NONE;
	return m_TeamSide[nPlayerIdx].team;
}

//==========================================================================
// �`�[���T�C�h�擾 (AI)
//==========================================================================
CGameManager::ETeamSide CEntry_SetUpTeam::GetTeamSideAI(int nOccurIdx)
{
	int nNumAI = 0;	// AI������
	for (int i = 0; i < CGameManager::MAX_PLAYER; i++)
	{ // �`�[�����v�̃v���C���[�������J��Ԃ�

		// AI�ł͂Ȃ��ꍇ����
		if (m_TeamSide[i].nPadIdx > -1) { continue; }

		// ������AI�𔭌��ł��Ă��Ȃ��ꍇ����
		if (nNumAI < nOccurIdx) { nNumAI++; continue; }	// AI�����������Z

		// �����Ԗڂɔ�������AI�̃`�[���T�C�h��Ԃ�
		return m_TeamSide[i].team;
	}

	// ������AI�����Ȃ���
	assert(false);
	return CGameManager::ETeamSide::SIDE_NONE;
}

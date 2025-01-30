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
#include "padUI.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTFILE	= "data\\TEXT\\entry\\team_setting.txt";
	const std::string TOP_LINE	= "#==============================================================================";	// �e�L�X�g�̃��C��
	const std::string TEXT_LINE	= "#------------------------------------------------------------------------------";	// �e�L�X�g�̃��C��
	const int PRIORITY = 5;	// �D�揇��

	namespace pad
	{
		const std::string TEXTURE = "data\\TEXTURE\\entry\\playerMarker001.png";	// �R���g���[���[UI�e�N�X�`��
		const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(4, 1);	// �e�N�X�`��������
		const float WIDTH = 45.0f;	// ����
		const MyLib::Vector3 ARROWSPACE_NUMBER = MyLib::Vector3(60.0f, 0.0f, 0.0f);	// ���������̋�
		const float ARROWWIDTH_NUMBER = 20.0f;	// ���������̖��T�C�Y

		namespace none
		{
			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x, 550.0f, 0.0f);	// ���S�ʒu
			const float OFFSET_Y = 50.0f;	// Y���W�I�t�Z�b�g
		}

		namespace left
		{
			namespace up
			{
				const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x - 120.0f, 386.0f, 0.0f);	// ����ʒu
			}

			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, 550.0f, 0.0f);	// �����S�ʒu
			const float OFFSET_X = 70.0f;	// X���W�I�t�Z�b�g
		}

		namespace right
		{
			namespace up
			{
				const MyLib::Vector3 POS = MyLib::Vector3(left::up::POS.x + 640.0f, 386.0f, 0.0f);	// �E��ʒu
			}

			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, 550.0f, 0.0f);	// �E���S�ʒu
			const float OFFSET_X = 70.0f;	// X���W�I�t�Z�b�g
		}
		const MyLib::Vector3 OFFSET = MyLib::Vector3(70.0f, 70.0f, 0.0f);	// ���S�ʒu
	}

	namespace num
	{
		const std::string TEXTURE = "data\\TEXTURE\\number\\school.png";	// �����e�N�X�`��
		const float WIDTH = 200.0f;	// ����
		const float ALPHARATE_MAX = 1.0f;	// �s�����x�����̍ő�l
		const float ALPHARATE_MIN = 0.2f;	// �s�����x�����̍ő�l
		const float SCALERATE_MAX = 1.0f;	// �g�劄���̍ő�l
		const float SCALERATE_MIN = 0.95f;	// �g�劄���̍ŏ��l
	}

	namespace teamside
	{
		const std::string TEXTURE = "data\\TEXTURE\\entry\\team.png";	// �����e�N�X�`��
		const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(1, 2);	// �e�N�X�`��������
		const MyLib::Vector3 POS = MyLib::Vector3(640.0f, 310.0f, 0.0f);	// ���S�ʒu
		const float WIDTH = 120.0f;		// ����
	}

	namespace gameset
	{
		const std::string TEXTURE = "data\\TEXTURE\\entry\\board.png";	// �����e�N�X�`��
		const MyLib::Vector3 POS = MyLib::Vector3(640.0f, 360.0f, 0.0f);	// ���S�ʒu
		const float WIDTH = 640.0f;		// ����
	}
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEntry_SetUpTeam::CEntry_SetUpTeam() : CEntryScene(),
	m_pTransUI	(nullptr),	// �J��UI���
	m_pBG		(nullptr)	// �w�i���
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�

		m_apTeamSideUI[i] = nullptr;	// �`�[���T�C�hUI���
		m_vecAddIdx[i].clear();			// �ǉ����ꂽ�C���f�b�N�X
		m_apNumInTeam[i] = nullptr;		// �`�[���l�����
		m_nMaxChangeIdx[i] = -1;		// �ő吔�ύX����C���f�b�N�X
		m_nPlayerNum[i] = 0;			// �v���C���[�̐�
		m_fTimeNumInTeam[i] = 0.0f;		// �`�[���l���̃^�C�}�[
	}

	for (int i = 0; i < CGameManager::MAX_PLAYER; i++)
	{ // �`�[�����v�̃v���C���[�������J��Ԃ�

		m_TeamSide[i].team = CGameManager::ETeamSide::SIDE_NONE;	// �`�[���T�C�h
		m_TeamSide[i].nPadIdx = -1;	// ���쌠�C���f�b�N�X
	}

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // �p�b�h�F���̍ő吔���J��Ԃ�

		m_apPadUI[i] = nullptr;			// �R���g���[���[UI���
		m_TeamSide[i].nPadIdx = i;		// ���쌠�C���f�b�N�X
		m_nEntryIdx[i] = -1;			// �G���g���[�̃C���f�b�N�X
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
	// �w�i�̐���
	if (FAILED(CreateBG()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

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

	// �`�[���T�C�hUI�̐���
	if (FAILED(CreateTeamSideUI()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �O��̃Z�b�g�A�b�v�Ǎ�
	Load();

	// �ǉ����ꂽ�C���f�b�N�X���Z�b�g // TODO�F���񃊃Z�b�g�����Ⴄ����v����
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�

		m_vecAddIdx[i].clear();
	}

	return S_OK;
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
		m_apNumInTeam[i]->SetPosition(MyLib::Vector3(390.0f + (640.0f * (float)i), 386.0f, 0.0f));	// TODO�F���������ʒu�̒���

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
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // �`�[�����v�̃v���C���[�������J��Ԃ�

		// �R���g���[���[UI�̐���
		m_apPadUI[i] = CPadUI::Create
		( // ����
			MyLib::Vector3(VEC3_SCREEN_CENT.x, 100.0f + (50.0f * (float)i), 0.0f),	// �ʒu
			pad::WIDTH,		// ����
			i,				// �e�N�X�`���c������
			PRIORITY		// �D�揇��
		);
		if (m_apPadUI[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}

		// �����`���OFF�ɂ���
		m_apPadUI[i]->SetEnableDisp(false);

		// �e�N�X�`���p�^�[���̐ݒ�
		m_apPadUI[i]->SetPatternAnim(i);
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
// �w�i�̐���
//==========================================================================
HRESULT CEntry_SetUpTeam::CreateBG()
{
	// �J��UI�̐���
	m_pBG = CObject2D::Create(PRIORITY);
	if (m_pBG == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �ʒu�ݒ�
	m_pBG->SetPosition(gameset::POS);

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist(gameset::TEXTURE);
	m_pBG->BindTexture(nTexID);

	// ���������ɃT�C�Y��ݒ�
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, gameset::WIDTH);
	m_pBG->SetSize(size);
	m_pBG->SetSizeOrigin(m_pBG->GetSize());

	return S_OK;
}

//==========================================================================
// �`�[���T�C�hUI�̐���
//==========================================================================
HRESULT CEntry_SetUpTeam::CreateTeamSideUI()
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�

		// �ʒu�̐ݒ�
		MyLib::Vector3 setpos = teamside::POS;
		setpos.x += ((CGameManager::ETeamSide)(i) == CGameManager::ETeamSide::SIDE_LEFT) ? -320.0f : 320.0f;

		// �`�[���l���̐���
		m_apTeamSideUI[i] = CObject2D_Anim::Create( // ����
			setpos,				// �ʒu
			teamside::PTRN.x,	// �e�N�X�`����������
			teamside::PTRN.y,	// �e�N�X�`���c������
			0.0f,				// �Đ�����
			false,				// �����j��
			PRIORITY			// �D�揇��
		);
		if (m_apTeamSideUI[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}

		// �����T�C�h�̃p�^�[���ɕύX
		m_apTeamSideUI[i]->SetPatternAnim(i);

		// �����Đ���OFF�ɂ���
		m_apTeamSideUI[i]->SetEnableAutoPlay(false);

		// �e�N�X�`���̊���
		CTexture* pTexture = CTexture::GetInstance();
		int nTexID = pTexture->Regist(teamside::TEXTURE);
		m_apTeamSideUI[i]->BindTexture(nTexID);

		// ���������ɃT�C�Y��ݒ�
		MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, teamside::WIDTH);
		size.y /= (float)teamside::PTRN.y;	// �������Ŋ���
		m_apTeamSideUI[i]->SetSize(size);
		m_apTeamSideUI[i]->SetSizeOrigin(m_apTeamSideUI[i]->GetSize());
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

	// �`�[���l���̍X�V
	UpdateNumInUI(fDeltaTime, fDeltaRate, fSlowRate);

	// �p�b�hUI�̍X�V
	UpdatePadUI(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�[���l���̍X�V
//==========================================================================
void CEntry_SetUpTeam::UpdateNumInUI(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�
		
		// �`�[���l���̃^�C�}�[
		m_fTimeNumInTeam[i] += fDeltaTime * fSlowRate;

#if 0
		// ���쌠�������Ă���ꍇ�ő�l
		const int nUserIdx = m_nMaxChangeIdx[i];	// ���[�U�[�C���f�b�N�X
		if (nUserIdx > -1)
		{
			m_fTimeNumInTeam[i] = 2.0f;
		}
#endif

		// null�͎���
		if (m_apNumInTeam[i] == nullptr) continue;

		// �F�擾
		D3DXCOLOR col = m_apNumInTeam[i]->GetColor();
		MyLib::Vector2 size = m_apNumInTeam[i]->GetSize(), sizeOrigin = m_apNumInTeam[i]->GetSizeOrigin();

		// �s�����x�ƃX�P�[���ύX
		if (m_fTimeNumInTeam[i] <= 1.0f)
		{
			col.a = UtilFunc::Correction::EasingEaseIn(num::ALPHARATE_MIN, num::ALPHARATE_MAX, 0.0f, 1.0f, m_fTimeNumInTeam[i]);
		}
		else
		{
			col.a = UtilFunc::Correction::EasingEaseOut(num::ALPHARATE_MAX, num::ALPHARATE_MIN, 1.0f, 2.0f, m_fTimeNumInTeam[i]);
		}
		m_apNumInTeam[i]->SetColor(col);

		// �X�P�[���ݒ�
		float scaleRate = UtilFunc::Transformation::ValueToRate(col.a, num::ALPHARATE_MIN, num::ALPHARATE_MAX);
		scaleRate = num::SCALERATE_MIN + (num::SCALERATE_MAX - num::SCALERATE_MIN) * scaleRate;
		m_apNumInTeam[i]->SetSize(sizeOrigin * scaleRate);

		if (m_fTimeNumInTeam[i] >= 2.0f)
		{
			m_fTimeNumInTeam[i] = 0.f;
		}
	}
}

//==========================================================================
// �p�b�hUI�̍X�V
//==========================================================================
void CEntry_SetUpTeam::UpdatePadUI(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		// ���G���g���[�̏ꍇ����
		const int nUserIdx = m_nEntryIdx[i];	// ���[�U�[�C���f�b�N�X
		if (nUserIdx <= -1) { continue; }

		// �����������Ă���ꍇ�͖���\��
		if (m_apPadUI[nUserIdx] != nullptr)
		m_apPadUI[nUserIdx]->SetEnableDispArrow(!IsUserReady(nUserIdx));
	}
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

	// �w�i�̍폜
	SAFE_KILL(m_pBG);

	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�

		// �`�[���l���̍폜
		SAFE_KILL(m_apNumInTeam[i]);
		SAFE_KILL(m_apTeamSideUI[i]);
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
	std::vector<CPadUI*> vecNone;	// �`�[���w��Ȃ��z��
	std::vector<CPadUI*> vecLeft;	// ���`�[���z��
	std::vector<CPadUI*> vecRight;	// �E�`�[���z��
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

	// ��r�֐�
	auto func = [](CPadUI* p1, CPadUI* p2)
	{
		// �v���C���[�C���f�b�N�X�擾
		int nPlayerIdx1 = p1->GetPatternAnim();
		if (nPlayerIdx1 >= mylib_const::MAX_PLAYER) { nPlayerIdx1 /= 2; }

		// �v���C���[�C���f�b�N�X�擾
		int nPlayerIdx2 = p1->GetPatternAnim();
		if (nPlayerIdx2 >= mylib_const::MAX_PLAYER) { nPlayerIdx2 /= 2; }

		return nPlayerIdx1 < nPlayerIdx2;
	};

	// �z����v�f���\�[�g����
	std::sort(vecNone.begin(),  vecNone.end(),  func);
	std::sort(vecLeft.begin(),  vecLeft.end(),  func);
	std::sort(vecRight.begin(), vecRight.end(), func);

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
	PosAdjPadUIToNumIn(vecLeft, CGameManager::ETeamSide::SIDE_LEFT);

	// �E�`�[���̈ʒu�␳
	PosAdjPadUIToNumIn(vecRight, CGameManager::ETeamSide::SIDE_RIGHT);

	// ���`�[���l�������̈ʒu�␳
	const int nLeftUpIdx = m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_LEFT];		// ���`�[���l�������p�b�h�C���f�b�N�X
	if (nLeftUpIdx > -1)
	{ // ���삷��v���C���[�������ꍇ

		// �ʒu�𔽉f
		m_apPadUI[nLeftUpIdx]->SetPosition(pad::left::up::POS);

		// ���̈ʒu�͐����ɂ���
		for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
		{
			m_apPadUI[nLeftUpIdx]->GetArrowUI(i)->SetPosition(m_apNumInTeam[CGameManager::ETeamSide::SIDE_LEFT]->GetPosition());
			m_apPadUI[nLeftUpIdx]->GetArrowUI(i)->SetOriginPosition(m_apNumInTeam[CGameManager::ETeamSide::SIDE_LEFT]->GetPosition());
		}
	}

	// �E�`�[���l�������̈ʒu�␳
	const int nRightUpIdx = m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_RIGHT];	// �E�`�[���l�������p�b�h�C���f�b�N�X
	if (nRightUpIdx > -1)
	{ // ���삷��v���C���[�������ꍇ

		// �ʒu�𔽉f
		m_apPadUI[nRightUpIdx]->SetPosition(pad::right::up::POS);

		// ���̈ʒu�͐����ɂ���
		for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
		{
			m_apPadUI[nRightUpIdx]->GetArrowUI(i)->SetPosition(m_apNumInTeam[CGameManager::ETeamSide::SIDE_RIGHT]->GetPosition());
			m_apPadUI[nRightUpIdx]->GetArrowUI(i)->SetOriginPosition(m_apNumInTeam[CGameManager::ETeamSide::SIDE_RIGHT]->GetPosition());
		}
	}
}

//==========================================================================
// �����ɂ���l�������ƂɃR���g���[���[UI�ʒu�␳
//==========================================================================
void CEntry_SetUpTeam::PosAdjPadUIToNumIn(const std::vector<CPadUI*>& vecPadUI, CGameManager::ETeamSide side)
{
	// ���`�[���̈ʒu�␳
	const int nNumLeft = (int)vecPadUI.size();	// �v�f��

	MyLib::Vector3 setpos;	// �ݒ�ʒu
	switch (side)
	{
	case CGameManager::SIDE_LEFT:
		setpos = pad::left::POS;
		break;

	case CGameManager::SIDE_RIGHT:
		setpos = pad::right::POS;
		break;
	}

	if (nNumLeft == 1)
	{// 1�̂Ƃ����S
		vecPadUI[0]->SetPosition(setpos);
	}
	else if (nNumLeft == 2)
	{// 2

		// ���ɂ��炷
		setpos.x -= pad::OFFSET.x;
		for (int i = 0; i < nNumLeft; i++)
		{
			vecPadUI[i]->SetPosition(setpos);

			// �E�ɂ��炷
			setpos.x += pad::OFFSET.x * 2.0f;
		}

	}
	else if (nNumLeft == 3)
	{// 3

		// ���ɂ��炷
		setpos.x -= pad::OFFSET.x;
		setpos.y -= pad::OFFSET.y * 0.5f;
		for (int i = 0; i < 2; i++)
		{
			vecPadUI[i]->SetPosition(setpos);

			// �E�ɂ��炷
			if (i == 0)
			{
				setpos.x += pad::OFFSET.x * 2.0f;
			}
		}

		// �Ō�̈ʒu
		setpos.x -= pad::OFFSET.x * 1.0f;
		setpos.y += pad::OFFSET.y;
		vecPadUI[2]->SetPosition(setpos);

	}
	else if (nNumLeft == 4)
	{// 4

		// ���ɂ��炷
		setpos.x -= pad::OFFSET.x;
		setpos.y -= pad::OFFSET.y * 0.5f;
		for (int i = 0; i < 4; i++)
		{
			vecPadUI[i]->SetPosition(setpos);

			if (i == 1)
			{// �E�[
				setpos.x -= pad::OFFSET.x * 2.0f;
				setpos.y += pad::OFFSET.y;
			}
			else
			{
				// �ݒ�ʒu���炷
				setpos.x += pad::OFFSET.x * 2.0f;
			}
		}
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
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, i)
		||  pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, i))
		{ // �G���g���[���삪�s��ꂽ�ꍇ

			for (int j = 0; j < mylib_const::MAX_PLAYER; j++)
			{ // �p�b�h�F���̍ő吔���J��Ԃ�

				// ���ɃG���g���[����Ă���ꍇ������
				if (m_nEntryIdx[j] == i) { break; }

				// ���ɃG���g���[����Ă���ꍇ����
				if (m_nEntryIdx[j] != -1) { continue; }

				// ����̃v���C���[�C���f�b�N�X��ǉ�
				m_nEntryIdx[j] = i;

				// �R���g���[���[UI�̎����`���ON�ɂ���
				m_apPadUI[i]->SetEnableDisp(true);

				// �T�E���h�̍Đ�
				PLAY_SOUND(CSound::ELabel::LABEL_SE_DECIDE_05);
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
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, nUserIdx)
		||  pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_Y, nUserIdx))
		{ // ���E���삪�s��ꂽ�ꍇ

			// �T�E���h�̍Đ�
			PLAY_SOUND(CSound::ELabel::LABEL_SE_CANCEL);

			std::vector<int>::iterator itr;	// ���������z��C�e���[�^�[
			if (IsUserReady(nUserIdx, &itr))
			{ // �����������Ă���ꍇ

				// ���������z�񂩂�C�e���[�^�[���폜
				m_vecAddIdx[m_TeamSide[nUserIdx].team].erase(itr);

				// �L�����Z��
				m_apPadUI[nUserIdx]->Cancel();
				break;
			}
			else
			{ // �����������Ă��Ȃ��ꍇ

				// �G���g���[������
				DeleteEntry(&m_nEntryIdx[i]);

				auto func = [](int n1, int n2)
				{
					if (n1 == -1) return false;
					if (n2 == -1) return true;

					return n1 < n2;
				};

				std::sort(&m_nEntryIdx[0], &m_nEntryIdx[0] + mylib_const::MAX_PLAYER, func);
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
			||  pPad->GetLStickTrigger(nUserIdx, CInputGamepad::STICK_CROSS_LEFT))
			{
				if (m_TeamSide[nUserIdx].team != CGameManager::ETeamSide::SIDE_LEFT)
				{ // ���[����Ȃ����ɍ��ړ����삪�s��ꂽ�ꍇ

					// �T�E���h�̍Đ�
					PLAY_SOUND(CSound::ELabel::LABEL_SE_CONTROERMOVE);

					// ���Ɉړ�
					m_TeamSide[nUserIdx].team = (m_TeamSide[nUserIdx].team == CGameManager::ETeamSide::SIDE_NONE) ? CGameManager::ETeamSide::SIDE_LEFT : CGameManager::ETeamSide::SIDE_NONE;
					m_apPadUI[nUserIdx]->SetAction(CArrowUI::EDirection::DIRECTION_L);
				}
			}
			else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nUserIdx)
				 ||  pPad->GetLStickTrigger(nUserIdx, CInputGamepad::STICK_CROSS_RIGHT))
			{
				if (m_TeamSide[nUserIdx].team != CGameManager::ETeamSide::SIDE_RIGHT)
				{ // �E�[����Ȃ����ɉE�ړ����삪�s��ꂽ�ꍇ

					// �T�E���h�̍Đ�
					PLAY_SOUND(CSound::ELabel::LABEL_SE_CONTROERMOVE);

					// �E�Ɉړ�
					m_TeamSide[nUserIdx].team = (m_TeamSide[nUserIdx].team == CGameManager::ETeamSide::SIDE_NONE) ? CGameManager::ETeamSide::SIDE_RIGHT : CGameManager::ETeamSide::SIDE_NONE;
					m_apPadUI[nUserIdx]->SetAction(CArrowUI::EDirection::DIRECTION_R);
				}
			}
		}

		// �I���`�[����ۑ�
		const int nSide = (int)m_TeamSide[nUserIdx].team;

		//--------------------------
		// �`�[�����l���ς�
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, nUserIdx)
		||  pPad->GetLStickTrigger(nUserIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_UP))
		{ // ��ړ����삪�s��ꂽ�ꍇ

			if (IsUserTeamSelect(nUserIdx))
			{ // �`�[���I��

				assert(!(nSide <= CGameManager::ETeamSide::SIDE_NONE || nSide >= CGameManager::ETeamSide::SIDE_MAX));
				if (m_nMaxChangeIdx[nSide] <= -1)
				{ // �ő吔�ύX�̑��쌠��N�������Ă��Ȃ�

					// �����̃T�C�h��ύX����p�C���f�b�N�X�ێ�
					m_nMaxChangeIdx[nSide] = nUserIdx;

						// �T�E���h�̍Đ�
						PLAY_SOUND(CSound::ELabel::LABEL_SE_CONTROERMOVE);

						// ���𐔎��p�ɂ���
						CArrowUI* pArrow = nullptr;
					for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
					{
						pArrow = m_apPadUI[nUserIdx]->GetArrowUI(i);
						pArrow->SetOffset(pad::ARROWSPACE_NUMBER);
						pArrow->SetSizeByWidth(pad::ARROWWIDTH_NUMBER);
					}
				}
			}
		}

		//--------------------------
		// ������������
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, nUserIdx)
		||  pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, nUserIdx))
		{ // �����������삪�s��ꂽ�ꍇ

			if (IsUserTeamSelect(nUserIdx)
			&&  !IsUserMaxChange(nUserIdx))
			{ // �`�[���I�𒆊��A�ő吔�ύX�����������Ă��Ȃ��ꍇ

				assert(!(nSide <= CGameManager::ETeamSide::SIDE_NONE || nSide >= CGameManager::ETeamSide::SIDE_MAX));
				const int nCurInTeam = static_cast<int>(m_vecAddIdx[nSide].size());	// ���݂̃`�[���l��
				if (!IsUserReady(nUserIdx) && m_nPlayerNum[nSide] > nCurInTeam)
				{ // �������������A�ő�`�[���l�������̏ꍇ

					// ���������z��ɒǉ�
					m_vecAddIdx[nSide].push_back(nUserIdx);

					// ����
					m_apPadUI[nUserIdx]->Decide();

					// �T�E���h�̍Đ�
					PLAY_SOUND(CSound::ELabel::LABEL_SE_DECIDE_00);
				}
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
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nUserIdx)
		||  pPad->GetLStickTrigger(nUserIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_LEFT))
		{ // ���Z���삪�s��ꂽ�ꍇ

			// �`�[���l�������Z
			m_nPlayerNum[nSide]--;

			// ���Ɉړ�
			m_apPadUI[nUserIdx]->SetAction(CArrowUI::EDirection::DIRECTION_L);

			// �T�E���h�̍Đ�
			if (m_nPlayerNum[nSide] >= 1)
			{
				PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW);
			}
			else
			{
				PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_HIGH);
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nUserIdx)
			 ||  pPad->GetLStickTrigger(nUserIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_RIGHT))
		{ // ���Z���삪�s��ꂽ�ꍇ

			// �`�[���l�������Z
			m_nPlayerNum[nSide]++;

			// ���Ɉړ�
			m_apPadUI[nUserIdx]->SetAction(CArrowUI::EDirection::DIRECTION_R);

			// �T�E���h�̍Đ�
			if (m_nPlayerNum[nSide] <= CGameManager::MAX_SIDEPLAYER)
			{
				PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW);
			}
			else
			{
				PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_HIGH);
			}
		}

		// �`�[���l����␳
		m_nPlayerNum[nSide] = UtilFunc::Transformation::Clamp(m_nPlayerNum[nSide], 1, CGameManager::MAX_SIDEPLAYER);

		//--------------------------
		// �ő吔�ύX����
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, nUserIdx)
		||  pPad->GetLStickTrigger(nUserIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_DOWN))
		{ // ���ړ����삪�s��ꂽ�ꍇ

			// �T�E���h�̍Đ�
			PLAY_SOUND(CSound::ELabel::LABEL_SE_CONTROERMOVE);

			// ���̊Ԋu���R���g���[���[�p�ɂ���
			CArrowUI* pArrow = nullptr;
			for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
			{
				pArrow = m_apPadUI[nUserIdx]->GetArrowUI(i);
				pArrow->SetOffset(pArrow->GetOffsetOrigin());
				pArrow->SetSizeByWidth(pArrow->GetSizeWidthOrigin());
			}

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

		// �Z�[�u
		Save(m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT], m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT]);

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
bool CEntry_SetUpTeam::IsUserTeamSelect(const int nUserIdx) const
{
	// �I�𒆃`�[���̎w�肪���邩��Ԃ�
	return (m_TeamSide[nUserIdx].team != CGameManager::ETeamSide::SIDE_NONE);
}

//==========================================================================
// �ő吔�ύX�t���O�擾
//==========================================================================
bool CEntry_SetUpTeam::IsUserMaxChange(const int nUserIdx) const
{
	// �`�[�������I���̏ꍇ������
	const int nSide = m_TeamSide[nUserIdx].team;	// �I���`�[��
	if (nSide == CGameManager::ETeamSide::SIDE_NONE) { return false; }

	// �ő吔�ύX�̑��쌠�C���f�b�N�X���������[�U�[����Ԃ�
	return (m_nMaxChangeIdx[nSide] == nUserIdx);
}

//==========================================================================
// �G���g���[���v���C���[�C���f�b�N�X�擾
//==========================================================================
std::vector<int> CEntry_SetUpTeam::GetEntryPlayerIdx()
{
	std::vector<int> vecIdx;
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // �p�b�h�F���̍ő吔���J��Ԃ�

		// ���G���g���[�̏ꍇ����
		const int nUserIdx = m_nEntryIdx[i];	// ���[�U�[�C���f�b�N�X
		if (nUserIdx <= -1) { continue; }

		// �������������Ă��Ȃ��ꍇ����
		if (!IsUserReady(nUserIdx)) { continue; }

		// �C���f�b�N�X�ǉ�
		vecIdx.push_back(nUserIdx);
	}

	return vecIdx;
}

//==========================================================================
// �Z�[�u����
//==========================================================================
void CEntry_SetUpTeam::Save(std::vector<int>& rVecLeft, std::vector<int>& rVecRight)
{
	// �t�@�C�����J��
	std::ofstream file(TEXTFILE);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�`�[���ݒ�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}

	// ���o���̏����o��
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	�`�[���ݒ�e�L�X�g [team_setting.txt]" << std::endl;
	file << "#	Author : ���c �E��" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;

	// �`�[���l���̏����o��
	file << "#------------------------------------------------------------------------------" << std::endl;
	file << "#	�`�[���l�����" << std::endl;
	file << "#------------------------------------------------------------------------------" << std::endl;
	file << "TEAMIDX" << std::endl;

	// ���`�[���̏����o��
	file << "	LEFT  = ";
	for (const int nPadIdx : rVecLeft)
	{
		file << nPadIdx << " ";
	}
	file << "END" << std::endl;

	// �E�`�[���̏����o��
	file << "	RIGHT = ";
	for (const int nPadIdx : rVecRight)
	{
		file << nPadIdx << " ";
	}
	file << "END" << std::endl;

	file << "END_TEAMIDX" << std::endl;

	// �t�@�C�������
	file.close();
}

//==========================================================================
// ���[�h
//==========================================================================
void CEntry_SetUpTeam::Load()
{
	// �t�@�C�����J��
	std::ifstream file(TEXTFILE);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�`�[���ݒ�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}

	// �t�@�C����Ǎ�
	std::string str;	// �Ǎ�������
	while (file >> str)
	{ // �t�@�C���̏I�[�ł͂Ȃ��ꍇ���[�v

		if (str.front() == '#') { std::getline(file, str); }	// �R�����g�A�E�g
		else if (str == "TEAMIDX")
		{
			do { // END_TEAMIDX��ǂݍ��ނ܂Ń��[�v

				// �������ǂݍ���
				file >> str;

				if (str.front() == '#') { std::getline(file, str); }	// �R�����g�A�E�g
				else if (str == "LEFT")
				{
					// ���[�h�C���f�b�N�X
					LoadIdx(&file, CGameManager::ETeamSide::SIDE_LEFT);
				}
				else if (str == "RIGHT")
				{
					// ���[�h�C���f�b�N�X
					LoadIdx(&file, CGameManager::ETeamSide::SIDE_RIGHT);
				}
			} while (str != "END_TEAMIDX");	// END_TEAMIDX��ǂݍ��ނ܂Ń��[�v
		}
	}

	for (int team = 0; team < CGameManager::ETeamSide::SIDE_MAX; team++)
	{ // �`�[�������J��Ԃ�

		// �Œ�l�ɕ␳
		if (m_nPlayerNum[team] == 0) { m_nPlayerNum[team]++; }
	}

	// �t�@�C�������
	file.close();
}

//==========================================================================
// ���[�h�C���f�b�N�X
//==========================================================================
void CEntry_SetUpTeam::LoadIdx(std::ifstream* pFile, const CGameManager::ETeamSide team)
{
	// �t�@�C���X�g���[�������ݒ�̏ꍇ������
	if (pFile == nullptr) { assert(false); return; }

	std::string str;	// �Ǎ�������
	*pFile >> str;		// ����Ǎ�
	while (1)
	{ // END��ǂݍ��ނ܂Ń��[�v

		// �������ǂݍ���
		*pFile >> str;

		if (str == "END")
		{
			break;
		}
		else
		{
			int nPadIdx = std::stoi(str);
			m_nPlayerNum[team]++;
			if (nPadIdx <= -1) { continue; }

			for (int j = 0; j < mylib_const::MAX_PLAYER; j++)
			{ // �p�b�h�F���̍ő吔���J��Ԃ�

				// ���ɃG���g���[����Ă���ꍇ����
				if (m_nEntryIdx[j] >= 0 && m_nEntryIdx[j] != nPadIdx) { continue; }

				// ����̃v���C���[�C���f�b�N�X��ǉ�
				m_nEntryIdx[j] = nPadIdx;

				// �R���g���[���[UI�̎����`���ON�ɂ���
				m_apPadUI[nPadIdx]->SetEnableDisp(true);
				break;
			}

			// �I���`�[���ݒ�
			m_TeamSide[nPadIdx].team = team;
		}
	}
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
			Save(m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT], m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT]);
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
// �Z�[�u������
//==========================================================================
void CEntry_SetUpTeam::SaveInit()
{
	// �Z�[�u����
	std::vector<int> vec;
	Save(vec, vec);
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
	if (nPlayerIdx >= CGameManager::MAX_PLAYER || nPlayerIdx <= -1) return 9999999;
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

		// �`�[���w�肪�Ȃ�����
		if (m_TeamSide[i].team == CGameManager::SIDE_NONE) { continue; }

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

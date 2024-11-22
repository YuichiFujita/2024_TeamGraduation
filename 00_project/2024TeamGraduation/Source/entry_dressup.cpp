//=============================================================================
// 
// �h���X�A�b�v�ݒ菈�� [entry_dressup.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "entry_dressup.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "entry_setupTeam.h"
#include "dressup.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTFILE = "data\\TEXT\\entry\\setupTeam.txt";
	const std::string TOP_LINE = "#==============================================================================";	// �e�L�X�g�̃��C��
	const std::string TEXT_LINE = "#------------------------------------------------------------------------------";	// �e�L�X�g�̃��C��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEntry_Dressup::CEntry_Dressup() : CEntryScene()
{
	
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
	// �ǂݍ���
	Load();

	// �`�[�����ݒ�
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetEntryScene()->GetSetupTeam();
	if (pSetupTeam == nullptr) return E_FAIL;

	int nPlyerNum = pSetupTeam->GetAllPlayerNum();

	// �����ւ���񃊃T�C�Y
	m_vecDressupInfo.resize(nPlyerNum);

	// ��������
	for (int i = 0; i < nPlyerNum; i++)
	{
		// �v���C���[����
		MyLib::Vector3 pos = MyLib::Vector3(200.0f, 0.0f, 0.0f);
		MyLib::Vector3 offset = MyLib::Vector3(200.0f * i, 0.0f, 0.0f);
		m_vecDressupInfo[i].pPlayer = CPlayer::Create
		(
			pos + offset, 						// �ʒu
			CGameManager::ETeamSide::SIDE_NONE,	// �`�[���T�C�h
			CPlayer::EFieldArea::FIELD_ENTRY,	// �|�W�V����
			CPlayer::EBaseType::TYPE_USER,		// �x�[�X�^�C�v
			CPlayer::EBody::BODY_NORMAL,		// �̌n
			CPlayer::EHandedness::HAND_R		// ������
		);

		// �G���g���[�����ԍ�
		int entryIdx = pSetupTeam->GetEntryIdx(i);

		// �������ւ�����
		m_vecDressupInfo[i].pHair = CDressup::Create(
			CDressup::EType::TYPE_HAIR,		// �����ւ��̎��
			m_vecDressupInfo[i].pPlayer,	// �ύX����v���C���[
			CPlayer::ID_HAIR);				// �ύX�ӏ��̃C���f�b�N�X
		
		// �A�N�Z�؂�ւ�����
		m_vecDressupInfo[i].pAccessory = CDressup::Create(
			CDressup::EType::TYPE_HAIR,		// �����ւ��̎��
			m_vecDressupInfo[i].pPlayer,	// �ύX����v���C���[
			CPlayer::ID_ACCESSORY);			// �ύX�ӏ��̃C���f�b�N�X

		// ���삷��C���f�b�N�X�ݒ�
		m_vecDressupInfo[i].pHair->SetControllIdx(entryIdx);
		m_vecDressupInfo[i].pAccessory->SetControllIdx(entryIdx);
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEntry_Dressup::Uninit()
{
}

//==========================================================================
// �X�V����
//==========================================================================
void CEntry_Dressup::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�̍X�V
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ����̃T�C�Y
	int size = static_cast<int>(m_vecDressupInfo.size());

	for (int i = 0; i < size; i++)
	{
		// �����ւ��̍X�V
		m_vecDressupInfo[i].pHair->Update(fDeltaTime, fDeltaRate, fSlowRate);
		m_vecDressupInfo[i].pAccessory->Update(fDeltaTime, fDeltaRate, fSlowRate);

		// �̌^�ύX
		ChangeBodyType(i);

		// ������ύX
		ChangeHandedness(i);
	}

}

//==========================================================================
// �̌^�ύX
//==========================================================================
void CEntry_Dressup::ChangeBodyType(int i)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �̌^�擾
	CPlayer::EBody body = m_vecDressupInfo[i].pPlayer->GetBodyType();
	CPlayer::EBody oldBody = body;

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, i))
	{// ���[�v

		// ���֕ύX
		int afterBody = (body + 1) % (CPlayer::EBody::BODY_MAX - 1);
		body = static_cast<CPlayer::EBody>(afterBody);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, i))
	{// �t���[�v

		// �O�֕ύX
		int deforeBody = (body + (CPlayer::EBody::BODY_MAX - 1)) % CPlayer::EBody::BODY_MAX;
		body = static_cast<CPlayer::EBody>(deforeBody);
	}

	if (body != oldBody)
	{// �ύX����Ă�����

		// �v���C���[�Đ���
		ReCreatePlayer(i, m_vecDressupInfo[i].pPlayer->GetHandedness(), body);
	}

}

//==========================================================================
// ������ύX
//==========================================================================
void CEntry_Dressup::ChangeHandedness(int i)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// ������擾
	CPlayer::EHandedness handedness = m_vecDressupInfo[i].pPlayer->GetHandedness();
	CPlayer::EHandedness oldHandedness = handedness;

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, i))
	{// ���[�v

		// ���֕ύX
		int afterHandedness = (handedness + 1) % (CPlayer::EHandedness::HAND_MAX - 1);
		handedness = static_cast<CPlayer::EHandedness>(afterHandedness);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, i))
	{// �t���[�v

		// �O�֕ύX
		int deforeHandedness = (handedness + (CPlayer::EHandedness::HAND_MAX - 1)) % CPlayer::EHandedness::HAND_MAX;
		handedness = static_cast<CPlayer::EHandedness>(deforeHandedness);
	}

	if (handedness != oldHandedness)
	{// �ύX����Ă�����

		// �v���C���[�Đ���
		ReCreatePlayer(i, handedness, m_vecDressupInfo[i].pPlayer->GetBodyType());
	}
}

//==========================================================================
// �v���C���[�Đ���
//==========================================================================
void CEntry_Dressup::ReCreatePlayer(int i, CPlayer::EHandedness handedness, CPlayer::EBody body)
{
	// ���ۑ�
	MyLib::Vector3 pos = m_vecDressupInfo[i].pPlayer->GetPosition();

	// �폜
	m_vecDressupInfo[i].pPlayer->Kill();

	// �Đ���
	m_vecDressupInfo[i].pPlayer = CPlayer::Create
	(
		pos, 								// �ʒu
		CGameManager::ETeamSide::SIDE_NONE,	// �`�[���T�C�h
		CPlayer::EFieldArea::FIELD_ENTRY,	// �|�W�V����
		CPlayer::EBaseType::TYPE_USER,		// �x�[�X�^�C�v
		body,								// �̌n
		handedness							// ������
	);
}

//==========================================================================
// �Z�[�u����
//==========================================================================
void CEntry_Dressup::Save()
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


	// �t�@�C�������
	File.close();
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
	if (ImGui::TreeNode("SetUpTeam"))
	{
		//=============================
		// �Z�[�u
		//=============================s
		if (ImGui::Button("Save"))
		{
			Save();
		}

		

		ImGui::TreePop();
	}
}

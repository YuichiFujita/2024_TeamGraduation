//=============================================================================
//
// �R�}���h���� [command.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "command.h"
#include "input_gamepad.h"
#include "debugproc.h"
#include "manager.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCommand::CCommand()
{
	// �l�̃N���A
	m_nNowFlame = 0;
	m_nMaxFlame = 0;
	m_nId = 0;
	m_nType = 0;
	m_Info.clear();
}

//=============================================================================
// �I������
//=============================================================================
void CCommand::Uninit()
{
	m_Info.clear();
}

//=============================================================================
// �ǂݍ���
//=============================================================================
void CCommand::Load(std::string& string)
{
	// �t�@�C�����J��
	std::ifstream File(string);
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

		// �X�g���[���쐬
		std::istringstream lineStream(line);

		if (line.find("MAX_FRAME") != std::string::npos)
		{// MODELSET�Ŕz�u���ǂݍ���
			int frame = -1;

			// ���n��
			lineStream >>
				hoge >>
				hoge >>
				frame;	// �z�u���̎��

			SetMaxFlame(frame);
		}
		else if (line.find("PLAY_MOTION") != std::string::npos)
		{// PLAY_MOTION�ōĐ����[�V�����ݒ�
			int id = 0;

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>
				id;	// ���[�V�����̎��

			SetId(id);
		}

		if (line.find("COMMANDSET") != std::string::npos)
		{// COMMANDSET�Ŕz�u���ǂݍ���
			int process = -1;

			// �ǂݍ��ݏ��
			while (line.find("END_COMMANDSET") == std::string::npos)
			{
				std::getline(File, line);
				if (line.find("KEY") != std::string::npos)
				{// TYPE�Ŕz�u���̎��

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// ���n��
					lineStream >>
						hoge >>
						hoge >>
						process;	// �z�u���̎��
					continue;
				}
			}

			// �R�}���h�ݒ�
			if (process >= 0)
			{
				SetCommand(process);
			}
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// �t�@�C�������
	File.close();
}

//=============================================================================
// �R�}���h�ݒ�
//=============================================================================
void CCommand::SetCommand(int nKey)
{
	Info info;
	info.nButton = nKey;
	info.bActive = false;
	m_Info.push_back(info);
}

//=============================================================================
// ���݂̃t���[������ݒ�
//=============================================================================
void CCommand::SetNowFlame(int nFlame)
{
	m_nNowFlame = nFlame;

	if (m_nNowFlame < 0 || m_nNowFlame >= m_nMaxFlame) Reset();	// �ő�t���[���𒴂���
}

//=============================================================================
// ���͏󋵏�����
//=============================================================================
void CCommand::Reset()
{
	// �t���O�����Z�b�g
	for (auto it = m_Info.begin(); it != m_Info.end(); it++) {
		it->bActive = false;
	}

	m_nNowFlame = 0;
}

//=============================================================================
// 
// �p�b�h�ݒ�
// 
//=============================================================================
// �R�}���h���͊m�F
//=============================================================================
bool CCommandPad::GetCommand()
{
	bool bValue = false;
	DIRECTION direction = GetDirection();
	auto it = m_Info.begin();
	auto old = it;

	if (it->bActive) SetNowFlame(GetNowFlame() + 1);	// ��ڂ����͂���Ă�����J�E���g����

	for (it = m_Info.begin(); it != m_Info.end(); it++) 
	{
		if (it->bActive) 
		{ // ���ɓ��͍ς�
			CManager::GetInstance()->GetDebugProc()->Print("���͂��ꂽ��^^ [ %d ]\n", GetId()); 
			old = it; 
			continue; 
		}
		else if (it->nButton == direction)
		{ // ���͕�������v
			it->bActive = true;
			break;
		}
		else { // ���̓L�[����v���Ȃ�
			if (it != m_Info.begin()) {	// �ŏ��ȊO
				if (old->nButton == direction) { // �O��Ɠ��͏�񂪈�v
					old = it;
					return bValue;
				}
			}

			Reset();
			break;
		}

		old = it;
	}

	if (it == m_Info.end()) bValue = true;	// �Ō�̃R�}���h�܂œ��͂ł���

	return bValue;	// �R�}���h�������s��Ԃ�
}

//=============================================================================
// ���͕����擾
//=============================================================================
CCommandPad::DIRECTION CCommandPad::GetDirection()
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();
	float rot = pPad->GetStickRotR(0);
	float size = D3DX_PI * 0.125f;

	// �������犄��o��
	if		(rot <= 0.0f + size				&& rot >= 0.0f - size)				return DIRECTION::STICK_UP;
	else if ((rot <= -D3DX_PI + size && rot >= -D3DX_PI) || 
		(rot >= D3DX_PI - size && rot <= D3DX_PI))			return DIRECTION::STICK_DOWN;
	else if (rot <= -D3DX_PI * 0.5f + size	&& rot >= -D3DX_PI * 0.5f - size)	return DIRECTION::STICK_LEFT;
	else if (rot <= D3DX_PI * 0.5f + size	&& rot >= D3DX_PI * 0.5f - size)	return DIRECTION::STICK_RIGHT;
	else if (rot <= -D3DX_PI * 0.25f + size && rot >= -D3DX_PI * 0.25f - size)	return DIRECTION::STICK_LEFTUP;
	else if (rot <= D3DX_PI * 0.25f + size	&& rot >= D3DX_PI * 0.25f - size)	return DIRECTION::STICK_RIGHTUP;
	else if (rot <= -D3DX_PI * 0.75f + size && rot >= -D3DX_PI * 0.75f - size)	return DIRECTION::STICK_LEFTDOWN;
	else if (rot <= D3DX_PI * 0.75f + size	&& rot >= D3DX_PI * 0.75f - size)	return DIRECTION::STICK_RIGHTDOWN;

	return DIRECTION::STICK_NONE;	// ���͂���Ă��Ȃ��ꍇ
}

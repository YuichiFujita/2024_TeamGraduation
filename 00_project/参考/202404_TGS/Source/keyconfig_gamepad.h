//=============================================================================
//
// �Q�[���p�b�h�L�[�R���t�B�O�w�b�_�[ [keyconfig_gamepad.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _KEYCONFIG_GAMEPAD_H_
#define _KEYCONFIG_GAMEPAD_H_	// ��d�C���N���[�h�h�~

#include "keyconfig.h"
#include "input_gamepad.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �L�[�R���t�B�O
class CkeyConfigPad: public CKeyConfig
{
public:

	// �R���X�g���N�^
	CkeyConfigPad();
	~CkeyConfigPad(){}

	// �����o�֐�
	void Uninit() override;
	bool GetPress(const int type, const int nId = 0) override;
	bool GetTrigger(const int type, const int nId = 0) override;
	bool GetRelease(const int type, const int nId = 0) override;
	bool GetRepeat(const int type, const int nId = 0) override;
	void Load(const std::string& file) override;
	void Setting(const int type, const int nId = 0) override;
	int GetKey(const int type, const int nId = 0) override;
	void Join(const int type, const int nKey, const int nId = 0) override { Join(type, static_cast<CInputGamepad::BUTTON>(nKey)); }

public:
	void Join(const int action, const CInputGamepad::BUTTON key);

private:

	// �����o�ϐ�
	std::map<int, CInputGamepad::BUTTON> m_Info;

};

#endif
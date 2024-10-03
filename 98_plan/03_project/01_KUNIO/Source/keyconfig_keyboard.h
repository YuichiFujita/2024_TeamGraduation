//=============================================================================
//
// �L�[�{�[�h�L�[�R���t�B�O�w�b�_�[ [keyconfig_keyboard.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _KEYCONFIG_KEYBOARD_H_
#define _KEYCONFIG_KEYBOARD_H_	// ��d�C���N���[�h�h�~

#include "keyconfig.h"
#include "input_gamepad.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �L�[�R���t�B�O
class CkeyConfigKeyboard : public CKeyConfig
{
public:

	// �R���X�g���N�^
	CkeyConfigKeyboard();
	~CkeyConfigKeyboard() {}

	// �����o�֐�
	void Uninit() override;
	bool GetPress(const int type, const int nId = 0) override;
	bool GetTrigger(const int type, const int nId = 0) override;
	bool GetRelease(const int type, const int nId = 0) override;
	bool GetRepeat(const int type, const int nId = 0) override;
	void Load(const std::string& file) override;
	void Setting(const int type, const int nId = 0) override;
	int GetKey(const int type, const int nId = 0) override;
	void Join(const int type, const int nKey, const int nId = 0)override { Join(type, nKey); }

public:
	void Set(const int action, const int key);

private:

	// �����o�ϐ�
	std::map<int, int> m_Info;
};

#endif

//==========================================================================
// 
//  �E�ړ��L�[���� [bindKeyRight.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "bindKeyRight.h"
#include "manager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const CInputGamepad::BUTTON BUTTON = CInputGamepad::BUTTON_RIGHT;
	const int KEY = DIK_D;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBindKeyRight::CBindKeyRight()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBindKeyRight::~CBindKeyRight()
{

}

//==========================================================================
//	�����L�[�v���X�̎擾����
//==========================================================================
bool CBindKeyRight::IsPress(const int nPadIdx) const
{
	// �C���v�b�g���̎擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// �L�[�{�[�h���
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// �p�b�h���

	// ���̓t���O��Ԃ�
	return (pKey->GetPress(KEY) || pPad->GetPress(BUTTON, nPadIdx));
}

//==========================================================================
//	�����L�[�g���K�[�̎擾����
//==========================================================================
bool CBindKeyRight::IsTrigger(const int nPadIdx) const
{
	// �C���v�b�g���̎擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// �L�[�{�[�h���
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// �p�b�h���

	// ���̓t���O��Ԃ�
	return (pKey->GetTrigger(KEY) || pPad->GetTrigger(BUTTON, nPadIdx));
}

//==========================================================================
//	�����X�e�B�b�N�X���̎擾����
//==========================================================================
bool CBindKeyRight::IsStickTilt(const int nPadIdx) const
{
	// �C���v�b�g���̎擾
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���

	// �X���t���O��Ԃ�
	return (pPad->GetStickMoveL(nPadIdx).x > 0.0f);
}

//==========================================================================
//	�����L�[�����̎擾����
//==========================================================================
float CBindKeyRight::GetRotation() const
{
	// �E�̌�����Ԃ�
	return HALF_PI;
}

//==========================================================================
//	�����L�[�����񋓂̎擾����
//==========================================================================
CPlayer::EDashAngle CBindKeyRight::GetAngle() const
{
	// �E�̕����񋓂�Ԃ�
	return CPlayer::EDashAngle::ANGLE_RIGHT;
}

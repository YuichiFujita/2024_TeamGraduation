//==========================================================================
// 
//  ���ړ��L�[���� [bindKeyLeft.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "bindKeyLeft.h"
#include "manager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const CInputGamepad::BUTTON BUTTON = CInputGamepad::BUTTON_LEFT;
	const int KEY = DIK_A;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBindKeyLeft::CBindKeyLeft()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBindKeyLeft::~CBindKeyLeft()
{

}

//==========================================================================
//	�����L�[�v���X�̎擾����
//==========================================================================
bool CBindKeyLeft::IsPress(const int nPadIdx) const
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
bool CBindKeyLeft::IsTrigger(const int nPadIdx) const
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
bool CBindKeyLeft::IsStickTilt(const int nPadIdx) const
{
	// �C���v�b�g���̎擾
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���

	// �X���t���O��Ԃ�
	return (pPad->GetStickMoveL(nPadIdx).x < 0.0f);
}

//==========================================================================
//	�����L�[�����̎擾����
//==========================================================================
float CBindKeyLeft::GetRotation() const
{
	// ���̌�����Ԃ�
	return -HALF_PI;
}

//==========================================================================
//	�����L�[�����񋓂̎擾����
//==========================================================================
CPlayer::EDashAngle CBindKeyLeft::GetAngle() const
{
	// ���̕����񋓂�Ԃ�
	return CPlayer::EDashAngle::ANGLE_LEFT;
}

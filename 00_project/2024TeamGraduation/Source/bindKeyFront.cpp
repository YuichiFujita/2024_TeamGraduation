//==========================================================================
// 
//  �O�i�L�[���� [bindKeyFront.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "bindKeyFront.h"
#include "manager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const CInputGamepad::BUTTON BUTTON = CInputGamepad::BUTTON_UP;
	const int KEY = DIK_W;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBindKeyFront::CBindKeyFront()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBindKeyFront::~CBindKeyFront()
{

}

//==========================================================================
//	�����L�[�v���X�̎擾����
//==========================================================================
bool CBindKeyFront::IsPress(const int nPadIdx) const
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
bool CBindKeyFront::IsTrigger(const int nPadIdx) const
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
bool CBindKeyFront::IsStickTilt(const int nPadIdx) const
{
	// �C���v�b�g���̎擾
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���

	// �X���t���O��Ԃ�
	return (pPad->GetStickMoveL(nPadIdx).y > 0.0f);
}

//==========================================================================
//	�����L�[�����̎擾����
//==========================================================================
float CBindKeyFront::GetRotation() const
{
	// �O���̌�����Ԃ�
	return D3DX_PI;
}

//==========================================================================
//	�����L�[�����񋓂̎擾����
//==========================================================================
CPlayer::EDashAngle CBindKeyFront::GetAngle() const
{
	// �O���̕����񋓂�Ԃ�
	return CPlayer::EDashAngle::ANGLE_UP;
}

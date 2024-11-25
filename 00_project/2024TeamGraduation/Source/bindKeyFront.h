//==========================================================================
// 
//  �O�i�L�[�w�b�_�[ [bindKeyFront.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _BIND_KEY_FRONT_H_
#define _BIND_KEY_FRONT_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "bindKey.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �O�i�L�[�N���X
class CBindKeyFront : public CBindKey
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CBindKeyFront();
	virtual ~CBindKeyFront();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual bool IsPress(const int nPadIdx) const override;		// �����L�[�v���X�擾
	virtual bool IsTrigger(const int nPadIdx) const override;	// �����L�[�g���K�[�擾
	virtual bool IsStickTilt(const int nPadIdx) const override;	// �����X�e�B�b�N�X���擾
	virtual float GetRotation() const override;					// �����L�[�����擾
	virtual CPlayer::EDashAngle GetAngle() const override;		// �����L�[�����񋓎擾
};

#endif

//==========================================================================
// 
//  ��ރL�[�w�b�_�[ [bindKeyBack.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _BIND_KEY_BACK_H_
#define _BIND_KEY_BACK_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "bindKey.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ��ރL�[�N���X
class CBindKeyBack : public CBindKey
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CBindKeyBack();
	virtual ~CBindKeyBack();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual bool IsPress(const int nPadIdx = 0) const override;		// �����L�[�v���X�擾
	virtual bool IsTrigger(const int nPadIdx = 0) const override;	// �����L�[�g���K�[�擾
	virtual float GetRotation() const override;						// �����L�[�����擾
	virtual CPlayer::EDashAngle GetAngle() const override;			// �����L�[�����񋓎擾
};

#endif

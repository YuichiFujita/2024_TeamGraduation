//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYERCONTROL_H_
#define _PLAYERCONTROL_H_	// ��d�C���N���[�h�h�~

#define BRESSRANGE (0)

#include "player.h"

class CSuffocation;
class CEffekseerObj;

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerControlMove
{
public:

	// �R���X�g���N�^
	CPlayerControlMove() {}

	virtual void Move(CPlayer* player);	// �ړ�

private:
	



};


#endif
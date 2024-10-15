//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYERCONTROL_ACTION_H_
#define _PLAYERCONTROL_ACTION_H_	// ��d�C���N���[�h�h�~

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
class CPlayerControlAction
{
public:

	// �R���X�g���N�^
	CPlayerControlAction();

	virtual void Action(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �L���b�`

private:
	
	//=============================
	// �����o�֐�
	//=============================
	void Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �L���b�`
	void Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ����

	//=============================
	// �����o�ϐ�
	//=============================
};


#endif
//=============================================================================
// 
//  �v���C���[AI�R���g���[��_�ړ��w�b�_�[ [playerAIcontrol_move.h]
//  Author :Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAI_CONTROL_START_H_
#define _PLAYERAI_CONTROL_START_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerAIcontrol_mode.h"

//==========================================================================
// �O���錾
//==========================================================================
class CSuffocation;
class CEffekseerObj;
class CPlayerUserControlMove;

//==========================================================================
// �v���C���[AI�R���g���[��_�ړ��N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerAIControlStart : public CPlayerAIControlMode
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerAIControlStart();
	~CPlayerAIControlStart();

	static CPlayerAIControlStart* Create(CPlayer* pPlayer);

	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

private:
	//=============================
	// �����o�֐�
	//=============================
	void Gooooo();
	void SeeBall();							// �{�[��������
	bool IsPicksUpBall();					// �{�[�����E�����f

	//=============================
	// �����o�ϐ�
	//=============================

};

#endif
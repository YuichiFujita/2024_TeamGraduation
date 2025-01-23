//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_H_
#define _PLAYERAICONTROL_H_	// ��d�C���N���[�h�h�~

#include "player.h"
#include "playerAIcontrol_mode.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIControlMove;		// �ړ�(AI)
class CPlayerAIControlAction;	// �A�N�V����(AI)
class CPlayerAIControlMode;		// ���[�h(AI)
class CPlayer;

//==========================================================================
// �萔��`
//==========================================================================

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
class CPlayerAIControl
{
public:

	// �R���X�g���N�^
	CPlayerAIControl();

	// ����
	static CPlayerAIControl* Create(CPlayer* player);

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//-----------------------------
	// �ݒ�,�擾
	//-----------------------------
	void SetPlayer(CPlayer* player) { m_pAI = player; }						// �����̐ݒ�
	CPlayer* GetPlayer() { return m_pAI; }									// �擾

	void SetMode(CPlayerAIControlMode::EMode mode) { m_pAIControlMode->SetMode(mode); }	// ���[�h�ݒ�
	CPlayerAIControlMode::EMode GetMode() { return  m_pAIControlMode->GetMode(); }		// �擾

private:
	//=============================
	// �����o�֐�
	//=============================
	void ModeStart();
	void ModeManager();
	void ChangeMode(CPlayer* pPlayer, CPlayerAIControlMode::EMode mode);

	//=============================
	// �����o�ϐ�
	//=============================
	CPlayer* m_pAI;							// �������
	CPlayerAIControlMode* m_pAIControlMode;	// ���[�h

	bool m_bStart;
};

#endif

//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_H_
#define _PLAYERAICONTROL_H_	// ��d�C���N���[�h�h�~

#include "player.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIControlMove;		// �ړ�(AI)
class CPlayerAIControlAction;	// �A�N�V����(AI)
class CPlayer;

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
class CPlayerAIOutControl
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EMode	// ���[�h
	{
		MODE_NONE = 0,	// �Ȃ�
		MODE_THROW,		// ����
		MODE_MOVE,		// �s��
		MODE_MAX
	};

private:

	//=============================
	// �񋓌^��`
	//=============================
	enum EThrow
	{// ����
		THROW_NONE = 0,
		THROW_NORMAL,
		THROW_PASS,
		THROW_MAX
	};

	enum EMove
	{// �s��
		MOVE_NONE = 0,
		MOVE_FIND,
		MOVE_MEETING,
		MOVE_MAX
	};

public:

	// �R���X�g���N�^
	CPlayerAIOutControl();

	static CPlayerAIOutControl* Create(CPlayer* player);

	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetMode(EMode mode) { m_eMode = mode; }
	void SetPlayerInfo(CPlayer* player) { m_pAIOut = player; }

private:
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPlayerAIOutControl::* MODE_FUNC)(const float, const float, const float);
	static MODE_FUNC m_ModeFunc[];			// ���[�h�֐�

	typedef void(CPlayerAIOutControl::* THROW_FUNC)(const float, const float, const float);
	static THROW_FUNC m_ThrowFunc[];			// ���[�h�֐�

	typedef void(CPlayerAIOutControl::* MOVE_FUNC)(const float, const float, const float);
	static MOVE_FUNC m_MoveFunc[];			// �s���֐�

	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// �Ȃ�
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��������
	void ModeMoveManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �L���b�`����

	// ����
	void ThrowNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// �Ȃ�
	void ThrowNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �ʏ�
	void ThrowPass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �p�X

	// �s��
	void MoveNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �Ȃ�
	void MoveRetrieve(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���
	void MoveMeeting(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �Ζ�


	//=============================
	// �����o�֐�
	//=============================
	void ModeManager();				// ���[�h�Ǘ�
	CPlayer* GetThrowTarget();		// ������^�[�Q�b�g
	void Pass();					// �p�X
	bool IsPass();					// �p�X����
	void Throw();					// ������
	void LookBall();				// �{�[��������
	void AreaCheck();				// �{�[���̃G���A

	//-----------------------------
	// ���̑��֐�
	//-----------------------------

	//=============================
	// �����o�ϐ�
	//=============================
	CPlayer* m_pAIOut;			// �������g
	CPlayer* m_pTarget;
	EMode m_eMode;
	EThrow m_eThrow;
	EMove m_eMove;

	bool m_bStart;
	bool m_bEnd;
};

#endif

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
		MODE_CATCH,		// �L���b�`
		MODE_MAX
	};

private:

	//=============================
	// �񋓌^��`
	//=============================
	enum ECatch
	{
		CATCH_NONE = 0,
		CATCH_FIND,
		CATCH_MAX
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


	typedef void(CPlayerAIOutControl::* CATCH_FUNC)(const float, const float, const float);
	static CATCH_FUNC m_CatchFunc[];			// �L���b�`�֐�


	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// �Ȃ�
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��������
	void ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �L���b�`����

	// �L���b�`
	void CatchNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void RetrieveBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);


	//=============================
	// �����o�֐�
	//=============================
	void ModeManager();
	CPlayer* GetThrowTarget();		// ������^�[�Q�b�g
	void Pass();
	bool IsPass();
	void Throw();

	void LookBall();

	void AreaCheck();

	//-----------------------------
	// ���̑��֐�
	//-----------------------------

	//=============================
	// �����o�ϐ�
	//=============================
	CPlayer* m_pAIOut;			// �������g

	EMode m_eMode;
	ECatch m_eCatch;

	bool m_bStart;
	bool m_bEnd;
};

#endif

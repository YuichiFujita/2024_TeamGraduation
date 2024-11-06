//=============================================================================
// 
//  AI�v���C���[�w�b�_�[ [playerAI.h]
//  Author : ���c�E��
// 
//=============================================================================

#ifndef _PLAYER_AI_
#define _PLAYER_AI_		// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "player.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIControlMove;		// �ړ�(AI)
class CPlayerAIControlAction;	// �A�N�V����(AI)

//==========================================================================
// �N���X��`
//==========================================================================
// AI�v���C���[�N���X
class CPlayerAI : public CPlayer
{
public:
	//=============================
	// �񋓌^��`
	//=============================
	enum EThrowMode
	{
		MODE_NONE = 0,	// �ʏ�
		MODE_WALK,		// ����
		MODE_DASH,		// ����
		MODE_QUICK,		// ������
		MODE_DELAY,		// �x�点��
		MODE_MAX
	};
	
	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerAI();
	~CPlayerAI();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;

	//=============================
	// �����o�֐�
	//=============================
	void Kill();	// �폜

	//=============================
	// �p�^�[��
	//=============================
	void ChangeMoveControl(CPlayerAIControlMove* control);		// �ړ��̑���ύX
	void ChangeActionControl(CPlayerAIControlAction* control);	// �A�N�V�����̑���ύX

	//=============================
	// �ÓI�֐�
	//=============================
	/*
		@brief		��������
		@details	�v���C���[�C���f�b�N�X
		@details	�`�[���T�C�h
	*/
	static CPlayerAI *Create(const CGameManager::TeamSide team, const MyLib::Vector3& rPos);

protected:

	virtual void Debug() override;	// �f�o�b�O����

private:
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPlayerAI::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐�

	//=============================
	// �����o�֐�
	//=============================
	SHitInfo Hit(CBall* pBall) override;

	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��ԍX�V
	void StateNone();				// �Ȃ�
	void StateThrowManager(CBall* pBall);	// 
	void StateCatchManager();

	void Throw();	// �ʏ퓊��
	void JumpThrow();	// �W�����v����
	void SpecialThrow();	// �X�y�V��������

	//-----------------------------
	// ���̑��֐�
	//-----------------------------
	void Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// ����
	void DeleteControl() override;	// ����폜

	//=============================
	// �����o�ϐ�
	//=============================
	EThrowMode m_throwMode;
};

#endif

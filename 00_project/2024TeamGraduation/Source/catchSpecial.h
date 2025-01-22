//=============================================================================
// 
//  �L���b�`�X�y�V�������� [catchSpecial.h]
//  Author : Kai Takada
// 
//=============================================================================

#ifndef _CATCHSPECIAL_H_
#define _CATCHSPECIAL_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "ball.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayer;

//==========================================================================
// �N���X��`
//==========================================================================
// �L���b�`�X�y�V�����N���X��`
class CCatchSpecial
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// ��Ԓ�`
	enum EState
	{
		STATE_NONE = 0,			// �Ȃɂ��Ȃ�
		STATE_BEAM_SUCC,		// �X�y�V�����L���b�`(�r�[��)(����)
		STATE_BEAM_FAIL,		// �X�y�V�����L���b�`(�r�[��)(���s)
		STATE_MAX
	};

	// ��ԓ���Ԓ�`(����)
	enum EMomentumState
	{
		MOMENTUM_NONE = 0,		// �Ȃ�
		MOMENTUM_SLIDE,			// �Y�U�U�[
		MOMENTUM_BRAKE,			// �ς���
		MOMENTUM_RESULT,		// ����(����)(���s)
		MOMENTUM_END,			// �I��
		MOMENTUM_MAX
	};

	CCatchSpecial(CPlayer* pPlayer, EState state);
	~CCatchSpecial();

	//=============================
	// �����o�֐�
	//=============================
	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetState(EState state);									// ��Ԑݒ�
	EState GetState() { return m_state; }							// ��Ԏ擾
	void SetMomentumState(EMomentumState state);					// ������Ԑݒ�
	EMomentumState GetMomentumState() { return m_momentumState; }	// ������Ԑݒ�

	void SetEnableSuccess(bool bSuccess) { m_bSuccess = bSuccess; }		// �����t���O�ݒ�
	bool IsState() { return m_bSuccess; }								// �����t���O�擾

	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }		// �v���C���[�ݒ�
	CPlayer* GetPlayer() { return m_pPlayer; }						// �v���C���[�擾

	void Debug();		// �����t���O�擾

	//-----------------------------
	// ����֐�
	//-----------------------------
	static EState Check_Kamehameha(const CPlayer* pPlayer, const bool bJust);

	static CCatchSpecial::EState Check(CPlayer* pPlayer, bool bJust, CBall::ESpecial state);		// �`�F�b�N
	static CCatchSpecial* Create(CPlayer* pPlayer, EState state);			// ����

private:
	//=============================
	// �֐����X�g
	//=============================
	// ��Ԋ֐�
	typedef void(CCatchSpecial::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐�

	// ������Ԋ֐�
	typedef void(CCatchSpecial::* MOMENTUM_FUNC)(const float, const float, const float);
	static MOMENTUM_FUNC m_MomentumFunc[];	// ��ԓ���Ԋ֐�

	// ��ԊJ�n�֐�
	typedef void(CCatchSpecial::* START_FUNC)();
	static START_FUNC m_StartFunc[];	// ��ԊJ�n�֐�

	// ������ԊJ�n�֐�
	typedef void(CCatchSpecial::* MOMENTUM_START_FUNC)();
	static MOMENTUM_START_FUNC m_MomentumStartFunc[];	// ��ԊJ�n�֐�

	// ����֐�
	static std::vector<std::function<EState(const CPlayer*, const bool)>> s_CheckFunc;

	//=============================
	// �����o�֐�
	//=============================
	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// �Ȃ�
	void StateKamehameSucc(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���߂͂ߔg�H�L���b�`����
	void StateKamehameFail(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���߂͂ߔg�H�L���b�`���s

	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	void MomentumStateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �Ȃ�
	void MomentumStateSlide(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �Y�U�U�[
	void MomentumStateBrake(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �ς���
	void MomentumStateResult(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ����(����)(���s)
	void MomentumStateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �I��
	
	//-----------------------------
	// ��ԊJ�n�֐�
	//-----------------------------
	void StateStartNone();		// �Ȃ�

	//-----------------------------
	// ������ԊJ�n�֐�
	//-----------------------------
	void MomentumStartSlide();		// �Y�U�U�[
	void MomentumStartResult();		// ����

	void Success();		// ����������
	void Failure();		// ���s������

	//=============================
	// �����o�ϐ�
	//=============================
	EState m_state;					// ���
	float m_fStateTime;				// ��Ԏ���
	EMomentumState m_momentumState;	// ��ԓ����
	float m_fMomentumStateTime;		// ��ԓ���Ԏ���
	CPlayer* m_pPlayer;				// �v���C���[
	bool m_bSuccess;				// �����t���O
};

#endif

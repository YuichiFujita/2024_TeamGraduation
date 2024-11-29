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

	CCatchSpecial(CPlayer* pPlayer, EState state);
	~CCatchSpecial();

	//=============================
	// �����o�֐�
	//=============================
	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetState(EState state);			// ��Ԑݒ�
	EState GetState() { return m_state; }	// ��Ԏ擾

	void SetEnableSuccess(bool bSuccess) { m_bSuccess = bSuccess; }		// �����t���O�ݒ�
	bool IsState() { return m_bSuccess; }								// �����t���O�擾

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

	static std::vector<std::function<EState(const CPlayer*, const bool)>> s_CheckFunc;

	//=============================
	// �����o�֐�
	//=============================
	//-----------------------------
	// �X�y�V�����L���b�`��Ԋ֐�
	//-----------------------------
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// �Ȃ�
	void State_Kamehame_Succ(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���߂͂ߔg�H�L���b�`����
	void State_Kamehame_Fail(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���߂͂ߔg�H�L���b�`���s

	void Success();		// ����������
	void Failure();		// ���s������

	//=============================
	// �����o�ϐ�
	//=============================
	EState m_state;				// ���
	float m_fStateTime;			// ��Ԏ���
	CPlayer* m_pPlayer;			// �v���C���[
	bool m_bSuccess;			// �����t���O
};

#endif

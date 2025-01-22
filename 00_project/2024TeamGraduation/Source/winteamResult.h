//==========================================================================
//
//	�����`�[���w�b�_�[ [winteamResult.h]
//	Author�F���n�Ή�
//
//==========================================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _WINTEAM_RESULT_H_
#define _WINTEAM_RESULT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object2D_Anim.h"
#include "arrowUI.h"
#include "gameManager.h"

//************************************************************
// �N���X��`
//************************************************************
// �����`�[���N���X
class CWinTeamResult : public CObject2D_Anim
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EState
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_SPAWN,	// �o��
		STATE_LOOP,		// ���[�v
		STATE_FADEOUT,	// �t�F�[�h�A�E�g
		STATE_MAX
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CWinTeamResult(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CWinTeamResult() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V

	//=============================
	// �����o�֐�
	//=============================
	void SetState(EState state);			// ��Ԑݒ�
	EState GetState() { return m_state; }	// ��Ԏ擾
	void SetSizeByWidth(const float width);	// ��������T�C�Y�ݒ�

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CWinTeamResult* Create(CGameManager::ETeamSide winSIde);	// ����

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��Ԋ֐�
	typedef void(CWinTeamResult::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐�

	//=============================
	// �����o�֐�
	//=============================
	// ���
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��ԍX�V
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {}	// �Ȃ�
	void StateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �o��
	void StateLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���[�v
	void StateFadeOut(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �t�F�[�h�A�E�g

	// ����
	HRESULT CreateAudience();	// �ϋq�̐���

	//=============================
	// �����o�ϐ�
	//=============================
	// ���
	EState m_state;		// ���
	float m_fStateTime;	// ��ԃ^�C�}�[

	// ���̑�
	CGameManager::ETeamSide m_winTeam;	// �����`�[��
	CObject2D_Anim* m_pAudience;	// �ϋq
};

#endif	// _ARROW_UI_H_

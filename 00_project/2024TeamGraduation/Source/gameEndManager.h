//============================================================
//
//	�Q�[���I�����o�}�l�[�W���[�w�b�_�[ [gameEndManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GAME_END_MANAGER_H_
#define _GAME_END_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CLightPoint;
class CPlayer;

//************************************************************
//	�N���X��`
//************************************************************
// �Q�[���I�����o�}�l�[�W���[�N���X
class CGameEndManager : public CObject
{
public:
	// �萔
	static constexpr int NUM_LIGHT = 4;	// ���C�g��

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,		// �������Ȃ����
		STATE_LOOK_PLAYER,	// �v���C���[�J�������
		STATE_DEATH_WAIT,	// �v���C���[���S�ҋ@���
		STATE_GAME_SET,		// �Q�[���Z�b�g�o�����
		STATE_WAIT,			// �ҋ@���
		STATE_END,			// �I�����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CGameEndManager(CPlayer* pDeathPlayer);

	// �f�X�g���N�^
	~CGameEndManager() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw() override;		// �`��

	// �����o�֐�
	void SetState(const EState state)	{ m_state = state; }	// ��Ԑݒ�
	EState GetState(void) const			{ return m_state; }		// ��Ԏ擾

	// �ÓI�����o�֐�
	static CGameEndManager *Create(CPlayer* pDeathPlayer);	// ����

private:
	// �G�C���A�X��`
	typedef void (CGameEndManager::*AFuncUpdateState)(const float, const float, const float);	// ��ԍX�V�̊֐��|�C���^�^

	// �ÓI�����o�ϐ�
	static AFuncUpdateState m_aFuncUpdateState[];	// ��ԍX�V�֐�
	static CGameEndManager* m_pInstance;			// ���g�̃C���X�^���X

	// �����o�֐�
	void UpdateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �������Ȃ��X�V
	void UpdateLookPlayer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �v���C���[�J�����X�V
	void UpdateDeathWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �v���C���[���S�ҋ@�X�V
	void UpdateGameSet(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �Q�[���Z�b�g�o���X�V
	void UpdateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �ҋ@�X�V
	void UpdateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �I���X�V
	void SetLightPosition();	// ���C�g�ʒu�ݒ�

	// �����o�ϐ�
	CLightPoint* m_apLight[NUM_LIGHT];	// ���C�g���
	CPlayer* m_pDeathPlayer;	// ���S�v���C���[
	EState m_state;		// ���
	float m_fCurTime;	// ���݂̑ҋ@����
};

#endif	// _GAME_END_MANAGER_H_

//============================================================
//
//	�v���C���[�o�ꉉ�o�w�b�_�[ [playerSpawnManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_SPAWN_MANAGER_H_
#define _PLAYER_SPAWN_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CPlayer;

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�o�ꉉ�o�N���X
class CPlayerSpawnManager : public CObject
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,		// �������Ȃ����
		STATE_CUTIN,		// �J�b�g�C�����
		STATE_END,			// �I�����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CPlayerSpawnManager();

	// �f�X�g���N�^
	~CPlayerSpawnManager() override;

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
	static CPlayerSpawnManager* Create();		// ����
	static CPlayerSpawnManager* GetInstance();	// �擾

private:
	// �G�C���A�X��`
	typedef void (CPlayerSpawnManager::*AFuncUpdateState)(const float, const float, const float);	// ��ԍX�V�̊֐��|�C���^�^

	// �ÓI�����o�ϐ�
	static AFuncUpdateState m_aFuncUpdateState[];	// ��ԍX�V�֐�
	static CPlayerSpawnManager* m_pInstance;		// ���g�̃C���X�^���X

	// �����o�֐�
	void UpdateCutIn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �J�b�g�C���X�V

	// �����o�ϐ�
	EState m_state;		// ���
	float m_fCurTime;	// ���݂̑ҋ@����
};

#endif	// _PLAYER_SPAWN_MANAGER_H_

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
#include "listmanager.h"

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
		STATE_WALK_Z,		// Z���ړ����
		STATE_ROTATE,		// ��]���
		STATE_WALK_X,		// X���ړ����
		STATE_BOW,			// �����V���
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
	void RegistPlayer(CPlayer* pPlayer);		// �v���C���[�o�^
	void DeletePlayer(CPlayer* pPlayer);		// �v���C���[�폜
	inline void SetState(const EState state)	{ m_state = state; }	// ��Ԑݒ�
	inline EState GetState(void) const			{ return m_state; }		// ��Ԏ擾

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
	void UpdateWalkAxisZ(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// Z���ړ���Ԃ̍X�V
	void UpdateRotate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ��]��Ԃ̍X�V
	void UpdateWalkAxisX(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// X���ړ���Ԃ̍X�V
	void UpdateBow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �����V��Ԃ̍X�V

	// �����o�ϐ�
	CListManager<CPlayer> m_listLeft;	// ���`�[���v���C���[
	CListManager<CPlayer> m_listRight;	// �E�`�[���v���C���[
	EState m_state;		// ���
	float m_fCurTime;	// ���݂̑ҋ@����
};

#endif	// _PLAYER_SPAWN_MANAGER_H_

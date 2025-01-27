//============================================================
//
//	�X�y�V�������o�}�l�[�W���[�w�b�_�[ [specialManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SPECIAL_MANAGER_H_
#define _SPECIAL_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CPlayer;
class CLightPoint;
class CSpotLight;
class CCutIn;

//************************************************************
//	�N���X��`
//************************************************************
// �X�y�V�������o�}�l�[�W���[�N���X
class CSpecialManager : public CObject
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,		// �������Ȃ����
		STATE_CUTIN,		// �J�b�g�C�����
		STATE_HYPE_TRANS,	// ����オ��J�ڏ��
		STATE_HYPE,			// ����オ����
		STATE_STAG,			// �X�y�V�������o���	// ���̏�Ԃ̓X�y�V�������ƂɃJ�������[�N�ύX
		STATE_FOLLOW_TRANS,	// �Ǐ]�J�ڏ��
		STATE_END,			// �I�����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CSpecialManager(CPlayer* pAttack, CPlayer* pTarget);

	// �f�X�g���N�^
	~CSpecialManager() override;

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
	static CSpecialManager *Create(CPlayer* pAttack, CPlayer* pTarget);	// ����

private:
	// �W�����v�v���C���[�\����
	struct SPlayerJump
	{
		CPlayer* pPlayer;				// �v���C���[���
		MyLib::Vector3 posJumpStart;	// �W�����v�J�n�ʒu
		MyLib::Vector3 posJumpEnd;		// �W�����v�I���ʒu
	};

	// �G�C���A�X��`
	typedef void (CSpecialManager::*AFuncUpdateState)(const float, const float, const float);	// ��ԍX�V�̊֐��|�C���^�^
	typedef void (CSpecialManager::*AFuncUpdateSpecial)(const float, const float, const float);	// �X�y�V�����X�V�̊֐��|�C���^�^

	// �ÓI�����o�ϐ�
	static AFuncUpdateState m_aFuncUpdateState[];		// ��ԍX�V�֐�
	static AFuncUpdateSpecial m_aFuncUpdateSpecial[];	// �X�y�V�����X�V�֐�
	static CSpecialManager* m_pInstance;				// ���g�̃C���X�^���X

	// �����o�֐�
	void UpdateCutIn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �J�b�g�C���X�V
	void UpdateHypeTrans(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ����オ��J�ڍX�V
	void UpdateHype(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ����オ��X�V
	void UpdateStag(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �X�y�V�������o�X�V
	void UpdateFollowTrans(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �Ǐ]�J�ڍX�V
	void UpdateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �I���X�V
	void UpdateKamehameha(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���߂͂ߔg�̍X�V
	MyLib::Vector3 GetDestAttackPosition() const;	// �U���v���C���[�ڕW�ʒu�擾
	void SetPlayerHypePosition();	// �v���C���[����グ�ʒu�ݒ�
	void SetJumpAttackTeam();		// �U���`�[���̃W�����v�ݒ�
	void SetLightPosition();		// ���C�g�ʒu�ݒ�
	HRESULT SetDarkGym();			// �̈�وÂ�����ݒ�

	// �����o�ϐ�
	CLightPoint* m_pCenterLight;	// ���C�g���
	CSpotLight* m_pAttackLight;		// �U���v���C���[���Ƃ炷���C�g
	CSpotLight* m_pTargetLight;		// �W�I�v���C���[���Ƃ炷���C�g
	CPlayer* m_pAttackPlayer;		// �U���v���C���[
	CPlayer* m_pTargetPlayer;		// �W�I�v���C���[
	CCutIn* m_pCutIn;	// �J�b�g�C�����
	EState m_state;		// ���
	float m_fCurTime;	// ���݂̑ҋ@����
	bool m_bJump;		// �W�����v�t���O
	float m_fJumpTime;	// ���݂̃W�����v����
	std::vector<SPlayerJump> m_vecJump;	// �W�����v�v���C���[���
};

#endif	// _SPECIAL_MANAGER_H_

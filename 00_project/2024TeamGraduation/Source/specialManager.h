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
		STATE_NONE = 0,	// �������Ȃ����
		STATE_FADEOUT,	// �t�F�[�h�A�E�g���
		STATE_END,		// �I�����
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CSpecialManager(const CPlayer* pAttack, const CPlayer* pTarget);

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
	static CSpecialManager *Create(const CPlayer* pAttack, const CPlayer* pTarget);	// ����

private:
	// ��ԍX�V�̊֐��|�C���^�^�G�C���A�X��`
	typedef void (CSpecialManager::*AFuncUpdateState)(const float fDeltaTime);

	// �ÓI�����o�ϐ�
	static AFuncUpdateState m_aFuncUpdateState[];	// ��ԍX�V�֐�
	static CSpecialManager* m_pThisClass;			// ���g�̃C���X�^���X

	// �����o�֐�
	void UpdateFadeOut(const float fDeltaTime);	// �t�F�[�h�A�E�g�X�V
	void UpdateEnd(const float fDeltaTime);		// �I���X�V

	void SetLightPosition();	// ���C�g�ʒu�ݒ�

	// �����o�ϐ�
	const CPlayer* m_pAttackPlayer;	// �U���v���C���[
	const CPlayer* m_pTargetPlayer;	// �W�I�v���C���[
	CLightPoint* m_pAttackLight;	// �U���v���C���[���Ƃ炷���C�g
	CLightPoint* m_pTargetLight;	// �W�I�v���C���[���Ƃ炷���C�g
	EState m_state;		// ���
	float m_fCurTime;	// ���݂̑ҋ@����
};

#endif	// _SPECIAL_MANAGER_H_

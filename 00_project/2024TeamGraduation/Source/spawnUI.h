//============================================================
//
//	�o�ꉉ�o�`�[�����\��UI�w�b�_�[ [spawnUI.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SPAWN_UI_H_
#define _SPAWN_UI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "gamemanager.h"

//************************************************************
// �O���錾
//************************************************************
class CObject2D_Anim;
class CScrollString2D;

//************************************************************
//	�N���X��`
//************************************************************
// �o�ꉉ�o�`�[�����\��UI�N���X
class CSpawnUI : public CObject
{
public:
	// �R���X�g���N�^
	CSpawnUI(const CGameManager::ETeamSide team);

	// �f�X�g���N�^
	~CSpawnUI() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw() override;		// �`��
	void SetPosition(const MyLib::Vector3& rPos) override;	// �ʒu�ݒ�
	void SetEnableDisp(const bool bDisp) override;			// �`��󋵐ݒ�

	// �ÓI�����o�֐�
	static CSpawnUI* Create(const CGameManager::ETeamSide team);	// ����

	// �����o�֐�
	inline void SetSpawn() { m_state = STATE_SPAWN; }	// �����J�n�ݒ�

private:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_SPAWN,	// �������
		STATE_HUTATUNA,	// ������
		STATE_TEAMNAME,	// �`�[�������
		STATE_DESPAWN,	// �������
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// ��Ԋ֐����X�g
	typedef void(CSpawnUI::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFuncList[];	// �֐��̃��X�g

	// �����o�֐�
	void UpdateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �������Ȃ��̍X�V
	void UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �����̍X�V
	void UpdateHutatuna(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ����̍X�V
	void UpdateTeamname(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �`�[�����̍X�V
	void UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �����̍X�V
	void SetPositionRelative();	// ���Έʒu�ݒ�
	HRESULT CreateUI();	// UI����

	// �����o�ϐ�
	CObject2D_Anim* m_pBG;			// �w�i���
	CScrollString2D* m_pHutatuna;	// ������
	CScrollString2D* m_pTeamName;	// �`�[�������
	const CGameManager::ETeamSide m_team;	// �`�[��
	EState m_state;		// ���
	float m_fCurTime;	// �o�ߎ���
};

#endif	// _SPAWN_UI_H_

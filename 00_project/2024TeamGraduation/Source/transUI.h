//============================================================
//
//	�J��UI�w�b�_�[ [transUI.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TRANS_UI_H_
#define _TRANS_UI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
// �O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �J��UI�N���X
class CTransUI : public CObject
{
public:
	// �R���X�g���N�^
	CTransUI();

	// �f�X�g���N�^
	~CTransUI() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw() override;		// �`��
	void SetEnableDisp(const bool bDisp) override;	// �`��󋵐ݒ�

	// �ÓI�����o�֐�
	static CTransUI* Create();	// ����

	// �����o�֐�
	bool UpdateDecide(const bool bAutoUninit = false);	// ���葀��X�V
	void SetEnableDispUI(const bool bDisp);				// UI�\���󋵐ݒ�

private:
	// ��ԗ�
	enum EState
	{
		STATE_DISP_OFF = 0,	// �\��OFF���
		STATE_SPAWN_BG,		// �w�i�������
		STATE_SPAWN_STRING,	// �����������
		STATE_DISP_ON,		// �\��ON���
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// ��Ԋ֐����X�g
	typedef void(CTransUI::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFuncList[];	// �֐��̃��X�g

	// �����o�֐�
	void UpdateDispOFF(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �\��OFF�̍X�V
	void UpdateSpawnBG(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �w�i�����̍X�V
	void UpdateSpawnString(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���������̍X�V
	void UpdateDispON(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �\��ON�̍X�V
	void SetDisp(const bool bDisp);	// �\���󋵐ݒ�
	bool IsDispTransState();		// �\���J�ڏ�Ԃ��̊m�F
	HRESULT CreateUI();				// UI����

	// �����o�ϐ�
	CObject2D* m_pBG;		// �w�i���
	CObject2D* m_pString;	// �������
	EState m_state;			// ���
	float m_fCurTime;		// �o�ߎ���
};

#endif	// _TRANS_UI_H_

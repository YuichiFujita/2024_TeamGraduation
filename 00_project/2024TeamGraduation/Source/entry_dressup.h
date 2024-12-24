//==========================================================================
// 
//  �h���X�A�b�v�ݒ菈�� [entry_dressup.h]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
// 
//==========================================================================

#ifndef _ENTRY_DRESSUP_H_
#define _ENTRY_DRESSUP_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "entryscene.h"
#include "player.h"

//==========================================================================
// �O���錾
//==========================================================================
class CDressupUI;
class CEntryRuleManager;

//==========================================================================
// �N���X��`
//==========================================================================
// �h���X�A�b�v�ݒ�N���X
class CEntry_Dressup : public CEntryScene
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// ���
	enum EState
	{
		STATE_DRESSUP = 0,	// �����ւ����
		STATE_SETTING,		// �Q�[���ݒ���
		STATE_END,			// �I�����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CEntry_Dressup();
	~CEntry_Dressup();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;	// ������
	virtual void Uninit() override;		// �I��
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V

	//-----------------------------
	// ���̑�
	//-----------------------------
	virtual void Debug() override;

	//=============================
	// �����o�֐�
	//=============================
	void SetState(const EState state);					// ��Ԑݒ�
	inline EState GetState() const { return m_state; }	// ��Ԏ擾

private:

	//=============================
	// �����o�֐�
	//=============================
	bool TransSetupTeam();	// �`�[���ݒ�J��
	void TransSetting();	// �����ւ��J��
	bool IsAllReady();		// �����S�����t���O�擾
	void Save();			// �Z�[�u
	void Load();			// ���[�h

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<CDressupUI*> m_vecDressInfo;	// �����ւ����
	CEntryRuleManager* m_pRuleManager;	// ���[���}�l�[�W���[
	EState m_state;	// ���
};

#endif
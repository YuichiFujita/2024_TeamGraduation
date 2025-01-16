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
class CSelectUI;
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
	inline CEntry_Dressup* GetDressupTeam() override { return this; }	// �h���X�A�b�v�ݒ�V�[���擾

	//-----------------------------
	// ���̑�
	//-----------------------------
	virtual void Debug() override;

	//=============================
	// �����o�֐�
	//=============================
	void SetState(const EState state);					// ��Ԑݒ�
	inline EState GetState() const	{ return m_state; }	// ��Ԏ擾
	void SetDressUIReady(const int nPlayerIdx, const bool bReady);			// �����ւ�UI���������t���O�̐ݒ�
	bool IsDressUIReady(const int nPlayerIdx);								// �����ւ�UI���������ς݂��̊m�F
	void SetDressUIControl(const int nPadIdx, const int nPlayerIdx);		// �����ւ�UI���쌠�̐ݒ�
	bool IsSelectOK(const int nPadIdx, const int nPlayerIdx) const;			// �I���\���̊m�F
	void SetSelectUISelect(const int nPadIdx, const bool bSelect);			// �I��UI�I�𑀍�t���O�̐ݒ�
	MyLib::Vector3 GetDressUIPosition(const int nPlayerIdx) const;			// �����ւ�UI�ʒu�擾
	inline int GetNumPlayer() const	{ return (int)m_vecDressInfo.size(); }	// �v���C���[���擾

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
	std::vector<CSelectUI*> m_vecSelect;	// �I�����
	CEntryRuleManager* m_pRuleManager;	// ���[���}�l�[�W���[
	EState m_state;	// ���
};

#endif
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
#include "playerManager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CString2D;
class CDressupUI;
class CSelectUI;
class CObject2D;
class CObject2D_Anim;
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

	// �J�ڐ�
	enum ETrans
	{
		TRANS_BACK = 0,	// �߂�
		TRANS_NEXT,		// �i��
		TRANS_MAX		// ���̗񋓌^�̑���
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
	bool TransSetting();								// �����ւ��J��
	void SetState(const EState state);					// ��Ԑݒ�
	inline EState GetState() const { return m_state; }	// ��Ԏ擾
	void SetDressUIReady(const int nPlayerIdx, const bool bReady);			// �����ւ�UI���������t���O�̐ݒ�
	bool IsDressUIReady(const int nPlayerIdx);								// �����ւ�UI���������ς݂��̊m�F
	void SetDressUIControl(const int nPadIdx, const int nPlayerIdx);		// �����ւ�UI���쌠�̐ݒ�
	bool IsSelectOK(const int nPadIdx, const int nPlayerIdx) const;			// �I���\���̊m�F
	void SetSelectUISelect(const int nPadIdx, const bool bSelect);			// �I��UI�I�𑀍�t���O�̐ݒ�
	MyLib::Vector3 GetDressUIPosition(const int nPlayerIdx) const;			// �����ւ�UI�ʒu�擾
	inline int GetNumPlayer() const { return (int)m_vecDressInfo.size(); }	// �v���C���[���擾
	MyLib::Vector3 GetNameUIPosition(const CGameManager::ETeamSide team);	// ���OUI�ʒu�擾
	MyLib::Vector2 GetNameUISize(const CGameManager::ETeamSide team);		// ���OUI�傫���擾
	MyLib::Vector3 GetAreaUIPosition(const CPlayer::EFieldArea area);		// �|�W�V�����ύXUI�ʒu�擾
	MyLib::Vector2 GetAreaUISize(const CPlayer::EFieldArea area);			// �|�W�V�����ύXUI�傫���擾
	MyLib::Vector3 GetTransUIPosition(const ETrans trans);					// �J��UI�ʒu�擾
	MyLib::Vector2 GetTransUISize(const ETrans trans);						// �J��UI�傫���擾
	inline CString2D* GetNameString2D(const CGameManager::ETeamSide team) { return m_apTeamName[team]; }	// ���O������|�C���^�擾

private:

	//=============================
	// �����o�֐�
	//=============================
	bool IsAllReady();	// �����S�����t���O�擾
	void Save();		// �Z�[�u
	void Load();		// ���[�h

	//=============================
	// �����o�ϐ�
	//=============================
	CString2D* m_apTeamName[CGameManager::SIDE_MAX];	// �`�[�������
	CDressupUI* m_apDressInfo[CPlayerManager::OUT_MAX];	// �O�쒅���ւ����
	CObject2D_Anim* m_apAreaUI[CGameManager::SIDE_MAX];	// �|�W�V�����ύXUI���
	CObject2D* m_apTransUI[TRANS_MAX];			// �J��UI���
	std::vector<CDressupUI*> m_vecDressInfo;	// ���쒅���ւ����
	std::vector<CSelectUI*> m_vecSelect;		// �I�����
	CEntryRuleManager* m_pRuleManager;			// ���[���}�l�[�W���[
	EState m_state;								// ���
};

#endif
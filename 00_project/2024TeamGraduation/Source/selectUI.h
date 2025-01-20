//============================================================
//
//	�I��UI�w�b�_�[ [selectUI.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SELECT_UI_H_
#define _SELECT_UI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "gamemanager.h"

//************************************************************
// �O���錾
//************************************************************
class CObject2D;		// �I�u�W�F�N�g2D�N���X
class CObject2D_Anim;	// �I�u�W�F�N�g2D�A�j���N���X
class CEntry_Dressup;	// �����ւ��V�[���N���X

//************************************************************
//	�N���X��`
//************************************************************
// �I��UI�N���X
class CSelectUI : public CObject
{
public:
	// �R���X�g���N�^
	CSelectUI(CGameManager::ETeamSide team, const int nPlayerIdx, const int nPadIdx);

	// �f�X�g���N�^
	~CSelectUI() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw() override;		// �`��
	void SetEnableDisp(const bool bDraw) override;			// �`��󋵐ݒ�
	void SetPosition(const MyLib::Vector3& pos) override;	// �ʒu�ݒ�

	// �ÓI�����o�֐�
	static CSelectUI* Create(CGameManager::ETeamSide team, const int nPlayerIdx, const int nPadIdx, const MyLib::Vector3& pos);	// ����

	// �����o�֐�
	inline void SetSelect(const bool bSelect)	{ m_bSelect = bSelect; }	// �I�𑀍�t���O�ݒ�
	inline bool IsSelect() const				{ return m_bSelect; }		// �I�𑀍�t���O�擾
	inline int GetPadIdx() const				{ return m_nPadIdx; }		// ���쌠�C���f�b�N�X�擾
	inline MyLib::PosGrid2 GetSelectIdx() const	{ return m_select; }		// �I���C���f�b�N�X�擾

private:
	// �I���
	enum ESelect
	{
		SELECT_NAME = 0,	// ���O
		SELECT_DRESSUP,		// �����ւ�
		SELECT_AREA,		// �|�W�V����
		SELECT_TRANS,		// �J��
		SELECT_MAX			// ���̗񋓌^�̑���
	};

	// �I���֐����X�g
	typedef void(CSelectUI::*SELECT_FUNC)(const float, const float, const float);
	static SELECT_FUNC m_SelectFuncList[];	// �֐��̃��X�g

	// �����o�֐�
	void UpdateName(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���O�̍X�V
	void UpdateDressup(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �����ւ��̍X�V
	void UpdateArea(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �|�W�V�����̍X�V
	void UpdateTrans(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �J�ڂ̍X�V
	HRESULT CreateUI();			// UI����
	void UpdateSelect();		// �I���X�V
	void UpdateDecideDressup();	// ����X�V
	void UpdateCancelDressup();	// �L�����Z���X�V
	void UpdateSelectPlayer();	// �I���v���C���[�X�V
	void SetPositionRelative();	// ���Έʒu�ݒ�
	bool IsSelectOK() const;	// �I�𑀍�\���̊m�F

	int GetNumSelectX(const int nSelectY) const;	// X�I�������擾
	int GetMoveYSelectX(const int nNextSelectY);	// Y�ړ�����X�␳�ʒu�擾
	void UpdateSelectX(const int nSelectY);			// X�I���X�V

	// �����o�ϐ�
	CObject2D_Anim* m_pPadUI;	// �R���g���[���[UI���
	CObject2D* m_pFrame;		// �t���[�����
	const int m_nPadIdx;		// ���쌠�C���f�b�N�X
	MyLib::PosGrid2 m_select;	// �I���C���f�b�N�X
	bool m_bSelect;				// �I�𑀍�t���O
	const CGameManager::ETeamSide m_team;	// �`�[��
};

#endif	// _SELECT_UI_H_

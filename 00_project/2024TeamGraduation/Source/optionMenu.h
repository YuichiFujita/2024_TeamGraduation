//==========================================================================
// 
//  �I�v�V�������j���[�w�b�_�[ [optionMenu.h]
//  Author : ���n�Ή�
// 
//==========================================================================

#ifndef _OPTIONMENU_H_
#define _OPTIONMENU_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object.h"

//==========================================================================
// �O���錾
//==========================================================================
class CMultiNumber;

//==========================================================================
// �N���X��`
//==========================================================================
// �I�v�V�������j���[�N���X
class COptionMenu : public CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EState // ���
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_SELECT,	// �I��
		STATE_EDIT,		// �ҏW
		STATE_MAX
	};

	enum ESelect // �I����
	{
		SELECT_MASTERVOLUME = 0,	// �}�X�^�[�{�����[��
		SELECT_SE,					// SE
		SELECT_BGM,					// BGM
		SELECT_VIB,					// �o�C�u���[�V����
		SELECT_BACK,				// �߂�
		SELECT_RANKING,				// �����L���O
		SELECT_MAX
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	COptionMenu(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~COptionMenu();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;	// �폜

	//=============================
	// �����o�֐�
	//=============================
	void SetState(EState state);			// ��Ԑݒ�
	EState GetState() { return m_state; }	// ��Ԏ擾

	void Debug();	// �f�o�b�O����

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static COptionMenu* Create();	// ����

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��Ԋ֐�
	typedef void(COptionMenu::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐�

	//=============================
	// �����o�ϐ�
	//=============================
	// ���
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��ԍX�V
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {}
	void StateSelect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �I��
	void StateEdit(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �ҏW

	// �X�V
	void UpdateObj(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �I�u�W�F�N�g�X�V
	void UpdateColor(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �F�X�V
	void UpdateSelect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �I�����X�V
	void UpdateMarker(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �}�[�J�[�X�V
	void UpdateSound(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �T�E���h�X�V
	void UpdateVib(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �o�C�u�t���O�X�V

	// ����
	HRESULT CreateBoard();		// �{�[�h����
	HRESULT CreatePaper();		// ������
	HRESULT CreateTitle(CObject2D** ppObj2D, const std::string& texture, const MyLib::Vector3& pos);	// ���o������
	HRESULT CreateSelect();		// �I��������
	HRESULT CreateNumber();		// ��������
	HRESULT CreatePin();		// �s������
	HRESULT CreateVibFrag();	// �o�C�u�t���O����
	HRESULT CreateDesign(CObject2D** ppObj2D, const std::string& texture, const MyLib::Vector3& pos, const MyLib::Vector3& rot);	// �f�U�C������
	HRESULT CreateSelectMarker();	// �I�����̃}�[�J�[����

	//=============================
	// �����o�ϐ�
	//=============================
	// ���
	EState m_state;		// ���
	float m_fStateTime;	// ��ԃ^�C�}�[

	// �I����
	ESelect m_select;		// �I����
	ESelect m_oldSelect;	// �O��̑I����

	// ���̑�
	CObject2D* m_pBoard;	// �{�[�h
	CObject2D* m_pPaper;	// ��
	CObject2D* m_pTitleSound;	// �T�E���h�̌��o��
	CObject2D* m_pTitlePad;		// �R���g���[���[�̌��o��
	CObject2D* m_pSelectUI[ESelect::SELECT_MAX];	// �I������UI
	CMultiNumber* m_pNumberMaster;	// �}�X�^�[�{�����[��
	CMultiNumber* m_pNumberSE;		// SE
	CMultiNumber* m_pNumberBGM;		// BGM
	CObject2D* m_pVibFrag;			// �o�C�u�̃t���O
	CObject2D* m_pPin;				// �s��s
	CObject2D* m_pDesign_01;		// �f�U�C��
	CObject2D* m_pDesign_02;		// �f�U�C��
	CObject2D* m_pSelectMarker;		// �I�����̃}�[�J�[
	float m_fMarkerTime;			// �}�[�J�[�̃^�C�}�[
};

#endif

//============================================================
//
//	�����ւ�UI�w�b�_�[ [dressupUI.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _DRESSUP_UI_H_
#define _DRESSUP_UI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "player.h"
#include "gamemanager.h"

//************************************************************
// �O���錾
//************************************************************
class CDressup;			// �����ւ��N���X
class CBallFake;		// ������\���p�{�[���N���X
class CObject2D;		// �I�u�W�F�N�g2D�N���X
class CObject2D_Anim;	// �I�u�W�F�N�g2D�A�j���N���X
class CEntry_Dressup;	// �h���X�A�b�v�ݒ�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �����ւ�UI�N���X
class CDressupUI : public CObject
{
public:
	// �G�f�B�b�g������
	enum EEditType
	{
		EDIT_PROCESS = 0,	// ���ۂ̕ύX
		EDIT_CHANGETYPE,	// �ύX������
		EDIT_MAX
	};

	// �ύX������
	enum EChangeType
	{
		TYPE_HAIR = 0,		// ��
		TYPE_ACCESSORY,		// �A�N�Z�T���[
		TYPE_FACE,			// ��
		TYPE_BODY,			// �̌^
		TYPE_HANDEDNESS,	// ������
		TYPE_MAX
	};

	// �R���X�g���N�^
	CDressupUI(CEntry_Dressup* pParent, const CPlayer::EFieldArea typeArea, const int nPlayerIdx);

	// �f�X�g���N�^
	~CDressupUI() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw() override;		// �`��
	void SetType(const TYPE type) override;					// ��ސݒ�
	void SetEnableDisp(const bool bDraw) override;			// �`��󋵐ݒ�
	void SetPosition(const MyLib::Vector3& pos) override;	// �ʒu�ݒ�

	// �ÓI�����o�֐�
	static CDressupUI* Create	// ����
	( // ����
		CEntry_Dressup* pParent,			// �e�N���X���
		const CPlayer::EFieldArea typeArea,	// �v���C���[�|�W�V����
		const int nPlayerIdx,				// �v���C���[�C���f�b�N�X
		const MyLib::Vector3& rPos			// ���_�ʒu
	);

	// �����o�֐�
	void SetReady(const bool bReady);			// ���������t���O�ݒ�
	void SetPadIdx(const int nPadIdx);			// ���쌠�C���f�b�N�X�ݒ�
	int GetMyPlayerIdx() const;					// �v���C���[�C���f�b�N�X�擾
	CGameManager::ETeamSide GetTeam() const;	// �`�[���T�C�h�擾
	int GetHairNowIdx() const;					// ���̃C���f�b�N�X�ԍ��擾
	int GetAccessoryNowIdx() const;				// �A�N�Z�̃C���f�b�N�X�ԍ��擾
	int GetFaceNowIdx() const;					// ��̃C���f�b�N�X�ԍ��擾
	CPlayer::EBody GetBodyType() const;			// �̌^�擾
	CPlayer::EHandedness GetHandedness() const;	// ������擾
	inline CPlayer::EFieldArea GetArea() const	{ return m_typeArea; }		// �v���C���[�|�W�V�����擾
	inline int GetPadIdx() const				{ return m_nPadIdx; }		// ���쌠�C���f�b�N�X�擾
	inline EEditType GetTypeEdit() const		{ return m_typeEdit; }		// �G�f�B�b�g��ގ擾
	inline EChangeType GetTypeChange() const	{ return m_typeChange; }	// �ύX��ގ擾
	inline bool IsReady() const					{ return m_bReady; }		// ���������t���O�擾

private:
	// �����o�֐�
	HRESULT CreateUI();					// UI����
	HRESULT CreateChangeIcon();			// �ύX��ރA�C�R������
	HRESULT CreateReadyCheck();			// ���������`�F�b�N����
	HRESULT CreatePlayerFrame();		// �v���C���[�t���[������
	HRESULT CreatePlayerUI();			// �v���C���[UI����
	HRESULT CreateSetup();				// �Z�b�g�A�b�v����
	HRESULT CreateRenderTexture();		// �����_�[�e�N�X�`������
	void CreateTexture();				// �e�N�X�`���쐬
	void SetPositionRelative();			// ���Έʒu�ݒ�
	void UpdateControl(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ����X�V
	void UpdateUI();					// UI�X�V
	void UpdatePlayerUI();				// �v���C���[UI�X�V
	void ChangeEditType(int nPadIdx);	// �G�f�B�b�g�����ޕύX
	void ChangeChangeType(int nPadIdx);	// �ύX����ӏ��̎�ޕύX
	void ChangeBodyType(int nPadIdx);	// �̌^�ύX
	void ChangeHandedness(int nPadIdx);	// ������ύX
	HRESULT ReCreatePlayer(CPlayer::EHandedness handedness, CPlayer::EBody body);	// �v���C���[�Đ���

	// �ÓI�����o�ϐ�
	static int m_nNumAI;	// AI����

	// �����o�ϐ�
	CEntry_Dressup* m_pParent;		// �e�N���X���
	CRenderTexture* m_pRenderScene;	// �V�[�������_�[�e�N�X�`��
	CObject2D_Anim* m_pChangeIcon;	// �ύX��ރA�C�R�����
	CObject2D_Anim* m_pReadyCheck;	// ���������`�F�b�N���
	CObject2D_Anim* m_pPlayerFrame;	// �v���C���[�t���[�����
	CObject2D* m_pPlayerUI;			// �v���C���[UI���
	int m_nOrdinalAI;				// ���g���������ꂽ���� (AI�̂�)
	int m_nPadIdx;					// ���쌠�C���f�b�N�X
	bool m_bReady;					// ���������t���O
	CBallFake* m_pBall;				// �{�[��
	CPlayer* m_pPlayer;				// �v���C���[
	CDressup* m_pHair;				// �������ւ�
	CDressup* m_pAccessory;			// �A�N�Z�����ւ�
	CDressup* m_pFace;				// �璅���ւ�
	EEditType m_typeEdit;			// �G�f�B�b�g������
	EChangeType m_typeChange;		// �ύX������
	const int m_nPlayerIdx;					// �v���C���[�C���f�b�N�X
	const CPlayer::EFieldArea m_typeArea;	// �v���C���[�|�W�V����
};

#endif	// _DRESSUP_UI_H_

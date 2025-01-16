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
	CSelectUI(const int nPlayerIdx, const int nPadIdx);

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
	static CSelectUI* Create(const int nPlayerIdx, const int nPadIdx, const MyLib::Vector3& pos);	// ����

	// �����o�֐�
	inline void SetSelect(const bool bSelect) { m_bSelect = bSelect; }	// �I�𑀍�t���O�ݒ�
	inline bool IsSelect() const	{ return m_bSelect; }				// �I�𑀍�t���O�擾
	inline int GetPadIdx() const	{ return m_nPadIdx; }				// ���쌠�C���f�b�N�X�擾
	inline int GetSelectIdx() const	{ return m_nSelectPlayerIdx; }		// �I���v���C���[�C���f�b�N�X�擾

private:
	// �����o�֐�
	HRESULT CreateUI();			// UI����
	void UpdateSelect();		// �I���X�V
	void UpdateDecide();		// ����X�V
	void UpdateCancel();		// �L�����Z���X�V
	void SetPositionRelative();	// ���Έʒu�ݒ�

	// �����o�ϐ�
	CObject2D_Anim* m_pPadUI;	// �R���g���[���[UI���
	CObject2D* m_pFrame;		// �t���[�����
	const int m_nPadIdx;		// ���쌠�C���f�b�N�X
	int m_nSelectPlayerIdx;		// �I���v���C���[�C���f�b�N�X
	bool m_bSelect;				// �I�𑀍�t���O
};

#endif	// _SELECT_UI_H_

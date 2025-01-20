//==========================================================================
//
//	�p�b�hUI�w�b�_�[ [padUI.h]
//	Author�F���n�Ή�
//
//==========================================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PAD_UI_H_
#define _PAD_UI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object2D_Anim.h"
#include "arrowUI.h"

//************************************************************
// �O���錾
//************************************************************

//************************************************************
// �N���X��`
//************************************************************
// �p�b�hUI�N���X
class CPadUI : public CObject2D_Anim
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPadUI(int nPriority);
	~CPadUI() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void SetPosition(const MyLib::Vector3& pos) override;	// �ʒu�ݒ�
	virtual void SetEnableDisp(bool bDisp) override;		// �`��󋵐ݒ�

	//=============================
	// �����o�֐�
	//=============================
	void Decide();		// ����
	void Cancel();		// �L�����Z��
	void SetSizeByWidth(const float width);					// ��������T�C�Y�ݒ�
	void SetAction(CArrowUI::EDirection dir);				// �A�N�V�����ݒ�
	void SetEnableDispArrow(bool bDisp);					// ���̕`��ݒ�
	CArrowUI* GetArrowUI(int i) { return m_apArrow[i]; };	// ���UI�̏��擾

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CPadUI* Create(const MyLib::Vector3& pos, const float width, const int pattern, int nPriority = mylib_const::PRIORITY_DEF2D);	// ����

private:

	//=============================
	// �����o�֐�
	//=============================
	HRESULT CreateArrowUI();	// ���UI�̐���

	//=============================
	// �����o�ϐ�
	//=============================
	CArrowUI* m_apArrow[CArrowUI::EDirection::DIRECTION_MAX];	// ���̏��
};

#endif	// _ARROW_UI_H_

//============================================================
//
//	�h���X�A�b�vUI�w�b�_�[ [dressupUI.h]
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

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �h���X�A�b�vUI�N���X
class CDressupUI : public CObject
{
public:
	// �R���X�g���N�^
	CDressupUI();

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
	void SetRotation(const MyLib::Vector3& rot) override;	// �����ݒ�

	// �ÓI�����o�֐�
	static CDressupUI* Create	// ����
	( // ����
		const MyLib::Vector3& rPos	// ���_�ʒu
	);

	// �����o�֐�


private:
	// �����o�֐�
	void SetPositionRelative();	// ���Έʒu�ݒ�

	// �����o�ϐ�
	
};

#endif	// _DRESSUP_UI_H_
